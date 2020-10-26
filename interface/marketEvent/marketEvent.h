/*
 * marketEvent.h
 *
 *  Created on: 2020��8��28��
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INTERFACE_MARKETEVENT_H_
#define WORKSPACE_TRADER_INTERFACE_MARKETEVENT_H_
#include <map>
#include <functional>
#include <string>
struct MsgStruct;
struct MarketEvent
{
    bool init();

    void handle(MsgStruct& msg);
    void regMsgFun();

    std::map<std::string, std::function<void(MsgStruct& msg)>> msgFuncMap;
};



#endif /* WORKSPACE_TRADER_INTERFACE_MARKETEVENT_H_ */
