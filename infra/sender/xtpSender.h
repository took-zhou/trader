/*
 * ctpSender.h
 *
 *  Created on: 2020.08.29
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_XTPSENDER_H_
#define WORKSPACE_TRADER_INFRA_XTPSENDER_H_
#include <map>
#include <string>
#include "common/extern/xtp/inc/xtp_trader_api.h"
#include "common/self/utils.h"
#include "trader/infra/recer/xtpRecer.h"
#include "trader/infra/sender/sendApi.h"

struct XtpTraderInfo {
  XTP::API::TraderApi *traderApi;
  XtpTraderSpi *traderSpi;
  int frontId;
  int sessionId;
};

struct XtpSender : SendApi {
 public:
  XtpSender(void);
  bool ReqUserLogin();
  bool ReqUserLogout();
  bool InsertOrder(utils::OrderContent &content);
  bool CancelOrder(utils::OrderContent &content);
  bool ReqAvailableFunds(const int requestId);
  bool ReqInstrumentInfo(const utils::InstrumtntID &ins_exch, const int requestId);
  bool ReqTransactionCost(const utils::InstrumtntID &ins_exch, const int requestId);
  bool LossConnection();

 private:
  bool init(void);
  bool release(void);

  static std::map<std::string, XtpTraderInfo> XtpTraderInfoMap;
  int nRequestID = 0;
  bool isInit = false;
};

#endif /* WORKSPACE_TRADER_INFRA_XTPSENDER_H_ */
