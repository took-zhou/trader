#ifndef TRADER_ORDER_INDEX_H
#define TRADER_ORDER_INDEX_H
#include <memory>
#include <string>
#include <unordered_map>

struct OrderLookup {
 public:
  struct OrderPara {
    OrderPara(uint64_t session_id, const std::string &order_ref) : session_id(session_id), order_ref(order_ref) {}
    uint64_t session_id;
    std::string order_ref;
  };

  OrderLookup();

  bool BuildOrderIndex(const std::string &order_index, const std::shared_ptr<OrderPara> &para);
  bool DelOrderIndex(const std::string &order_index);
  OrderPara *GetOrderPara(const std::string &order_index);

 private:
  std::unordered_map<std::string, std::shared_ptr<OrderPara>> order_index_map_;
};

#endif