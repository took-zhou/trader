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
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/basetype.h"
#include "common/self/dci/Role.h"
constexpr char BUY_ = '0';
constexpr char SELL_ = '1';

struct TradedOrder
{
    double price {0.0};
    int volume   {0};
    std::string  direction{"unknow"};
    std::string date{"unknow"};
    std::string time{"unknow"};
};

struct OrderContent : CThostFtdcInputOrderField
{
    std::string identityId      {"unknow"};
    std::string orderRef        {"unknow"};
    int frontId                 {INVALID_S32};
    int sessionId               {INVALID_S32};
    std::string investorId      {"unknow"};
    std::string userId          {"unknow"};
    std::string instrumentID    {"unknow"};
    std::string currentStateStr    {"Invalid"};
    char currentStateChar          {'I'};
    bool activeCancleIndication {false};

    bool isFlowFinish           {false};

    TradedOrder tradedOrder;
    bool isValid() const
    {
        return identityId!= std::string("");
    };
    IMPL_ROLE(CThostFtdcInputOrderField);
};

struct OrderManage
{
    bool addOrder(std::string orderKey);
    void delOrder(const std::string& orderKey);
    CThostFtdcInputOrderField* getOrder(const std::string& orderKey);
    OrderContent& getOrderContent(const std::string orderKey);
    OrderContent& getOrderCOntentByIdentityId(const std::string& identityKey);
    void printOrderInfo(const CThostFtdcInputOrderField& order, const std::string& orderName="no_name_order");
    void printOrderInfo(const std::string orderKey, const std::string& orderName="no_name_order");
    bool buildOrder(const std::string orderKey, const strategy_trader::OrderInsertReq& orderInsertReq);
    int requestIdBuildAlg();
public:
    std::map<std::string, CThostFtdcInputOrderField*> orderMaps;
    int requestID{0};
};

struct OrderSave
{
    static bool loadDynmicSaveFiles(){return true;}
    static bool saveOnRtnOrderOrderState(const OrderContent& orderContent);
    static bool delOneRecordByOnRtnOrder(const OrderContent& orderContent);
    static bool saveSuccessOrderInsert(OrderContent& order);
    static bool saveSuccCancelOrder(OrderContent& order, std::string reason);
};

#endif /* WORKSPACE_TRADER_DOMAIN_COMPONENTS_ORDER_H_ */
