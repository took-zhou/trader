/*
 * order.cpp
 *
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
#include <fstream>
#include <iomanip>
#include<iostream>
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
    OrderContent* tmpOrder = new OrderContent();

    orderMaps.insert(std::pair<std::string, CThostFtdcInputOrderField*>(orderKey,tmpOrder));
    INFO_LOG("add new order[%s] ok", orderKey.c_str());
    return true;
}

void OrderManage::delOrder(std::string orderKey)
{
    auto it = orderMaps.find(orderKey);
    if(it != orderMaps.end())
    {
        delete it->second;
        it->second = nullptr;
        orderMaps.erase(it);
        INFO_LOG("del order[%s] ok",orderKey.c_str());
    }
}
CThostFtdcInputOrderField* OrderManage::getOrder(const std::string orderKey)
{
    if(orderMaps.find(orderKey) != orderMaps.end())
    {
        return orderMaps.at(orderKey);
    }
    INFO_LOG("can not find order by orkerKey[%s]",orderKey.c_str());
    return nullptr;
}

OrderContent& OrderManage::getOrderContent(const std::string orderKey)
{
    auto* ctpOrder = getOrder(orderKey);
    if(ctpOrder == nullptr)
    {
        static OrderContent staticOrderContent = OrderContent();
        return staticOrderContent;
    }
    return *(static_cast<OrderContent*>(ctpOrder));
}

OrderContent& OrderManage::getOrderCOntentByIdentityId(std::string& identityKey)
{
    for(auto& orderMap : orderMaps)
    {
        auto& orderContent = *static_cast<OrderContent*>(orderMap.second);
        if(orderContent.identityId == identityKey)
        {
            return orderContent;
        }
    }
    static OrderContent staticOrderContent = OrderContent();
    ERROR_LOG("can not find ordercontent by identity[%s]",identityKey.c_str());
    return staticOrderContent;
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
    const auto& orderTypeCfg = jsonCfg.getConfig("order_type",orderTypeSubTitle);
    std::string orderPrinceType = orderTypeCfg["OrderPriceType"].get<std::string>();
    order.OrderPriceType = orderPrinceType[0];

    std::string combHedgeFlag = orderTypeCfg["CombHedgeFlag"].get<std::string>();
    strcpy(order.CombHedgeFlag, combHedgeFlag.c_str());

    std::string timeCondition = orderTypeCfg["TimeCondition"].get<std::string>();
    order.TimeCondition = timeCondition[0];

    std::string gTDDate = orderTypeCfg["GTDDate"].get<std::string>();
    strcpy(order.GTDDate, gTDDate.c_str());

    std::string volumeCondition = orderTypeCfg["VolumeCondition"].get<std::string>();
    order.VolumeCondition = volumeCondition[0];

    order.MinVolume = orderTypeCfg["MinVolume"].get<int>();

    std::string contingentCondition = orderTypeCfg["ContingentCondition"].get<std::string>();
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

namespace{
    bool buildTotalTitles(const std::string& filePath)
    {
        std::string titles = "idectityId,frontId,sessionId,orderRef,instrumentId,price,time,direction,volumn,reason\n";
        ofstream outfile(filePath, std::ofstream::app);
        if(!outfile.is_open())
        {
            ERROR_LOG("open file[%s] failed",titles.c_str());
            return false;
        }
        outfile << titles;
        outfile.close();
        return true;
    }
}

bool OrderSave::saveSuccCancelOrder(OrderContent& order, std::string reason)
{
    auto& jsonCfg = utils::JsonConfig::getInstance();
    std::string filePath = jsonCfg.getConfig("trader","AllOrderActionsFilePath").get<std::string>();
    if(!utils::isFileExist(filePath))
    {
        utils::creatFile(filePath);
        if(!buildTotalTitles(filePath))
        {
            return false;
        }
    }
    else if(utils::getFileSize(filePath) == 0 || utils::getFileSize(filePath) == -1)
    {
        if(!buildTotalTitles(filePath))
        {
            return false;
        }
    }
    std::string saveContent = "";
    ofstream outfile(filePath, ios::app);
    if(!outfile.is_open())
    {
        ERROR_LOG("open file[%s] failed",filePath.c_str());
        return false;
    }
    saveContent += order.identityId+",";
    saveContent += utils::intToString(order.frontId)+",";
    saveContent += utils::intToString(order.sessionId)+",";
    saveContent += order.orderRef+",";
    saveContent += order.instrumentID+",";
    saveContent += utils::doubleToStringConvert(order.tradedOrder.price)+",";
    saveContent += order.tradedOrder.time+",";
    std::string direction = order.ROLE(CThostFtdcInputOrderField).Direction == THOST_FTDC_D_Buy? "buy":"sell";
    saveContent += direction+",";
    saveContent += utils::intToString(order.tradedOrder.volume)+",";
    saveContent += reason + "\n";
    outfile << saveContent;
    outfile.close();
    return true;
}

bool OrderSave::saveSuccessOrderInsert(OrderContent& order)
{
    auto& jsonCfg = utils::JsonConfig::getInstance();
    std::string filePath = jsonCfg.getConfig("trader","TraderSuccOrderFilePath").get<std::string>();
    if(!utils::isFileExist(filePath))
    {
        utils::creatFile(filePath);
        std::string titles = "idectityId,frontId,sessionId,orderRef,instrumentId,price,time,direction,volumn\n";
        ofstream outfile(filePath, ios::app);
        if(!outfile.is_open())
        {
            ERROR_LOG("open file[%s] failed",titles.c_str());
            return false;
        }
        outfile << titles;
        outfile.close();
    }
    std::string saveContent = "";
    ofstream outfile(filePath, ios::app);
    if(!outfile.is_open())
    {
        ERROR_LOG("open file[%s] failed",filePath.c_str());
        return false;
    }
    saveContent += order.identityId+",";
    saveContent += utils::intToString(order.frontId)+",";
    saveContent += utils::intToString(order.sessionId)+",";
    saveContent += order.orderRef+",";
    saveContent += order.instrumentID+",";
    saveContent += utils::doubleToStringConvert(order.tradedOrder.price)+",";
    saveContent += order.tradedOrder.time+",";
    saveContent += order.tradedOrder.direction+",";
    saveContent += utils::intToString(order.tradedOrder.volume)+"\n";
    outfile << saveContent;
    outfile.close();
    return true;
}

bool OrderSave::delOneRecordByOnRtnOrder(const OrderContent& orderContent)
{
    INFO_LOG("begin delOneRecordByOnRtnOrder, identity is[%s], orderRef is[%s], state[%s]",
            orderContent.identityId.c_str(),orderContent.orderRef.c_str(),orderContent.currentStateStr.c_str());
//    auto& jsonCfg = utils::JsonConfig::getInstance();
//    std::string filePath = jsonCfg.getConfig("trader","DynmicSaveOrderFilePath").get<std::string>();
//    if(!utils::isFileExist(filePath))
//    {
//        ERROR_LOG("not find file[%s]",filePath.c_str());
//        return false;
//    }
//
//    std::ifstream istrm(filePath,std::ios::binary);
//    if (!istrm.is_open())
//    {
//        ERROR_LOG("failed to open %s",filePath.c_str());
//        return false;
//    }
//    json saveContent;
//    istrm >> saveContent;
//    istrm.close();
//    JsonPrint(saveContent);
//    if(!saveContent.contains(orderContent.identityId))
//    {
//        ERROR_LOG("not find identity[%s] in record file[%s]",orderContent.identityId.c_str(),filePath.c_str());
//        return false;
//    }
//    saveContent.erase(orderContent.identityId);
//    ofstream saveStream(filePath,ios::trunc);
//    saveStream << setw(4) << saveContent << endl;
//    JsonPrint(saveContent);
//    saveStream.close();
    return true;
}

bool OrderSave::saveOnRtnOrderOrderState(const OrderContent& orderContent)
{
    INFO_LOG("begin saveOnRtnOrderOrderState, identity is[%s], orderRef is[%s], state[%s]",
            orderContent.identityId.c_str(),orderContent.orderRef.c_str(),orderContent.currentStateStr.c_str());
//    auto& jsonCfg = utils::JsonConfig::getInstance();
//    std::string filePath = jsonCfg.getConfig("trader","DynmicSaveOrderFilePath").get<std::string>();
//    if(!utils::isFileExist(filePath))
//    {
//        utils::creatFile(filePath);
//        json saveContent;
//        json tmp;
//        tmp["identityId"] = orderContent.identityId;
//        tmp["orderRef"] = orderContent.orderRef;
//        tmp["frontId"] = orderContent.frontId;
//        tmp["sessionId"] = orderContent.sessionId;
//        tmp["investorId"] = orderContent.investorId;
//        tmp["userId"] = orderContent.userId;
//        tmp["instrumentID"] = orderContent.instrumentID;
//        tmp["currentStateStr"] = orderContent.currentStateStr;
//        tmp["currentStateChar"] = utils::charToString(orderContent.currentStateChar);
//        saveContent[orderContent.identityId] = tmp;
//        JsonPrint(saveContent);
//        ofstream saveStream(filePath,ios::trunc);
//        saveStream << setw(4) << saveContent << endl;
//        saveStream.close();
//    }
//    else if(utils::getFileSize(filePath) == 0 || utils::getFileSize(filePath) == -1)
//    {
//        json saveContent;
//        json tmp;
//        tmp["identityId"] = orderContent.identityId;
//        tmp["orderRef"] = orderContent.orderRef;
//        tmp["frontId"] = orderContent.frontId;
//        tmp["sessionId"] = orderContent.sessionId;
//        tmp["investorId"] = orderContent.investorId;
//        tmp["userId"] = orderContent.userId;
//        tmp["instrumentID"] = orderContent.instrumentID;
//        tmp["currentStateStr"] = orderContent.currentStateStr;
//        tmp["currentStateChar"] = utils::charToString(orderContent.currentStateChar);
//        saveContent[orderContent.identityId] = tmp;
//        JsonPrint(saveContent);
//        ofstream saveStream(filePath,ios::trunc);
//        saveStream << setw(4) << saveContent << endl;
//        saveStream.close();
//    }
//    else
//    {
//        std::ifstream istrm(filePath,std::ios::binary);
//        if (!istrm.is_open())
//        {
//            ERROR_LOG("failed to open %s",filePath.c_str());
//            return false;
//        }
//        json saveContent;
//        istrm >> saveContent;
//        istrm.close();
//        if(saveContent.contains(orderContent.identityId))
//        {
//            auto& tmp = saveContent[orderContent.identityId];
//            tmp["currentStateStr"] = orderContent.currentStateStr;
//            tmp["currentStateChar"] = utils::charToString(orderContent.currentStateChar);
//        }
//        else
//        {
//            json tmp;
//            tmp["identityId"] = orderContent.identityId;
//            tmp["orderRef"] = orderContent.orderRef;
//            tmp["frontId"] = orderContent.frontId;
//            tmp["sessionId"] = orderContent.sessionId;
//            tmp["investorId"] = orderContent.investorId;
//            tmp["userId"] = orderContent.userId;
//            tmp["instrumentID"] = orderContent.instrumentID;
//            tmp["currentStateStr"] = orderContent.currentStateStr;
//            tmp["currentStateChar"] = utils::charToString(orderContent.currentStateChar);
//            saveContent[orderContent.identityId] = tmp;
//        }
//        JsonPrint(saveContent);
//        ofstream saveStream(filePath,ios::trunc);
//        saveStream << setw(4) << saveContent << endl;
//        saveStream.close();
//    }
    INFO_LOG("new state save ok, identity is[%s], orderRef is[%s]",orderContent.identityId.c_str(),orderContent.orderRef.c_str());
    return true;
}
