#ifndef WORKSPACE_TRADER_INFRA_MTPRECER_H_
#define WORKSPACE_TRADER_INFRA_MTPRECER_H_

#include "common/extern/mtp/inc/mtp_trader_api.h"

class MtpTraderSpi : public mtp::api::TraderSpi {
 public:
  virtual ~MtpTraderSpi(){};

  virtual void OnRspUserLogin(const MtpLoginLogoutStruct *login_info);
  virtual void OnRspUserLogout(const MtpLoginLogoutStruct *logout_info);
  virtual void OnRtnTrade(const MtpOrderInfoStruct *trade_info);
  virtual void OnRtnOrder(const MtpOrderInfoStruct *order_info);
  virtual void OnRtnOrderInsert(const MtpOrderInfoStruct *order_info);
  virtual void OnRtnOrderAction(const MtpOrderInfoStruct *order_info);
  virtual void OnRspTradingAccount(const MtpAccountInfo *account_info);
  virtual void OnRspMarginRate(const MtpMarginInfo *margin_info);
  virtual void OnRspCommissionRate(const MtpCommissionInfo *commission_info);
  virtual void OnFrontDisconnected(int reason);
  bool GetFrontDisconnected(void);

 private:
  bool front_disconnected_ = false;
};

#endif /* WORKSPACE_TRADER_INFRA_MTPRECER_H_ */