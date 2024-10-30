/*
 * mtp_sender.h
 *
 *  Created on: 2020.08.29
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_MTPSENDER_H_
#define WORKSPACE_TRADER_INFRA_MTPSENDER_H_
#include <map>
#include <string>
#include "common/extern/mtp/inc/mtp_trader_api.h"
#include "common/self/utils.h"
#include "trader/infra/recer/mtp_recer.h"
#include "trader/infra/sender/send_api.h"

struct MtpTraderInfo {
  std::string user_id;
  std::string user_name;
  int front_id;
};

struct MtpSender : SendApi {
 public:
  MtpSender(void);
  ~MtpSender(void);
  bool ReqUserLogin();
  bool ReqUserLogout();
  bool InsertOrder(utils::OrderContent &content);
  bool CancelOrder(utils::OrderContent &content);
  bool ReqAvailableFunds();
  bool ReqInstrumentInfo(const utils::InstrumtntID &ins_exch);
  bool ReqTransactionCost(const utils::InstrumtntID &ins_exch);
  bool LossConnection();

  static std::map<int, MtpTraderInfo> mtp_trader_info_map;

 private:
  bool Init(void);
  bool Release(void);
  static mtp::api::TraderApi *trader_api;
  static MtpTraderSpi *trader_spi;

  bool is_init_ = false;
};

#endif /* WORKSPACE_TRADER_INFRA_MTPSENDER_H_ */
