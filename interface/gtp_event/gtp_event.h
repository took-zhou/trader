/*
 * gtp_event.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INTERFACE_GTPEVENT_H_
#define WORKSPACE_TRADER_INTERFACE_GTPEVENT_H_
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "common/self/utils.h"

struct GtpEvent {
  GtpEvent();
  void Handle(utils::ItpMsg &msg);
  void RegMsgFun();

  void OnRtnTradeHandle(utils::ItpMsg &msg);
  void OnRtnOrderHandle(utils::ItpMsg &msg);
  void OnRtnOrderInsertHandle(utils::ItpMsg &msg);
  void OnRtnOrderActionHandle(utils::ItpMsg &msg);
  void OnRspTradingAccountHandle(utils::ItpMsg &msg);
  void OnRspMarginRateHandle(utils::ItpMsg &msg);
  void OnRspCommissionRateHandle(utils::ItpMsg &msg);

 private:
  std::map<std::string, std::function<void(utils::ItpMsg &msg)>> msg_func_map_;

  bool SendEmail(const utils::OrderContent &content);
};

#endif /* WORKSPACE_TRADER_INTERFACE_GTPEVENT_H_ */
