
#ifndef QUERY_H
#define QUERY_H

#include "dci/Role.h"
#include "json.h"
#include "trade_components/ctpClient.h"
using json = nlohmann::json;

struct TradeMsgHead;
struct SocketClient;
struct SocketHandle;
struct TradePart;
struct QueryActiveContracts
{
    bool ReqQryInstrument();

private:
    bool sendRspOnQryInstrument(const json& instrumentInfo);
public:
    USE_ROLE(SocketClient);
    USE_ROLE(TradePart);
    USE_ROLE(CtpClient);
};

struct Query:QueryActiveContracts
{
public:
    bool handleQueryMsg(const json& msgBody);
private:
    bool parseMsgBody(int sockfd, char* msg, const unsigned short length);
public:
    IMPL_ROLE(QueryActiveContracts)
};

#endif // !
