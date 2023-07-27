#ifndef TRADER_ORDER_ALLOCATE_H
#define TRADER_ORDER_ALLOCATE_H
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "common/self/utils.h"

struct OrderAllocate {
 public:
  OrderAllocate();
  void UpdateOrderList(utils::OrderContent &content);

  std::vector<std::shared_ptr<utils::OrderContent>> order_list;

 private:
  enum AssignMode { kCycle = 0, kShare = 1 };
  AssignMode assign_mode_ = kCycle;

  const double minimum_account_available_ = 10000.0;
  bool BuildCycleOrderContent(utils::OrderContent &content);
  bool BuildShareOrderContent(utils::OrderContent &content);
  bool CycleOpenOrder(utils::OrderContent &content);
  bool ShareOpenOrder(utils::OrderContent &content);
  bool CloseOrder(utils::OrderContent &content);
};

#endif