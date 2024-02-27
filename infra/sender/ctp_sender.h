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
  int front_id;
  std::string user_id;
  std::string user_name;
  CThostFtdcTraderApi *trader_api;
};

struct CtpApiSpiInfo {
  CThostFtdcTraderApi *trader_api;
  CtpTraderSpi *trader_spi;
};

struct CtpSender : SendApi {
 public:
  CtpSender(void);
  bool ReqUserLogin();
  bool ReqUserLogout();
  bool InsertOrder(utils::OrderContent &content);
  bool CancelOrder(utils::OrderContent &content);
  bool ReqAvailableFunds();
  bool ReqInstrumentInfo(const utils::InstrumtntID &ins_exch);
  bool ReqTransactionCost(const utils::InstrumtntID &ins_exch);
  bool LossConnection();

  static std::map<uint64_t, CtpTraderInfo> ctp_trader_info_map;

 private:
  bool BuildOrder(utils::OrderContent &content, CThostFtdcInputOrderField &order);
  bool Init(void);
  bool Authenticate(void);
  bool Login(void);
  bool Confirm(void);
  bool Release(void);

  static std::map<std::string, CtpApiSpiInfo> ctp_api_spi_info_map;
  static CThostFtdcInputOrderField default_order_field;
  int request_id_ = 0;
  bool is_init_ = false;
};

#endif /* WORKSPACE_TRADER_INFRA_CTPSENDER_H_ */
