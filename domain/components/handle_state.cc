#include "trader/domain/components/handle_state.h"
#include "common/extern/log/log.h"
#include "common/self/protobuf/strategy-market.pb.h"
#include "common/self/semaphore.h"
#include "trader/domain/components/trader_time_state.h"
#include "trader/domain/trader_service.h"

HandleState::HandleState() {
  ;
  ;
}

void HandleState::HandleEvent(void) {
  auto &market_ser = TraderService::GetInstance();
  static SubTimeState prev_sub_time_state = kInInitSts;
  SubTimeState now_sub_time_state = market_ser.ROLE(TraderTimeState).GetSubTimeState();

  if (prev_sub_time_state != now_sub_time_state && market_ser.login_state == kLoginState) {
    HandleStateChange();
    prev_sub_time_state = now_sub_time_state;
  }
}

void HandleState::HandleStateChange(void) {
  char date_buff[10];
  GetTradeData(date_buff);
  trder_date = date_buff;
  auto &trader_ser = TraderService::GetInstance();

  if (trader_ser.ROLE(TraderTimeState).GetSubTimeState() == kInNightLogin ||
      trader_ser.ROLE(TraderTimeState).GetSubTimeState() == kInDayLogin) {
    trader_ser.ROLE(AccountAssign).HandleTraderOpen();
  } else if (trader_ser.ROLE(TraderTimeState).GetSubTimeState() == kInDayLogout) {
    trader_ser.ROLE(OrderLookup).HandleTraderClose();
    trader_ser.ROLE(AccountAssign).HandleTraderClose();
  }
}

void HandleState::HandleStateChange(const std::string &date, uint8_t state) {
  auto &trader_ser = TraderService::GetInstance();
  trder_date = date;

  if (trader_ser.ROLE(TraderTimeState).GetSubTimeState() == kInNightLogin ||
      trader_ser.ROLE(TraderTimeState).GetSubTimeState() == kInDayLogin) {
    trader_ser.ROLE(AccountAssign).HandleTraderOpen();
  } else if (trader_ser.ROLE(TraderTimeState).GetSubTimeState() == kInDayLogout) {
    trader_ser.ROLE(OrderLookup).HandleTraderClose();
    trader_ser.ROLE(AccountAssign).HandleTraderClose();
  }
}

int HandleState::IsLeapYear(int year) {
  if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
    return 1;
  } else {
    return 0;
  }
}

void HandleState::GetTradeData(char *buff) {
  int year, mon, day, mon_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  auto &trader_ser = TraderService::GetInstance();
  auto timenow = trader_ser.ROLE(TraderTimeState).GetTimeNow();
  if (timenow != nullptr) {
    year = 1900 + timenow->tm_year;
    mon = 1 + timenow->tm_mon;
    day = timenow->tm_mday;

    if (IsLeapYear(year) == 1) {
      mon_days[1] = 29;
    }

    if (19 <= timenow->tm_hour && timenow->tm_hour <= 23) {
      if (timenow->tm_wday == 5) {
        day += 3;
        while (day > mon_days[mon - 1]) {
          day -= mon_days[mon - 1];
          mon++;
          if (mon > 12) {
            mon = 1;
            year++;
            if (IsLeapYear(year) == 1) {
              mon_days[1] = 29;
            } else {
              mon_days[1] = 28;
            }
          }
        }
        sprintf(buff, "%04d%02d%02d", year, mon, day);
      } else {
        day += 1;
        while (day > mon_days[mon - 1]) {
          day -= mon_days[mon - 1];
          mon++;
          if (mon > 12) {
            mon = 1;
            year++;
            if (IsLeapYear(year) == 1) {
              mon_days[1] = 29;
            } else {
              mon_days[1] = 28;
            }
          }
        }
        sprintf(buff, "%04d%02d%02d", year, mon, day);
      }
    } else if (1 <= timenow->tm_hour && timenow->tm_hour <= 3 && timenow->tm_wday == 6) {
      day += 2;
      while (day > mon_days[mon - 1]) {
        day -= mon_days[mon - 1];
        mon++;
        if (mon > 12) {
          mon = 1;
          year++;
          if (IsLeapYear(year) == 1) {
            mon_days[1] = 29;
          } else {
            mon_days[1] = 28;
          }
        }
      }
      sprintf(buff, "%04d%02d%02d", year, mon, day);
    } else {
      sprintf(buff, "%04d%02d%02d", year, mon, day);
    }
  }
}