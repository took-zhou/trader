/*
 * proxyRecer.cpp
 *      Author: Administrator
 */

#include "trader/infra/recer/proxyRecer.h"
#include "trader/infra/zmqBase.h"
#include "common/extern/libzmq/include/zhelpers.h"
#include "common/extern/log/log.h"
#include <map>
#include "common/self/utils.h"

extern MsgStruct NilMsgStruct;
extern std::map<std::string, EventType> TitleToEvent;

void ProxyRecer::init()
{
    INFO_LOG("begin ProxyRecer::init");
    auto& zmqBase = ZmqBase::getInstance();
    //strategy_trader
    zmqBase.SubscribeTopic("strategy_trader.OrderInsertReq ");
    zmqBase.SubscribeTopic("strategy_trader.AccountStatusReq");

    //trader_trader
    zmqBase.SubscribeTopic("trader_trader.HeartBeat");

    INFO_LOG("sub topics ok");
}


bool ProxyRecer::checkSessionAndTitle(std::vector<std::string>& sessionAndTitle)
{
    return true;
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
    INFO_LOG("return msg");
    return msg;
}
