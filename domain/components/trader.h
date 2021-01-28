/*
 * trade.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_DOMAIN_TRADE_H_
#define WORKSPACE_TRADER_DOMAIN_TRADE_H_

#include <thread>

#include "trader/domain/components/ctpTradeApi/ctpTradeApi.h"
#include "trader/domain/components/settlementConfirm.h"
#include "common/self/dci/Role.h"
#include "trader/domain/components/order.h"
#include "trader/domain/components/tmpStore.h"

struct Trader : CtpTraderApi
              , SettlementConfirm
              , OrderManage
              , TmpStore
{
    bool init()
    {
        auto ctpMarketLogInOutFuc = [&](){
            INFO_LOG("ctpMarketLogInOutFuc ok");
            ROLE(CtpTraderApi).runLogInAndLogOutAlg();
        };

        INFO_LOG("ctpMarketLogInOutFuc prepare ok");
        std::thread(ctpMarketLogInOutFuc).detach();

        return true;
    }
    IMPL_ROLE(CtpTraderApi);
    IMPL_ROLE(SettlementConfirm);
    IMPL_ROLE(OrderManage);
    IMPL_ROLE(TmpStore);
};


#endif /* WORKSPACE_TRADER_DOMAIN_TRADE_H_ */
