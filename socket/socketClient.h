#ifndef __SOCKET_CLIENT_H
#define __SOCKET_CLIENT_H
#include<string>
#include "dci/Role.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "json.h"

using json = nlohmann::json;
using namespace std;

struct SocketClient
{
//    SocketClient();

    void init();
    int getSocketFd();
    bool sendStringMsg(const string msg);
    bool recMsg(char* strMsg, const size_t length);
    bool socketClose();
    bool routeLogin();
    bool sendJsonMsg(const json& jsonMsg);
    bool recUselessMsgBody(const size_t length);
    bool routerReconnect();
    void startHeadBeatCheckTimer();
public:
    int newSocket;
    bool isRouterConnected{false};
    string socket_ip{""};
    unsigned short int socket_port{0};
private:
    struct sockaddr_in serv_addr;
};

#endif

