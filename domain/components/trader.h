/*
 * trade.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_DOMAIN_TRADE_H_
#define WORKSPACE_TRADER_DOMAIN_TRADE_H_

#include <thread>

#include "common/self/dci/Role.h"
#include "trader/domain/components/ctpTradeApi/ctpTradeApi.h"
#include "trader/domain/components/ctpTradeApi/traderTimeState.h"
#include "trader/domain/components/order.h"
#include "trader/domain/components/settlementConfirm.h"
#include "trader/domain/components/tmpStore.h"

struct Trader : CtpTraderApi, SettlementConfirm, OrderManage, TmpStore, TraderTimeState {
  bool init() {
    // 启动登录登出状态转换线程
    auto loginStateRun = [&]() { ROLE(TraderTimeState).update(); };
    INFO_LOG("loginStateRun prepare ok");
    std::thread(loginStateRun).detach();

    auto ctpMarketLogInOutFuc = [&]() {
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
  IMPL_ROLE(TraderTimeState);
};

#endif /* WORKSPACE_TRADER_DOMAIN_TRADE_H_ */
