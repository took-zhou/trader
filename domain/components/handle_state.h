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

  // 回测模式
  void HandleStateChange(const std::string &date, uint8_t state);

  std::string trder_date = "";

 private:
  void GetTradeData(char *);
  int IsLeapYear(int year);
};

#endif