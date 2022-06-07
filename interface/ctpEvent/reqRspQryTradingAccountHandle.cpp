/*
 * onRspQryTradingAccountHandle.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */
#include "trader/interface/ctpEvent/reqRspQryTradingAccountHandle.h"

#include <string>
#include "common/extern/ctp/inc/ThostFtdcUserApiStruct.h"
#include "common/extern/json/json.h"
#include "common/self/define.h"

using json = nlohmann::json;

void ReqRspQryTradingAccountHandle::reqQryTradingAccountHandle() {}

void ReqRspQryTradingAccountHandle::onRspQryTradingAccountHandle(CThostFtdcTradingAccountField &rsp) {}
