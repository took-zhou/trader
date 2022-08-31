/*
 * order.cpp
 *
 *      Author: Administrator
 */

#include "trader/domain/components/orderManage.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include "common/extern/json/json.h"
#include "common/extern/log/log.h"
#include "common/self/basetype.h"
#include "common/self/fileUtil.h"
#include "common/self/utils.h"
#include "trader/interface/traderEvent.h"

OrderManage::OrderManage() { ; }

utils::OrderContent *OrderManage::getOrder(const std::string &orderKey) {
  auto it = orderMaps.find(orderKey);
  if (it != orderMaps.end()) {
    return it->second.get();
  } else {
    return nullptr;
  }
}

bool OrderManage::delOrder(const std::string &orderKey) {
  auto it = orderMaps.find(orderKey);
  if (it != orderMaps.end()) {
    INFO_LOG("total ordermap size [%d]", (int)orderMaps.size());
    orderMaps.erase(it);
    INFO_LOG("del order[%s] ok", orderKey.c_str());
    INFO_LOG("total ordermap size [%d]", (int)orderMaps.size());

  } else {
    ERROR_LOG("not find order in local ref is[%s]", orderKey.c_str());
  }
}

bool OrderManage::buildOrder(const std::string &orderKey, const std::shared_ptr<utils::OrderContent> &content) {
  if (orderMaps.find(orderKey) != orderMaps.end()) {
    return false;
  }

  orderMaps[orderKey] = content;
}
