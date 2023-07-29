/*
 * otp_event.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INTERFACE_OTPEVENT_H_
#define WORKSPACE_TRADER_INTERFACE_OTPEVENT_H_
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "common/self/utils.h"

struct OtpEvent {
  OtpEvent();
  void Handle(utils::ItpMsg &msg);
  void RegMsgFun();

  void OnBusinessRejectHandle(utils::ItpMsg &msg);
  void OnOrderReportHandle(utils::ItpMsg &msg);
  void OnTradeReportHandle(utils::ItpMsg &msg);
  void OnQueryCashAssetHandle(utils::ItpMsg &msg);
  void OnQueryCommissionRateHandle(utils::ItpMsg &msg);

 private:
  std::map<std::string, std::function<void(utils::ItpMsg &msg)>> msg_func_map_;

  bool SendEmail(const utils::OrderContent &content);
};

#endif /* WORKSPACE_TRADER_INTERFACE_OTPEVENT_H_ */
