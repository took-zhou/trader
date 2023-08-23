#ifndef WORKSPACE_TRADER_INFRA_FTPRECER_H_
#define WORKSPACE_TRADER_INFRA_FTPRECER_H_

#include "common/extern/ftp/inc/ftp_trader_api.h"
#include "common/self/utils.h"

class FtpTraderSpi : public ftp::api::TraderSpi {
 public:
  virtual ~FtpTraderSpi(){};

  virtual void OnRspUserLogin(const FtpLoginLogoutStruct *login_info);
  virtual void OnRspUserLogout(const FtpLoginLogoutStruct *logout_info);
  virtual void OnRtnTrade(const FtpOrderInfoStruct *trade_info);
  virtual void OnRtnOrder(const FtpOrderInfoStruct *order_info);
  virtual void OnRtnOrderInsert(const FtpOrderInfoStruct *trade_info);
  virtual void OnRtnOrderAction(const FtpOrderInfoStruct *order_info);
  virtual void OnRspTradingAccount(const FtpAccountInfo *account_info);
  virtual void OnRspMarginRate(const FtpMarginInfo *margin_info);
  virtual void OnRspCommissionRate(const FtpCommissionInfo *commission_info);
};

#endif /* WORKSPACE_TRADER_INFRA_FTPRECER_H_ */