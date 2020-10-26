/*
 * ctpEvent.h
 *
 *  Created on: 2020��8��28��
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INTERFACE_CTPEVENT_H_
#define WORKSPACE_TRADER_INTERFACE_CTPEVENT_H_
#include <map>
#include <functional>
#include <string>
struct MsgStruct;
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

    std::map<std::string, std::function<void(MsgStruct& msg)>> msgFuncMap;

};



#endif /* WORKSPACE_TRADER_INTERFACE_CTPEVENT_H_ */
