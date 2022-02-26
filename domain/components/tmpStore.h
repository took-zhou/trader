/*
 * tmpStore.h
 *
 *  Created on: 2020年10月25日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_DOMAIN_COMPONENTS_TMPSTORE_H_
#define WORKSPACE_TRADER_DOMAIN_COMPONENTS_TMPSTORE_H_
#include <string>

struct AccountInfo
{
    bool rsp_is_null              {false};                //请求是否空标志
    std::string ProcessRandomId   {""};                   ///请求进程标识
    std::string PreCredit         {""};                   ///上次信用额度
    std::string PreDeposit        {""};                   ///上次存款额
    std::string PreBalance        {""};                   ///上次结算准备金
    std::string PreMargin         {""};                   ///上次占用的保证金
    std::string InterestBase      {""};                   ///利息基数
    std::string Interest          {""};                   ///利息收入
    std::string Deposit           {""};                   ///入金金额
    std::string Withdraw          {""};                   ///出金金额
    std::string FrozenMargin      {""};                   ///冻结的保证金
    std::string FrozenCash        {""};                  ///冻结的资金
    std::string FrozenCommission  {""};                  ///冻结的手续费
    std::string CurrMargin        {""};                  ///当前保证金总额
    std::string CashIn            {""};                  ///资金差额
    std::string Commission        {""};                  ///手续费
    std::string CloseProfit       {""};                  ///平仓盈亏
    std::string PositionProfit    {""};                  ///持仓盈亏
    std::string Balance           {""};                  ///期货结算准备金
    std::string Available         {""};                  ///可用资金
    std::string WithdrawQuota     {""};                  ///可取资金
    std::string Reserve           {""};                  ///基本准备金
    std::string TradingDay        {""};                  ///交易日
    std::string SettlementID      {""};                  ///结算编号
    std::string Credit            {""};                  ///信用额度
    std::string Mortgage          {""};                  ///质押金额
    std::string ExchangeMargin    {""};                  ///交易所保证金
    std::string DeliveryMargin    {""};                  ///投资者交割保证金
    std::string ExchangeDeliveryMargin    {""};          ///交易所交割保证金
    std::string ReserveBalance    {""};                  ///保底期货结算准备金
    std::string CurrencyID        {""};                  ///币种代码
    std::string PreFundMortgageIn {""};                  ///上次货币质入金额
    std::string PreFundMortgageOut        {""};          ///上次货币质出金额
    std::string FundMortgageIn    {""};                  ///货币质入金额
    std::string FundMortgageOut   {""};                  ///货币质出金额
    std::string FundMortgageAvailable     {""};          ///货币质押余额
    std::string MortgageableFund          {""};          ///可质押货币金额
    std::string SpecProductMargin         {""};          ///特殊产品占用保证金
    std::string SpecProductFrozenMargin   {""};          ///特殊产品冻结保证金
    std::string SpecProductCommission     {""};          ///特殊产品手续费
    std::string SpecProductFrozenCommission  {""};       ///特殊产品冻结手续费
    std::string SpecProductPositionProfit {""};          ///特殊产品持仓盈亏
    std::string SpecProductCloseProfit    {""};          ///特殊产品平仓盈亏
    std::string SpecProductPositionProfitByAlg  {""};    ///根据持仓盈亏算法计算的特殊产品持仓盈亏
    std::string SpecProductExchangeMargin {""};          ///特殊产品交易所保证金
    std::string BizType                   {""};          ///业务类型
    std::string FrozenSwap                {""};          ///延时换汇冻结金额
    std::string RemainSwap                {""};          ///剩余换汇额度
    std::string PreMortgage               {""};          ///上次质押金额
};

struct MarginRate
{
    bool rsp_is_null                        {false};                //请求是否空标志
    std::string ProcessRandomId             {""};                    ///请求进程标识
    std::string LongMarginRatioByMoney      {""};                    ///多头保证金率
    std::string LongMarginRatioByVolume     {""};                    ///多头保证金费
    std::string ShortMarginRatioByMoney     {""};                    ///空头保证金率
    std::string ShortMarginRatioByVolume    {""};                    ///空头保证金费
};

struct CommissionRate
{
    bool rsp_is_null                    {false};              //请求是否空标志
    std::string ProcessRandomId         {""};                 ///请求进程标识
    std::string OpenRatioByMoney        {""};                 ///开仓手续费率
    std::string OpenRatioByVolume       {""};                 ///开仓手续费
    std::string CloseRatioByMoney       {""};                 ///平仓手续费率
    std::string CloseRatioByVolume      {""};                 ///平仓手续费
    std::string CloseTodayRatioByMoney  {""};                 ///平今手续费率
    std::string CloseTodayRatioByVolume {""};                 ///平今手续费
};

struct InstrumentInfo
{
    bool rsp_is_null                        {false};              //请求是否空标志
    std::string ProcessRandomId             {""};                 ///请求进程标识
    std::string  price_tick                 {""};
    int  max_market_order_volume   {0};
    int  min_market_order_volume   {0};
    int  max_limit_order_volume    {0};
    int  min_limit_order_volume    {0};
    std::string  volume_multiple            {""};
    int is_trading                         {0};
};

struct TmpStore
{
    AccountInfo accountInfo;
    MarginRate marginRate;
    CommissionRate commissionRate;
    InstrumentInfo instrumentInfo;
};



#endif /* WORKSPACE_TRADER_DOMAIN_COMPONENTS_TMPSTORE_H_ */
