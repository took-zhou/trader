#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include "socketClient.h"
#include "log.h"
#include "getconfig.h"
#include "json.h"
#include "timer.h"

using json = nlohmann::json;

void SocketClient::init()
{
    WARNING_LOG("SocketClient init");
    newSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (newSocket < 0)
    {
        ERROR_LOG("ERROR opening socket");
    }

    memset(&serv_addr, 0, sizeof(struct sockaddr_in));

    serv_addr.sin_family = PF_INET;
    socket_ip = getConfig("trade","RouteIp");
    serv_addr.sin_addr.s_addr = inet_addr(socket_ip.c_str());
    socket_port = (unsigned short int)stoi(getConfig("trade","RoutePort"));
    serv_addr.sin_port = htons(socket_port);

    WARNING_LOG("connecting .....");
    size_t cnt{0};
    while(true)
    {
        if (connect(newSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            WARNING_LOG("connect to route server %s:%u failed, try %zu",socket_ip.c_str(),socket_port,cnt++);
            sleep(5);
            continue;
        }

        INFO_LOG("connect to route server %s:%u successful!",socket_ip.c_str(),socket_port);
        if(routeLogin())
        {
            INFO_LOG("login route successful");
            break;
        }
        ERROR_LOG("login route error!");
    }
    isRouterConnected = true;

    auto headBeatTriggerFunc = [this](){
        while (true)
        {
            if(this->isRouterConnected)
            {
                sendHeatBeatMsgToRoute();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(HEADBEAT_CHECK_PERIOD));
        }
    };
    std::thread(headBeatTriggerFunc).detach();

    //startHeadBeatCheckTimer();
}

bool SocketClient::routeLogin()
{
    json nameBody;
    nameBody["name"] = TRADENAME;
    string nameStr = nameBody.dump() + '\0';

    TradeMsgHead msgHead{0};
    msgHead.dataTypeId = (unsigned short)(ModuleName::CLIENT_NAME_ID);
    strcpy(msgHead.fromClientName , TRADENAME);
    strcpy(msgHead.toClientName , ROUTENAME);
    msgHead.length =nameStr.size();

    int n = write(newSocket, &msgHead, sizeof(TradeMsgHead));
    if (n < 0)
    {
        ERROR_LOG("ERROR writing to route for login");
        return false;
    }
    JsonPrint(nameBody);
    n = write(newSocket, nameStr.c_str(), msgHead.length);
    if (n < 0)
    {
        ERROR_LOG("ERROR writing to route for login");
        return false;
    }
    return true;
}

int SocketClient::getSocketFd()
{
    return newSocket;
}

void SocketClient::sendHeatBeatMsgToRoute()
{
    json headBeatMsg;
    headBeatMsg["status"] = "alive";

    TradeMsgHead rspMsgHead{ 0 };
    rspMsgHead.dataTypeId = (unsigned short)(ModuleName::CLIENT_HEARTBEAT_ID);
    strcpy(rspMsgHead.fromClientName, TRADENAME);
    strcpy(rspMsgHead.toClientName, TRADENAME);

    sendMsg(rspMsgHead, headBeatMsg);
}
void SocketClient::sendMsg(TradeMsgHead& msgHead, const json& msgBody)
{
    msgHead.length = (msgBody.dump() + '\0').length();
    if (!sendMsgHead(msgHead))
    {
        return;
    }

    if (!sendJsonMsg(msgBody))
    {
        return;
    }
}
bool SocketClient::sendMsgHead(TradeMsgHead& msgHead)
{
    if (write(newSocket, &msgHead, sizeof(TradeMsgHead)) < 0)
    {
        ERROR_LOG("send Msg head error!"); // @suppress("Invalid arguments")
        return false;
    }
    return true;
}


bool SocketClient::sendStringMsg(const string msg)
{
    const char* charMsg = msg.c_str();
    int n = write(newSocket,charMsg,strlen(charMsg));
    if (n < 0)
    {
        ERROR_LOG("ERROR writing to route");
        return false;
    }
    return true;
}
bool SocketClient::sendJsonMsg(const json& jsonMsg)
{
    string jsonMsgStr = jsonMsg.dump() + '\0';
    const char* charMsg = jsonMsgStr.c_str();
    int n = write(newSocket,charMsg,jsonMsgStr.length());
    if (n < 0)
    {
        ERROR_LOG("ERROR writing to route");
        return false;
    }
    return false;
}

bool SocketClient::recMsg(char* strMsg, const size_t length)
{
    int n = read(newSocket,strMsg,length);
    if (n < 0)
    {
      ERROR_LOG("ERROR writing to socket");
    }

}

bool SocketClient::recUselessMsgBody(const size_t length)
{
    char dustbin[length];
    int n = read(newSocket,dustbin,length);
    if (n < 0)
    {
      ERROR_LOG("ERROR writing to socket");
    }
}

bool SocketClient::socketClose()
{
    if(close(newSocket) < 0)
    {
        INFO_LOG("close socket ok");
        return true;
    }
    return false;
}

bool SocketClient::routerReconnect()
{
    WARNING_LOG("%s","go into routerReconnect");
    close(this->newSocket);
    this->newSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (newSocket < 0)
    {
        ERROR_LOG("ERROR opening socket");
    }

    WARNING_LOG("Re connecting .....");
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));

    serv_addr.sin_family = PF_INET;
    socket_ip = getConfig("trade","RouteIp");
    serv_addr.sin_addr.s_addr = inet_addr(socket_ip.c_str());
    socket_port = (unsigned short int)stoi(getConfig("trade","RoutePort"));
    serv_addr.sin_port = htons(socket_port);
    size_t cnt{0};
    while(true)
    {
        if (connect(this->newSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            WARNING_LOG("reconnect to route server %s:%u failed, try %zu",socket_ip.c_str(),socket_port,cnt++);
            sleep(5);
            continue;
        }

        INFO_LOG("reconnect to route server %s:%u successful!",socket_ip.c_str(),socket_port);
        if(routeLogin())
        {
            INFO_LOG("relogin route successful");
            break;
        }

     }
    INFO_LOG("%s","isRouterConnected again");
    this->isRouterConnected = true;
}

void SocketClient::startHeadBeatCheckTimer()
{
    auto& timerPool = TimeoutTimerPool::getInstance();


    if (not timerPool.isTimerExist(ROUTE_HEADBEAT_TIMER))
    {
        auto timerOutFunc = [&]() {
            this->isRouterConnected = false;
            this->routerReconnect();
        };
        timerPool.addTimer(ROUTE_HEADBEAT_TIMER, timerOutFunc, HEADBEAT_TIME_OUT_LENGTH);
    }
    timerPool.getTimerByName(ROUTE_HEADBEAT_TIMER)->start();
}

