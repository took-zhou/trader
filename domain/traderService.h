/*
 * traderService.h
 *
 *  Created on: 2020��8��29��
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_DOMAIN_TRADERSERVICE_H_
#define WORKSPACE_TRADER_DOMAIN_TRADERSERVICE_H_

#include "trader/domain/components/trader.h"
#include "common/self/timer.h"

struct TraderSevice : Trader
{
    TraderSevice(){};
    TraderSevice(const TraderSevice&) = delete;
    TraderSevice& operator=(const TraderSevice&) = delete;
    static TraderSevice& getInstance()
    {
        static TraderSevice instance;
        return instance;
    }

    TimeoutTimerPool& getTimeoutTimerPool();
    IMPL_ROLE(Trader);
};



#endif /* WORKSPACE_TRADER_DOMAIN_TRADERSERVICE_H_ */
