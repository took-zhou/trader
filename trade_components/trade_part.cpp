#include "trade_part.h"
#include "log.h"
#include "define.h"
#include <unistd.h>
#include "OrderRef.h"
#include "semaphorePart.h"
#include "orderAction.h"
extern GlobalSem globalSem;
extern FillFlag fillFlag;
extern CThostFtdcInstrumentField InstrumentInfo;
namespace
{
    void msgHeadShow(const TradeMsgHead& msgHead)
    {
        json jhead;
        jhead["dataTypeId"] = msgHead.dataTypeId;
        jhead["fromClientName"] = msgHead.fromClientName;
        jhead["toClientName"] = msgHead.toClientName;
        jhead["length"] = msgHead.length;
        JsonPrint(jhead);
    }

    void msgBodyShow(const json& recCfg)
    {
        JsonPrint(recCfg);
    }

}

InsertResult repResult;
bool isInsertOrderSuccess()
{
    if(repResult == InsertResult::Success)
    {
        return true;
    }
    return false;
}

bool TradePart::adjustOrderPrice(CThostFtdcInputOrderField& order, const json& recCfg)
{
    if (order.Direction == BUY_)
    {
        double limitPriceBuy = recCfg["limitPriceBuy"].get<double>();
//        double priceUnit = recCfg["priceUnit"].get<double>();
        double priceUnit = reqFoPriceUnitFromCtp(order.InstrumentID, order.ExchangeID);
        double price = order.LimitPrice + priceUnit;
        if((price - limitPriceBuy) > 1e-5)
        {
            WARNING_LOG("price has exceed limitPriceBuy"); // @suppress("Invalid arguments")
            return false;
        }
        order.LimitPrice = price;
        string orderRef = genOrderRef();
        strcpy(order.OrderRef, orderRef.c_str());
        WARNING_LOG("adjust buy price %f",order.LimitPrice);
        return true;
    }
    else if(order.Direction == SELL_)
    {
        double limitPriceSell = recCfg["limitPriceSell"].get<double>();
//        double priceUnit = recCfg["priceUnit"].get<double>();
        double priceUnit = reqFoPriceUnitFromCtp(order.InstrumentID, order.ExchangeID);
        double price = order.LimitPrice - priceUnit;
        if((limitPriceSell - price) > 1e-5)
        {
            WARNING_LOG("price has lower than limitPriceSell"); // @suppress("Invalid arguments")
            return false;
        }
        order.LimitPrice = price;
        string orderRef = genOrderRef();
        strcpy(order.OrderRef, orderRef.c_str());
        WARNING_LOG("adjust sell price %f",order.LimitPrice);
        return true;
    }
    else
    {
        return false;
    }
}
bool TradePart::insertOrderByMsg(const int socketfd, const TradeMsgHead& msgHead)
{
    char *bodyMsg = new char[msgHead.length];
    std::memset(bodyMsg,0,msgHead.length);
    if(!parseMsgBody(socketfd,bodyMsg,msgHead.length))
    {
       ERROR_LOG("parse body msg error!"); // @suppress("Invalid arguments")
       return false;
    }
    INFO_LOG("parse body msg to char buff ok!"); // @suppress("Invalid arguments")
    json recCfg;
    if(!ROLE(OrderManage).fillOrderByJsonString(bodyMsg,recCfg))
    {
        delete[] bodyMsg;
        ERROR_LOG("fillOrderByJsonString failed!"); // @suppress("Invalid arguments")
        return false;
    }
    delete[] bodyMsg;
    INFO_LOG("parse body msg to json format ok!"); // @suppress("Invalid arguments")
    auto& pTraderApi= ROLE(CtpClient).sh;
    while(true)
    {
        auto& RequestID = ROLE(Trader_Info).RequestID;
        pTraderApi.ReqOrderInsert_Ordinary_hai(ROLE(OrderManage).order1,RequestID);
        RequestID++;
        sem_wait(&globalSem.sem); // @suppress("Function cannot be resolved")

        if(isInsertOrderSuccess())
        {
            break;
        }
        if(!adjustOrderPrice(ROLE(OrderManage).order1, recCfg))
        {
            ERROR_LOG("indert order1 failed!"); // @suppress("Invalid arguments")
            return false;
        }
    }
    INFO_LOG("order1 insert success!"); // @suppress("Invalid arguments")

    while(true)
    {   auto& RequestID = ROLE(Trader_Info).RequestID;
        pTraderApi.ReqOrderInsert_Ordinary_hai(ROLE(OrderManage).order2,RequestID);
        RequestID++;
        sem_wait(&globalSem.sem); // @suppress("Function cannot be resolved")
        if(isInsertOrderSuccess())
        {
            break;
        }
        if(!adjustOrderPrice(ROLE(OrderManage).order2,recCfg))
        {
            ERROR_LOG("indert order2 failed!"); // @suppress("Invalid arguments")
            return false;
        }
    }

    INFO_LOG("order2 insert success!"); // @suppress("Invalid arguments")
    return true;
}

