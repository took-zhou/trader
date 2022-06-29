/*
 * order.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_DOMAIN_COMPONENTS_ORDER_H_
#define WORKSPACE_TRADER_DOMAIN_COMPONENTS_ORDER_H_
#include <map>
#include <string>
#include "common/extern/ctp/inc/ThostFtdcUserApiStruct.h"
#include "common/self/basetype.h"
#include "common/self/dci/Role.h"
#include "common/self/protobuf/strategy-trader.pb.h"
constexpr char BUY_ = '0';
constexpr char SELL_ = '1';

struct TradedOrder {
  double price{0.0};
  int volume{0};
  std::string direction{"unknow"};
  std::string date{"unknow"};
  std::string time{"unknow"};
};

struct OrderIdentify {
  std::string prid{"unknow"};      // 策略进程随机标识
  std::string identity{"unknow"};  // 订单随机标识
};

struct OrderContent {
  OrderIdentify identityId;
  std::string orderRef{"unknow"};
  int frontId{INVALID_S32};
  int sessionId{INVALID_S32};
  std::string investorId{"unknow"};
  std::string userId{"unknow"};
  std::string instrumentID{"unknow"};
  bool activeCancleIndication{false};
  unsigned int total_volume;
  float limit_price;
  unsigned int left_volume;
  bool isFlowFinish{false};

  TradedOrder tradedOrder;
};

struct OrderManage {
 public:
  OrderManage();
  bool addOrder(const std::string &orderKey);
  void delOrder(const std::string &orderKey);
  bool buildOrder(const std::string &orderKey, const strategy_trader::OrderInsertReq &orderInsertReq,
                  CThostFtdcInputOrderField &orderfield);
  int requestIdBuildAlg();
  std::map<std::string, OrderContent *> orderMaps;

 private:
  static CThostFtdcInputOrderField defaultOrderField;
  int requestID{0};
};

#endif /* WORKSPACE_TRADER_DOMAIN_COMPONENTS_ORDER_H_ */
