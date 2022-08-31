/*
 * ctpEvent.h
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

#include "common/self/basetype.h"
#include "common/self/dci/Role.h"
#include "common/self/utils.h"

struct XtpEvent {
  XtpEvent();
  void handle(utils::ItpMsg &msg);
  void regMsgFun();

  void OnTradeEventHandle(utils::ItpMsg &msg);
  void OnOrderEventHandle(utils::ItpMsg &msg);
  void OnCancelOrderErrorHandle(utils::ItpMsg &msg);

  void OnRspQryTradingAccountHandle(utils::ItpMsg &msg);
  void OnRspQryInstrumentHandle(utils::ItpMsg &msg);
  void OnRspQryInstrumentMarginRateHandle(utils::ItpMsg &msg);
  void OnRspQryInstrumentCommissionRateHandle(utils::ItpMsg &msg);

 private:
  std::map<std::string, std::function<void(utils::ItpMsg &msg)>> msgFuncMap;

  bool sendEmail(const utils::OrderContent &content);
};

#endif /* WORKSPACE_TRADER_INTERFACE_CTPEVENT_H_ */
