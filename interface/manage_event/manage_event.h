/*
 * manage_event.h
 *
 *  Created on: 2020.11.13
 *      Author: Administrator
 */
#ifndef WORKSPACE_TRADER_INTERFACE_MANAGEEVENT_MANAGEEVENT_H_
#define WORKSPACE_TRADER_INTERFACE_MANAGEEVENT_MANAGEEVENT_H_

#include <functional>
#include <map>
#include <string>

#include "common/self/utils.h"

struct ManageEvent {
 public:
  ManageEvent();
  void Handle(utils::ItpMsg &msg);
  void RegMsgFun();

  void AccountStatusReqHandle(utils::ItpMsg &msg);

 private:
  std::map<std::string, std::function<void(utils::ItpMsg &msg)>> msg_func_map_;
};

#endif /* WORKSPACE_TRADER_INTERFACE_MANAGEEVENT_MANAGEEVENT_H_ */
