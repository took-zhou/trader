/*
 * ctp_recer.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_CTPRECER_H_
#define WORKSPACE_TRADER_INFRA_CTPRECER_H_

#include "common/extern/ctp/inc/ThostFtdcTraderApi.h"
#include "common/self/utils.h"

class CtpTraderSpi : public CThostFtdcTraderSpi {
 public:
  ~CtpTraderSpi(){};

 public:
  ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
  void OnFrontConnected();

  ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
  ///@param nReason 错误原因
  ///        0x1001 网络读失败
  ///        0x1002 网络写失败
  ///        0x2001 接收心跳超时
  ///        0x2002 发送心跳失败
  ///        0x2003 收到错误报文
  void OnFrontDisconnected(int reason);

  ///心跳超时警告。当长时间未收到报文时，该方法被调用。
  ///@param nTimeLapse 距离上次接收报文的时间
  void OnHeartBeatWarning(int time_lapse){};

  ///客户端认证响应
  void OnRspAuthenticate(CThostFtdcRspAuthenticateField *rsp_authenticate_field, CThostFtdcRspInfoField *rsp_info, int request_id,
                         bool is_last);

  ///登录请求响应
  void OnRspUserLogin(CThostFtdcRspUserLoginField *rsp_user_login, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last);

  ///登出请求响应
  void OnRspUserLogout(CThostFtdcUserLogoutField *user_logout, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last);

  ///用户口令更新请求响应
  void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *user_password_update, CThostFtdcRspInfoField *rsp_info, int request_id,
                               bool is_last){};

  ///资金账户口令更新请求响应
  void OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *trading_account_password_update,
                                         CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///查询用户当前支持的认证模式的回复
  void OnRspUserAuthMethod(CThostFtdcRspUserAuthMethodField *rsp_user_auth_method, CThostFtdcRspInfoField *rsp_info, int request_id,
                           bool is_last){};

  ///获取图形验证码请求的回复
  void OnRspGenUserCaptcha(CThostFtdcRspGenUserCaptchaField *rsp_gen_user_captcha, CThostFtdcRspInfoField *rsp_info, int request_id,
                           bool is_last){};

  ///获取短信验证码请求的回复
  void OnRspGenUserText(CThostFtdcRspGenUserTextField *rsp_gen_user_text, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///报单录入请求响应
  void OnRspOrderInsert(CThostFtdcInputOrderField *input_order, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///预埋单录入请求响应
  void OnRspParkedOrderInsert(CThostFtdcParkedOrderField *parked_order, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///预埋撤单录入请求响应
  void OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *parked_order_action, CThostFtdcRspInfoField *rsp_info, int request_id,
                              bool is_last){};

  ///报单操作请求响应
  void OnRspOrderAction(CThostFtdcInputOrderActionField *input_order_action, CThostFtdcRspInfoField *rsp_info, int request_id,
                        bool is_last);

  ///查询最大报单数量响应
  void OnRspQryMaxOrderVolume(CThostFtdcQryMaxOrderVolumeField *query_max_order_volume, CThostFtdcRspInfoField *rsp_info, int request_id,
                              bool is_last){};

  ///投资者结算结果确认响应
  void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *settlement_info_confirm, CThostFtdcRspInfoField *rsp_info,
                                  int request_id, bool is_last);

  ///删除预埋单响应
  void OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *remove_parked_order, CThostFtdcRspInfoField *rsp_info, int request_id,
                              bool is_last){};

  ///删除预埋撤单响应
  void OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *remove_parked_order_action, CThostFtdcRspInfoField *rsp_info,
                                    int request_id, bool is_last){};

  ///执行宣告录入请求响应
  void OnRspExecOrderInsert(CThostFtdcInputExecOrderField *input_exec_order, CThostFtdcRspInfoField *rsp_info, int request_id,
                            bool is_last){};

  ///执行宣告操作请求响应
  void OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *input_exec_order_action, CThostFtdcRspInfoField *rsp_info, int request_id,
                            bool is_last){};

  ///询价录入请求响应
  void OnRspForQuoteInsert(CThostFtdcInputForQuoteField *input_for_quote, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///报价录入请求响应
  void OnRspQuoteInsert(CThostFtdcInputQuoteField *input_quote, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///报价操作请求响应
  void OnRspQuoteAction(CThostFtdcInputQuoteActionField *input_quote_action, CThostFtdcRspInfoField *rsp_info, int request_id,
                        bool is_last){};

  ///批量报单操作请求响应
  void OnRspBatchOrderAction(CThostFtdcInputBatchOrderActionField *input_batch_order_action, CThostFtdcRspInfoField *rsp_info,
                             int request_id, bool is_last){};

  ///期权自对冲录入请求响应
  void OnRspOptionSelfCloseInsert(CThostFtdcInputOptionSelfCloseField *input_option_self_close, CThostFtdcRspInfoField *rsp_info,
                                  int request_id, bool is_last){};

  ///期权自对冲操作请求响应
  void OnRspOptionSelfCloseAction(CThostFtdcInputOptionSelfCloseActionField *input_option_self_close_action,
                                  CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///申请组合录入请求响应
  void OnRspCombActionInsert(CThostFtdcInputCombActionField *input_comb_action, CThostFtdcRspInfoField *rsp_info, int request_id,
                             bool is_last){};

  ///请求查询报单响应
  void OnRspQryOrder(CThostFtdcOrderField *order, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///请求查询成交响应
  void OnRspQryTrade(CThostFtdcTradeField *trade, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///请求查询投资者持仓响应
  void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *investor_position, CThostFtdcRspInfoField *rsp_info, int request_id,
                                bool is_last){};

  ///请求查询资金账户响应
  void OnRspQryTradingAccount(CThostFtdcTradingAccountField *trading_account, CThostFtdcRspInfoField *rsp_info, int request_id,
                              bool is_last);

  ///请求查询投资者响应
  void OnRspQryInvestor(CThostFtdcInvestorField *investor, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///请求查询交易编码响应
  void OnRspQryTradingCode(CThostFtdcTradingCodeField *trading_code, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///请求查询合约保证金率响应
  void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *instrument_margin_rate, CThostFtdcRspInfoField *rsp_info,
                                    int request_id, bool is_last);

  ///请求查询合约手续费率响应
  void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *instrument_commission_rate,
                                        CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last);

  ///请求查询交易所响应
  void OnRspQryExchange(CThostFtdcExchangeField *exchange, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///请求查询产品响应
  void OnRspQryProduct(CThostFtdcProductField *product, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///请求查询合约响应
  void OnRspQryInstrument(CThostFtdcInstrumentField *instrument, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last);

  ///请求查询行情响应
  void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *depth_market_data, CThostFtdcRspInfoField *rsp_info, int request_id,
                               bool is_last){};

  ///请求查询投资者结算结果响应
  void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *settlement_info, CThostFtdcRspInfoField *rsp_info, int request_id,
                              bool is_last){};

  ///请求查询转帐银行响应
  void OnRspQryTransferBank(CThostFtdcTransferBankField *transfer_bank, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///请求查询投资者持仓明细响应
  void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *investor_position_detail, CThostFtdcRspInfoField *rsp_info,
                                      int request_id, bool is_last){};

  ///请求查询客户通知响应
  void OnRspQryNotice(CThostFtdcNoticeField *notice, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///请求查询结算信息确认响应
  void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *settlement_info_confirm, CThostFtdcRspInfoField *rsp_info,
                                     int request_id, bool is_last){};

  ///请求查询投资者持仓明细响应
  void OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *investor_position_combine_detail,
                                             CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///查询保证金监管系统经纪公司资金账户密钥响应
  void OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *cfmmc_trading_account_key, CThostFtdcRspInfoField *rsp_info,
                                      int request_id, bool is_last){};

  ///请求查询仓单折抵信息响应
  void OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField *e_warrant_offset, CThostFtdcRspInfoField *rsp_info, int request_id,
                              bool is_last){};

  ///请求查询投资者品种/跨品种保证金响应
  void OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField *investor_product_group_margin,
                                          CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///请求查询交易所保证金率响应
  void OnRspQryExchangeMarginRate(CThostFtdcExchangeMarginRateField *exchange_margin_rate, CThostFtdcRspInfoField *rsp_info, int request_id,
                                  bool is_last){};

  ///请求查询交易所调整保证金率响应
  void OnRspQryExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *exchange_margin_rate_adjust,
                                        CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///请求查询汇率响应
  void OnRspQryExchangeRate(CThostFtdcExchangeRateField *exchange_rate, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///请求查询二级代理操作员银期权限响应
  void OnRspQrySecAgentACIDMap(CThostFtdcSecAgentACIDMapField *sec_agent_acid_map, CThostFtdcRspInfoField *rsp_info, int request_id,
                               bool is_last){};

  ///请求查询产品报价汇率
  void OnRspQryProductExchRate(CThostFtdcProductExchRateField *product_exch_rate, CThostFtdcRspInfoField *rsp_info, int request_id,
                               bool is_last){};

  ///请求查询产品组
  void OnRspQryProductGroup(CThostFtdcProductGroupField *product_group, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///请求查询做市商合约手续费率响应
  void OnRspQryMMInstrumentCommissionRate(CThostFtdcMMInstrumentCommissionRateField *mm_instrument_commission_rate,
                                          CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///请求查询做市商期权合约手续费响应
  void OnRspQryMMOptionInstrCommRate(CThostFtdcMMOptionInstrCommRateField *mm_option_instr_comm_rate, CThostFtdcRspInfoField *rsp_info,
                                     int request_id, bool is_last){};

  ///请求查询报单手续费响应
  void OnRspQryInstrumentOrderCommRate(CThostFtdcInstrumentOrderCommRateField *instrument_order_comm_rate, CThostFtdcRspInfoField *rsp_info,
                                       int request_id, bool is_last){};

  ///请求查询资金账户响应
  void OnRspQrySecAgentTradingAccount(CThostFtdcTradingAccountField *trading_account, CThostFtdcRspInfoField *rsp_info, int request_id,
                                      bool is_last){};

  ///请求查询二级代理商资金校验模式响应
  void OnRspQrySecAgentCheckMode(CThostFtdcSecAgentCheckModeField *sec_agent_check_mode, CThostFtdcRspInfoField *rsp_info, int request_id,
                                 bool is_last){};

  ///请求查询二级代理商信息响应
  void OnRspQrySecAgentTradeInfo(CThostFtdcSecAgentTradeInfoField *sec_agent_trade_info, CThostFtdcRspInfoField *rsp_info, int request_id,
                                 bool is_last){};

  ///请求查询期权交易成本响应
  void OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField *option_instr_trade_cost, CThostFtdcRspInfoField *rsp_info,
                                    int request_id, bool is_last){};

  ///请求查询期权合约手续费响应
  void OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField *option_instr_comm_rate, CThostFtdcRspInfoField *rsp_info,
                                   int request_id, bool is_last);

  ///请求查询执行宣告响应
  void OnRspQryExecOrder(CThostFtdcExecOrderField *exec_order, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///请求查询询价响应
  void OnRspQryForQuote(CThostFtdcForQuoteField *for_quote, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///请求查询报价响应
  void OnRspQryQuote(CThostFtdcQuoteField *quote, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///请求查询期权自对冲响应
  void OnRspQryOptionSelfClose(CThostFtdcOptionSelfCloseField *option_self_close, CThostFtdcRspInfoField *rsp_info, int request_id,
                               bool is_last){};

  ///请求查询投资单元响应
  void OnRspQryInvestUnit(CThostFtdcInvestUnitField *invest_unit, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///请求查询组合合约安全系数响应
  void OnRspQryCombInstrumentGuard(CThostFtdcCombInstrumentGuardField *comb_instrument_guard, CThostFtdcRspInfoField *rsp_info,
                                   int request_id, bool is_last){};

  ///请求查询申请组合响应
  void OnRspQryCombAction(CThostFtdcCombActionField *comb_action, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///请求查询转帐流水响应
  void OnRspQryTransferSerial(CThostFtdcTransferSerialField *transfer_serial, CThostFtdcRspInfoField *rsp_info, int request_id,
                              bool is_last){};

  ///请求查询银期签约关系响应
  void OnRspQryAccountregister(CThostFtdcAccountregisterField *accountregister, CThostFtdcRspInfoField *rsp_info, int request_id,
                               bool is_last){};

  ///错误应答
  void OnRspError(CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///报单通知
  void OnRtnOrder(CThostFtdcOrderField *order);

  ///成交通知
  void OnRtnTrade(CThostFtdcTradeField *trade);

  ///报单录入错误回报
  void OnErrRtnOrderInsert(CThostFtdcInputOrderField *input_order, CThostFtdcRspInfoField *rsp_info){};

  ///报单操作错误回报
  void OnErrRtnOrderAction(CThostFtdcOrderActionField *order_action, CThostFtdcRspInfoField *rsp_info){};

  ///合约交易状态通知
  void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *instrument_status){};

  ///交易所公告通知
  void OnRtnBulletin(CThostFtdcBulletinField *bulletin){};

  ///交易通知
  void OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *trading_notice_info){};

  ///提示条件单校验错误
  void OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *error_conditional_order){};

  ///执行宣告通知
  void OnRtnExecOrder(CThostFtdcExecOrderField *exec_order){};

  ///执行宣告录入错误回报
  void OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *input_exec_order, CThostFtdcRspInfoField *rsp_info){};

  ///执行宣告操作错误回报
  void OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *exec_order_action, CThostFtdcRspInfoField *rsp_info){};

  ///询价录入错误回报
  void OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField *input_for_quote, CThostFtdcRspInfoField *rsp_info){};

  ///报价通知
  void OnRtnQuote(CThostFtdcQuoteField *quote){};

  ///报价录入错误回报
  void OnErrRtnQuoteInsert(CThostFtdcInputQuoteField *input_quote, CThostFtdcRspInfoField *rsp_info){};

  ///报价操作错误回报
  void OnErrRtnQuoteAction(CThostFtdcQuoteActionField *quote_action, CThostFtdcRspInfoField *rsp_info){};

  ///询价通知
  void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *for_quote_rsp){};

  ///保证金监控中心用户令牌
  void OnRtnCFMMCTradingAccountToken(CThostFtdcCFMMCTradingAccountTokenField *cfmmc_trading_account_token){};

  ///批量报单操作错误回报
  void OnErrRtnBatchOrderAction(CThostFtdcBatchOrderActionField *batch_order_action, CThostFtdcRspInfoField *rsp_info){};

  ///期权自对冲通知
  void OnRtnOptionSelfClose(CThostFtdcOptionSelfCloseField *option_self_close){};

  ///期权自对冲录入错误回报
  void OnErrRtnOptionSelfCloseInsert(CThostFtdcInputOptionSelfCloseField *input_option_self_close, CThostFtdcRspInfoField *rsp_info){};

  ///期权自对冲操作错误回报
  void OnErrRtnOptionSelfCloseAction(CThostFtdcOptionSelfCloseActionField *option_self_close_action, CThostFtdcRspInfoField *rsp_info){};

  ///申请组合通知
  void OnRtnCombAction(CThostFtdcCombActionField *comb_action){};

  ///申请组合录入错误回报
  void OnErrRtnCombActionInsert(CThostFtdcInputCombActionField *input_comb_action, CThostFtdcRspInfoField *rsp_info){};

  ///请求查询签约银行响应
  void OnRspQryContractBank(CThostFtdcContractBankField *contract_bank, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///请求查询预埋单响应
  void OnRspQryParkedOrder(CThostFtdcParkedOrderField *parked_order, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///请求查询预埋撤单响应
  void OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *parked_order_action, CThostFtdcRspInfoField *rsp_info, int request_id,
                                 bool is_last){};

  ///请求查询交易通知响应
  void OnRspQryTradingNotice(CThostFtdcTradingNoticeField *trading_notice, CThostFtdcRspInfoField *rsp_info, int request_id,
                             bool is_last){};

  ///请求查询经纪公司交易参数响应
  void OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *broker_trading_params, CThostFtdcRspInfoField *rsp_info,
                                   int request_id, bool is_last){};

  ///请求查询经纪公司交易算法响应
  void OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *broker_trading_algos, CThostFtdcRspInfoField *rsp_info, int request_id,
                                  bool is_last){};

  ///请求查询监控中心用户令牌
  void OnRspQueryCFMMCTradingAccountToken(CThostFtdcQueryCFMMCTradingAccountTokenField *query_cfmmc_trading_account_token,
                                          CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last){};

  ///银行发起银行资金转期货通知
  void OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *rsp_transfer){};

  ///银行发起期货资金转银行通知
  void OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *rsp_transfer){};

  ///银行发起冲正银行转期货通知
  void OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *rsp_repeal){};

  ///银行发起冲正期货转银行通知
  void OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *rsp_repeal){};

  ///期货发起银行资金转期货通知
  void OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *rsp_transfer){};

  ///期货发起期货资金转银行通知
  void OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *rsp_transfer){};

  ///系统运行时期货端手工发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
  void OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *rsp_repeal){};

  ///系统运行时期货端手工发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
  void OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *rsp_repeal){};

  ///期货发起查询银行余额通知
  void OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *notify_query_account){};

  ///期货发起银行资金转期货错误回报
  void OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *req_transfer, CThostFtdcRspInfoField *rsp_info){};

  ///期货发起期货资金转银行错误回报
  void OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *req_transfer, CThostFtdcRspInfoField *rsp_info){};

  ///系统运行时期货端手工发起冲正银行转期货错误回报
  void OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *req_repeal, CThostFtdcRspInfoField *rsp_info){};

  ///系统运行时期货端手工发起冲正期货转银行错误回报
  void OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *req_repeal, CThostFtdcRspInfoField *rsp_info){};

  ///期货发起查询银行余额错误回报
  void OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *req_query_account, CThostFtdcRspInfoField *rsp_info){};

  ///期货发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
  void OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *rsp_repeal){};

  ///期货发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
  void OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *rsp_repeal){};

  ///期货发起银行资金转期货应答
  void OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *req_transfer, CThostFtdcRspInfoField *rsp_info, int request_id,
                                     bool is_last){};

  ///期货发起期货资金转银行应答
  void OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *req_transfer, CThostFtdcRspInfoField *rsp_info, int request_id,
                                     bool is_last){};

  ///期货发起查询银行余额应答
  void OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *req_query_account, CThostFtdcRspInfoField *rsp_info,
                                          int request_id, bool is_last){};

  ///银行发起银期开户通知
  void OnRtnOpenAccountByBank(CThostFtdcOpenAccountField *open_account){};

  ///银行发起银期销户通知
  void OnRtnCancelAccountByBank(CThostFtdcCancelAccountField *cancel_account){};

  ///银行发起变更银行账号通知
  void OnRtnChangeAccountByBank(CThostFtdcChangeAccountField *change_account){};

  bool front_disconnected = false;
  int re_connect = 0;
  int session_id = 0;
  int front_id = 0;
  std::string user_id;
  std::string req_transaction_cost_exchange;
  std::string req_transaction_cost_instrument;
};

#endif /* WORKSPACE_TRADER_INFRA_CTPRECER_H_ */
