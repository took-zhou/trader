/*
 * InsertResult.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#include "trader/domain/components/InsertResult.h"
#include "common/extern/log/log.h"

bool InsertResult::addResultMonitor(std::string orderRef) {
  auto iter = rspResultMap.find(orderRef);
  if (iter == rspResultMap.end()) {
    rspResultMap.insert(RspResultPair(orderRef, InsertRspResult::UnKnow));
    return true;
  }
  ERROR_LOG("addResultMonitor error ref is [%s]", orderRef.c_str());
  return false;
}

void InsertResult::delResultMonitor(std::string orderRef) {
  auto it = rspResultMap.find(orderRef);
  if (it != rspResultMap.end()) {
    rspResultMap.erase(it);
    INFO_LOG("del ResultMonitor[%s] ok", orderRef.c_str());
  }
}

bool InsertResult::setResultState(std::string orderRef, InsertRspResult result) {
  auto it = rspResultMap.find(orderRef);
  if (it != rspResultMap.end()) {
    it->second = result;
    INFO_LOG("change state of [%s]  to [%d]", orderRef.c_str(), (int)result);
    return true;
  }
  ERROR_LOG("can not find [%s] in result monitor", orderRef.c_str());
  return false;
}

InsertRspResult InsertResult::getRspMonitorResult(std::string orderRef) {
  auto iter = rspResultMap.find(orderRef);
  if (iter == rspResultMap.end()) {
    ERROR_LOG("getRspMonitorResult error ref is [%s]", orderRef.c_str());
    return InsertRspResult::UnKnow;
  }
  return iter->second;
}
