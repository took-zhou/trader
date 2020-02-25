#ifndef __ORDERFILL_H__
#define __ORDERFILL_H__
#include "ThostFtdcUserApiStruct.h"
#include<cstring>
#include "dci/Role.h"
#include "json.h"
#include "trade_components/PrintCheck.h"

using json = nlohmann::json;


constexpr char BUY1_SELL2 = '0';
constexpr char SELL1_BUY2 = '1';

constexpr char BUY_ = '0';
constexpr char SELL_ = '1';

struct OrderManage
{
    OrderManage()
    {
        std::memset(&order, 0, sizeof(CThostFtdcInputOrderField));
        std::memset(&order1, 0, sizeof(CThostFtdcInputOrderField));
        std::memset(&order2, 0, sizeof(CThostFtdcInputOrderField));
    }
    bool fillOrder();
    bool fillOrderByJsonFile();
    bool fillOrderByJsonString(const json& orderData);
private:
    bool determineBuyAndSaleDirection(const json orderData, char* direction_flag);
    bool buildFirstOrder(const json& orderData, const char& direction);
    bool buildSecondOrder(const json& orderData, const char& direction);
public:
    CThostFtdcInputOrderField order;
    CThostFtdcInputOrderField order1;
    CThostFtdcInputOrderField order2;
public:
    USE_ROLE(PintCheck);
};

struct FillFlag
{
    bool SingleInstrumentQuery {false};
};

#endif // !__ORDERFILL_H__

