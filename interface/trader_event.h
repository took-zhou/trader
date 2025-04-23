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
#include "trader/interface/ftp_event/ftp_event.h"
#include "trader/interface/gtp_event/gtp_event.h"
#include "trader/interface/market_event/market_event.h"
#include "trader/interface/mtp_event/mtp_event.h"
#include "trader/interface/otp_event/otp_event.h"
#include "trader/interface/self_event/self_event.h"
#include "trader/interface/strategy_event/strategy_event.h"
#include "trader/interface/xtp_event/xtp_event.h"
#include "trader/interface/ytp_event/ytp_event.h"

#include <atomic>
#include <functional>
#include <map>
#include <string>
#include <thread>
#include "common/self/dci/role.h"

struct TraderEvent : BtpEvent,
                     CtpEvent,
                     OtpEvent,
                     XtpEvent,
                     FtpEvent,
                     GtpEvent,
                     MtpEvent,
                     YtpEvent,
                     StrategyEvent,
                     MarketEvent,
                     CtpviewEvent,
                     SelfEvent {
 public:
  TraderEvent();
  ~TraderEvent();
  TraderEvent(const TraderEvent &) = delete;
  TraderEvent &operator=(const TraderEvent &) = delete;
  static TraderEvent &GetInstance() {
    static TraderEvent instance;
    return instance;
  }

  bool Run();
  bool Stop();
  void RegSessionFunc();
  IMPL_ROLE(BtpEvent);
  IMPL_ROLE(CtpEvent);
  IMPL_ROLE(OtpEvent);
  IMPL_ROLE(XtpEvent);
  IMPL_ROLE(FtpEvent);
  IMPL_ROLE(GtpEvent);
  IMPL_ROLE(MtpEvent);
  IMPL_ROLE(YtpEvent);
  IMPL_ROLE(StrategyEvent);
  IMPL_ROLE(MarketEvent);
  IMPL_ROLE(CtpviewEvent);
  IMPL_ROLE(SelfEvent);

 private:
  void OrderRecTask();
  void QueryRecTask();
  void ItpRecTask();
  std::map<std::string, std::function<void(utils::ItpMsg msg)>> session_func_map_;
  std::thread order_rec_thread_;
  std::thread query_rec_thread_;
  std::thread itp_rec_thread_;
  std::atomic<bool> running_{false};
  uint32_t delay_ms_ = 1000;
};

#endif /* WORKSPACE_TRADER_INTERFACE_TRADEREVENT_H_ */
