#include "orderAction.h"
#include <fstream>
#include "log.h"
#include "getconfig.h"
#include "OrderRef.h"
#include <iostream>
#include "systemInfo.h"

using json = nlohmann::json;
using namespace std;
FillFlag fillFlag;
//namespace  {
//    bool orderDataCheck(const json& orderCfg)
//    {
////        string brokerId = orderCfg["BrokerID"].get<string>();
//        if(json.is_string(orderCfg["BrokerID"]))
//        {
//            return false;
//        }
//    }
//}

std::unordered_map<std::string, std::string> OrderPriceTypeMap =
{
        {"THOST_FTDC_OPT_AnyPrice","1"},
        {"THOST_FTDC_OPT_LimitPrice","2"},
        {"THOST_FTDC_OPT_BestPrice","3"},
        {"THOST_FTDC_OPT_LastPrice","4"},
        {"THOST_FTDC_OPT_LastPricePlusOneTicks","5"},
        {"THOST_FTDC_OPT_LastPricePlusTwoTicks","6"},
        {"THOST_FTDC_OPT_LastPricePlusThreeTicks","7"},
        {"THOST_FTDC_OPT_AskPrice1","8"},
        {"THOST_FTDC_OPT_AskPrice1PlusOneTicks","9"},
        {"THOST_FTDC_OPT_AskPrice1PlusTwoTicks","A"},
        {"THOST_FTDC_OPT_AskPrice1PlusThreeTicks","B"},
        {"THOST_FTDC_OPT_BidPrice1","C"},
        {"THOST_FTDC_OPT_BidPrice1PlusOneTicks","D"},
        {"THOST_FTDC_OPT_BidPrice1PlusTwoTicks","E"},
        {"THOST_FTDC_OPT_BidPrice1PlusThreeTicks","F"},
        {"THOST_FTDC_OPT_FiveLevelPrice","G"}
};

std::unordered_map<std::string, std::string> ContingentConditionMap =
{
        {"THOST_FTDC_CC_Immediately","1"},
        {"THOST_FTDC_CC_Touch","2"},
        {"THOST_FTDC_CC_TouchProfit","3"},
        {"THOST_FTDC_CC_ParkedOrder","4"},
        {"THOST_FTDC_CC_LastPriceGreaterThanStopPrice","5"},
        {"THOST_FTDC_CC_LastPriceGreaterEqualStopPrice","6"},
        {"THOST_FTDC_CC_LastPriceLesserThanStopPrice","7"},
        {"THOST_FTDC_CC_LastPriceLesserEqualStopPrice","8"},
        {"THOST_FTDC_CC_AskPriceGreaterThanStopPrice","9"},
        {"THOST_FTDC_CC_AskPriceGreaterEqualStopPrice","A"},
        {"THOST_FTDC_CC_AskPriceLesserThanStopPrice","B"},
        {"THOST_FTDC_CC_AskPriceLesserEqualStopPrice","C"},
        {"THOST_FTDC_CC_BidPriceGreaterThanStopPrice","D"},
        {"THOST_FTDC_CC_BidPriceGreaterEqualStopPrice","E"},
        {"THOST_FTDC_CC_BidPriceLesserThanStopPrice","F"},
        {"THOST_FTDC_CC_BidPriceLesserEqualStopPrice","H"}
};

std::unordered_map<std::string, std::string> TimeConditionMap =
{
        {"THOST_FTDC_TC_GFD","1"},
        {"THOST_FTDC_TC_GFS","2"},
        {"THOST_FTDC_TC_GFD","3"},
        {"THOST_FTDC_TC_GTD","4"},
        {"THOST_FTDC_TC_GTC","5"},
        {"THOST_FTDC_TC_GFA","6"}
};

std::unordered_map<std::string, std::string> VolumeConditionMap =
{
        {"THOST_FTDC_VC_AV","1"},
        {"THOST_FTDC_VC_MV","2"},
        {"THOST_FTDC_VC_CV","3"}
};


