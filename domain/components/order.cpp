/*
 * order.cpp
 *
 *  Created on: 2020��8��30��
 *      Author: Administrator
 */

#include "trader/domain/components/order.h"
#include "common/extern/json/json.h"
#include "common/extern/log/log.h"
#include "common/self/fileUtil.h"
#include "common/self/basetype.h"
#include "common/self/utils.h"
#include "trader/interface/traderEvent.h"
#include "trader/infra/define.h"
//#include <string.h>
//using namespace nlohmann;
using json = nlohmann::json;
namespace
{
    void showOrderInfo(const CThostFtdcInputOrderField& order, const std::string& orderName)
    {
        std::string direction;
        direction.push_back(order.Direction);
        std::string forceCloseReason;
        forceCloseReason.push_back(order.ForceCloseReason);
        std::string orderPriceType;
        orderPriceType.push_back(order.OrderPriceType);
        std::string timeCondition;
        timeCondition.push_back(order.TimeCondition);
        json jorder ={
                {orderName.c_str(),{
                        {"BrokerID",    order.BrokerID},
                        {"ExchangeID",  order.ExchangeID},
                        {"InvestorID",  order.InvestorID},
                        {"AccountID",   order.AccountID},
                        {"ClientID",    order.ClientID},
                        {"GTDDate",     order.GTDDate},
                        {"UserID",      order.UserID},
                        {"RequestID",   order.RequestID},
                        {"InvestUnitID",order.InvestUnitID},
                        {"BusinessUnit",order.BusinessUnit},
                        {"ClientID",    order.ClientID},
                        {"OrderRef",    order.OrderRef},
                        {"IPAddress",   order.IPAddress},
                        {"MacAddress",  order.MacAddress},

                        {"InstrumentID",    order.InstrumentID},
                        {"Direction",       direction},
                        {"CombOffsetFlag",  order.CombOffsetFlag},
                        {"LimitPrice",      order.LimitPrice},
                        {"OrderPriceType",  orderPriceType},
                        {"TimeCondition",   timeCondition},
                        {"ContingentCondition",order.ContingentCondition},
                        {"MinVolume",           order.MinVolume},
                        {"StopPrice",           order.StopPrice},
                        {"VolumeTotalOriginal", order.VolumeTotalOriginal},
                        {"VolumeCondition",     order.VolumeCondition},
                        {"CombHedgeFlag",       order.CombHedgeFlag},

                        {"CurrencyID",      order.CurrencyID},
                        {"UserForceClose",  order.UserForceClose},
                        {"ForceCloseReason",forceCloseReason},
                        {"IsAutoSuspend",   order.IsAutoSuspend},
                        {"IsSwapOrder",     order.IsSwapOrder},
                        }
                }
        };
        JsonPrint(jorder);
    }
}

bool OrderManage::addOrder(std::string orderKey)
{
    if(orderMaps.find(orderKey) != orderMaps.end())
    {
        return false;
    }
    CThostFtdcInputOrderField tmpOrder{0};
    orderMaps.insert(std::pair<std::string, CThostFtdcInputOrderField>(orderKey,tmpOrder));
    INFO_LOG("add new order[%s] ok", orderKey.c_str());
    return true;
}
void OrderManage::delOrder(std::string orderKey)
{
    auto it = orderMaps.find(orderKey);
    if(it != orderMaps.end())
    {
        orderMaps.erase(it);
        INFO_LOG("del order[%s] ok",orderKey.c_str());
    }
}
CThostFtdcInputOrderField* OrderManage::getOrder(const std::string orderKey)
{
    if(orderMaps.find(orderKey) != orderMaps.end())
    {
        return &orderMaps.at(orderKey);
    }
    INFO_LOG("can not find order by orkerKey[%s]",orderKey.c_str());
    return nullptr;
}

void OrderManage::printOrderInfo(const CThostFtdcInputOrderField& order, const std::string& orderName)
{
    showOrderInfo(order, orderName);
}

void OrderManage::printOrderInfo(const std::string orderKey, const std::string& orderName)
{
    auto* order = getOrder(orderKey);
    showOrderInfo(*order, orderName);
}

int OrderManage::requestIdBuildAlg()
{
    if(requestID == S32_MAX)
    {
        requestID = 1;
        return requestID;
    }
    requestID++;
    return requestID;
}

