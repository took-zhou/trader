/*
 * strategyEvent.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INTERFACE_STRATEGYEVENT_H_
#define WORKSPACE_TRADER_INTERFACE_STRATEGYEVENT_H_
#include <functional>
#include <map>
#include <string>

struct OrderIdentify;
struct MsgStruct;

struct StrategyEvent {
  bool init();
  void handle(MsgStruct &msg);
  void regMsgFun();

  void OrderInsertReqHandle(MsgStruct &msg);
  void pubOrderInsertRsp(OrderIdentify identity, bool result, std::string reason);

  void OrderCancelReqHandle(MsgStruct &msg);
  void pubOrderCancelRsp(OrderIdentify identity, bool result, const std::string &reason);

  void AccountStatusReqHandle(MsgStruct &msg);
  void pubAccountStatusRsp(const std::string &identity, bool result, const std::string &reason = "success");

  void MarginRateReqHandle(MsgStruct &msg);
  void pubMarginRateRsp(const std::string &identity, bool result, const std::string &reason = "success");

  void CommissionRateReqHandle(MsgStruct &msg);
  void pubCommissionRateRsp(const std::string &identity, bool result, const std::string &reason = "success");

  void InstrumentReqHandle(MsgStruct &msg);
  void pubInstrumentRsp(const std::string &identity, bool result, const std::string &reason = "success");

  bool sendEmail(const std::string &identity);

  std::map<std::string, std::function<void(MsgStruct &msg)>> msgFuncMap;
};

#endif /* WORKSPACE_TRADER_INTERFACE_STRATEGYEVENT_H_ */