void TradePart::handleTradeMsg(const TradeMsgHead& msgHead)
{
    INFO_LOG("begin to handle trade Msg"); // @suppress("Invalid arguments")
    int sockfd = ROLE(SocketClient).newSocket;

    if (insertOrderByMsg(sockfd, msgHead))
    {
        sendResult(InsertResult::Success);
        INFO_LOG("order pair insert success!"); // @suppress("Invalid arguments")
        return;
    }

    sendResult(InsertResult::Failed);
    ERROR_LOG("order pair insert failed!"); // @suppress("Invalid arguments")
    return;
}

bool TradePart::sendMsgHead(TradeMsgHead& msgHead)
{
    if(write(ROLE(SocketClient).newSocket, &msgHead, sizeof(TradeMsgHead)) < 0)
    {
        ERROR_LOG("send Msg head error!"); // @suppress("Invalid arguments")
        return false;
    }
    return true;
}
bool TradePart::sendMsgBody(const char* msgBody, size_t length)
{
    if(write(ROLE(SocketClient).newSocket, msgBody, length) < 0)
    {
        ERROR_LOG("send Msg Body error!"); // @suppress("Invalid arguments")
        return false;
    }
    return true;
}

bool TradePart::sendMsgBody(const json& msgBody)
{
    string msgBodyString = msgBody.dump() + '\0';
    const char* msgBodyStr = msgBodyString.c_str();
    if(write(ROLE(SocketClient).newSocket, msgBodyStr,msgBodyString.length()) < 0)
    {
        ERROR_LOG("send Msg Body error!"); // @suppress("Invalid arguments")
        return false;
    }
    return true;
}

bool TradePart::sendMsgBody(const string& msgBody)
{
    const char* msgBodyStr = msgBody.c_str();
    if(write(ROLE(SocketClient).newSocket, msgBodyStr,msgBody.length()) < 0)
    {
        ERROR_LOG("send Msg Body error!"); // @suppress("Invalid arguments")
        return false;
    }
    return true;
}

bool TradePart::sendMsg(TradeMsgHead& msgHead, const json& msgBody)
{
    msgHead.length = (msgBody.dump() + '\0').length();
    if(!sendMsgHead(msgHead))
    {
        return false;
    }

    if(!sendMsgBody(msgBody))
    {
        return false;
    }

    return true;
}

bool TradePart::sendResult(InsertResult result)
{

    json rspResult;
    rspResult["result"] = result;

    TradeMsgHead rspMsgHead{0};
    rspMsgHead.dataTypeId = (unsigned short)(ModuleName::TRADE_ORDER_STATUS_ID);
    strcpy(rspMsgHead.fromClientName , TRADENAME);
    strcpy(rspMsgHead.toClientName , STRATEGYNAME);

    if(! sendMsg(rspMsgHead, rspResult))
    {
        return false;
    }
    return true;
}

bool TradePart::handleMsgFromJsonFile()
{
    if (!ROLE(OrderManage).fillOrderByJsonFile())
    {
        printf("fill order error!\n");
        return false;
    }
    printf("fill order success!\n");
    auto& pTraderApi= ROLE(CtpClient).sh;
    auto& RequestID = ROLE(Trader_Info).RequestID;
    pTraderApi.ReqOrderInsert_Ordinary_hai(ROLE(OrderManage).order, RequestID);
    sem_wait(&globalSem.sem); // @suppress("Function cannot be resolved")
    RequestID++;
    return true;
}


bool TradePart::parseMsgBody(int sockfd, char* msg, const unsigned short length)
{
    if(msg == nullptr)
    {
        ERROR_LOG("the pointer msg is nullptr!"); // @suppress("Invalid arguments")
        return false;
    }
    int n;
    n = recv(sockfd, msg, length, 0);
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
    return true;
}

double TradePart::reqFoPriceUnitFromCtp(TThostFtdcInstrumentIDType instrumentId, TThostFtdcExchangeIDType exchangeId)
{
    CThostFtdcQryInstrumentField reqBody = { 0 };
    std::strcpy(reqBody.InstrumentID, instrumentId);
    std::strcpy(reqBody.ExchangeID, exchangeId);
    fillFlag.SingleInstrumentQuery = true;
    ROLE(CtpClient).sh.ReqQryInstrument_hai(reqBody);
    sem_wait(&globalSem.sem_query);
    fillFlag.SingleInstrumentQuery = false;
    return InstrumentInfo.PriceTick;
}

