/*
 * manageEvent.cpp
 *
 *  Created on: 2020.11.13
 *      Author: Administrator
 */
#include "trader/interface/ctpviewEvent/ctpviewEvent.h"
#include "common/extern/log/log.h"
#include "common/self/protobuf/ctpview-trader.pb.h"
#include "trader/domain/traderService.h"
#include "trader/infra/define.h"

bool CtpviewEvent::init() {
  regMsgFun();

  return true;
}

void CtpviewEvent::regMsgFun() {
  int cnt = 0;
  msgFuncMap.clear();
  msgFuncMap["LoginControl"] = [this](MsgStruct &msg) { LoginControlHandle(msg); };
  msgFuncMap["BugInjection"] = [this](MsgStruct &msg) { BugInjectionHandle(msg); };

  for (auto &iter : msgFuncMap) {
    INFO_LOG("msgFuncMap[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
}

void CtpviewEvent::handle(MsgStruct &msg) {
  auto iter = msgFuncMap.find(msg.msgName);
  if (iter != msgFuncMap.end()) {
    iter->second(msg);
    return;
  }
  ERROR_LOG("can not find func for msgName [%s]!", msg.msgName.c_str());
  return;
}

void CtpviewEvent::LoginControlHandle(MsgStruct &msg) {
  ctpview_trader::message login_control;
  login_control.ParseFromString(msg.pbMsg);
  auto indication = login_control.login_control();

  int command = indication.command();
  auto &traderSer = TraderSevice::getInstance();

  INFO_LOG("force set time state: %d", command);
  traderSer.ROLE(Trader).ROLE(TraderTimeState).set_time_state(command);
}

void CtpviewEvent::BugInjectionHandle(MsgStruct &msg) {
  ctpview_trader::message bug_injection;
  bug_injection.ParseFromString(msg.pbMsg);

  auto injection = bug_injection.bug_injection();

  ctpview_trader::BugInjection_InjectionType type = injection.type();
  INFO_LOG("set bug injection type: %d", type);

  if (type == ctpview_trader::BugInjection_InjectionType_double_free) {
    int *a = new int;
    std::shared_ptr<int> p(a);
    delete a;
  }
}
