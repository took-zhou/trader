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
#include "timer.h"
#include "define.h"
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


void HandleSel::tradeHandle(const json& msgBody, void* __this)
{

    DEBUG_LOG("tradeHandle reach");
    TradePart* _this = (TradePart*)__this;
    _this->handleTradeMsg(msgBody);
}

void HandleSel::marketHandle(const json& msgBody, void* __this)
{
    Query* _this = (Query*)__this;
    _this->handleQueryMsg(msgBody);
}

void HandleSel::routeHandle(const json& msgBody, void* __this)
{
    if (msgBody.count("status") != 0 and msgBody["status"].get<std::string>() == std::string("alive"))
    {
        auto& timerPool = TimeoutTimerPool::getInstance();
        
        if (not timerPool.isTimerExist(ROUTE_HEADBEAT_TIMER))
        {
            SocketClient* socketPart = (SocketClient*)__this;
            auto timerOutFunc = [&]() {
                INFO_LOG("%s","route head beat time out!");
                socketPart->isRouterConnected = false;
                socketPart->routerReconnect();
            };
            timerPool.addTimer(ROUTE_HEADBEAT_TIMER, timerOutFunc, HEADBEAT_TIME_OUT_LENGTH);
        }
        timerPool.getTimerByName(ROUTE_HEADBEAT_TIMER)->stop();
        timerPool.getTimerByName(ROUTE_HEADBEAT_TIMER)->restart();
        return;
    }
}

void HandleSel::msgHandleSel()
{
    INFO_LOG("begin to handle MSG"); // @suppress("Invalid arguments")
    int sockfd = ROLE(SocketClient).newSocket;
    TradeMsgHead msgHead{0};
    while(true)
    {
        while(1)    // route must connected
        {
            if(ROLE(SocketClient).isRouterConnected)
            {
                break;
            }
        }
        INFO_LOG("%s","**********************wait for msghead ......**********************");
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
//                tradeThread = std::thread(tradeHandle,msgHead, (void*)&ROLE(TradePart)); // @suppress("Type cannot be resolved")
                json msgBody;
                if(!getMsgBody(msgBody, msgHead))
                {
                    break;
                }
                std::thread tradeThread(tradeHandle,msgBody, (void*)&ROLE(TradePart));
                tradeThread.detach();
//                tradeThread.join();
                break;
            }
            case ClientType::Route:
            {
                INFO_LOG("create route thread by msg");
                json msgBody;
                if (!getMsgBody(msgBody, msgHead))
                {
                    break;
                }
                std::thread routeThread(routeHandle, msgBody, (void*)&ROLE(SocketClient)); // @suppress("Type cannot be resolved")
                routeThread.detach();
                break;
            }
            case ClientType::Market:
            {
                INFO_LOG("create Market thread by msg");
                json msgBody;
                if(!getMsgBody(msgBody, msgHead))
                {
                    break;
                }
                std::thread marketThread(marketHandle,msgBody,(void*)&ROLE(Query)); // @suppress("Type cannot be resolved")
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

bool HandleSel::getMsgBody(json& msgBody, TradeMsgHead& msgHead)
{
    char *bodyMsg = new char[msgHead.length];
    std::memset(bodyMsg,0,msgHead.length);

    int sockfd = ROLE(SocketClient).newSocket;
    int n = recv(sockfd, bodyMsg, msgHead.length, 0);
    if(n < 0)
    {
        ERROR_LOG("read body msg from socket error!"); // @suppress("Invalid arguments")
        return false;
    }
    if(n == 0)
    {
        ERROR_LOG("router is disconnected!!!!!");
        ROLE(SocketClient).isRouterConnected = false;
        ROLE(SocketClient).routerReconnect();
        return false;
    }
    msgBody = json::parse(string(bodyMsg));
    delete[] bodyMsg;
    return true;
}



