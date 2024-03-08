#ifndef WORKSPACE_TRADER_INFRA_BTPRECER_H_
#define WORKSPACE_TRADER_INFRA_BTPRECER_H_

#include "common/extern/btp/inc/btp_trader_api.h"

class BtpTraderSpi : public btp::api::TraderSpi {
 public:
  virtual ~BtpTraderSpi(){};

  virtual void OnRspUserLogin(const BtpLoginLogoutStruct *login_info);
  virtual void OnRspUserLogout(const BtpLoginLogoutStruct *logout_info);
  virtual void OnRtnTrade(const BtpOrderInfoStruct *trade_info);
  virtual void OnRtnOrder(const BtpOrderInfoStruct *order_info);
  virtual void OnRtnOrderInsert(const BtpOrderInfoStruct *order_info);
  virtual void OnRtnOrderAction(const BtpOrderInfoStruct *order_info);
  virtual void OnRspTradingAccount(const BtpAccountInfo *account_info);
  virtual void OnRspMarginRate(const BtpMarginInfo *margin_info);
  virtual void OnRspCommissionRate(const BtpCommissionInfo *commission_info);
};

#endif /* WORKSPACE_TRADER_INFRA_CTPRECER_H_ */