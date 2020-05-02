
#include <iostream>
#include <semaphore.h>
#include <unistd.h>  //read()
#include "query.h"
#include "main.h"
#include<string.h>
#include "log.h"
#include "define.h"
#include "socket/socketClient.h"
#include <cstring>
#include <unordered_map>
#include "trade_part.h"
#include <string>
#include "semaphorePart.h"
extern GlobalSem globalSem;
using namespace std;

json AllInstruments;

namespace{
    unordered_map<string, int> QueryType =
    {
            {"unknow", 0},
            {"activeInstruments", 1}
    };
}

bool Query::handleQueryMsg(const json& msgBody)
{
    INFO_LOG("parse body msg to char buff ok!"); // @suppress("Invalid arguments")
    switch(QueryType[msgBody["reqFor"].get<string>()])
    {
        case 1:
        {
            ROLE(QueryActiveContracts).ReqQryInstrument();
            break;
        }
        default:
            break;
    }
    return true;
}

bool QueryActiveContracts::ReqQryInstrument()
{
    CThostFtdcQryInstrumentField qryBody{0};

    std::strcpy(qryBody.ExchangeID, "");
    std::strcpy(qryBody.InstrumentID, "");
//    ROLE(CtpClient).pTraderApi.ReqQryInstrument_hai(qryBody);
    ROLE(CtpClient).sh.ReqQryInstrument_hai(qryBody);
    sem_wait(&globalSem.sem_query);

    AllInstruments["result"] = "0";
    if(sendRspOnQryInstrument(AllInstruments))
    {
        return true;
    }
    return false;
}

bool QueryActiveContracts::sendRspOnQryInstrument(const json& instrumentInfo)
{
    TradeMsgHead rspMsgHead{0};
    rspMsgHead.dataTypeId = (unsigned short)(ModuleName::TRADE_INSTRUMENT_ID);
    strcpy(rspMsgHead.fromClientName, TRADENAME);
    strcpy(rspMsgHead.toClientName, MARKETNAME);

    ROLE(SocketClient).sendMsg(rspMsgHead, instrumentInfo);

    return true;
}

bool Query::parseMsgBody(int sockfd, char* msg, const unsigned short length)
{
    if(msg == nullptr)
    {
        ERROR_LOG("the pointer msg is nullptr!"); // @suppress("Invalid arguments")
        return false;
    }
    int n;
    n = read(sockfd, msg, length);
    if(n < 0)
    {
        ERROR_LOG("read body msg from socket error!"); // @suppress("Invalid arguments")
        return false;
    }
    return true;
}

