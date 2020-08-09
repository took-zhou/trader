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
    bool isHeadBeatHead(TradeMsgHead& msgHead)
    {
        if(msgHead.fromClientName == std::string("trade") and msgHead.toClientName == std::string("trade"))
        {
            return true;
        }
        return false;
    }

    void msgPrint(TradeMsgHead& msgHead, json& msgBody, HandleSel* __this)
    {
        if (not isHeadBeatHead(msgHead))
        {
            __this->ROLE(PintCheck).printMsgHead(msgHead);
            INFO_LOG("msgBody is:");
            JsonPrint(msgBody);
        }
        else
        {
            static int heartBeatCnt = 0;
            if (heartBeatCnt >= 120)
            {
                INFO_LOG("msgBody is:");
                JsonPrint(msgBody);
                heartBeatCnt = 0;
                return;
            }
            heartBeatCnt += 1;
        }
    }
}

void HandleSel::strategyHandle(const json& msgBody, void* __this)
{

    DEBUG_LOG("strategyHandle reach");
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

void HandleSel::tradeHandle(const json& msgBody, void* __this)
{
    if (msgBody.count("status") != 0 and msgBody["status"].get<std::string>() == std::string("alive"))
    {
        auto& timerPool = TimeoutTimerPool::getInstance();
        if (not timerPool.isTimerExist(ROUTE_HEADBEAT_TIMER))
        {
            auto timerOutFunc = [__this]() {
                SocketClient* socketPart = (SocketClient*)__this;
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
    TradeMsgHead msgHead{0};
    while(true)
    {
        if(!ROLE(SocketClient).isRouterConnected)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        json msgBody;
        if(!getMsg(ROLE(SocketClient).newSocket,msgHead,msgBody))
        {
            continue;
        }
        msgPrint(msgHead, msgBody,this);
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

        switch (ClientTypeMap[msgHead.fromClientName])
        {
            case ClientType::Strategy:
            {

                if(!ROLE(LogInPart).isLogIN)
                {
                    ERROR_LOG("%s","ctp not login, continue for next msg!");
                    ROLE(TradePart).sendResult(InsertResult::Failed);
                    break;
                }
                std::thread strategyThread(strategyHandle,msgBody, (void*)&ROLE(TradePart));
                strategyThread.detach();
                break;
            }
            case ClientType::Route:
            {
                std::thread routeThread(routeHandle, msgBody, (void*)&ROLE(SocketClient));
                routeThread.detach();
                break;
            }
            case ClientType::Market:
            {
                std::thread marketThread(marketHandle,msgBody,(void*)&ROLE(Query));
                marketThread.detach();
                break;
            }
            case ClientType::Trade:
            {
                std::thread tradeThread(tradeHandle, msgBody, (void*)&ROLE(SocketClient));
                tradeThread.detach();
                break;
            }
            case ClientType::Gui:
            {
                break;
            }
            default:
            {
                ERROR_LOG("the fromClientName  is unknow");
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
        return false;
    }
    msgBody = json::parse(string(bodyMsg));
    delete[] bodyMsg;
    return true;
}

bool HandleSel::getMsg(int sockfd, TradeMsgHead& msgHead, json& msgBody)
{
    if(!parseMsgHead(sockfd,msgHead))
    {
        return false;
    }
    if(!getMsgBody(msgBody, msgHead))
    {
        return false;
    }
    return true;
}



