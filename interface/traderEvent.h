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
#include "trader/interface/xtpEvent/xtpEvent.h"

#include <functional>
#include <map>
#include <string>
#include "common/self/dci/Role.h"
#include "common/self/utils.h"

struct TraderEvent : CtpEvent, XtpEvent, StrategyEvent, MarketEvent, ManageEvent, CtpviewEvent {
  TraderEvent();
  TraderEvent(const TraderEvent &) = delete;
  TraderEvent &operator=(const TraderEvent &) = delete;
  static TraderEvent &getInstance() {
    static TraderEvent instance;
    return instance;
  }

  bool run();
  void regSessionFunc();
  IMPL_ROLE(CtpEvent);
  IMPL_ROLE(XtpEvent);
  IMPL_ROLE(StrategyEvent);
  IMPL_ROLE(MarketEvent);
  IMPL_ROLE(ManageEvent);
  IMPL_ROLE(CtpviewEvent);

  std::map<std::string, std::function<void(utils::ItpMsg msg)>> sessionFuncMap;
};

#endif /* WORKSPACE_TRADER_INTERFACE_TRADEREVENT_H_ */
