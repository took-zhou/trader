/*
 * trader_service.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */
#ifndef WORKSPACE_TRADER_DOMAIN_TRADERSERVICE_H_
#define WORKSPACE_TRADER_DOMAIN_TRADERSERVICE_H_

#include <thread>
#include "common/self/dci/role.h"
#include "trader/domain/components/account_assign.h"
#include "trader/domain/components/diagnostic.h"
#include "trader/domain/components/group_assign.h"
#include "trader/domain/components/handle_state.h"
#include "trader/domain/components/order_allocate.h"
#include "trader/domain/components/order_lookup.h"
#include "trader/domain/components/order_manage.h"
#include "trader/domain/components/python_api.h"
#include "trader/domain/components/trader_time_state.h"

enum TraderLoginState { kErrorState = 0, kLoginState = 1, kLogoutState = 2, kManualExit = 3, kLossConnection = 4 };

struct TraderService : OrderManage,
                       TraderTimeState,
                       AccountAssign,
                       GroupAssign,
                       OrderLookup,
                       OrderAllocate,
                       HandleState,
                       Diagnostic,
                       PythonApi {
  TraderService();
  ~TraderService();
  TraderService(const TraderService &) = delete;
  TraderService &operator=(const TraderService &) = delete;
  static TraderService &GetInstance() {
    static TraderService instance;
    return instance;
  }

  IMPL_ROLE(OrderManage);
  IMPL_ROLE(TraderTimeState);
  IMPL_ROLE(AccountAssign);
  IMPL_ROLE(GroupAssign);
  IMPL_ROLE(OrderLookup);
  IMPL_ROLE(OrderAllocate);
  IMPL_ROLE(HandleState);
  IMPL_ROLE(Diagnostic);
  IMPL_ROLE(PythonApi);

  bool UpdateLoginState(TraderLoginState state);
  TraderLoginState GetLoginState();
  void Run();

 private:
  void FastBackTask();
  void RealTimeTask();
  bool HandleErrorState();
  bool HandleLoginState();
  bool HandleLogoutState();
  bool HandleLossConnection();
  bool RealTimeLoginLogoutChange();
  bool FastBackLoginLogoutChange();
  void InitDatabase();
  TraderLoginState login_state_ = kLogoutState;
  uint32_t try_login_heartbeat_ = 0;
  uint32_t try_login_count_ = 0;
  std::thread fast_back_thread_;
  std::thread real_time_thread_;
  std::atomic<bool> running_{false};
};

#endif /* WORKSPACE_TRADER_DOMAIN_TRADERSERVICE_H_ */
