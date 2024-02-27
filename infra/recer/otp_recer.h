#ifndef WORKSPACE_TRADER_INFRA_OTPRECER_H_
#define WORKSPACE_TRADER_INFRA_OTPRECER_H_

#include "common/extern/otp/inc/oes_api/oes_async_api.h"
#include "common/self/utils.h"

class OtpTraderSpi {
 public:
  OtpTraderSpi(){};
  ~OtpTraderSpi(){};

  /* 连接或重新连接完成后的回调函数 */
  int32_t OnConnected(eOesApiChannelTypeT channel_type, OesApiSessionInfoT *session_info, OesApiSubscribeInfoT *subscribe_info = NULL);
  /* 连接断开后的回调函数 */
  int32_t OnDisconnected(eOesApiChannelTypeT channel_type, OesApiSessionInfoT *session_info);
  /* 委托拒绝回报回调 */
  void OnBusinessReject(const OesOrdRejectT *order_reject);
  /* 委托确认回报回调 */
  void OnOrderReport(const OesOrdCnfmT *order_report);
  /* 成交确认回报回调 */
  void OnTradeReport(const OesTrdCnfmT *trade_report);
  /* 查询资金信息回调 */
  void OnQueryCashAsset(const OesApiSessionInfoT *session, const OesCashAssetItemT *cash_asset);
  /* 查询佣金信息回调 */
  void OnQueryCommissionRate(const OesApiSessionInfoT *session, const OesCommissionRateItemT *commission_rate);
};

#endif /* WORKSPACE_TRADER_INFRA_CTPRECER_H_ */