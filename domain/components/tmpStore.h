/*
 * tmpStore.h
 *
 *  Created on: 2020年10月25日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_DOMAIN_COMPONENTS_TMPSTORE_H_
#define WORKSPACE_TRADER_DOMAIN_COMPONENTS_TMPSTORE_H_
#include <string>

struct AccountInfo {
  bool rsp_is_null{false};          //请求是否空标志
  std::string ProcessRandomId{""};  ///请求进程标识
  std::string Balance{""};          ///动态权益
  std::string Available{""};        ///可用资金
};

struct MarginRate {
  bool rsp_is_null{false};                   //请求是否空标志
  std::string ProcessRandomId{""};           ///请求进程标识
  std::string LongMarginRatioByMoney{""};    ///多头保证金率
  std::string LongMarginRatioByVolume{""};   ///多头保证金费
  std::string ShortMarginRatioByMoney{""};   ///空头保证金率
  std::string ShortMarginRatioByVolume{""};  ///空头保证金费
};

struct CommissionRate {
  bool rsp_is_null{false};                  //请求是否空标志
  std::string ProcessRandomId{""};          ///请求进程标识
  std::string OpenRatioByMoney{""};         ///开仓手续费率
  std::string OpenRatioByVolume{""};        ///开仓手续费
  std::string CloseRatioByMoney{""};        ///平仓手续费率
  std::string CloseRatioByVolume{""};       ///平仓手续费
  std::string CloseTodayRatioByMoney{""};   ///平今手续费率
  std::string CloseTodayRatioByVolume{""};  ///平今手续费
};

struct InstrumentInfo {
  bool rsp_is_null{false};          //请求是否空标志
  std::string ProcessRandomId{""};  ///请求进程标识
  std::string price_tick{""};
  int max_market_order_volume{0};
  int min_market_order_volume{0};
  int max_limit_order_volume{0};
  int min_limit_order_volume{0};
  std::string volume_multiple{""};
  int is_trading{0};
};

struct LoginInfo {
  int FrontID;
  int SessionID;
  long unsigned int OrderRef;
};

struct TmpStore {
  AccountInfo accountInfo;
  MarginRate marginRate;
  CommissionRate commissionRate;
  InstrumentInfo instrumentInfo;
  LoginInfo logingInfo;
};

#endif /* WORKSPACE_TRADER_DOMAIN_COMPONENTS_TMPSTORE_H_ */
