/*
 * traderEvent.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INTERFACE_TRADEREVENT_H_
#define WORKSPACE_TRADER_INTERFACE_TRADEREVENT_H_

#include "trader/interface/ctpEvent/ctpEvent.h"
#include "trader/interface/ctpviewEvent/ctpviewEvent.h"
#include "trader/interface/manageEvent/manageEvent.h"
#include "trader/interface/marketEvent/marketEvent.h"
#include "trader/interface/strategyEvent/strategyEvent.h"

#include <functional>
#include <map>
#include <string>
#include "common/self/dci/Role.h"
struct MsgStruct;
struct TraderEvent : CtpEvent, StrategyEvent, MarketEvent, ManageEvent, CtpviewEvent {
  TraderEvent(){};
  TraderEvent(const TraderEvent &) = delete;
  TraderEvent &operator=(const TraderEvent &) = delete;
  static TraderEvent &getInstance() {
    static TraderEvent instance;
    return instance;
  }
  bool init();
  bool run();
  void regSessionFunc();
  IMPL_ROLE(CtpEvent);
  IMPL_ROLE(StrategyEvent);
  IMPL_ROLE(MarketEvent);
  IMPL_ROLE(ManageEvent);
  IMPL_ROLE(CtpviewEvent);

  std::map<std::string, std::function<void(MsgStruct msg)>> sessionFuncMap;
};

#endif /* WORKSPACE_TRADER_INTERFACE_TRADEREVENT_H_ */
