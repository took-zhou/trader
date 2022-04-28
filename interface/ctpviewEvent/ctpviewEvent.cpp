/*
 * interactorEvent.cpp
 *
 *  Created on: 2020.11.13
 *      Author: Administrator
 */
#include "common/extern/log/log.h"
#include "common/self/protobuf/ctpview-trader.pb.h"
#include "trader/interface/ctpviewEvent/ctpviewEvent.h"
#include "trader/infra/define.h"
#include "trader/domain/traderService.h"

bool CtpviewEvent::init()
{
    regMsgFun();

    return true;
}
void CtpviewEvent::regMsgFun()
{
    int cnt = 0;
    msgFuncMap.clear();
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("LoginControl", [this](MsgStruct& msg){LoginControlHandle(msg);}));

    for(auto iter : msgFuncMap)
    {
        INFO_LOG("msgFuncMap[%d] key is [%s]",cnt, iter.first.c_str());
        cnt++;
    }
}

void CtpviewEvent::handle(MsgStruct& msg)
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

void CtpviewEvent::LoginControlHandle(MsgStruct& msg)
{
    ctpview_trader::message login_control;
    login_control.ParseFromString(msg.pbMsg);
    auto indication = login_control.login_control();

    int command = indication.command();
    auto& traderSer = TraderSevice::getInstance();

    traderSer.ROLE(Trader).ROLE(TraderTimeState).set_time_state(command);
}
