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
#include "trader/infra/recer_sender.h"

TraderService::TraderService() {
  auto &json_cfg = utils::JsonConfig::GetInstance();
  auto api_type = json_cfg.GetConfig("common", "ApiType");

  InitDatabase();
  if (api_type == "ftp") {
    FastBackTask();
  } else {
    RealTimeTask();
  }
};

void TraderService::FastBackTask() {
  auto trader_period_task = [&]() {
    uint32_t period_count = 0;
    while (1) {
      // trader_period_task begin
      FastBackLoginLogoutChange();
      if (period_count % 1 == 0) {
        ROLE(AccountAssign).ReqAccountStatus();
        FdManage::GetInstance().OpenThingsUp();
      }
      // trader_period_task end
      std::this_thread::sleep_for(std::chrono::seconds(1));
      period_count++;
    }
  };
  std::thread(trader_period_task).detach();
  INFO_LOG("trader period task prepare ok");
}

void TraderService::RealTimeTask() {
  auto trader_period_task = [&]() {
    uint32_t period_count = 0;
    while (1) {
      // trader_period_task begin
      ROLE(TraderTimeState).Update();
      ROLE(HandleState).HandleEvent();
      if (period_count % 10 == 0) {
        ROLE(Diagnostic).MonitorStatus();
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

bool TraderService::RealTimeLoginLogoutChange() {
  if (login_state_ == kLogoutState) {
    HandleLogoutState();
  } else if (login_state_ == kErrorState) {
    HandleErrorState();
  } else if (login_state_ == kLoginState) {
    HandleLoginState();
  } else if (login_state_ == kLossConnection) {
    HandleLossConnection();
  }

  return 0;
}

bool TraderService::HandleErrorState() {
  auto &recer_sender = RecerSender::GetInstance();
  if (ROLE(TraderTimeState).GetTimeState() == kLoginTime) {
    if (try_login_heartbeat_++ % 600 == 599 && try_login_count_++ <= 3 && recer_sender.ROLE(Sender).ROLE(ItpSender).ReqUserLogin()) {
      recer_sender.ROLE(Sender).ROLE(ItpSender).ReqAvailableFunds();
      UpdateLoginState(kLoginState);
    }
  } else if (ROLE(TraderTimeState).GetTimeState() == kLogoutTime) {
    recer_sender.ROLE(Sender).ROLE(ItpSender).ReqUserLogout();
    UpdateLoginState(kLogoutState);
  }
  return true;
}

bool TraderService::HandleLoginState() {
  auto &recer_sender = RecerSender::GetInstance();
  if (ROLE(TraderTimeState).GetTimeState() == kLogoutTime) {
    recer_sender.ROLE(Sender).ROLE(ItpSender).ReqUserLogout();
    UpdateLoginState(kLogoutState);
  } else if (recer_sender.ROLE(Sender).ROLE(ItpSender).LossConnection()) {
    UpdateLoginState(kLossConnection);
  }
  return true;
}

bool TraderService::HandleLogoutState() {
  auto &recer_sender = RecerSender::GetInstance();
  if (ROLE(TraderTimeState).GetTimeState() == kLoginTime) {
    try_login_heartbeat_ = 0;
    try_login_count_ = 0;
    if (recer_sender.ROLE(Sender).ROLE(ItpSender).ReqUserLogin()) {
      recer_sender.ROLE(Sender).ROLE(ItpSender).ReqAvailableFunds();
      UpdateLoginState(kLoginState);
    } else {
      UpdateLoginState(kErrorState);
    }
  }
  return true;
}

bool TraderService::HandleLossConnection() {
  auto &recer_sender = RecerSender::GetInstance();
  if (ROLE(TraderTimeState).GetTimeState() == kLoginTime) {
    if (try_login_count_++ <= 60) {
      if (recer_sender.ROLE(Sender).ROLE(ItpSender).ReqUserLogin()) {
        recer_sender.ROLE(Sender).ROLE(ItpSender).ReqAvailableFunds();
        UpdateLoginState(kLoginState);
      }
    }
  } else if (ROLE(TraderTimeState).GetTimeState() == kLogoutTime) {
    recer_sender.ROLE(Sender).ROLE(ItpSender).ReqUserLogout();
    UpdateLoginState(kLogoutState);
  }
  return true;
}

bool TraderService::FastBackLoginLogoutChange() { return 0; }

void TraderService::InitDatabase() {
  char *error_msg = nullptr;
  const char *sql = "create table if not exists service_info(compile_time TEXT, login_state INT);";
  if (sqlite3_exec(FdManage::GetInstance().GetTraderConn(), sql, NULL, NULL, &error_msg) != SQLITE_OK) {
    ERROR_LOG("Sql error %s.", error_msg);
    sqlite3_free(error_msg);
    sqlite3_close(FdManage::GetInstance().GetTraderConn());
  }

  sql = "insert into service_info(compile_time, login_state) select '', 3 where not exists (select * from service_info);";
  if (sqlite3_exec(FdManage::GetInstance().GetTraderConn(), sql, NULL, NULL, &error_msg) != SQLITE_OK) {
    ERROR_LOG("Sql error %s.", error_msg);
    sqlite3_free(error_msg);
    sqlite3_close(FdManage::GetInstance().GetTraderConn());
  }
}

bool TraderService::UpdateLoginState(TraderLoginState state) {
  char *error_msg = nullptr;
  char sql[100] = {0};

  login_state_ = state;
  sprintf(sql, "update service_info set compile_time='%s', login_state=%d;", utils::GetCompileTime().c_str(), login_state_);
  if (sqlite3_exec(FdManage::GetInstance().GetTraderConn(), sql, NULL, NULL, &error_msg) != SQLITE_OK) {
    ERROR_LOG("Sql error %s.", error_msg);
    sqlite3_free(error_msg);
    sqlite3_close(FdManage::GetInstance().GetTraderConn());
  }
  return 0;
}

TraderLoginState TraderService::GetLoginState() { return login_state_; }
