/*
 * marketEvent.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#include "trader/interface/market_event/market_event.h"
#include "common/extern/log/log.h"

#include "common/self/protobuf/market-trader.pb.h"
#include "common/self/utils.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"
#include "trader/interface/ctp_event/ctp_event.h"

MarketEvent::MarketEvent() { RegMsgFun(); }

void MarketEvent::RegMsgFun() {
  int cnt = 0;
  msg_func_map.clear();
  msg_func_map["QryInstrumentReq"] = [this](utils::ItpMsg &msg) { QryInstrumentReqHandle(msg); };

  for (auto &iter : msg_func_map) {
    INFO_LOG("msg_func_map[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
}

void MarketEvent::Handle(utils::ItpMsg &msg) {
  auto iter = msg_func_map.find(msg.msg_name);
  if (iter != msg_func_map.end()) {
    iter->second(msg);
    return;
  }
  ERROR_LOG("can not find func for msg name [%s]!", msg.msg_name.c_str());
  return;
}

void MarketEvent::QryInstrumentReqHandle(utils::ItpMsg &msg) {
  market_trader::message message;
  message.ParseFromString(msg.pb_msg);
  auto &trader_ser = TraderSevice::GetInstance();
  if (trader_ser.login_state != kLoginState) {
    ERROR_LOG("itp not login!");
    return;
  }
  auto &req = message.qry_instrument_req();
  if (req.identity() != std::string("all")) {
    ERROR_LOG("qry_instrument_req identity[%s] is not [all]", req.identity().c_str());
    return;
  }

  utils::InstrumtntID ins_exch;
  ins_exch.exch = "";
  ins_exch.ins = "";

  auto &recer_sender = RecerSender::GetInstance();
  recer_sender.ROLE(Sender).ROLE(ItpSender).ReqInstrumentInfo(ins_exch, 10000000);  // 10000000代表是market的请求
}
