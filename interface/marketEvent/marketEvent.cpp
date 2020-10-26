/*
 * marketEvent.cpp
 *
 *  Created on: 2020��8��30��
 *      Author: Administrator
 */


#include "trader/interface/marketEvent/marketEvent.h"
#include "common/extern/log/log.h"
#include "trader/infra/define.h"
bool MarketEvent::init()
{
    regMsgFun();

    return true;
}


void MarketEvent::regMsgFun()
{
    msgFuncMap.clear();
}

void MarketEvent::handle(MsgStruct& msg)
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
