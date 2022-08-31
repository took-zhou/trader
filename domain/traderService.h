/*
 * traderService.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */
#ifndef WORKSPACE_TRADER_DOMAIN_TRADERSERVICE_H_
#define WORKSPACE_TRADER_DOMAIN_TRADERSERVICE_H_

#include "trader/domain/components/orderManage.h"
#include "trader/domain/components/traderTimeState.h"
#include "trader/infra/recerSender.h"

enum TRADER_LOGIN_STATE { ERROR_STATE = 0, LOGIN_STATE = 1, LOGOUT_STATE = 2 };

struct TraderSevice : OrderManage, TraderTimeState {
  TraderSevice();
  TraderSevice(const TraderSevice &) = delete;
  TraderSevice &operator=(const TraderSevice &) = delete;
  static TraderSevice &getInstance() {
    static TraderSevice instance;
    return instance;
  }

  IMPL_ROLE(OrderManage);
  IMPL_ROLE(TraderTimeState);

  TRADER_LOGIN_STATE login_state = LOGOUT_STATE;
};

#endif /* WORKSPACE_TRADER_DOMAIN_TRADERSERVICE_H_ */
