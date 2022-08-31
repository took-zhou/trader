/*
 * ctpviewEvent.h
 *
 *  Created on: 2020.11.13
 *      Author: Administrator
 */
#ifndef WORKSPACE_TRADER_INTERFACE_CTPVIEWEVENT_CTPVIEWEVENT_H_
#define WORKSPACE_TRADER_INTERFACE_CTPVIEWEVENT_CTPVIEWEVENT_H_

#include <functional>
#include <map>
#include <string>

#include "common/self/utils.h"

struct CtpviewEvent {
  CtpviewEvent();
  void handle(utils::ItpMsg &msg);
  void regMsgFun();

  void LoginControlHandle(utils::ItpMsg &msg);
  void BugInjectionHandle(utils::ItpMsg &msg);

  std::map<std::string, std::function<void(utils::ItpMsg &msg)>> msgFuncMap;
};

#endif /* WORKSPACE_TRADER_INTERFACE_CTPVIEWEVENT_CTPVIEWEVENT_H_ */
