#ifndef WORKSPACE_TRADER_INFRA_XTPRECER_H_
#define WORKSPACE_TRADER_INFRA_XTPRECER_H_

#include "common/extern/xtp/inc/xtp_trader_api.h"
#include "common/self/utils.h"

class XtpTraderSpi : public XTP::API::TraderSpi {
 public:
  virtual ~XtpTraderSpi(){};

  virtual void OnDisconnected(uint64_t session_id, int reason);

  virtual void OnError(XTPRI *error_info);

  virtual void OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error_info, uint64_t session_id);

  virtual void OnTradeEvent(XTPTradeReport *trade_info, uint64_t session_id);

  virtual void OnCancelOrderError(XTPOrderCancelInfo *cancel_info, XTPRI *error_info, uint64_t session_id);

  virtual void OnQueryOrder(XTPQueryOrderRsp *order_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

  virtual void OnQueryTrade(XTPQueryTradeRsp *trade_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_idt);

  virtual void OnQueryPosition(XTPQueryStkPositionRsp *investor_position, XTPRI *error_info, int request_id, bool is_last,
                               uint64_t session_id);

  virtual void OnQueryAsset(XTPQueryAssetRsp *trading_account, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

  virtual void OnQueryStructuredFund(XTPStructuredFundInfo *fund_info, XTPRI *error_info, int request_id, bool is_last,
                                     uint64_t session_id);

  virtual void OnQueryFundTransfer(XTPFundTransferNotice *fund_transfer_info, XTPRI *error_info, int request_id, bool is_last,
                                   uint64_t session_id);

  virtual void OnFundTransfer(XTPFundTransferNotice *fund_transfer_info, XTPRI *error_info, uint64_t session_id);

  virtual void OnQueryETF(XTPQueryETFBaseRsp *etf_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

  virtual void OnQueryETFBasket(XTPQueryETFComponentRsp *etf_component_info, XTPRI *error_info, int request_id, bool is_last,
                                uint64_t session_id);

  virtual void OnQueryIPOInfoList(XTPQueryIPOTickerRsp *ipo_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

  virtual void OnQueryIPOQuotaInfo(XTPQueryIPOQuotaRsp *quota_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

  virtual void OnQueryOptionAuctionInfo(XTPQueryOptionAuctionInfoRsp *option_info, XTPRI *error_info, int request_id, bool is_last,
                                        uint64_t session_id);

  virtual void OnCreditCashRepay(XTPCrdCashRepayRsp *cash_repay_info, XTPRI *error_info, uint64_t session_id);

  virtual void OnQueryCreditCashRepayInfo(XTPCrdCashRepayInfo *cash_repay_info, XTPRI *error_info, int request_id, bool is_last,
                                          uint64_t session_id);

  virtual void OnQueryCreditFundInfo(XTPCrdFundInfo *fund_info, XTPRI *error_info, int request_id, uint64_t session_id);

  virtual void OnQueryCreditDebtInfo(XTPCrdDebtInfo *debt_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

  virtual void OnQueryCreditTickerDebtInfo(XTPCrdDebtStockInfo *debt_info, XTPRI *error_info, int request_id, bool is_last,
                                           uint64_t session_id);

  virtual void OnQueryCreditAssetDebtInfo(double remain_amount, XTPRI *error_info, int request_id, uint64_t session_id);

  virtual void OnQueryCreditTickerAssignInfo(XTPClientQueryCrdPositionStkInfo *assign_info, XTPRI *error_info, int request_id, bool is_last,
                                             uint64_t session_id);

  virtual void OnQueryCreditExcessStock(XTPClientQueryCrdSurplusStkRspInfo *stock_info, XTPRI *error_info, int request_id,
                                        uint64_t session_id);

  void OnRspUserLogin(void);
  void OnRspUserLogout(void);
  bool GetFrontDisconnect(void);

 private:
  bool front_disconnected_ = false;
};

#endif /* WORKSPACE_TRADER_INFRA_CTPRECER_H_ */