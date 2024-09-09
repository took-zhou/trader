/*
 * order.cpp
 *
 *      Author: Administrator
 */

#include "trader/domain/components/order_manage.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include "common/extern/log/log.h"

#include "common/self/utils.h"

OrderManage::OrderManage() { ; }

const std::shared_ptr<utils::OrderContent> &OrderManage::GetOrder(const std::string &order_key) {
  auto iter = order_map_.find(order_key);
  if (iter != order_map_.end()) {
    return iter->second;
  } else {
    return empty_;
  }
}

bool OrderManage::DelOrder(const std::string &order_key) {
  auto iter = order_map_.find(order_key);
  if (iter != order_map_.end()) {
    INFO_LOG("total ordermap size [%d]", (int)order_map_.size());
    order_map_.erase(iter);
    INFO_LOG("del order[%s] ok", order_key.c_str());
    INFO_LOG("total ordermap size [%d]", (int)order_map_.size());

  } else {
    ERROR_LOG("not find order in local ref is[%s]", order_key.c_str());
  }

  return true;
}

bool OrderManage::BuildOrder(const std::string &order_key, const std::shared_ptr<utils::OrderContent> &content) {
  if (order_map_.find(order_key) != order_map_.end()) {
    return false;
  }

  order_map_[order_key] = content;

  return true;
}
