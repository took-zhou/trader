/*
 * strategy_event.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INTERFACE_STRATEGYEVENT_H_
#define WORKSPACE_TRADER_INTERFACE_STRATEGYEVENT_H_
#include <functional>
#include <map>
#include <string>
#include "common/self/utils.h"

struct StrategyEvent {
 public:
  StrategyEvent();
  void Handle(utils::ItpMsg &msg);
  void RegMsgFun();

  void OrderInsertReqHandle(utils::ItpMsg &msg);
  void OrderCancelReqHandle(utils::ItpMsg &msg);
  void AccountStatusReqHandle(utils::ItpMsg &msg);
  void TransactionCostReqHandle(utils::ItpMsg &msg);
  void InstrumentReqHandle(utils::ItpMsg &msg);

 private:
  std::map<std::string, std::function<void(utils::ItpMsg &msg)>> msg_func_map;
};

#endif /* WORKSPACE_TRADER_INTERFACE_STRATEGYEVENT_H_ */
