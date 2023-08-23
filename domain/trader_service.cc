/*
 * traderService.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#include "trader/domain/trader_service.h"
#include <thread>
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "trader/domain/components/fd_manage.h"

TraderService::TraderService() {
  auto &json_cfg = utils::JsonConfig::GetInstance();
  auto api_type = json_cfg.GetConfig("common", "ApiType");
  if (api_type == "ftp") {
    auto trader_period_task = [&]() {
      uint32_t period_count = 0;
      while (1) {
        // trader_period_task begin
        if (period_count % 10 == 0) {
          ROLE(AccountAssign).ReqAccountStatus();
          FdManage::GetInstance().OpenThingsUp();
          FastBackLoginLogoutChange();
        }
        // trader_period_task end
        std::this_thread::sleep_for(std::chrono::seconds(1));
        period_count++;
      }
    };
    std::thread(trader_period_task).detach();
    INFO_LOG("trader period task prepare ok");
  } else {
    auto trader_period_task = [&]() {
      uint32_t period_count = 0;
      while (1) {
        // trader_period_task begin
        ROLE(TraderTimeState).Update();
        ROLE(HandleState).HandleEvent();
        if (period_count % 10 == 0) {
          ROLE(AccountAssign).ReqAccountStatus();
          FdManage::GetInstance().OpenThingsUp();
        }
        RealTimeLoginLogoutChange();
        // trader_period_task end

        std::this_thread::sleep_for(std::chrono::seconds(1));
        period_count++;
      }
    };
    std::thread(trader_period_task).detach();
    INFO_LOG("trader period task prepare ok");
  }
};

bool TraderService::RealTimeLoginLogoutChange() {
  auto &recer_sender = RecerSender::GetInstance();
  if (ROLE(TraderTimeState).GetTimeState() == kLoginTime && login_state == kLogoutState) {
    if (recer_sender.ROLE(Sender).ROLE(ItpSender).ReqUserLogin()) {
      login_state = kLoginState;
    } else {
      login_state = kErrorState;
      try_login_heartbeat_ = 0;
      try_login_count_ = 0;
    }
  } else if (ROLE(TraderTimeState).GetTimeState() == kLoginTime && login_state == kErrorState) {
    if (try_login_heartbeat_++ % 600 == 599 && try_login_count_++ <= 3 && recer_sender.ROLE(Sender).ROLE(ItpSender).ReqUserLogin()) {
      login_state = kLoginState;
    }
  } else if (ROLE(TraderTimeState).GetTimeState() == kLogoutTime && login_state != kLogoutState) {
    recer_sender.ROLE(Sender).ROLE(ItpSender).ReqUserLogout();
    login_state = kLogoutState;
  } else if (recer_sender.ROLE(Sender).ROLE(ItpSender).LossConnection() && login_state != kLogoutState) {
    HandleAccountExitException();
  }

  return 0;
}

bool TraderService::FastBackLoginLogoutChange() { return 0; }

bool TraderService::HandleAccountExitException() {
  bool ret = true;
  auto &recer_sender = RecerSender::GetInstance();
  recer_sender.ROLE(Sender).ROLE(ItpSender).ReqUserLogin();
  return ret;
}