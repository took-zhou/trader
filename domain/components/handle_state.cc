#include "trader/domain/components/handle_state.h"
#include "common/self/protobuf/strategy-market.pb.h"

#include "trader/domain/trader_service.h"

HandleState::HandleState() {}

void HandleState::HandleEvent(void) {
  auto &trader_ser = TraderService::GetInstance();

  SubTimeState now_sub_time_state = trader_ser.ROLE(TraderTimeState).GetSubTimeState();

  if (prev_sub_time_state_ != now_sub_time_state && trader_ser.GetLoginState() == kLoginState) {
    HandleStateChange();
    prev_sub_time_state_ = now_sub_time_state;
  }
}

void HandleState::HandleStateChange(void) {
  auto &trader_ser = TraderService::GetInstance();
  char time_buff[24], date_buff[10];
  auto timenow = trader_ser.ROLE(TraderTimeState).GetTimeNow();
  strftime(time_buff, sizeof(time_buff), "%Y-%m-%d %H:%M:%S", timenow);
  trader_ser.ROLE(PythonApi).GetTickNature().GetTradeDate().GetLoginDate(time_buff, date_buff);
  trder_date_ = date_buff;

  if (trader_ser.ROLE(TraderTimeState).GetSubTimeState() == kInNightLogin ||
      trader_ser.ROLE(TraderTimeState).GetSubTimeState() == kInDayLogin) {
    trader_ser.ROLE(AccountAssign).HandleTraderOpen();
  } else if (trader_ser.ROLE(TraderTimeState).GetSubTimeState() == kInDayLogout) {
    trader_ser.ROLE(OrderLookup).HandleTraderClose();
    trader_ser.ROLE(AccountAssign).HandleTraderClose();
  }
}

std::string &HandleState::GetTraderDate() { return trder_date_; }
