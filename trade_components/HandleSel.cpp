#include "HandleSel.h"
#include "socketClient.h"
#include "log.h"
#include "PrintCheck.h"
#include "define.h"
#include <unistd.h>
#include <unordered_map>
#include <string>
#include <functional>
#include <vector>

namespace
{
    enum struct ClientType
    {
        Trade = 0,
        Market,
        Route,
        Gui,
        Strategy
    };
    std::unordered_map<string, ClientType> ClientTypeMap =
    {
            {"trade",   ClientType::Trade},
            {"market",  ClientType::Market},
            {"route",   ClientType::Route},
            {"gui",     ClientType::Gui},
            {"strategy",     ClientType::Strategy}
    };

    std::vector<std::string> MsName {"trade", "market", "route", "gui", "strategy"};

    bool isNameInMsNameList(const std::string name)
    {
        for(auto iter : MsName)
        {
            if(iter == name)
            {
                return true;
            }
        }
        return false;
    }

    bool checkMsName(const TradeMsgHead& msgHead)
    {
        if(! isNameInMsNameList(msgHead.fromClientName))
        {
            ERROR_LOG("fromClientName is not MsName");
            return false;
        }

        if(! isNameInMsNameList(msgHead.toClientName))
        {
            ERROR_LOG("toClientName is not MsName");
            return false;
        }
        return true;
    }

}


void HandleSel::tradeHandle(const TradeMsgHead& msgHead, void* __this)
{

    DEBUG_LOG("tradeHandle reach");
    TradePart* _this = (TradePart*)__this;
    _this->handleTradeMsg(msgHead);
}

void HandleSel::marketHandle(const TradeMsgHead& msgHead, void* __this)
{
    Query* _this = (Query*)__this;
    _this->handleQueryMsg(msgHead);
}

void HandleSel::msgHandleSel()
{
    INFO_LOG("begin to handle MSG"); // @suppress("Invalid arguments")
    int sockfd = ROLE(SocketClient).newSocket;
    TradeMsgHead msgHead{0};
    while(true)
    {

        INFO_LOG("wait for msghead ......");
        if(!parseMsgHead(sockfd, msgHead))
        {
            ERROR_LOG("parse head msg error!");
            continue;
        }
        INFO_LOG("msgHead parse ok! msgHead is:"); // @suppress("Invalid arguments");
        ROLE(PintCheck).printMsgHead(msgHead);
        if(! checkMsName(msgHead))
        {
            continue;
        }
        if(ClientTypeMap[msgHead.toClientName] !=  ClientTypeMap["trade"])
        {
            ERROR_LOG("toClientName is not trade"); // @suppress("Invalid arguments")
            ROLE(SocketClient).recUselessMsgBody((size_t)msgHead.length);
            continue;
        }

        if(!ROLE(LogInPart).isLogIN)
        {
            ERROR_LOG("client does not logged in");
            while(!ROLE(LogInPart).isLogIN){}
            continue;
        }

        switch (ClientTypeMap[msgHead.fromClientName])
        {
            case ClientType::Strategy:
            {
                INFO_LOG("create stategy thread by msg");
                tradeThread = std::thread(tradeHandle,msgHead, (void*)&ROLE(TradePart)); // @suppress("Type cannot be resolved")
                tradeThread.join();
                break;
            }
            case ClientType::Route:
            {
                break;
            }
            case ClientType::Market:
            {
                INFO_LOG("create Market thread by msg");
                marketThread = std::thread(marketHandle,msgHead,(void*)&ROLE(Query)); // @suppress("Type cannot be resolved")
                marketThread.detach();
                break;
            }
            case ClientType::Gui:
            {
                break;
            }
            default:
            {
                ERROR_LOG("the fromClientName  is unknow");
                ROLE(SocketClient).recUselessMsgBody((size_t)msgHead.length);
                break;
            }
        }

    }
}

bool HandleSel::parseMsgHead(int sockfd, TradeMsgHead& msgHead)
{
    int n;
    n = recv(sockfd, &msgHead, sizeof(TradeMsgHead),0);
    if(n < 0 )
    {
        ERROR_LOG("read head msg from socket error, recv length:[%d]", n); // @suppress("Invalid arguments")
        return false;
    }
    if(n == 0)
    {
        ERROR_LOG("router is disconnected!!!!!");
        ROLE(SocketClient).isRouterConnected = false;
        ROLE(SocketClient).routerReconnect();
        return false;
    }

    return true;
}



