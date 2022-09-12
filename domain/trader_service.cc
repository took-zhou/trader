/*
 * traderService.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#include "trader/domain/trader_service.h"
#include <thread>
#include "common/extern/log/log.h"

TraderSevice::TraderSevice() {
  // 开启发布线程
  auto loginStateRun = [&]() { ROLE(TraderTimeState).Update(); };
  std::thread(loginStateRun).detach();

  auto traderLogInOutFuc = [&]() {
    auto &recerSender = RecerSender::getInstance();
    while (1) {
      if (ROLE(TraderTimeState).get_time_state() == kLoginTime && login_state == kLogoutState) {
        if (recerSender.ROLE(Sender).ROLE(ItpSender).ReqUserLogin()) {
          login_state = kLoginState;
        } else {
          login_state = kErrorState;
        }
      } else if (ROLE(TraderTimeState).get_time_state() == kLogoutTime && login_state != kLogoutState) {
        recerSender.ROLE(Sender).ROLE(ItpSender).ReqUserLogout();
        login_state = kLogoutState;
      } else if (recerSender.ROLE(Sender).ROLE(ItpSender).LossConnection() && login_state != kLogoutState) {
        recerSender.ROLE(Sender).ROLE(ItpSender).ReqUserLogin();
      }

      std::this_thread::sleep_for(1000ms);
    }
  };
  std::thread(traderLogInOutFuc).detach();
  INFO_LOG("traderLogInOutFuc prepare ok");
};