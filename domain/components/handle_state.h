#ifndef HANDLE_TRADER_STATE_H
#define HANDLE_TRADER_STATE_H

#include <cstdint>
#include <string>

struct HandleState {
 public:
  HandleState();
  ~HandleState(){};

  // 实时模式
  void HandleEvent(void);
  void HandleStateChange(void);
  std::string &GetTraderDate();

 private:
  std::string trder_date_ = "";
  void GetTradeData(char *);
};

#endif