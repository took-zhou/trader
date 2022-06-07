/*
 * ctpRspEvent.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */
#ifndef WORKSPACE_TRADE_CTP_EVENT_CTPRSPEVENT_H_
#define WORKSPACE_TRADE_CTP_EVENT_CTPRSPEVENT_H_
#include <string>
#include "common/self/dci/Role.h"
#include "trader/interface/ctpEvent/reqRspQryTradingAccountHandle.h"
struct CtpRspEvent : ReqRspQryTradingAccountHandle {
  CtpRspEvent();
  CtpRspEvent(const CtpRspEvent &) = delete;
  CtpRspEvent &operator=(const CtpRspEvent &) = delete;
  static CtpRspEvent &getInstance() {
    static CtpRspEvent instance;
    return instance;
  }
  IMPL_ROLE(ReqRspQryTradingAccountHandle);
};

#endif /* WORKSPACE_TRADE_CTP_EVENT_CTPRSPEVENT_H_ */
