/*
 * order.cpp
 *
 *      Author: Administrator
 */

#include "trader/domain/components/order_manage.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include "common/extern/json/json.h"
#include "common/extern/log/log.h"

#include "common/self/file_util.h"
#include "common/self/utils.h"
#include "trader/interface/trader_event.h"

OrderManage::OrderManage() { ; }

utils::OrderContent *OrderManage::get_order(const std::string &orderKey) {
  auto it = order_map.find(orderKey);
  if (it != order_map.end()) {
    return it->second.get();
  } else {
    return nullptr;
  }
}

bool OrderManage::DelOrder(const std::string &orderKey) {
  auto it = order_map.find(orderKey);
  if (it != order_map.end()) {
    INFO_LOG("total ordermap size [%d]", (int)order_map.size());
    order_map.erase(it);
    INFO_LOG("del order[%s] ok", orderKey.c_str());
    INFO_LOG("total ordermap size [%d]", (int)order_map.size());

  } else {
    ERROR_LOG("not find order in local ref is[%s]", orderKey.c_str());
  }
}

bool OrderManage::BuildOrder(const std::string &orderKey, const std::shared_ptr<utils::OrderContent> &content) {
  if (order_map.find(orderKey) != order_map.end()) {
    return false;
  }

  order_map[orderKey] = content;
}
