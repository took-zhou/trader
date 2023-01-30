/*
 * trader_event.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INTERFACE_TRADEREVENT_H_
#define WORKSPACE_TRADER_INTERFACE_TRADEREVENT_H_

#include "trader/interface/btp_event/btp_event.h"
#include "trader/interface/ctp_event/ctp_event.h"
#include "trader/interface/ctpview_event/ctpview_event.h"
#include "trader/interface/market_event/market_event.h"
#include "trader/interface/self_event/self_event.h"
#include "trader/interface/strategy_event/strategy_event.h"
#include "trader/interface/xtp_event/xtp_event.h"

#include <functional>
#include <map>
#include <string>
#include "common/self/dci/role.h"
#include "common/self/utils.h"

struct TraderEvent : BtpEvent, CtpEvent, XtpEvent, StrategyEvent, MarketEvent, CtpviewEvent, SelfEvent {
  TraderEvent();
  TraderEvent(const TraderEvent &) = delete;
  TraderEvent &operator=(const TraderEvent &) = delete;
  static TraderEvent &GetInstance() {
    static TraderEvent instance;
    return instance;
  }

  bool Run();
  void RegSessionFunc();
  IMPL_ROLE(BtpEvent);
  IMPL_ROLE(CtpEvent);
  IMPL_ROLE(XtpEvent);
  IMPL_ROLE(StrategyEvent);
  IMPL_ROLE(MarketEvent);
  IMPL_ROLE(CtpviewEvent);
  IMPL_ROLE(SelfEvent);

  std::map<std::string, std::function<void(utils::ItpMsg msg)>> session_func_map;
};

#endif /* WORKSPACE_TRADER_INTERFACE_TRADEREVENT_H_ */
