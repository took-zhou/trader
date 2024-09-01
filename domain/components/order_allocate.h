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
  std::vector<std::shared_ptr<utils::OrderContent>> &GetOderList();

 private:
  bool BuildOrderContent(utils::OrderContent &content);
  bool OpenOrder(utils::OrderContent &content);
  bool CycleOpenOrder(const std::string &group_id, utils::OrderContent &content);
  bool ShareOpenOrder(const std::string &group_id, utils::OrderContent &content);
  bool CloseOrder(utils::OrderContent &content);
  bool SequenceCloseOrder(const std::string &group_id, utils::OrderContent &content);
  std::vector<std::shared_ptr<utils::OrderContent>> order_list_;
  const double minimum_account_available_ = 100.0;
};

#endif