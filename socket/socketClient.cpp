#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include "socketClient.h"
#include "log.h"
#include "getconfig.h"
#include "define.h"
#include "json.h"

using json = nlohmann::json;
SocketClient::SocketClient()
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
    close(newSocket);
    newSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (newSocket < 0)
    {
        ERROR_LOG("ERROR opening socket");
    }

//    memset(&serv_addr, 0, sizeof(struct sockaddr_in));

//    serv_addr.sin_family = PF_INET;
//    socket_ip = getConfig("trade","RouteIp");
//    serv_addr.sin_addr.s_addr = inet_addr(socket_ip.c_str());
//    socket_port = (unsigned short int)stoi(getConfig("trade","RoutePort"));
//    serv_addr.sin_port = htons(socket_port);
    WARNING_LOG("Re connecting .....");
    size_t cnt{0};
    while(true)
    {
        if (connect(newSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
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
    isRouterConnected = true;
}
