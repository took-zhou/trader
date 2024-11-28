/*
 * gtp_sender.h
 *
 *  Created on: 2020.08.29
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_GTPSENDER_H_
#define WORKSPACE_TRADER_INFRA_GTPSENDER_H_
#include <map>
#include <string>
#include "common/extern/gtp/inc/gtp_trader_api.h"
#include "common/self/utils.h"
#include "trader/infra/recer/gtp_recer.h"
#include "trader/infra/sender/send_api.h"

struct GtpTraderInfo {
  std::string user_id;
  std::string user_name;
  int front_id;
};

struct GtpSender : SendApi {
 public:
  GtpSender(void);
  ~GtpSender(void);
  bool ReqUserLogin();
  bool ReqUserLogout();
  bool InsertOrder(utils::OrderContent &content);
  bool CancelOrder(utils::OrderContent &content);
  bool ReqAvailableFunds();
  bool ReqInstrumentInfo(const utils::InstrumtntID &ins_exch);
  bool ReqTransactionCost(const utils::InstrumtntID &ins_exch);
  bool LossConnection();
  bool Release();

  static std::map<int, GtpTraderInfo> gtp_trader_info_map;

 private:
  bool Init(void);
  static gtp::api::TraderApi *trader_api;
  static GtpTraderSpi *trader_spi;

  bool is_init_ = false;
};

#endif /* WORKSPACE_TRADER_INFRA_GTPSENDER_H_ */
