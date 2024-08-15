#ifndef WORKSPACE_TRADER_INFRA_GTPRECER_H_
#define WORKSPACE_TRADER_INFRA_GTPRECER_H_

#include "common/extern/gtp/inc/gtp_trader_api.h"

class GtpTraderSpi : public gtp::api::TraderSpi {
 public:
  virtual ~GtpTraderSpi(){};

  virtual void OnRspUserLogin(const GtpLoginLogoutStruct *login_info);
  virtual void OnRspUserLogout(const GtpLoginLogoutStruct *logout_info);
  virtual void OnRtnTrade(const GtpOrderInfoStruct *trade_info);
  virtual void OnRtnOrder(const GtpOrderInfoStruct *order_info);
  virtual void OnRtnOrderInsert(const GtpOrderInfoStruct *order_info);
  virtual void OnRtnOrderAction(const GtpOrderInfoStruct *order_info);
  virtual void OnRspTradingAccount(const GtpAccountInfo *account_info);
  virtual void OnRspMarginRate(const GtpMarginInfo *margin_info);
  virtual void OnRspCommissionRate(const GtpCommissionInfo *commission_info);
  virtual void OnFrontDisconnected(int reason);
  bool GetFrontDisconnected(void);

 private:
  bool front_disconnected_ = false;
};

#endif /* WORKSPACE_TRADER_INFRA_GTPRECER_H_ */