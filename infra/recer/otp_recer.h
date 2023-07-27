#ifndef WORKSPACE_TRADER_INFRA_OTPRECER_H_
#define WORKSPACE_TRADER_INFRA_OTPRECER_H_

#include "common/extern/otp/inc/oes_api/oes_async_api.h"
#include "common/self/utils.h"

class OtpTraderSpi {
 public:
  OtpTraderSpi(){};
  ~OtpTraderSpi(){};

  void OnRspUserLogin(const std::string *login_info);
  void OnRspUserLogout(const std::string *logout_info);
  int32_t OnConnected(eOesApiChannelTypeT channel_type, OesApiSessionInfoT *session_info, OesApiSubscribeInfoT *subscribe_info = NULL);
  int32_t OnDisconnected(eOesApiChannelTypeT channel_type, OesApiSessionInfoT *session_info);
  void OnBusinessReject(const OesOrdRejectT *order_reject);
  void OnOrderReport(const OesOrdCnfmT *order_report);
  void OnTradeReport(const OesTrdCnfmT *trade_report);
  void OnQueryCashAsset(const OesApiSessionInfoT *session, const OesCashAssetItemT *cash_asset);
  void OnQueryCommissionRate(const OesApiSessionInfoT *session, const OesCommissionRateItemT *commission_rate);

 public:
  int32_t current_request_id = 0;
};

#endif /* WORKSPACE_TRADER_INFRA_CTPRECER_H_ */