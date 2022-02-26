/*
 * marketEvent.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INTERFACE_MARKETEVENT_H_
#define WORKSPACE_TRADER_INTERFACE_MARKETEVENT_H_
#include <map>
#include <functional>
#include <string>
#include <vector>
#include "common/self/dci/Role.h"
#include "common/self/basetype.h"
#include "common/extern/ctp/inc/ThostFtdcTraderApi.h"
struct MsgStruct;
struct CtpEvent;
struct MarketEvent
{
    bool init();

    void handle(MsgStruct& msg);
    void regMsgFun();

    void QryInstrumentReqHandle(MsgStruct& msg);
    void pubQryInstrumentRsp(CThostFtdcInstrumentField *field, bool result, bool isFinish);

    std::map<std::string, std::function<void(MsgStruct& msg)>> msgFuncMap;

    USE_ROLE(CtpEvent);
};



#endif /* WORKSPACE_TRADER_INTERFACE_MARKETEVENT_H_ */
