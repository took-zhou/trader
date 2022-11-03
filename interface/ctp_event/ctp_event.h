/*
 * ctp_event.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INTERFACE_CTPEVENT_H_
#define WORKSPACE_TRADER_INTERFACE_CTPEVENT_H_
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "common/extern/ctp/inc/ThostFtdcTraderApi.h"

#include "common/self/dci/role.h"
#include "common/self/utils.h"

struct CtpEvent {
  CtpEvent();
  void Handle(utils::ItpMsg &msg);
  void RegMsgFun();

  void OnRtnOrderHandle(utils::ItpMsg &msg);
  void OnRtnTradeHandle(utils::ItpMsg &msg);
  void OnRspOrderActionHandle(utils::ItpMsg &msg);

  void OnRspQryTradingAccountHandle(utils::ItpMsg &msg);
  void OnRspQryInstrumentHandle(utils::ItpMsg &msg);
  void OnRspQryInstrumentMarginRateHandle(utils::ItpMsg &msg);
  void OnRspQryInstrumentCommissionRateHandle(utils::ItpMsg &msg);
  void OnRspQryOptionInstrCommRateHandle(utils::ItpMsg &msg);

 private:
  std::map<std::string, std::function<void(utils::ItpMsg &msg)>> msg_func_map_;

  bool SendEmail(const utils::OrderContent &content);
};

#endif /* WORKSPACE_TRADER_INTERFACE_CTPEVENT_H_ */
