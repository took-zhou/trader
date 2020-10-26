/*
 * interactorEvent.cpp
 *
 *  Created on: 2020��8��30��
 *      Author: Administrator
 */

#include "trader/interface/interactorEvent/interactorEvent.h"
#include "common/extern/log/log.h"
#include "trader/infra/define.h"

bool InteractEvent::init()
{
    regMsgFun();

    return true;
}


void InteractEvent::regMsgFun()
{
    msgFuncMap.clear();
}

void InteractEvent::handle(MsgStruct& msg)
{
    auto iter = msgFuncMap.find(msg.msgName);
    if(iter != msgFuncMap.end())
    {
        iter->second(msg);
        return;
    }
    ERROR_LOG("can not find func for msgName [%s]!",msg.msgName.c_str());
    return;
}