bool OrderManage::addOrder(std::string orderKey)
{
    if(orderMaps.find(orderKey) != orderMaps.end())
    {
        return false;
    }
    CThostFtdcInputOrderField tmpOrder{0};
    orderMaps.insert(std::pair<std::string, CThostFtdcInputOrderField>(orderKey,tmpOrder));
    INFO_LOG("add new order ok");
    return true;
}
void OrderManage::delOrder(std::string orderKey)
{
    auto it = orderMaps.find(orderKey);
    if(it != orderMaps.end())
    {
        orderMaps.erase(it);
        INFO_LOG("add new order ok");
    }
}
CThostFtdcInputOrderField* OrderManage::getOrder(const std::string orderKey)
{
    if(orderMaps.find(orderKey) != orderMaps.end())
    {
        return &orderMaps.at(orderKey);
    }
    return nullptr;
}

bool OrderManage::fillOrderByJsonString(const json& orderData, std::string orderRef1, std::string orderRef2)
{
    JsonPrint(orderData);
    char bySellDirections[2] = {'0','0'};
    INFO_LOG("begin to determineBuyAndSaleDirection");
    if(!determineBuyAndSaleDirection(orderData, bySellDirections))
    {
        ERROR_LOG("determine BuyAnd Sale Direction error!");
    }

    INFO_LOG("begin to buildFirstOrder");
    if(!buildFirstOrder(orderData, bySellDirections[0], orderRef1))
    {
        ERROR_LOG("fill first order error!");
        return false;
    }
    INFO_LOG("buildFirstOrder OK");

    INFO_LOG("begin to buildSecondOrder");
    if(!buildSecondOrder(orderData, bySellDirections[1], orderRef2))
    {
        ERROR_LOG("fill second order error!");
        return false;
    }
    INFO_LOG("buildSecondOrder OK");
    INFO_LOG("fillOrderByJsonString ok");
    return true;
}

bool OrderManage::buildFirstOrder(const json& orderData, const char& direction,const std::string orderRef)
{
    static size_t requestID;
    json orderCfg;
    string cfgFileName = getConfig("trade","OrderCommonPath");
    ifstream istrm(cfgFileName,ios::binary);
    if (!istrm.is_open())
    {
        printf("failed to open %s",cfgFileName.c_str());
    }
    istrm >> orderCfg;
    istrm.close();
    if(! addOrder(orderRef))
    {
        return false;
    }
    auto* tmpOrder1 = getOrder(orderRef);
    if(tmpOrder1 ==nullptr)
    {
        return false;
    }
    auto& order1 = *tmpOrder1;
    INFO_LOG("begin to fill order");

    strcpy(order1.OrderRef, orderRef.c_str());
    string brokerId = orderCfg["BrokerID"].get<string>();
    strcpy(order1.BrokerID, brokerId.c_str());

    string investorId = orderCfg["InvestorID"].get<string>();
    strcpy(order1.InvestorID, investorId.c_str());

    string userId = orderCfg["UserID"].get<string>();
    strcpy(order1.UserID, userId.c_str());

    /*************************************************************************/
    string instrumentID = orderData["InstrumentID1"].get<string>();
    strcpy(order1.InstrumentID, instrumentID.c_str());
//    printf("InstrumentID fill ok!\n");

    string exchangeId = orderData["ExchangeID"].get<string>();
    strcpy(order1.ExchangeID, exchangeId.c_str());
//    printf("ExchangeID fill ok!\n");

    order1.Direction = direction;
//    printf("Direction fill ok!\n");

    order1.LimitPrice = orderData["LimitPrice1"].get<double>();
//    printf("LimitPrice fill ok!\n");

    order1.VolumeTotalOriginal = orderData["VolumeTotalOriginal1"].get<int>();
//    printf("VolumeTotalOriginal fill ok!\n");

    string combOffsetFlag = orderData["CombOffsetFlag"].get<string>();
    order1.CombOffsetFlag[0] = combOffsetFlag[0];
//    printf("CombOffsetFlag fill ok!\n");

    /*****************************************************************************/

    string orderPrinceType = orderCfg["OrderPriceType"].get<string>();
    order1.OrderPriceType = orderPrinceType[0];
//    printf("OrderPriceType fill ok!\n");

    ///缁勫悎鎶曟満濂椾繚鏍囧織
    string combHedgeFlag = orderCfg["CombHedgeFlag"].get<string>();
    strcpy(order1.CombHedgeFlag, combHedgeFlag.c_str());
//    printf("CombHedgeFlag fill ok!\n");

    string timeCondition = orderCfg["TimeCondition"].get<string>();
    order1.TimeCondition = timeCondition[0];
//    printf("TimeCondition fill ok!\n");

    string gTDDate = orderCfg["GTDDate"].get<string>();
    strcpy(order1.GTDDate, gTDDate.c_str());
//    printf("GTDDate fill ok!\n");

    ///鎴愪氦閲忕被鍨�1锟�7
    string volumeCondition = orderCfg["VolumeCondition"].get<string>();
    order1.VolumeCondition = volumeCondition[0];
//    printf("VolumeCondition fill ok!\n");

    order1.MinVolume = orderCfg["MinVolume"].get<int>();

    string contingentCondition = orderCfg["ContingentCondition"].get<string>();
    order1.ContingentCondition = contingentCondition[0];

    order1.StopPrice = orderCfg["StopPrice"].get<double>();

    string forceCloseReason = orderCfg["ForceCloseReason"].get<string>();
    order1.ForceCloseReason = forceCloseReason[0];

    order1.IsAutoSuspend = orderCfg["IsAutoSuspend"].get<int>();

    string businessUnit = orderCfg["BusinessUnit"].get<string>();
    strcpy(order1.BusinessUnit, businessUnit.c_str());

    order1.RequestID = ++requestID;

    order1.UserForceClose = orderCfg["UserForceClose"].get<int>();

    order1.IsSwapOrder = orderCfg["IsSwapOrder"].get<int>();

    string mac;
    if(!get_local_mac(mac))
    {
        ERROR_LOG("get_local_mac error!");
    }
    strcpy(order1.MacAddress,mac.c_str());

    string Ip;
    string hostName;
    if(!GetHostInfo(hostName,Ip))
    {
        ERROR_LOG("GetHostInfo error!");
    }
    strcpy(order1.IPAddress,Ip.c_str());
    INFO_LOG("fill order1 over! order1 is:");
    ROLE(PintCheck).printOrderInfo(order1,"order1");
    return true;
}

