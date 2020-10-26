/*
 * order.h
 *
 *  Created on: 2020��8��29��
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_DOMAIN_COMPONENTS_ORDER_H_
#define WORKSPACE_TRADER_DOMAIN_COMPONENTS_ORDER_H_
#include <map>
#include <string>
#include "common/extern/ctp/inc/ThostFtdcUserApiStruct.h"
#include "common/self/protobuf/strategy-trader.pb.h"
constexpr char BUY_ = '0';
constexpr char SELL_ = '1';

struct OrderManage
{
    bool addOrder(std::string orderKey);
    void delOrder(std::string orderKey);
    CThostFtdcInputOrderField* getOrder(const std::string orderKey);
    void printOrderInfo(const CThostFtdcInputOrderField& order, const std::string& orderName="no_name_order");
    void printOrderInfo(const std::string orderKey, const std::string& orderName="no_name_order");
    bool buildOrder(const std::string orderKey, const strategy_trader::OrderInsertReq& orderInsertReq);
    int requestIdBuildAlg();
public:
    std::map<std::string, CThostFtdcInputOrderField> orderMaps;
    int requestID{0};
};



#endif /* WORKSPACE_TRADER_DOMAIN_COMPONENTS_ORDER_H_ */
