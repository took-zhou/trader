#ifndef HANDLE_TRADER_STATE_H
#define HANDLE_TRADER_STATE_H

#include <cstdint>
#include <string>
#include "trader/domain/components/trader_time_state.h"

struct HandleState {
 public:
  HandleState();
  ~HandleState(){};

  // 实时模式
  void HandleEvent(void);
  void HandleStateChange(void);
  std::string &GetTraderDate();

 private:
  void GetTradeData(char *);
  std::string trder_date_ = "";
  SubTimeState prev_sub_time_state_ = kInInitSts;
};

#endif