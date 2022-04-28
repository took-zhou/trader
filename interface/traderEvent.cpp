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
extern co_chan<MsgStruct> orderMsgChan;
extern co_chan<MsgStruct> queryMsgChan;
constexpr U32 MAIN_THREAD_WAIT_TIME = 100000;
constexpr U32 RSP_HANDLE_THREAD_WAIT_TIME = 1000000;

void TraderEvent::regSessionFunc()
{
    int cnt = 0;
    sessionFuncMap.clear();
    sessionFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct msg)>>(std::string("market_trader"),         [this](MsgStruct msg){ROLE(MarketEvent).handle(msg);}));
    sessionFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct msg)>>(std::string("strategy_trader"),       [this](MsgStruct msg){ROLE(StrategyEvent).handle(msg);}));
    sessionFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct msg)>>(std::string("trader_trader"),         [this](MsgStruct msg){ROLE(SelfEvent).handle(msg);}));
    sessionFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct msg)>>(std::string("interactor_trader"),     [this](MsgStruct msg){ROLE(InteractEvent).handle(msg);}));
    sessionFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct msg)>>(std::string("ctp"),                   [this](MsgStruct msg){ROLE(CtpEvent).handle(msg);}));
    sessionFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct msg)>>(std::string("ctpview_trader"),        [this](MsgStruct msg){ROLE(CtpviewEvent).handle(msg);}));

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
    ROLE(CtpviewEvent).init();
    return true;
}

bool TraderEvent::run()
{
    auto orderRecRun = [&](){
        MsgStruct msg;
        while(1)
        {
            //INFO_LOG("orderRecRun while");
            orderMsgChan >> msg;
            INFO_LOG("orderRecRun*************msg name[%s]",msg.msgName.c_str());
            if(! msg.isValid())
            {
                ERROR_LOG(" invalid msg, session is [%s], msgName is [%s]",msg.sessionName.c_str(), msg.msgName.c_str());
                continue;
            }

            if(sessionFuncMap.find(msg.sessionName) != sessionFuncMap.end())
            {
                sessionFuncMap[msg.sessionName](msg);
            }
            else
            {
                ERROR_LOG("can not find[%s] in sessionFuncMap",msg.sessionName.c_str());
            }
        }
    };
    INFO_LOG("orderRecRun prepare ok");
    std::thread(orderRecRun).detach();

    auto queryRecRun = [&](){
        MsgStruct msg;
        while(1)
        {
            //INFO_LOG("queryRecRun while");
            queryMsgChan >> msg;
            INFO_LOG("queryRecRun*************msg name[%s]",msg.msgName.c_str());
            if(! msg.isValid())
            {
                ERROR_LOG(" invalid msg, session is [%s], msgName is [%s]",msg.sessionName.c_str(), msg.msgName.c_str());
                continue;
            }

            if(sessionFuncMap.find(msg.sessionName) != sessionFuncMap.end())
            {
                sessionFuncMap[msg.sessionName](msg);
                usleep(RSP_HANDLE_THREAD_WAIT_TIME);
            }
            else
            {
                ERROR_LOG("can not find[%s] in sessionFuncMap",msg.sessionName.c_str());
            }
        }
    };
    INFO_LOG("queryRecRun prepare ok");
    std::thread(queryRecRun).detach();

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

            if(sessionFuncMap.find(msg.sessionName) != sessionFuncMap.end())
            {
                sessionFuncMap[msg.sessionName](msg);
            }
            else
            {
                ERROR_LOG("can not find[%s] in sessionFuncMap",msg.sessionName.c_str());
            }
        }
    };
    INFO_LOG("ctpRecRun prepare ok");
    std::thread(ctpRecRun).detach();

    auto& recerSender = RecerSender::getInstance();
    recerSender.run();

    return true;
}
