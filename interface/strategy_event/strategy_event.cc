/*
 * strategyEvent.cpp
 */
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "common/extern/log/log.h"
#include "common/self/global_sem.h"
#include "common/self/profiler.h"
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/utils.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"
#include "trader/interface/strategy_event/strategy_event.h"

StrategyEvent::StrategyEvent() { RegMsgFun(); }

void StrategyEvent::Handle(utils::ItpMsg &msg) {
  auto iter = msg_func_map_.find(msg.msg_name);
  if (iter != msg_func_map_.end()) {
    iter->second(msg);
    return;
  }
  ERROR_LOG("can not find func for msg_name [%s]!", msg.msg_name.c_str());
  return;
}

void StrategyEvent::RegMsgFun() {
  int cnt = 0;
  msg_func_map_.clear();
  msg_func_map_["OrderInsertReq"] = [this](utils::ItpMsg &msg) { OrderInsertReqHandle(msg); };
  msg_func_map_["OrderCancelReq"] = [this](utils::ItpMsg &msg) { OrderCancelReqHandle(msg); };
  msg_func_map_["TransactionCostReq"] = [this](utils::ItpMsg &msg) { TransactionCostReqHandle(msg); };
  msg_func_map_["ActiveSafetyRsp"] = [this](utils::ItpMsg &msg) { StrategyAliveRspHandle(msg); };
  msg_func_map_["CheckTraderAliveReq"] = [this](utils::ItpMsg &msg) { CheckTraderAliveReqHandle(msg); };
  msg_func_map_["OrderPositionReq"] = [this](utils::ItpMsg &msg) { OrderPositionReqHandle(msg); };

  for (auto &iter : msg_func_map_) {
    INFO_LOG("msg_func_map_[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
  return;
}

void StrategyEvent::OrderCancelReqHandle(utils::ItpMsg &msg) {
#ifdef BENCH_TEST
  ScopedTimer t("OrderCancelReqHandle");
#endif
  strategy_trader::message message;
  message.ParseFromString(msg.pb_msg);

  auto &order_cancel_req = message.order_cancel_req();

  auto &trader_ser = TraderService::GetInstance();
  if (trader_ser.GetLoginState() != kLoginState) {
    ERROR_LOG("itp not login!");
    return;
  }

  std::string temp_key;
  temp_key += order_cancel_req.instrument();
  temp_key += ".";
  temp_key += order_cancel_req.index();
  auto pos = trader_ser.ROLE(OrderLookup).GetOrderIndexMap().find(temp_key);
  if (pos != trader_ser.ROLE(OrderLookup).GetOrderIndexMap().end()) {
    for (auto &item : pos->second) {
      auto content = trader_ser.ROLE(OrderManage).GetOrder(item.second->GetOrderRef());
      if (content != nullptr && content->total_volume > (content->success_volume + content->fail_volume)) {
        content->active_cancle_indication = true;
        auto &recer_sender = RecerSender::GetInstance();
        recer_sender.ROLE(Sender).ROLE(ItpSender).CancelOrder(*content);
      }
    }
  }
}

void StrategyEvent::OrderInsertReqHandle(utils::ItpMsg &msg) {
  PZone("f");
#ifdef BENCH_TEST
  ScopedTimer t("OrderInsertReqHandle");
#endif
  strategy_trader::message message;
  message.ParseFromString(msg.pb_msg);

  const auto &order_insert_req = message.order_insert_req();
  auto &trader_ser = TraderService::GetInstance();
  auto &order_allocate = trader_ser.ROLE(OrderAllocate);

  if (trader_ser.GetLoginState() != kLoginState) {
    ERROR_LOG("itp not login!");
    return;
  }

  auto content = utils::OrderContent();
  content.index = order_insert_req.index();
  content.exchange_id = order_insert_req.order().exchangeid();
  content.instrument_id = order_insert_req.order().instrument();
  content.total_volume = order_insert_req.order().volume_total_original();
  content.limit_price = order_insert_req.order().limitprice();
  content.direction = order_insert_req.order().direction();
  content.comboffset = order_insert_req.order().comb_offset_flag();
  content.order_type = order_insert_req.order().order_type();
  order_allocate.UpdateOrderList(content);
  if (order_allocate.GetOderList().size() == 0) {
    strategy_trader::message message;
    auto *insert_rsp = message.mutable_order_insert_rsp();
    insert_rsp->set_instrument(content.instrument_id);
    insert_rsp->set_index(content.index);
    insert_rsp->set_result(strategy_trader::Result::failed);
    insert_rsp->set_reason(strategy_trader::FailedReason::Account_Assign_Error);
    auto *rsp_info = insert_rsp->mutable_info();
    rsp_info->set_orderprice(content.limit_price);
    rsp_info->set_ordervolume(content.total_volume);

    message.SerializeToString(&msg.pb_msg);
    msg.session_name = "strategy_trader";
    msg.msg_name = "OrderInsertRsp";
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
  } else {
    for (auto &order : order_allocate.GetOderList()) {
      auto &recer_sender = RecerSender::GetInstance();
      bool result = recer_sender.ROLE(Sender).ROLE(ItpSender).InsertOrder(*order.get());
      if (!result) {
        trader_ser.ROLE(OrderManage).DelOrder(order->order_ref);
      }
    }
  }
}

// 将保证金和手续费综合成交易成本
void StrategyEvent::TransactionCostReqHandle(utils::ItpMsg &msg) {
  strategy_trader::message message;
  message.ParseFromString(msg.pb_msg);
  auto &trader_ser = TraderService::GetInstance();
  if (trader_ser.GetLoginState() != kLoginState) {
    ERROR_LOG("itp not login!");
    return;
  }

  auto &recer_sender = RecerSender::GetInstance();
  utils::InstrumtntID ins_exch;
  ins_exch.ins = message.transaction_cost_req().instrument_info().instrument_id();
  ins_exch.exch = message.transaction_cost_req().instrument_info().exchange_id();

  recer_sender.ROLE(Sender).ROLE(ItpSender).ReqTransactionCost(ins_exch);
}

void StrategyEvent::StrategyAliveRspHandle(utils::ItpMsg &msg) { GlobalSem::GetInstance().PostSemBySemName(SemName::kStrategyRsp); }

void StrategyEvent::CheckTraderAliveReqHandle(utils::ItpMsg &msg) {
  strategy_trader::message message;
  auto *trader_alive = message.mutable_trader_alive_rsp();
  trader_alive->set_alive_rsp(true);

  utils::ItpMsg send_msg;
  message.SerializeToString(&send_msg.pb_msg);
  send_msg.session_name = "strategy_trader";
  send_msg.msg_name = "CheckTraderAliveRsp";
  RecerSender::GetInstance().ROLE(Sender).ROLE(ProxySender).SendMsg(send_msg);
}

void StrategyEvent::OrderPositionReqHandle(utils::ItpMsg &msg) {
  strategy_trader::message message;
  message.ParseFromString(msg.pb_msg);

  auto &trader_ser = TraderService::GetInstance();
  auto &account_assign = trader_ser.ROLE(AccountAssign);
  auto &order_lookup = trader_ser.ROLE(OrderLookup);
  auto &position_req = message.position_req();

  std::string temp_key;
  temp_key += position_req.instrument();
  temp_key += ".";
  temp_key += position_req.index();

  uint32_t volume = 0;
  auto lookup_pos = order_lookup.GetOrderIndexMap().find(temp_key);
  if (lookup_pos != order_lookup.GetOrderIndexMap().end()) {
    for (auto &item : lookup_pos->second) {
      auto account_pos = account_assign.GetAccountInfoMap().find(item.first);
      if (account_pos != account_assign.GetAccountInfoMap().end()) {
        volume += item.second->GetYesterdayVolume();
        volume += item.second->GetTodayVolume();
      }
    }
  }

  strategy_trader::message message2;
  auto *positoin_rsp = message2.mutable_position_rsp();
  positoin_rsp->set_instrument(position_req.instrument());
  positoin_rsp->set_index(position_req.index());
  positoin_rsp->set_volume(volume);

  utils::ItpMsg send_msg;
  message2.SerializeToString(&send_msg.pb_msg);
  send_msg.session_name = "strategy_trader";
  send_msg.msg_name = "OrderPositionRsp";
  RecerSender::GetInstance().ROLE(Sender).ROLE(ProxySender).SendMsg(send_msg);
}
