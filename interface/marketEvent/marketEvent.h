/*
 * marketEvent.h
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
#include "common/self/basetype.h"
#include "common/self/dci/Role.h"
#include "common/self/utils.h"

struct MarketEvent {
  MarketEvent();

  void handle(utils::ItpMsg &msg);
  void regMsgFun();

  void QryInstrumentReqHandle(utils::ItpMsg &msg);
  void pubQryInstrumentRsp(CThostFtdcInstrumentField *field, bool result, bool isFinish);

  std::map<std::string, std::function<void(utils::ItpMsg &msg)>> msgFuncMap;
};

#endif /* WORKSPACE_TRADER_INTERFACE_MARKETEVENT_H_ */
