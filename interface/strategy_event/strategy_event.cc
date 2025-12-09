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
  ERROR_LOG("can not find func for msg name [%s]!", msg.msg_name.c_str());
  return;
}

void StrategyEvent::RegMsgFun() {
  msg_func_map_.clear();
  msg_func_map_["OrderInsertReq"] = [this](utils::ItpMsg &msg) { OrderInsertReqHandle(msg); };
  msg_func_map_["OrderCancelReq"] = [this](utils::ItpMsg &msg) { OrderCancelReqHandle(msg); };
  msg_func_map_["TransactionCostReq"] = [this](utils::ItpMsg &msg) { TransactionCostReqHandle(msg); };
  msg_func_map_["ActiveSafetyRsp"] = [this](utils::ItpMsg &msg) { StrategyAliveRspHandle(msg); };
  msg_func_map_["CheckTraderAliveReq"] = [this](utils::ItpMsg &msg) { CheckTraderAliveReqHandle(msg); };
  msg_func_map_["OrderPositionReq"] = [this](utils::ItpMsg &msg) { OrderPositionReqHandle(msg); };
  msg_func_map_["GroupSizeReq"] = [this](utils::ItpMsg &msg) { GroupSizeReqHandle(msg); };
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
    ERROR_LOG("itp not login.");
    return;
  }

  std::string temp_key;
  temp_key += order_cancel_req.instrument();
  temp_key += ".";
  temp_key += order_cancel_req.index();
  auto order_index_pos = trader_ser.ROLE(OrderLookup).GetOrderIndexMap().find(temp_key);
  if (order_index_pos == trader_ser.ROLE(OrderLookup).GetOrderIndexMap().end()) {
    ERROR_LOG("not find order index.");
    return;
  }
  for (auto &item : order_index_pos->second) {
    auto content = trader_ser.ROLE(OrderManage).GetOrder(item.second->GetYesterdayOrderRef());
    if (content != nullptr && content->once_volume > (content->success_volume + content->fail_volume)) {
      auto account_info_pos = trader_ser.ROLE(AccountAssign).GetAccountInfoMap().find(content->user_id);
      if (account_info_pos != trader_ser.ROLE(AccountAssign).GetAccountInfoMap().end()) {
        content->session_id = account_info_pos->second->GetSessionId();
        content->active_cancle_indication = true;
        auto &recer_sender = RecerSender::GetInstance();
        recer_sender.ROLE(Sender).ROLE(ItpSender).CancelOrder(*content);
      }
    }
    content = trader_ser.ROLE(OrderManage).GetOrder(item.second->GetTodayOrderRef());
    if (content != nullptr && content->once_volume > (content->success_volume + content->fail_volume)) {
      auto account_info_pos = trader_ser.ROLE(AccountAssign).GetAccountInfoMap().find(content->user_id);
      if (account_info_pos != trader_ser.ROLE(AccountAssign).GetAccountInfoMap().end()) {
        content->session_id = account_info_pos->second->GetSessionId();
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
  content.once_volume = order_insert_req.order().once_volume();
  content.hold_volume = order_insert_req.order().hold_volume();
  content.limit_price = order_insert_req.order().limit_price();
  content.direction = order_insert_req.order().direction();
  content.comboffset = order_insert_req.order().comb_offset_flag();
  content.order_type = order_insert_req.order().order_type();
  order_allocate.UpdateOrderList(content);
  if (content.once_volume > 0) {
    strategy_trader::message message;
    auto *insert_rsp = message.mutable_order_insert_rsp();
    insert_rsp->set_instrument(content.instrument_id);
    insert_rsp->set_index(content.index);
    insert_rsp->set_result(strategy_trader::Result::failed);
    insert_rsp->set_reason(strategy_trader::FailedReason::Account_Assign_Error);
    auto *rsp_info = insert_rsp->mutable_info();
    rsp_info->set_orderprice(content.limit_price);
    rsp_info->set_ordervolume(content.once_volume);

    message.SerializeToString(&msg.pb_msg);
    msg.session_name = "strategy_trader";
    msg.msg_name = "OrderInsertRsp";
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
    INFO_LOG("account assign error: %s, volume: %d", content.instrument_id.c_str(), content.once_volume);
  }
  for (auto &order : order_allocate.GetOderList()) {
    auto &recer_sender = RecerSender::GetInstance();
    bool result = recer_sender.ROLE(Sender).ROLE(ItpSender).InsertOrder(*order.get());
    if (!result) {
      trader_ser.ROLE(OrderManage).DelOrder(order->order_ref);
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
  utils::InstrumentID ins_exch;
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
  auto &group_assign = trader_ser.ROLE(GroupAssign);
  auto &order_index_map = order_lookup.GetOrderIndexMap();
  auto &account_info_map = account_assign.GetAccountInfoMap();
  auto &group_assign_map = group_assign.GetAccoutGroupMap();
  auto &position_req = message.position_req();

  std::string temp_key;
  temp_key += position_req.instrument();
  temp_key += ".";
  temp_key += position_req.index();

  uint32_t volume = 0;
  auto pos = order_index_map.find(temp_key);
  if (pos != order_index_map.end()) {
    for (auto &item : pos->second) {
      auto temp_user_split = utils::SplitString(item.first, ".");
      if (temp_user_split.size() != 2) {
        continue;
      }
      if (group_assign_map.find(temp_user_split[0]) == group_assign_map.end() ||
          account_info_map.find(temp_user_split[1]) == account_info_map.end()) {
        continue;
      }
      volume += item.second->GetYesterdayVolume();
      volume += item.second->GetTodayVolume();
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

void StrategyEvent::GroupSizeReqHandle(utils::ItpMsg &msg) {
  strategy_trader::message message;
  message.ParseFromString(msg.pb_msg);
  auto &group_size_req = message.group_size_req();

  auto &trader_ser = TraderService::GetInstance();
  auto &group_assign = trader_ser.ROLE(GroupAssign);
  if (group_size_req.size_req()) {
    auto group_size = group_assign.GetAccoutGroupMap().size();

    strategy_trader::message message2;
    auto *group_size_rsp = message2.mutable_group_size_rsp();
    group_size_rsp->set_size_rsp(group_size);

    utils::ItpMsg send_msg;
    message2.SerializeToString(&send_msg.pb_msg);
    send_msg.session_name = "strategy_trader";
    send_msg.msg_name = "GroupSizeRsp";
    RecerSender::GetInstance().ROLE(Sender).ROLE(ProxySender).SendMsg(send_msg);
  }
}
