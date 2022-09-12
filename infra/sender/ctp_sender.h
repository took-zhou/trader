/*
 * ctp_sender.h
 *
 *  Created on: 2020.08.29
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_CTPSENDER_H_
#define WORKSPACE_TRADER_INFRA_CTPSENDER_H_
#include <map>
#include <string>
#include <vector>
#include "common/extern/ctp/inc/ThostFtdcTraderApi.h"
#include "common/self/utils.h"
#include "trader/infra/recer/ctp_recer.h"
#include "trader/infra/sender/send_api.h"

struct CtpTraderInfo {
  CThostFtdcTraderApi *trader_api;
  CtpTraderSpi *trader_spi;
  int front_id;
  int session_id;
};

struct CtpSender : SendApi {
 public:
  CtpSender(void);
  bool ReqUserLogin();
  bool ReqUserLogout();
  bool InsertOrder(utils::OrderContent &content);
  bool CancelOrder(utils::OrderContent &content);
  bool ReqAvailableFunds(const int requestId);
  bool ReqInstrumentInfo(const utils::InstrumtntID &ins_exch, const int requestId);
  bool ReqTransactionCost(const utils::InstrumtntID &ins_exch, const int requestId);
  bool LossConnection();

  static std::map<std::string, CtpTraderInfo> kCtpTraderInfoMap;

 private:
  bool BuildOrder(utils::OrderContent &content, CThostFtdcInputOrderField &order);
  bool Init(void);
  bool Authenticate(void);
  bool Confirm(void);
  bool Release(void);

  static CThostFtdcInputOrderField kDefaultOrderField;
  int request_id = 0;
  bool is_init = false;
};

#endif /* WORKSPACE_TRADER_INFRA_CTPSENDER_H_ */
