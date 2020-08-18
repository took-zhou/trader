#ifndef __TRADE_PART__
#define __TRADE_PART__
#include "main.h"
#include "json.h"
#include "dci/Role.h"
#include "PrintCheck.h"
#include <semaphore.h>
#include "orderAction.h"
#include "socket/socketClient.h"
#include "ctpClient.h"
#include "trade_components/trader_commom.h"
#include "coroutine.h"
#include <atomic>

using json = nlohmann::json;
enum InsertResult
{
    Success = 0,
    Failed  = 1
};

struct TradePart
{
//    TradePart();
    virtual void handleTradeMsg(const json msgBody);

    bool insertOrderByMsg(const json& msgBody);
    bool sendResult(InsertResult result, const json& msgBody);

private:
    bool sendMsgHead(TradeMsgHead& msgHead);
    bool sendMsgBody(const char* msgBody, size_t length);
    bool sendMsgBody(const json& msgBody);
    bool sendMsgBody(const string& msgBody);
    bool sendMsg(TradeMsgHead& msgHead, const json& msgBody);

    double reqFoPriceUnitFromCtp(TThostFtdcInstrumentIDType instrumentId, TThostFtdcExchangeIDType exchangeId);
    bool adjustOrderPrice(CThostFtdcInputOrderField& order, const json& recCfg);

public:
    USE_ROLE(PintCheck);
    USE_ROLE(OrderManage);
    USE_ROLE(SocketClient);
    USE_ROLE(CtpClient);
    USE_ROLE(Trader_Info);


};

#endif
