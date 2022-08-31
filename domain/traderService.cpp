/*
 * traderService.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#include "trader/domain/traderService.h"
#include <thread>
#include "common/extern/log/log.h"

TraderSevice::TraderSevice() {
  // 开启发布线程
  auto loginStateRun = [&]() { ROLE(TraderTimeState).update(); };
  std::thread(loginStateRun).detach();

  auto traderLogInOutFuc = [&]() {
    auto &recerSender = RecerSender::getInstance();
    while (1) {
      if (ROLE(TraderTimeState).output.status == LOGIN_TIME && login_state == LOGOUT_STATE) {
        if (recerSender.ROLE(Sender).ROLE(ItpSender).ReqUserLogin()) {
          login_state = LOGIN_STATE;
        } else {
          login_state = ERROR_STATE;
        }
      } else if (ROLE(TraderTimeState).output.status == LOGOUT_TIME && login_state != LOGOUT_STATE) {
        recerSender.ROLE(Sender).ROLE(ItpSender).ReqUserLogout();
        login_state = LOGOUT_STATE;
      } else if (recerSender.ROLE(Sender).ROLE(ItpSender).LossConnection() && login_state != LOGOUT_STATE) {
        recerSender.ROLE(Sender).ROLE(ItpSender).ReqUserLogin();
      }

      std::this_thread::sleep_for(1000ms);
    }
  };
  std::thread(traderLogInOutFuc).detach();
  INFO_LOG("traderLogInOutFuc prepare ok");
};