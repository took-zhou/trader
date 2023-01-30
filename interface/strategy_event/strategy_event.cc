/*
 * strategyEvent.cpp
 */
#include <fstream>
#include <iomanip>
#include <iostream>

#include "common/extern/log/log.h"
#include "common/extern/protobuf/src/google/protobuf/text_format.h"
#include "common/self/file_util.h"
#include "common/self/profiler.h"
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/semaphore.h"
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

  auto &trader_ser = TraderSevice::GetInstance();
  if (trader_ser.login_state != kLoginState) {
    ERROR_LOG("itp not login!");
    return;
  }

  auto content = trader_ser.ROLE(OrderManage).GetOrder(order_cancel_req.order_ref());
  if (content != nullptr) {
    content->active_cancle_indication = true;

    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(ItpSender).CancelOrder(*content);
  }
}

void StrategyEvent::OrderInsertReqHandle(utils::ItpMsg &msg) {
  PZone("OrderInsertReqHandle");
#ifdef BENCH_TEST
  ScopedTimer t("OrderInsertReqHandle");
#endif
  strategy_trader::message message;
  message.ParseFromString(msg.pb_msg);

  const auto &order_insert_req = message.order_insert_req();
  auto &trader_ser = TraderSevice::GetInstance();

  if (trader_ser.login_state != kLoginState) {
    ERROR_LOG("itp not login!");
    return;
  }

  auto content = std::make_shared<utils::OrderContent>();
  content->prid = order_insert_req.process_random_id();
  content->index = order_insert_req.index();
  content->exchange_id = order_insert_req.order().exchangeid();
  content->instrument_id = order_insert_req.order().instrument();
  content->total_volume = order_insert_req.order().volume_total_original();
  content->left_volume = order_insert_req.order().volume_total_original();
  content->limit_price = order_insert_req.order().limitprice();
  content->direction = order_insert_req.order().direction();
  content->comboffset = order_insert_req.order().comb_offset_flag();
  content->order_type = order_insert_req.order().order_type();

  trader_ser.ROLE(AccountStatus).SelectAccountStatus(&content->session_id, &content->order_ref);

  auto &order_manage = trader_ser.ROLE(OrderManage);
  order_manage.BuildOrder(content->order_ref, content);

  {
    PZone("InsertOrder");
    auto &recer_sender = RecerSender::GetInstance();
    bool result = recer_sender.ROLE(Sender).ROLE(ItpSender).InsertOrder(*content.get());
    if (result == false) {
      order_manage.DelOrder(content->order_ref);
    } else {
      strategy_trader::message message;
      auto *insert_rsp = message.mutable_order_insert_rsp();
      insert_rsp->set_order_ref(content->order_ref);
      insert_rsp->set_instrument(content->instrument_id);
      insert_rsp->set_index(content->index);
      insert_rsp->set_result(strategy_trader::Result::Result_INVALID);

      message.SerializeToString(&msg.pb_msg);
      msg.session_name = "strategy_trader";
      msg.msg_name = "OrderInsertRsp." + content->prid;
      recer_sender.ROLE(Sender).ROLE(ProxySender).SendMsg(msg);
    }
  }

  return;
}

// 将保证金和手续费综合成交易成本
void StrategyEvent::TransactionCostReqHandle(utils::ItpMsg &msg) {
  strategy_trader::message message;
  message.ParseFromString(msg.pb_msg);
  auto process_random_id = message.transaction_cost_req().process_random_id();
  auto &trader_ser = TraderSevice::GetInstance();
  if (trader_ser.login_state != kLoginState) {
    ERROR_LOG("itp not login!");
    return;
  }

  auto &recer_sender = RecerSender::GetInstance();
  utils::InstrumtntID ins_exch;
  ins_exch.ins = message.transaction_cost_req().instrument_info().instrument_id();
  ins_exch.exch = message.transaction_cost_req().instrument_info().exchange_id();

  recer_sender.ROLE(Sender).ROLE(ItpSender).ReqTransactionCost(ins_exch, stoi(process_random_id));
}

void StrategyEvent::StrategyAliveRspHandle(utils::ItpMsg &msg) { GlobalSem::GetInstance().PostSemBySemName(GlobalSem::kStrategyRsp); }
