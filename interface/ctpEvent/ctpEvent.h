/*
 * ctpEvent.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INTERFACE_CTPEVENT_H_
#define WORKSPACE_TRADER_INTERFACE_CTPEVENT_H_
#include <map>
#include <functional>
#include <string>
#include <vector>
#include "common/self/dci/Role.h"
#include "common/self/basetype.h"
#include "common/extern/ctp/inc/ThostFtdcTraderApi.h"
struct MsgStruct;
struct MarketEvent;
struct StrategyEvent;
struct InstrumentQryTmp
{
    bool isOk{false};
    std::vector<CThostFtdcInstrumentField> partRspList;
};

struct CtpEvent
{
    bool init();

    void handle(MsgStruct& msg);
    void regMsgFun();

    void OnRspSettlementInfoConfirmHandle(MsgStruct& msg);
    void OnRspUserLoginHandle(MsgStruct& msg);
    void OnRspAuthenticateHandle(MsgStruct& msg);
    void OnErrRtnOrderInsertHandle(MsgStruct& msg);
    void OnRspOrderInsertHandle(MsgStruct& msg);
    void OnRtnOrderHandle(MsgStruct& msg);
    void OnRtnTradeHandle(MsgStruct& msg);
    void OnRspQryTradingAccountHandle(MsgStruct& msg);
    void OnRspQryInstrumentHandle(MsgStruct& msg);

    void OnRspOrderActionHandle(MsgStruct& msg);
    void OnErrRtnOrderActionHandle(MsgStruct& msg);
    std::map<std::string, std::function<void(MsgStruct& msg)>> msgFuncMap;

    USE_ROLE(MarketEvent);
    USE_ROLE(StrategyEvent);
    std::map<U32,InstrumentQryTmp> qryRspsMap;
private:
    U32 buildNewKey();
    bool getNotFullRspMap(U32& key);
    U32 addNewRspsList();
    void delRspsList(U32 key);
};



#endif /* WORKSPACE_TRADER_INTERFACE_CTPEVENT_H_ */
