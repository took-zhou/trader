/*
 * otp_sender.h
 *
 *  Created on: 2020.08.29
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_OTPSENDER_H_
#define WORKSPACE_TRADER_INFRA_OTPSENDER_H_
#include <map>
#include <string>
#include "common/extern/otp/inc/oes_api/oes_async_api.h"
#include "common/self/utils.h"
#include "trader/infra/recer/otp_recer.h"
#include "trader/infra/sender/send_api.h"

struct OtpTraderInfo {
  std::string user_id;
  std::string user_name;
  OesAsyncApiChannelT *ord_channel;
  OesAsyncApiChannelT *rpt_channel;
  OtpTraderSpi *trader_spi;
};

struct OtpSender : SendApi {
 public:
  OtpSender(void);
  ~OtpSender(void);
  bool ReqUserLogin();
  bool ReqUserLogout();
  bool InsertOrder(utils::OrderContent &content);
  bool CancelOrder(utils::OrderContent &content);
  bool ReqAvailableFunds();
  bool ReqInstrumentInfo(const utils::InstrumtntID &ins_exch);
  bool ReqTransactionCost(const utils::InstrumtntID &ins_exch);
  bool LossConnection();
  bool Release();

  static std::map<int, OtpTraderInfo> otp_trader_info_map;

 private:
  bool Init(void);
  OesAsyncApiContextT *async_context_;
  bool is_init_ = false;
};

#endif /* WORKSPACE_TRADER_INFRA_XTPSENDER_H_ */
