/*
 * ctp_sender.h
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
#include "trader/infra/recer/xtp_recer.h"
#include "trader/infra/sender/send_api.h"

struct XtpTraderInfo {
  std::string user_id;
  int front_id;
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

  static std::map<int, XtpTraderInfo> kXtpTraderInfoMap;

 private:
  bool init(void);
  bool release(void);
  static XTP::API::TraderApi *kTraderApi;
  static XtpTraderSpi *kTraderSpi;

  int request_id = 0;
  bool is_init = false;
};

#endif /* WORKSPACE_TRADER_INFRA_XTPSENDER_H_ */
