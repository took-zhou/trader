/*
 * strategyEvent.cpp
 */
#include <fstream>
#include <iomanip>
#include <iostream>

#include "common/extern/log/log.h"
#include "common/extern/protobuf/src/google/protobuf/text_format.h"
#include "common/self/file_util.h"
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/semaphore.h"
#include "common/self/utils.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"
#include "trader/interface/strategy_event/strategy_event.h"

StrategyEvent::StrategyEvent() { RegMsgFun(); }

void StrategyEvent::Handle(utils::ItpMsg &msg) {
  auto iter = msg_func_map.find(msg.msgName);
  if (iter != msg_func_map.end()) {
    iter->second(msg);
    return;
  }
  ERROR_LOG("can not find func for msgName [%s]!", msg.msgName.c_str());
  return;
}

void StrategyEvent::RegMsgFun() {
  int cnt = 0;
  msg_func_map.clear();
  msg_func_map["OrderInsertReq"] = [this](utils::ItpMsg &msg) { OrderInsertReqHandle(msg); };
  msg_func_map["AccountStatusReq"] = [this](utils::ItpMsg &msg) { AccountStatusReqHandle(msg); };
  msg_func_map["OrderCancelReq"] = [this](utils::ItpMsg &msg) { OrderCancelReqHandle(msg); };
  msg_func_map["TransactionCostReq"] = [this](utils::ItpMsg &msg) { TransactionCostReqHandle(msg); };
  msg_func_map["InstrumentReq"] = [this](utils::ItpMsg &msg) { InstrumentReqHandle(msg); };

  for (auto &iter : msg_func_map) {
    INFO_LOG("msg_func_map[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
  return;
}

void StrategyEvent::OrderCancelReqHandle(utils::ItpMsg &msg) {
#ifdef BENCH_TEST
  ScopedTimer t("OrderCancelReqHandle");
#endif
  strategy_trader::message reqMsg;
  reqMsg.ParseFromString(msg.pbMsg);

  auto &orderCancelReq = reqMsg.order_cancel_req();

  auto &traderSer = TraderSevice::getInstance();
  if (traderSer.login_state != kLoginState) {
    ERROR_LOG("itp not login!");
    return;
  }

  auto content = traderSer.ROLE(OrderManage).get_order(orderCancelReq.order_ref());
  if (content != nullptr) {
    content->activeCancleIndication = true;

    auto &recerSender = RecerSender::getInstance();
    recerSender.ROLE(Sender).ROLE(ItpSender).CancelOrder(*content);
  }
}

void StrategyEvent::AccountStatusReqHandle(utils::ItpMsg &msg) {
  strategy_trader::message reqMsg;
  reqMsg.ParseFromString(msg.pbMsg);
  auto reqInfo = reqMsg.account_status_req();
  auto prid = reqInfo.process_random_id();
  auto &traderSer = TraderSevice::getInstance();

  if (traderSer.login_state != kLoginState) {
    ERROR_LOG("itp not login!");
    return;
  }

  auto &recerSender = RecerSender::getInstance();
  recerSender.ROLE(Sender).ROLE(ItpSender).ReqAvailableFunds(stoi(prid));
}

void StrategyEvent::OrderInsertReqHandle(utils::ItpMsg &msg) {
#ifdef BENCH_TEST
  ScopedTimer t("OrderInsertReqHandle");
#endif
  strategy_trader::message reqMsg;
  reqMsg.ParseFromString(msg.pbMsg);

  const auto &orderInsertReq = reqMsg.order_insert_req();
  auto &traderSer = TraderSevice::getInstance();

  if (traderSer.login_state != kLoginState) {
    ERROR_LOG("itp not login!");
    return;
  }

  auto content = std::make_shared<utils::OrderContent>();
  content->prid = orderInsertReq.process_random_id();
  content->exchangeId = orderInsertReq.order().exchangeid();
  content->instrumentID = orderInsertReq.order().instrument();
  content->total_volume = orderInsertReq.order().volume_total_original();
  content->left_volume = orderInsertReq.order().volume_total_original();
  content->limit_price = orderInsertReq.order().limitprice();
  content->direction = orderInsertReq.order().direction();
  content->comboffset = orderInsertReq.order().comb_offset_flag();
  content->orderType = orderInsertReq.order().order_type();
  content->sessionId = orderInsertReq.session_id();
  content->orderRef = orderInsertReq.order_ref();

  auto &orderManage = traderSer.ROLE(OrderManage);
  orderManage.BuildOrder(orderInsertReq.order_ref(), content);

  auto &recerSender = RecerSender::getInstance();
  recerSender.ROLE(Sender).ROLE(ItpSender).InsertOrder(*content.get());

  return;
}

// 将保证金和手续费综合成交易成本
void StrategyEvent::TransactionCostReqHandle(utils::ItpMsg &msg) {
  strategy_trader::message reqMsg;
  reqMsg.ParseFromString(msg.pbMsg);
  auto prid = reqMsg.transaction_cost_req().process_random_id();
  auto &traderSer = TraderSevice::getInstance();
  if (traderSer.login_state != kLoginState) {
    ERROR_LOG("itp not login!");
    return;
  }

  utils::InstrumtntID ins_exch;
  ins_exch.ins = reqMsg.transaction_cost_req().instrument_info().instrument_id();
  ins_exch.exch = reqMsg.transaction_cost_req().instrument_info().exchange_id();

  auto &recerSender = RecerSender::getInstance();
  recerSender.ROLE(Sender).ROLE(ItpSender).ReqTransactionCost(ins_exch, stoi(prid));
}

void StrategyEvent::InstrumentReqHandle(utils::ItpMsg &msg) {
  strategy_trader::message reqMsg;
  reqMsg.ParseFromString(msg.pbMsg);
  auto prid = reqMsg.instrument_req().process_random_id();
  auto &traderSer = TraderSevice::getInstance();
  if (traderSer.login_state != kLoginState) {
    ERROR_LOG("itp not login!");
    return;
  }

  utils::InstrumtntID ins_exch;
  ins_exch.ins = reqMsg.instrument_req().instrument_info().instrument_id();
  ins_exch.exch = reqMsg.instrument_req().instrument_info().exchange_id();

  auto &recerSender = RecerSender::getInstance();
  recerSender.ROLE(Sender).ROLE(ItpSender).ReqInstrumentInfo(ins_exch, stoi(prid));
}
