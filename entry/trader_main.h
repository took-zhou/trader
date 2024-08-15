#ifndef TRADER_MAIN_H
#define TRADER_MAIN_H
#include <string>

struct TraderMain {
  TraderMain(){};
  TraderMain(const TraderMain &) = delete;
  TraderMain &operator=(const TraderMain &) = delete;
  static TraderMain &GetInstance() {
    static TraderMain instance;
    return instance;
  }

  void Entry(int argc, char *argv[]);
  void HoldOn(void);
  void Exit(void);
  const std::string &GetTraderName();

 private:
  std::string trader_name_ = "trader";
  bool is_hold_on_ = true;
};

#endif
