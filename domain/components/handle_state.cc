#include "trader/domain/components/handle_state.h"
#include "common/extern/log/log.h"
#include "common/self/protobuf/strategy-market.pb.h"
#include "common/self/semaphore.h"
#include "trader/domain/components/trader_time_state.h"
#include "trader/domain/trader_service.h"

HandleState::HandleState() {}

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

void HandleState::GetTradeData(char *buff) {
  auto &trader_ser = TraderService::GetInstance();
  auto timenow = trader_ser.ROLE(TraderTimeState).GetTimeNow();
  if (timenow != nullptr) {
    if (19 <= timenow->tm_hour && timenow->tm_hour <= 23) {
      if (timenow->tm_wday == 5) {
        time_t tsecond = mktime(timenow) + 259200;
        strftime(buff, 10, "%Y%m%d", localtime(&tsecond));
      } else {
        time_t tsecond = mktime(timenow) + 86400;
        strftime(buff, 10, "%Y%m%d", localtime(&tsecond));
      }
    } else if (1 <= timenow->tm_hour && timenow->tm_hour <= 3 && timenow->tm_wday == 6) {
      time_t tsecond = mktime(timenow) + 172800;
      strftime(buff, 10, "%Y%m%d", localtime(&tsecond));
    } else {
      strftime(buff, 10, "%Y%m%d", timenow);
    }
  }
}