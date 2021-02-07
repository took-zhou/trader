/*
 * traderEvent.cpp
 *
 *  Created on: 2020年9月20日
 *      Author: Administrator
 */

#include "trader/interface/traderEvent.h"
#include "common/extern/libgo/libgo/libgo.h"
#include "trader/infra/recerSender.h"

#include "trader/domain/traderService.h"
#include <thread>
#include <chrono>

extern co_chan<MsgStruct> ctpMsgChan;
constexpr U32 MAIN_THREAD_WAIT_TIME = 100000;

void TraderEvent::regSessionFunc()
{
    int cnt = 0;
    sessionFuncMap.clear();
    sessionFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct msg)>>(std::string("market_trader"),         [this](MsgStruct msg){ROLE(MarketEvent).handle(msg);}));
    sessionFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct msg)>>(std::string("strategy_trader"),       [this](MsgStruct msg){ROLE(StrategyEvent).handle(msg);}));
    sessionFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct msg)>>(std::string("trader_trader"),         [this](MsgStruct msg){ROLE(SelfEvent).handle(msg);}));
    sessionFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct msg)>>(std::string("interactor_trader"),     [this](MsgStruct msg){ROLE(InteractEvent).handle(msg);}));
    sessionFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct msg)>>(std::string("ctp"),                   [this](MsgStruct msg){ROLE(CtpEvent).handle(msg);}));

    for(auto iter : sessionFuncMap)
    {
        INFO_LOG("sessionFuncMap[%d] key is [%s]",cnt, iter.first.c_str());
        cnt++;
    }
}

bool TraderEvent::init()
{
    regSessionFunc();
    ROLE(MarketEvent).init();
    ROLE(StrategyEvent).init();
    ROLE(SelfEvent).init();
    ROLE(InteractEvent).init();
    ROLE(CtpEvent).init();
    return true;
}

bool TraderEvent::run()
{
    co::Scheduler* sched = co::Scheduler::Create();
    auto& recerSender = RecerSender::getInstance();
    INFO_LOG("go into run");
    auto proxyRecRun = [&](){
        while(1)
        {
            INFO_LOG("proxyRecRun while");
            MsgStruct msg = recerSender.ROLE(Recer).ROLE(ProxyRecer).receMsg();
            INFO_LOG("handle new msg, session is[%s],msgName is[%s]",msg.sessionName.c_str(), msg.msgName.c_str());
            if(! msg.isValid())
            {
                // ERROR_LOG(" invalid msg, session is [%s], msgName is [%s]",msg.sessionName.c_str(), msg.msgName.c_str());
                continue;
            }
            auto eventFunc = [this, msg]{
                if(sessionFuncMap.find(msg.sessionName) != sessionFuncMap.end())
                {
                    sessionFuncMap[msg.sessionName](msg);
                    return;
                }
                ERROR_LOG("can not find[%s] in sessionFuncMap",msg.sessionName.c_str());
            };
            std::thread(eventFunc).detach();
        }
    };
    INFO_LOG("proxyRecRun prepare ok");
    std::thread(proxyRecRun).detach();   // zmq 在libgo的携程里会跑死，单独拎出来一个线程。
    auto ctpRecRun = [&](){
        MsgStruct msg;
        while(1)
        {
            //INFO_LOG("ctpRecRun while");
            ctpMsgChan >> msg;
            //INFO_LOG("hai*************msg name[%s]",msg.msgName.c_str());
            if(! msg.isValid())
            {
                ERROR_LOG(" invalid msg, session is [%s], msgName is [%s]",msg.sessionName.c_str(), msg.msgName.c_str());
                continue;
            }
            auto eventFunc = [this, msg]{
                if(sessionFuncMap.find(msg.sessionName) != sessionFuncMap.end())
                {
                    sessionFuncMap[msg.sessionName](msg);
                }
            };
            std::thread(eventFunc).detach();
        }
    };
    INFO_LOG("ctpRecRun prepare ok");
    std::thread(ctpRecRun).detach();

    while (1)
    {
        usleep(MAIN_THREAD_WAIT_TIME);
    }
    return true;
}
