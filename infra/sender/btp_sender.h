/*
 * ctp_sender.h
 *
 *  Created on: 2020.08.29
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_BTPSENDER_H_
#define WORKSPACE_TRADER_INFRA_BTPSENDER_H_
#include <map>
#include <string>
#include "common/extern/btp/inc/btp_trader_api.h"
#include "common/self/utils.h"
#include "trader/infra/recer/btp_recer.h"
#include "trader/infra/sender/send_api.h"

struct BtpTraderInfo {
  std::string user_id;
  std::string user_name;
  int front_id;
};

struct BtpSender : SendApi {
 public:
  BtpSender(void);
  ~BtpSender(void);
  bool ReqUserLogin();
  bool ReqUserLogout();
  bool InsertOrder(utils::OrderContent &content);
  bool CancelOrder(utils::OrderContent &content);
  bool ReqAvailableFunds();
  bool ReqInstrumentInfo(const utils::InstrumtntID &ins_exch);
  bool ReqTransactionCost(const utils::InstrumtntID &ins_exch);
  bool LossConnection();
  bool Release();

  static std::map<int, BtpTraderInfo> btp_trader_info_map;

 private:
  bool Init(void);
  static btp::api::TraderApi *trader_api;
  static BtpTraderSpi *trader_spi;

  bool is_init_ = false;
};

#endif /* WORKSPACE_TRADER_INFRA_XTPSENDER_H_ */