bool OrderManage::buildSecondOrder(const json& orderData, const char& direction, const std::string orderRef)
{
    static size_t requestID2;
    json orderCfg;
    string cfgFileName = getConfig("trade","OrderCommonPath");
    ifstream istrm(cfgFileName,ios::binary);
    if (!istrm.is_open())
    {
        ERROR_LOG("failed to open %s",cfgFileName.c_str());
    }
    istrm >> orderCfg;
    istrm.close();
    if(! addOrder(orderRef))
    {
        return false;
    }
    auto* tmpOrder1 = getOrder(orderRef);
    if(tmpOrder1 ==nullptr)
    {
        return false;
    }
    auto& order2 = *tmpOrder1;
    INFO_LOG("begin to fill order");
    strcpy(order2.OrderRef, orderRef.c_str());
    string brokerId = orderCfg["BrokerID"].get<string>();
    strcpy(order2.BrokerID, brokerId.c_str());
//    printf("BrokerID fill ok!\n");

    string investorId = orderCfg["InvestorID"].get<string>();
    strcpy(order2.InvestorID, investorId.c_str());

    string userId = orderCfg["UserID"].get<string>();
    strcpy(order2.UserID, userId.c_str());
//    printf("UserID fill ok!\n");

    /*****************************************/
    string instrumentID = orderData["InstrumentID2"].get<string>();
    strcpy(order2.InstrumentID, instrumentID.c_str());
//    printf("InstrumentID fill ok!\n");

    string exchangeId = orderData["ExchangeID"].get<string>();
    strcpy(order2.ExchangeID, exchangeId.c_str());
//    printf("ExchangeID fill ok!\n");

    order2.Direction = direction;
//    printf("Direction fill ok!\n");

    order2.LimitPrice = orderData["LimitPrice2"].get<double>();
//    printf("LimitPrice fill ok!\n");

    order2.VolumeTotalOriginal = orderData["VolumeTotalOriginal2"].get<int>();
//    printf("VolumeTotalOriginal fill ok!\n");

    string combOffsetFlag = orderData["CombOffsetFlag"].get<string>();
    order2.CombOffsetFlag[0] = combOffsetFlag[0];
//    printf("CombOffsetFlag fill ok!\n");

    /*****************************************/

    string orderPrinceType = orderCfg["OrderPriceType"].get<string>();
    order2.OrderPriceType = orderPrinceType[0];
//    printf("OrderPriceType fill ok!\n");

    ///缁勫悎鎶曟満濂椾繚鏍囧織
    string combHedgeFlag = orderCfg["CombHedgeFlag"].get<string>();
    strcpy(order2.CombHedgeFlag, combHedgeFlag.c_str());
//    printf("CombHedgeFlag fill ok!\n");

    string timeCondition = orderCfg["TimeCondition"].get<string>();
    order2.TimeCondition = timeCondition[0];
//    printf("TimeCondition fill ok!\n");

    string gTDDate = orderCfg["GTDDate"].get<string>();
    strcpy(order2.GTDDate, gTDDate.c_str());
//    printf("GTDDate fill ok!\n");

    ///鎴愪氦閲忕被鍨�1锟�7
    string volumeCondition = orderCfg["VolumeCondition"].get<string>();
    order2.VolumeCondition = volumeCondition[0];
//    printf("VolumeCondition fill ok!\n");

    order2.MinVolume = orderCfg["MinVolume"].get<int>();
//    printf("MinVolume fill ok!\n");

    ///瑙﹀彂鏉′欢
    string contingentCondition = orderCfg["ContingentCondition"].get<string>();
    order2.ContingentCondition = contingentCondition[0];
//    printf("ContingentCondition fill ok!\n");

    ///姝㈡崯浠�1锟�7 TThostFtdcPriceType double
    order2.StopPrice = orderCfg["StopPrice"].get<double>();
//    printf("StopPrice fill ok!\n");

    ///寮哄钩鍘熷洜 TThostFtdcForceCloseReasonType char
    string forceCloseReason = orderCfg["ForceCloseReason"].get<string>();
    order2.ForceCloseReason = forceCloseReason[0];
//    printf("ForceCloseReason fill ok!\n");

    ///鑷姩鎸傝捣鏍囧織 TThostFtdcBoolType int
    order2.IsAutoSuspend = orderCfg["IsAutoSuspend"].get<int>();
//    printf("IsAutoSuspend fill ok!\n");

    ///涓氬姟鍗曞厓 TThostFtdcBusinessUnitType char[21]
    string businessUnit = orderCfg["BusinessUnit"].get<string>();
    strcpy(order2.BusinessUnit, businessUnit.c_str());
//    printf("BusinessUnit fill ok!\n");

    ///璇锋眰缂栧彿 TThostFtdcRequestIDType int
    order2.RequestID = ++requestID2;
//    printf("RequestID fill ok!\n");

    ///鐢ㄦ埛寮鸿瘎鏍囧織 TThostFtdcBoolType int
    order2.UserForceClose = orderCfg["UserForceClose"].get<int>();
//    printf("UserForceClose fill ok!\n");

    ///浜掓崲鍗曟爣蹇�1锟�7 TThostFtdcBoolType int
    order2.IsSwapOrder = orderCfg["IsSwapOrder"].get<int>();
//    printf("IsSwapOrder fill ok!\n");

    string mac;
    if(!get_local_mac(mac))
    {
        ERROR_LOG("get_local_mac error!"); // @suppress("Invalid arguments")
    }
    mac[17] = {'\0'};
    INFO_LOG("mac address:%s",mac.c_str()); // @suppress("Invalid arguments")
    strcpy(order2.MacAddress,mac.c_str());

    string Ip;
    string hostName;
    if(!GetHostInfo(hostName,Ip))
    {
        ERROR_LOG("GetHostInfo error!"); // @suppress("Invalid arguments")
    }
    strcpy(order2.IPAddress,Ip.c_str());
    INFO_LOG("fill order2 over! order2 is:");
    ROLE(PintCheck).printOrderInfo(order2,"order2");

    return true;
}

bool OrderManage::determineBuyAndSaleDirection(const json orderData, char* direction_flag)
{
    char Direction = orderData["Direction"].get<string>()[0];
    if( Direction!= BUY1_SELL2 && Direction!=SELL1_BUY2)
    {
        ERROR_LOG("Direction from strategy error!");
        return false;
    }
    if (Direction == BUY1_SELL2) {
        direction_flag[0] = BUY_;
        direction_flag[1] = SELL_;
    }
    else if (Direction == SELL1_BUY2) {
        direction_flag[0] = SELL_;
        direction_flag[1] = BUY_;
    }
    return true;
}
