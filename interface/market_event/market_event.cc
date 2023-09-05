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
  msg_func_map["MarketStateReq"] = [this](utils::ItpMsg &msg) { MarketStateReqHandle(msg); };

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
  auto &trader_ser = TraderService::GetInstance();
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
  recer_sender.ROLE(Sender).ROLE(ItpSender).ReqInstrumentInfo(ins_exch);  // 10000000代表是market的请求
}

void MarketEvent::MarketStateReqHandle(utils::ItpMsg &msg) {
  auto &trader_ser = TraderService::GetInstance();
  auto &recer_sender = RecerSender::GetInstance();
  market_trader::message message;
  message.ParseFromString(msg.pb_msg);
  auto &market_state = message.market_state_req();
  auto state = market_state.market_state();
  auto date = market_state.date();

  trader_ser.ROLE(HandleState).trder_date = date;
  if (state == market_trader::MarketStateReq_MarketState_day_open || state == market_trader::MarketStateReq_MarketState_night_open) {
    recer_sender.ROLE(Sender).ROLE(ItpSender).ReqUserLogin();
    trader_ser.UpdateLoginState(kLoginState);
    trader_ser.ROLE(AccountAssign).HandleTraderOpen();
  }

  if (state == market_trader::MarketStateReq_MarketState_day_close || state == market_trader::MarketStateReq_MarketState_night_close) {
    if (state == market_trader::MarketStateReq_MarketState_day_close) {
      trader_ser.ROLE(OrderLookup).HandleTraderClose();
      trader_ser.ROLE(AccountAssign).HandleTraderClose();
    }
    recer_sender.ROLE(Sender).ROLE(ItpSender).ReqUserLogout();
    trader_ser.UpdateLoginState(kLogoutState);
  }

  {
    market_trader::message message;
    auto *market_state_rsp = message.mutable_market_state_rsp();
    market_state_rsp->set_result(true);

    utils::ItpMsg msg;
    message.SerializeToString(&msg.pb_msg);
    msg.session_name = "market_trader";
    msg.msg_name = "MarketStateRsp";
    RecerSender::GetInstance().ROLE(Sender).ROLE(ProxySender).SendMsg(msg);
    INFO_LOG("market state handle ok.");
  }
}
