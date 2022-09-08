/*
 * strategyEvent.cpp
 */
#include <fstream>
#include <iomanip>
#include <iostream>

#include "common/extern/log/log.h"
#include "common/extern/protobuf/src/google/protobuf/text_format.h"
#include "common/self/fileUtil.h"
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/semaphorePart.h"
#include "common/self/utils.h"
#include "trader/domain/traderService.h"
#include "trader/infra/recerSender.h"
#include "trader/interface/strategyEvent/strategyEvent.h"

StrategyEvent::StrategyEvent() { regMsgFun(); }

void StrategyEvent::handle(utils::ItpMsg &msg) {
  auto iter = msgFuncMap.find(msg.msgName);
  if (iter != msgFuncMap.end()) {
    iter->second(msg);
    return;
  }
  ERROR_LOG("can not find func for msgName [%s]!", msg.msgName.c_str());
  return;
}

void StrategyEvent::regMsgFun() {
  int cnt = 0;
  msgFuncMap.clear();
  msgFuncMap["OrderInsertReq"] = [this](utils::ItpMsg &msg) { OrderInsertReqHandle(msg); };
  msgFuncMap["AccountStatusReq"] = [this](utils::ItpMsg &msg) { AccountStatusReqHandle(msg); };
  msgFuncMap["OrderCancelReq"] = [this](utils::ItpMsg &msg) { OrderCancelReqHandle(msg); };
  msgFuncMap["TransactionCostReq"] = [this](utils::ItpMsg &msg) { TransactionCostReqHandle(msg); };
  msgFuncMap["InstrumentReq"] = [this](utils::ItpMsg &msg) { InstrumentReqHandle(msg); };

  for (auto &iter : msgFuncMap) {
    INFO_LOG("sessionFuncMap[%d] key is [%s]", cnt, iter.first.c_str());
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
  if (traderSer.login_state != LOGIN_STATE) {
    ERROR_LOG("itp not login!");
    return;
  }

  auto content = traderSer.ROLE(OrderManage).getOrder(orderCancelReq.identity());
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

  if (traderSer.login_state != LOGIN_STATE) {
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

  if (traderSer.login_state != LOGIN_STATE) {
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
  content->userId = orderInsertReq.user();
  content->orderRef = orderInsertReq.identity();

  auto &orderManage = traderSer.ROLE(OrderManage);
  orderManage.buildOrder(orderInsertReq.identity(), content);

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
  if (traderSer.login_state != LOGIN_STATE) {
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
  if (traderSer.login_state != LOGIN_STATE) {
    ERROR_LOG("itp not login!");
    return;
  }

  utils::InstrumtntID ins_exch;
  ins_exch.ins = reqMsg.instrument_req().instrument_info().instrument_id();
  ins_exch.exch = reqMsg.instrument_req().instrument_info().exchange_id();

  auto &recerSender = RecerSender::getInstance();
  recerSender.ROLE(Sender).ROLE(ItpSender).ReqInstrumentInfo(ins_exch, stoi(prid));
}
