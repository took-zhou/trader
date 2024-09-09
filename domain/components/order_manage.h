/*
 * order.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_DOMAIN_COMPONENTS_ORDER_H_
#define WORKSPACE_TRADER_DOMAIN_COMPONENTS_ORDER_H_
#include <string>
#include <unordered_map>

#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/utils.h"

struct OrderManage {
 public:
  OrderManage();

  bool BuildOrder(const std::string &order_key, const std::shared_ptr<utils::OrderContent> &content);
  bool DelOrder(const std::string &order_key);

  const std::shared_ptr<utils::OrderContent> &GetOrder(const std::string &order_key);

 private:
  const std::shared_ptr<utils::OrderContent> empty_;
  std::unordered_map<std::string, std::shared_ptr<utils::OrderContent>> order_map_;
};

#endif /* WORKSPACE_TRADER_DOMAIN_COMPONENTS_ORDER_H_ */
