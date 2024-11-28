/*
 * ctp_sender.h
 *
 *  Created on: 2020.08.29
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_FTPSENDER_H_
#define WORKSPACE_TRADER_INFRA_FTPSENDER_H_
#include <map>
#include <string>
#include "common/extern/ftp/inc/ftp_trader_api.h"
#include "common/self/utils.h"
#include "trader/infra/recer/ftp_recer.h"
#include "trader/infra/sender/send_api.h"

struct FtpTraderInfo {
  std::string user_id;
  std::string user_name;
  int front_id;
};

struct FtpSender : SendApi {
 public:
  FtpSender(void);
  ~FtpSender(void);
  bool ReqUserLogin();
  bool ReqUserLogout();
  bool InsertOrder(utils::OrderContent &content);
  bool CancelOrder(utils::OrderContent &content);
  bool ReqAvailableFunds();
  bool ReqInstrumentInfo(const utils::InstrumtntID &ins_exch);
  bool ReqTransactionCost(const utils::InstrumtntID &ins_exch);
  bool LossConnection();
  bool Release();

  static std::map<int, FtpTraderInfo> ftp_trader_info_map;

 private:
  bool Init(void);
  static ftp::api::TraderApi *trader_api;
  static FtpTraderSpi *trader_spi;

  bool is_init_ = false;
};

#endif /* WORKSPACE_TRADER_INFRA_XTPSENDER_H_ */
