/*
 * traderEvent.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INTERFACE_TRADEREVENT_H_
#define WORKSPACE_TRADER_INTERFACE_TRADEREVENT_H_

#include "trader/interface/strategyEvent/strategyEvent.h"
#include "trader/interface/marketEvent/marketEvent.h"
#include "trader/interface/interactorEvent/interactorEvent.h"
#include "trader/interface/ctpEvent/ctpEvent.h"
#include "trader/interface/selfEvent/selfEvent.h"

#include "common/self/dci/Role.h"
#include <map>
#include <functional>
#include <string>
struct MsgStruct;
struct TraderEvent: CtpEvent
                  , StrategyEvent
                  , MarketEvent
                  , InteractEvent
                  , SelfEvent
{
    TraderEvent(){};
    TraderEvent(const TraderEvent&) = delete;
    TraderEvent& operator=(const TraderEvent&) = delete;
    static TraderEvent& getInstance()
    {
        static TraderEvent instance;
        return instance;
    }
    bool init();
    bool run();
    void regSessionFunc();
    IMPL_ROLE(CtpEvent);
    IMPL_ROLE(StrategyEvent);
    IMPL_ROLE(MarketEvent);
    IMPL_ROLE(InteractEvent);
    IMPL_ROLE(SelfEvent);

    std::map<std::string, std::function<void(MsgStruct msg)>> sessionFuncMap;
};



#endif /* WORKSPACE_TRADER_INTERFACE_TRADEREVENT_H_ */
