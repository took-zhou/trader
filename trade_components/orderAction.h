#ifndef __ORDERFILL_H__
#define __ORDERFILL_H__
#include "ThostFtdcUserApiStruct.h"
#include<cstring>
#include "dci/Role.h"
#include "json.h"
#include "trade_components/PrintCheck.h"
#include <unordered_map>
#include <map>
#include <string>
using json = nlohmann::json;


constexpr char BUY1_SELL2 = '0';
constexpr char SELL1_BUY2 = '1';

constexpr char BUY_ = '0';
constexpr char SELL_ = '1';


struct OrderManage
{
    OrderManage(){}
    bool fillOrderByJsonString(const json& orderData, std::string orderRef1, std::string orderRef2);
    bool addOrder(std::string orderKey);
    void delOrder(std::string orderKey);
    CThostFtdcInputOrderField* getOrder(const std::string orderKey);
    bool determineBuyAndSaleDirection(const json orderData, char* direction_flag);
    bool buildFirstOrder(const json& orderData, const char& direction, const std::string orderRef);
    bool buildSecondOrder(const json& orderData, const char& direction, const std::string orderRef);
public:
    std::map<std::string, CThostFtdcInputOrderField> orderMaps;
public:
    USE_ROLE(PintCheck);
};

struct FillFlag
{
    bool SingleInstrumentQuery {false};
};

#endif // !__ORDERFILL_H__

