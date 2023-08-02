/*
 * market_event.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */
#ifndef WORKSPACE_TRADER_INTERFACE_MARKETEVENT_H_
#define WORKSPACE_TRADER_INTERFACE_MARKETEVENT_H_
#include <functional>
#include <map>
#include <string>
#include <vector>
#include "common/extern/ctp/inc/ThostFtdcTraderApi.h"

#include "common/self/dci/role.h"
#include "common/self/utils.h"

struct MarketEvent {
  MarketEvent();

  void Handle(utils::ItpMsg &msg);
  void RegMsgFun();

  void QryInstrumentReqHandle(utils::ItpMsg &msg);
  void MarketStateReqHandle(utils::ItpMsg &msg);

  std::map<std::string, std::function<void(utils::ItpMsg &msg)>> msg_func_map;
};

#endif /* WORKSPACE_TRADER_INTERFACE_MARKETEVENT_H_ */
