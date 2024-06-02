/*
 * ctpview_event.h
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
 public:
  CtpviewEvent();
  void Handle(utils::ItpMsg &msg);
  void RegMsgFun();

  void LoginControlHandle(utils::ItpMsg &msg);
  void BugInjectionHandle(utils::ItpMsg &msg);
  void ProfilerControlHandle(utils::ItpMsg &msg);
  void UpdateParaHandle(utils::ItpMsg &msg);
  void ClearDiagnosticEventHandle(utils::ItpMsg &msg);
  void SendTestEmailHandle(utils::ItpMsg &msg);

 private:
  std::map<std::string, std::function<void(utils::ItpMsg &msg)>> msg_func_map_;
};

#endif /* WORKSPACE_TRADER_INTERFACE_CTPVIEWEVENT_CTPVIEWEVENT_H_ */
