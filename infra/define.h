/*
 * define.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_DEFINE_H_
#define WORKSPACE_TRADER_INFRA_DEFINE_H_

#include <string>
#include "common/extern/ctp/inc/ThostFtdcTraderApi.h"

constexpr char LIMIT_LIMIT_SUBTITLE[] = "limit_limit";
constexpr char LIMIT_FAK_SUBTITLE[] = "limit_fak";
constexpr char ORDER_BUILD_ERROR[] = "order_build_error";
constexpr char ORDER_FILL_ERROR[] = "order_fill_error";
constexpr char ORDER_CANCEL[] = "order_cancel";

enum struct EventType { Ctp_Event = 0, Market_Event = 1, Trader_Event = 2, Strategy_Event = 3, Interactor_Event = 4, INVALID = 5 };

struct MsgStruct {
  std::string sessionName{""};
  std::string msgName{""};
  std::string pbMsg{""};
  void *ctpMsg{nullptr};
  void *ctpMsgInfo{nullptr};
  bool bIsLast{false};

  bool isValid() { return sessionName != std::string("") and msgName != std::string(""); }
};

#endif /* WORKSPACE_STRATEGY_INFRA_DEFINE_H_ */
