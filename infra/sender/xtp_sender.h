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
  std::string user_name;
  int front_id;
};

struct XtpSender : SendApi {
 public:
  XtpSender(void);
  ~XtpSender(void);
  bool ReqUserLogin();
  bool ReqUserLogout();
  bool InsertOrder(utils::OrderContent &content);
  bool CancelOrder(utils::OrderContent &content);
  bool ReqAvailableFunds();
  bool ReqInstrumentInfo(const utils::InstrumtntID &ins_exch);
  bool ReqTransactionCost(const utils::InstrumtntID &ins_exch);
  bool LossConnection();

  static std::map<int, XtpTraderInfo> xtp_trader_info_map;

 private:
  bool Init(void);
  bool Release(void);
  static XTP::API::TraderApi *trader_api;
  static XtpTraderSpi *trader_spi;

  bool is_init_ = false;
};

#endif /* WORKSPACE_TRADER_INFRA_XTPSENDER_H_ */
