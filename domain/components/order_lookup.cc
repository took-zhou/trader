/*
 * order.cpp
 *
 *      Author: Administrator
 */

#include "trader/domain/components/order_lookup.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include "common/extern/json/json.h"
#include "common/extern/log/log.h"

#include "common/self/file_util.h"
#include "common/self/utils.h"
#include "trader/interface/trader_event.h"

OrderLookup::OrderLookup() { ; }

OrderLookup::OrderPara *OrderLookup::GetOrderPara(const std::string &order_index) {
  auto iter = order_index_map_.find(order_index);
  if (iter != order_index_map_.end()) {
    return iter->second.get();
  } else {
    return nullptr;
  }
}

bool OrderLookup::DelOrderIndex(const std::string &order_index) {
  auto iter = order_index_map_.find(order_index);
  if (iter != order_index_map_.end()) {
    INFO_LOG("total order index map size [%d]", (int)order_index_map_.size());
    order_index_map_.erase(iter);
    INFO_LOG("del order index[%s] ok", order_index.c_str());
    INFO_LOG("total order index map size [%d]", (int)order_index_map_.size());
  } else {
    ERROR_LOG("not find order index: %s", order_index.c_str());
  }

  return true;
}

bool OrderLookup::BuildOrderIndex(const std::string &order_index, const std::shared_ptr<OrderPara> &para) {
  auto pos = order_index_map_.find(order_index);
  if (pos == order_index_map_.end()) {
    order_index_map_[order_index] = para;
  } else {
    pos->second->session_id = para->session_id;
    pos->second->order_ref = para->order_ref;
  }

  return true;
}
