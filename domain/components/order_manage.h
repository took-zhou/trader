/*
 * order.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_DOMAIN_COMPONENTS_ORDER_H_
#define WORKSPACE_TRADER_DOMAIN_COMPONENTS_ORDER_H_
#include <map>
#include <string>
#include "common/extern/ctp/inc/ThostFtdcUserApiStruct.h"

#include "common/self/dci/role.h"
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/utils.h"

struct OrderManage {
 public:
  OrderManage();

  bool BuildOrder(const std::string &order_key, const std::shared_ptr<utils::OrderContent> &content);
  bool DelOrder(const std::string &order_key);

  utils::OrderContent *GetOrder(const std::string &order_key);

 private:
  std::map<std::string, std::shared_ptr<utils::OrderContent>> order_map_;
};

#endif /* WORKSPACE_TRADER_DOMAIN_COMPONENTS_ORDER_H_ */