bool OrderManage::buildOrder(const std::string orderKey, const strategy_trader::OrderInsertReq& orderInsertReq)
{
    auto orderIndication = orderInsertReq.order();
    auto& jsonCfg = utils::JsonConfig::getInstance();
    auto& order = *(getOrder(orderKey));
    strcpy(order.OrderRef, orderKey.c_str());

    std::string brokerId = jsonCfg.getConfig("trader","BrokerID").get<std::string>();
    strcpy(order.BrokerID, brokerId.c_str());

    std::string investorId = jsonCfg.getConfig("trader","InvestorID").get<std::string>();
    strcpy(order.InvestorID, investorId.c_str());

    std::string userId = jsonCfg.getConfig("trader","UserID").get<std::string>();
    strcpy(order.UserID, userId.c_str());

    /*************************************************************************/
    std::string instrumentID = orderIndication.instrument();
    strcpy(order.InstrumentID, instrumentID.c_str());

    std::string exchangeId = orderIndication.exchangeid();
    strcpy(order.ExchangeID, exchangeId.c_str());

    auto direction = orderIndication.direction();
    order.Direction = direction == strategy_trader::Direction::BUY ? BUY_ : SELL_;

    auto& limitPriceStr = orderIndication.limitprice();
    auto limitprice = atof(limitPriceStr.c_str());
    order.LimitPrice = limitprice;

    order.VolumeTotalOriginal = orderIndication.volume_total_original();

    auto& openClose = orderIndication.comb_offset_flag();
    if(openClose != "open" && openClose != "close")
    {
        ERROR_LOG("error comb_offset_flag [%s]",openClose.c_str());
        return false;
    }
    order.CombOffsetFlag[0] = openClose == "open"? THOST_FTDC_OF_Open : THOST_FTDC_OF_Close;

    /*****************************************************************************/
    auto orderType = orderIndication.order_type();
    std::string orderTypeSubTitle = "";
    if(orderType == strategy_trader::OrderType::limit_LIMIT)
    {
        orderTypeSubTitle = LIMIT_LIMIT_SUBTITLE;
    }
    else if(orderType == strategy_trader::OrderType::Limit_FAK)
    {
        orderTypeSubTitle = LIMIT_FAK_SUBTITLE;
    }
    if(orderTypeSubTitle == std::string(""))
    {
        ERROR_LOG("error OrderType");
        return false;
    }

    std::string orderPrinceType = jsonCfg.getConfig("order_type",orderTypeSubTitle)["OrderPriceType"].get<std::string>();
    order.OrderPriceType = orderPrinceType[0];

    std::string combHedgeFlag = jsonCfg.getConfig("order_type",orderTypeSubTitle)["CombHedgeFlag"].get<std::string>();
    strcpy(order.CombHedgeFlag, combHedgeFlag.c_str());

    std::string timeCondition = jsonCfg.getConfig("order_type",orderTypeSubTitle)["TimeCondition"].get<std::string>();
    order.TimeCondition = timeCondition[0];

    std::string gTDDate = jsonCfg.getConfig("order_type",orderTypeSubTitle)["GTDDate"].get<std::string>();
    strcpy(order.GTDDate, gTDDate.c_str());

    std::string volumeCondition = jsonCfg.getConfig("order_type",orderTypeSubTitle)["VolumeCondition"].get<std::string>();
    order.VolumeCondition = volumeCondition[0];

    order.MinVolume = jsonCfg.getConfig("order_type",orderTypeSubTitle)["MinVolume"].get<int>();

    std::string contingentCondition = jsonCfg.getConfig("order_type",orderTypeSubTitle)["ContingentCondition"].get<std::string>();
    order.ContingentCondition = contingentCondition[0];


    order.StopPrice = jsonCfg.getConfig("order_type",orderTypeSubTitle)["StopPrice"].get<double>();

    std::string forceCloseReason = jsonCfg.getConfig("order_type",orderTypeSubTitle)["ForceCloseReason"].get<std::string>();
    order.ForceCloseReason = forceCloseReason[0];

    order.IsAutoSuspend = jsonCfg.getConfig("order_type",orderTypeSubTitle)["IsAutoSuspend"].get<int>();

    std::string businessUnit = jsonCfg.getConfig("order_type",orderTypeSubTitle)["BusinessUnit"].get<std::string>();
    strcpy(order.BusinessUnit, businessUnit.c_str());

    order.RequestID = requestIdBuildAlg();

    order.UserForceClose = jsonCfg.getConfig("order_type",orderTypeSubTitle)["UserForceClose"].get<int>();
    order.IsSwapOrder = jsonCfg.getConfig("order_type",orderTypeSubTitle)["IsSwapOrder"].get<int>();

    string mac;
    if(!utils::get_local_mac(mac))
    {
        ERROR_LOG("get_local_mac error!");
    }
    strcpy(order.MacAddress,mac.c_str());

    string Ip;
    string hostName;
    if(!utils::GetHostInfo(hostName,Ip))
    {
        ERROR_LOG("GetHostInfo error!");
    }
    strcpy(order.IPAddress,Ip.c_str());
    INFO_LOG("fill order over! order is:");
    printOrderInfo(order,"order");

    return true;
}
