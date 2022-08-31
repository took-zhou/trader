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
#include "common/self/basetype.h"
#include "common/self/dci/Role.h"
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/utils.h"

struct OrderManage {
 public:
  OrderManage();

  bool buildOrder(const std::string &orderKey, const std::shared_ptr<utils::OrderContent> &content);
  utils::OrderContent *getOrder(const std::string &orderKey);
  bool delOrder(const std::string &orderKey);

 private:
  std::map<std::string, std::shared_ptr<utils::OrderContent>> orderMaps;
};

#endif /* WORKSPACE_TRADER_DOMAIN_COMPONENTS_ORDER_H_ */
