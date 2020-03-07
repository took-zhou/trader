
#ifndef MSGHANDLE_H
#define MSGHANDLE_H

#include "trade_components/orderAction.h"
#include "dci/Role.h"
#include "main.h"
#include <semaphore.h>
#include "basetype.h"
#include "define.h"
#include "socket/socketClient.h"
#include "trade_components/PrintCheck.h"
#include "trade_components/query.h"
#include "trade_components/HandleSel.h"
#include "trade_components/query.h"
#include "trade_components/trade_part.h"
#include "trade_components/login_part.h"
#include "trade_components/ctpClient.h"
#include "trade_components/settlementConfirm.h"
#include "trade_components/trader_commom.h"
#include "trade_components/InfoShow.h"

struct Trader :   OrderManage
                , SocketClient
                , TradePart
                , Query
                , PintCheck
                , HandleSel
                , LogInPart
                , CtpClient
                , SettlementConfirm
                , Trader_Info
                , InfoShow
{
    Trader(){}
    Trader(const Trader&)=delete;
    Trader& operator=(const Trader&)=delete;
    static Trader& getInstance()
    {
        static Trader instance;
        return instance;
    }
    void init();
public:
    IMPL_ROLE(PintCheck);
    IMPL_ROLE(SocketClient);
    IMPL_ROLE(OrderManage);
    IMPL_ROLE(Query);
    IMPL_ROLE(TradePart);
    IMPL_ROLE(CtpClient);
    IMPL_ROLE(LogInPart);
    IMPL_ROLE(SettlementConfirm);
    IMPL_ROLE(Trader_Info);
    IMPL_ROLE(InfoShow);
};

#endif
