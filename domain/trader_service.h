/*
 * trader_service.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */
#ifndef WORKSPACE_TRADER_DOMAIN_TRADERSERVICE_H_
#define WORKSPACE_TRADER_DOMAIN_TRADERSERVICE_H_

#include "trader/domain/components/account_assign.h"
#include "trader/domain/components/order_allocate.h"
#include "trader/domain/components/order_lookup.h"
#include "trader/domain/components/order_manage.h"
#include "trader/domain/components/trader_time_state.h"
#include "trader/infra/recer_sender.h"

enum TraderLoginState { kErrorState = 0, kLoginState = 1, kLogoutState = 2 };
enum TraderRunMode { kFastBack = 0, kRealTime };

struct TraderSevice : OrderManage, TraderTimeState, AccountAssign, OrderLookup, OrderAllocate {
  TraderSevice();
  TraderSevice(const TraderSevice &) = delete;
  TraderSevice &operator=(const TraderSevice &) = delete;
  static TraderSevice &GetInstance() {
    static TraderSevice instance;
    return instance;
  }

  IMPL_ROLE(OrderManage);
  IMPL_ROLE(TraderTimeState);
  IMPL_ROLE(AccountAssign);
  IMPL_ROLE(OrderLookup);
  IMPL_ROLE(OrderAllocate);

  TraderLoginState login_state = kLogoutState;
  TraderRunMode run_mode = kRealTime;

 private:
  bool RealTimeLoginLogoutChange();
  bool FastBackLoginLogoutChange();
  bool HandleAccountExitException();
  uint32_t try_login_heartbeat_ = 0;
  uint32_t try_login_count_ = 0;
};

#endif /* WORKSPACE_TRADER_DOMAIN_TRADERSERVICE_H_ */
