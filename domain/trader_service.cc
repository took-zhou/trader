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

TraderSevice::TraderSevice() {
  auto &json_cfg = utils::JsonConfig::GetInstance();
  auto api_type = json_cfg.GetConfig("common", "ApiType");
  if (api_type == "btp") {
    auto trader_log_in_out_fuc = [&]() {
      auto &recer_sender = RecerSender::GetInstance();
      while (1) {
        if (login_state == kLogoutState) {
          if (recer_sender.ROLE(Sender).ROLE(ItpSender).ReqUserLogin()) {
            login_state = kLoginState;
          } else {
            login_state = kErrorState;
          }
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
    };
    std::thread(trader_log_in_out_fuc).detach();
    INFO_LOG("trader_log_in_out_fuc prepare ok");
  } else {
    // 开启发布线程
    auto login_state_run = [&]() { ROLE(TraderTimeState).Update(); };
    std::thread(login_state_run).detach();

    // 开启发布线程
    auto pub_account_status_run = [&]() { ROLE(PubAccountStatus).ReqCycle(); };
    std::thread(pub_account_status_run).detach();

    // 开启主动安全监测线程
    auto check_safety_fuc = [&]() { ROLE(ActiveSafety).CheckSafety(); };
    std::thread(check_safety_fuc).detach();

    auto trader_log_in_out_fuc = [&]() {
      auto &recer_sender = RecerSender::GetInstance();
      while (1) {
        if (ROLE(TraderTimeState).GetTimeState() == kLoginTime && login_state == kLogoutState) {
          if (recer_sender.ROLE(Sender).ROLE(ItpSender).ReqUserLogin()) {
            login_state = kLoginState;
          } else {
            login_state = kErrorState;
          }
        } else if (ROLE(TraderTimeState).GetTimeState() == kLogoutTime && login_state != kLogoutState) {
          recer_sender.ROLE(Sender).ROLE(ItpSender).ReqUserLogout();
          login_state = kLogoutState;
        } else if (recer_sender.ROLE(Sender).ROLE(ItpSender).LossConnection() && login_state != kLogoutState) {
          recer_sender.ROLE(Sender).ROLE(ItpSender).ReqUserLogin();
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
    };
    std::thread(trader_log_in_out_fuc).detach();
    INFO_LOG("trader_log_in_out_fuc prepare ok");
  }
};