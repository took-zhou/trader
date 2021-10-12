/*
 * proxyRecer.cpp
 *      Author: Administrator
 */

#include "trader/infra/recer/proxyRecer.h"
#include "trader/infra/zmqBase.h"
#include "common/extern/libzmq/include/zhelpers.h"
#include "common/extern/log/log.h"
#include "common/extern/libgo/libgo/libgo.h"
#include "common/self/utils.h"

#include <map>

extern MsgStruct NilMsgStruct;
extern std::map<std::string, EventType> TitleToEvent;
extern co_chan<MsgStruct> orderMsgChan;
extern co_chan<MsgStruct> queryMsgChan;

void ProxyRecer::init()
{
    INFO_LOG("begin ProxyRecer::init");
    topicList.clear();
    //strategy_trader
    topicList.push_back("strategy_trader.OrderInsertReq");
    topicList.push_back("strategy_trader.AccountStatusReq");
    topicList.push_back("strategy_trader.OrderCancelReq");
    topicList.push_back("strategy_trader.MarginRateReq");
    topicList.push_back("strategy_trader.CommissionRateReq");
    //trader_trader
    topicList.push_back("trader_trader.HeartBeat");

    //market_trader
    topicList.push_back("market_trader.QryInstrumentReq");

    auto& zmqBase = ZmqBase::getInstance();
    for(auto& topic : topicList)
    {
        zmqBase.SubscribeTopic(topic.c_str());
    }

    INFO_LOG("sub topics ok");
}

bool ProxyRecer::checkSessionAndTitle(std::vector<std::string>& sessionAndTitle)
{
    return true;
}

bool ProxyRecer::isTopicInSubTopics(std::string title)
{
    for(auto& topic : topicList)
    {
        if(topic == title)
        {
            return true;
        }
    }
    return false;
}

MsgStruct ProxyRecer::receMsg()
{
    static MsgStruct NilMsgStruct;
    MsgStruct msg;
    auto& zmqBase = ZmqBase::getInstance();
    auto receiver = zmqBase.receiver;
    if(receiver == nullptr)
    {
        ERROR_LOG("receiver is nullptr");
    }
    INFO_LOG("prepare recv titleChar");
    char* recContent = s_recv(receiver);
    std::string content = std::string(recContent);
    auto spacePos = content.find_first_of(" ");
    auto title = content.substr(0, spacePos);
    auto pbMsg = content.substr(spacePos+1);
    if(! isTopicInSubTopics(title))
    {
        return NilMsgStruct;
    }
    INFO_LOG("recv msg, topic is[%s]",title.c_str());
    std::string tmpEventName = std::string(title);
    std::vector<std::string> sessionAndTitle = utils::splitString(tmpEventName, std::string("."));
    if(sessionAndTitle.size() != 2)
    {
        return NilMsgStruct;
    }

    if(! checkSessionAndTitle(sessionAndTitle))
    {
        return NilMsgStruct;
    }
    std::string session = sessionAndTitle.at(0);
    std::string msgTitle = sessionAndTitle.at(1);

    msg.sessionName = session;
    msg.msgName = msgTitle;
    msg.pbMsg = pbMsg;

    if (strstr(msgTitle.c_str(), "Order") == NULL)
    {
        queryMsgChan << msg;
    }
    else
    {
        orderMsgChan << msg;
    }

    return msg;
}
