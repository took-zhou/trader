/*
 * manageEvent.cpp
 *
 *  Created on: 2020.11.13
 *      Author: Administrator
 */
#include "trader/interface/manage_event/manage_event.h"
#include "common/extern/log/log.h"
#include "common/self/protobuf/manage-trader.pb.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"

#include "common/self/semaphore.h"

ManageEvent::ManageEvent() { RegMsgFun(); }

void ManageEvent::RegMsgFun() {
  int cnt = 0;
  msg_func_map.clear();
  msg_func_map["AccountStatusReq"] = [this](utils::ItpMsg &msg) { AccountStatusReqHandle(msg); };

  for (auto &iter : msg_func_map) {
    INFO_LOG("msg_func_map[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
}

void ManageEvent::Handle(utils::ItpMsg &msg) {
  auto iter = msg_func_map.find(msg.msgName);
  if (iter != msg_func_map.end()) {
    iter->second(msg);
    return;
  }
  ERROR_LOG("can not find func for msgName [%s]!", msg.msgName.c_str());
  return;
}

void ManageEvent::AccountStatusReqHandle(utils::ItpMsg &msg) {
  manage_trader::message reqMsg;
  reqMsg.ParseFromString(msg.pbMsg);
  auto reqInfo = reqMsg.account_status_req();
  auto identify = reqInfo.process_random_id();
  auto &traderSer = TraderSevice::getInstance();
  if (traderSer.login_state != kLoginState) {
    ERROR_LOG("itp not login!");
    return;
  }

  auto &recerSender = RecerSender::getInstance();
  recerSender.ROLE(Sender).ROLE(ItpSender).ReqAvailableFunds(stoi(identify));
}
