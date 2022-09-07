/*
 * marketEvent.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#include "trader/interface/marketEvent/marketEvent.h"
#include "common/extern/log/log.h"
#include "common/self/basetype.h"
#include "common/self/protobuf/market-trader.pb.h"
#include "common/self/utils.h"
#include "trader/domain/traderService.h"
#include "trader/infra/recerSender.h"
#include "trader/interface/ctpEvent/ctpEvent.h"

MarketEvent::MarketEvent() { regMsgFun(); }

void MarketEvent::regMsgFun() {
  int cnt = 0;
  msgFuncMap.clear();
  msgFuncMap["QryInstrumentReq"] = [this](utils::ItpMsg &msg) { QryInstrumentReqHandle(msg); };

  for (auto &iter : msgFuncMap) {
    INFO_LOG("msgFuncMap[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
}

void MarketEvent::handle(utils::ItpMsg &msg) {
  auto iter = msgFuncMap.find(msg.msgName);
  if (iter != msgFuncMap.end()) {
    iter->second(msg);
    return;
  }
  ERROR_LOG("can not find func for msgName [%s]!", msg.msgName.c_str());
  return;
}

void MarketEvent::QryInstrumentReqHandle(utils::ItpMsg &msg) {
  market_trader::message reqMsg;
  reqMsg.ParseFromString(msg.pbMsg);
  auto &traderSer = TraderSevice::getInstance();
  if (traderSer.login_state != LOGIN_STATE) {
    ERROR_LOG("itp not login!");
    pubQryInstrumentRsp(nullptr, true, false);
    return;
  }
  auto &req = reqMsg.qry_instrument_req();
  if (req.identity() != std::string("all")) {
    ERROR_LOG("qry_instrument_req identity[%s] is not [all]", req.identity().c_str());
    return;
  }

  utils::InstrumtntID ins_exch;
  ins_exch.exch = "";
  ins_exch.ins = "";

  auto &recerSender = RecerSender::getInstance();
  recerSender.ROLE(Sender).ROLE(ItpSender).ReqInstrumentInfo(ins_exch, utils::stringToInt(req.identity()));
}

void MarketEvent::pubQryInstrumentRsp(CThostFtdcInstrumentField *field, bool result, bool isFinish) {
  market_trader::message rsp;
  auto *qryInstruments = rsp.mutable_qry_instrument_rsp();

  if (result == false) {
    qryInstruments->set_result(market_trader::Result::failed);
    qryInstruments->set_finish_flag(true);
  } else {
    qryInstruments->set_instrument_id(field->InstrumentID);
    qryInstruments->set_exchange_id(field->ExchangeID);
    qryInstruments->set_price_tick(utils::doubleToStringConvert(field->PriceTick).c_str());
    qryInstruments->set_result(market_trader::Result::success);
    qryInstruments->set_finish_flag(isFinish);
  }

  utils::ItpMsg msg;
  rsp.SerializeToString(&msg.pbMsg);
  msg.sessionName = "market_trader";
  msg.msgName = "QryInstrumentRsp";
  auto &recerSender = RecerSender::getInstance();
  bool sendRes = recerSender.ROLE(Sender).ROLE(ProxySender).send(msg);

  if (sendRes == false) {
    ERROR_LOG("send OrderInsertRsp error");
  }
  return;
}
