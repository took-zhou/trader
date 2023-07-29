/*
 * ctp_event.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INTERFACE_XTPEVENT_H_
#define WORKSPACE_TRADER_INTERFACE_XTPEVENT_H_
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "common/self/dci/role.h"
#include "common/self/utils.h"

struct XtpEvent {
  XtpEvent();
  void Handle(utils::ItpMsg &msg);
  void RegMsgFun();

  void OnTradeEventHandle(utils::ItpMsg &msg);
  void OnOrderEventHandle(utils::ItpMsg &msg);
  void OnCancelOrderErrorHandle(utils::ItpMsg &msg);

  void OnQueryAssetHandle(utils::ItpMsg &msg);

 private:
  std::map<std::string, std::function<void(utils::ItpMsg &msg)>> msg_func_map_;

  bool SendEmail(const utils::OrderContent &content);
};

#endif /* WORKSPACE_TRADER_INTERFACE_CTPEVENT_H_ */
