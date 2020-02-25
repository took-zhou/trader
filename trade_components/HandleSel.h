#ifndef __HANDEL_SEL__
#define __HANDEL_SEL__

#include "dci/Role.h"
#include "PrintCheck.h"
#include "query.h"
#include "socket/socketClient.h"
#include "trade_part.h"
#include <thread>
#include "login_part.h"
struct HandleSel
{
public:
    void msgHandleSel();
    bool getMsgBody(json& msgBody, TradeMsgHead& msgHead);
private:
    bool parseMsgHead(int sockfd, TradeMsgHead& msgHead);
    static void marketHandle(const json& msgBody, void* __this);
    static void tradeHandle(const json& msgBody, void* __this);
public:
    USE_ROLE(SocketClient);
    USE_ROLE(PintCheck);
    USE_ROLE(Query);
    USE_ROLE(TradePart);
    USE_ROLE(LogInPart);
//    std::thread tradeThread;
//    std::thread marketThread;
};




#endif
