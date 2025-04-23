#ifndef WORKSPACE_TRADER_INFRA_YTPRECER_H_
#define WORKSPACE_TRADER_INFRA_YTPRECER_H_

#include "common/extern/ytp/inc/ytp_trader_api.h"

class YtpTraderSpi : public ytp::api::TraderSpi {
 public:
  virtual ~YtpTraderSpi(){};

  virtual void OnRspUserLogin(const YtpLoginLogoutStruct *login_info);
  virtual void OnRspUserLogout(const YtpLoginLogoutStruct *logout_info);
  virtual void OnRtnTrade(const YtpOrderInfoStruct *trade_info);
  virtual void OnRtnOrder(const YtpOrderInfoStruct *order_info);
  virtual void OnRtnOrderInsert(const YtpOrderInfoStruct *order_info);
  virtual void OnRtnOrderAction(const YtpOrderInfoStruct *order_info);
  virtual void OnRspTradingAccount(const YtpAccountInfo *account_info);
  virtual void OnRspMarginRate(const YtpMarginInfo *margin_info);
  virtual void OnRspCommissionRate(const YtpCommissionInfo *commission_info);
  virtual void OnFrontDisconnected(int reason);
  bool GetFrontDisconnected(void);

 private:
  bool front_disconnected_ = false;
};

#endif /* WORKSPACE_TRADER_INFRA_YTPRECER_H_ */