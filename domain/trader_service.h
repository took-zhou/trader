/*
 * trader_service.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */
#ifndef WORKSPACE_TRADER_DOMAIN_TRADERSERVICE_H_
#define WORKSPACE_TRADER_DOMAIN_TRADERSERVICE_H_

#include "trader/domain/components/control_para.h"
#include "trader/domain/components/order_manage.h"
#include "trader/domain/components/pub_account_status.h"
#include "trader/domain/components/trader_time_state.h"
#include "trader/infra/recer_sender.h"

enum TraderLoginState { kErrorState = 0, kLoginState = 1, kLogoutState = 2 };

struct TraderSevice : OrderManage, TraderTimeState, PubAccountStatus, ControlPara {
  TraderSevice();
  TraderSevice(const TraderSevice &) = delete;
  TraderSevice &operator=(const TraderSevice &) = delete;
  static TraderSevice &GetInstance() {
    static TraderSevice instance;
    return instance;
  }

  IMPL_ROLE(OrderManage);
  IMPL_ROLE(TraderTimeState);
  IMPL_ROLE(PubAccountStatus);
  IMPL_ROLE(ControlPara);

  TraderLoginState login_state = kLogoutState;
};

#endif /* WORKSPACE_TRADER_DOMAIN_TRADERSERVICE_H_ */
