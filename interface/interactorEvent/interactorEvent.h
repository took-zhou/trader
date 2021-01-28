/*
 * interactorEvent.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INTERFACE_INTERACTOREVENT_H_
#define WORKSPACE_TRADER_INTERFACE_INTERACTOREVENT_H_
#include <map>
#include <functional>
#include <string>

struct MsgStruct;

struct InteractEvent
{
    bool init();

    void handle(MsgStruct& msg);
    void regMsgFun();

    std::map<std::string, std::function<void(MsgStruct& msg)>> msgFuncMap;
};



#endif /* WORKSPACE_TRADER_INTERFACE_INTERACTOREVENT_H_ */
