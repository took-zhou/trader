/*
 * manageEvent.cpp
 *
 *  Created on: 2020.11.13
 *      Author: Administrator
 */
#include "trader/interface/manageEvent/manageEvent.h"
#include "common/extern/log/log.h"
#include "common/self/protobuf/manage-trader.pb.h"
#include "trader/domain/traderService.h"
#include "trader/infra/recerSender.h"

#include "common/self/semaphorePart.h"

ManageEvent::ManageEvent() { regMsgFun(); }

void ManageEvent::regMsgFun() {
  int cnt = 0;
  msgFuncMap.clear();
  msgFuncMap["AccountStatusReq"] = [this](utils::ItpMsg &msg) { AccountStatusReqHandle(msg); };

  for (auto &iter : msgFuncMap) {
    INFO_LOG("msgFuncMap[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
}

void ManageEvent::handle(utils::ItpMsg &msg) {
  auto iter = msgFuncMap.find(msg.msgName);
  if (iter != msgFuncMap.end()) {
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
  if (traderSer.login_state != LOGIN_STATE) {
    ERROR_LOG("itp not login!");
    return;
  }

  auto &recerSender = RecerSender::getInstance();
  recerSender.ROLE(Sender).ROLE(ItpSender).ReqAvailableFunds(stoi(identify));
}
