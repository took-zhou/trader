/*
 * strategyEvent.h
 *
 *  Created on: 2020��8��28��
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INTERFACE_STRATEGYEVENT_H_
#define WORKSPACE_TRADER_INTERFACE_STRATEGYEVENT_H_
#include <map>
#include <functional>
#include <string>
struct MsgStruct;

struct StrategyEvent
{
    bool init();
    void handle(MsgStruct& msg);
    void regMsgFun();
    void OrderInsertReqHandle(MsgStruct& msg);
    void pubOrderInsertRsp(std::string identity, bool result);

    void AccountStatusReqHandle(MsgStruct& msg);
    void pubAccountStatusRsq();
    std::map<std::string, std::function<void(MsgStruct& msg)>> msgFuncMap;
};



#endif /* WORKSPACE_TRADER_INTERFACE_STRATEGYEVENT_H_ */
