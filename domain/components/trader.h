/*
 * trade.h
 *
 *  Created on: 2020��8��28��
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_DOMAIN_TRADE_H_
#define WORKSPACE_TRADER_DOMAIN_TRADE_H_

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
        return true;
    }
    IMPL_ROLE(CtpTraderApi);
    IMPL_ROLE(SettlementConfirm);
    IMPL_ROLE(OrderManage);
    IMPL_ROLE(TmpStore);
};


#endif /* WORKSPACE_TRADER_DOMAIN_TRADE_H_ */
