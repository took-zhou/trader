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




bool OrderManage::fillOrder()
{
        INFO_LOG("begin to fill order"); // @suppress("Invalid arguments")
        ///经纪公司代码 TThostFtdcBrokerIDType char[11]
        strcpy(order.BrokerID, getConfig("trade", "BrokerID").c_str());

        ///投资者代砄1�7 TThostFtdcInvestorIDType char[13]
        strcpy(order.InvestorID, getConfig("trade", "InvestorID").c_str());

        ///报单引用 TThostFtdcOrderRefType char[13]
        // strcpy(order.OrderRef,"000000000001");
        strcpy(order.OrderRef, "000000000001");

        ///用户代码 TThostFtdcUserIDType char[16]
        strcpy(order.UserID, getConfig("trade", "UserID").c_str());

        strcpy(order.ExchangeID, "DCE");
        ///报单价格条件 TThostFtdcOrderPriceTypeType char
        //// THOST_FTDC_OPT_AnyPrice '1' 任意仄1�7
        //// THOST_FTDC_OPT_LimitPrice '2' 限价
        //// THOST_FTDC_OPT_BestPrice '3' 朄1�7优价
        //// THOST_FTDC_OPT_LastPrice '4' 朄1�7新价
        //// THOST_FTDC_OPT_LastPricePlusOneTicks '5' 朄1�7新价浮动上浮1个ticks
        //// THOST_FTDC_OPT_LastPricePlusTwoTicks '6' 朄1�7新价浮动上浮2个ticks
        //// THOST_FTDC_OPT_LastPricePlusThreeTicks '7' 朄1�7新价浮动上浮3个ticks
        //// THOST_FTDC_OPT_AskPrice1 '8' 卖一仄1�7
        //// THOST_FTDC_OPT_AskPrice1PlusOneTicks '9' 卖一价浮动上浄1�7�ticks
        //// THOST_FTDC_OPT_AskPrice1PlusTwoTicks 'A' 卖一价浮动上浄1�7�ticks
        //// THOST_FTDC_OPT_AskPrice1PlusThreeTicks 'B' 卖一价浮动上浄1�7�ticks
        //// THOST_FTDC_OPT_BidPrice1 'C' 买一仄1�7
        //// THOST_FTDC_OPT_BidPrice1PlusOneTicks 'D' 买一价浮动上浄1�7�ticks
        //// THOST_FTDC_OPT_BidPrice1PlusTwoTicks 'E' 买一价浮动上浄1�7�ticks
        //// THOST_FTDC_OPT_BidPrice1PlusThreeTicks 'F' 买一价浮动上浄1�7�ticks
        order.OrderPriceType = '2';

        ///组合弄1�7平标忄1�7 TThostFtdcCombOffsetFlagType char[5]
        //// THOST_FTDC_OF_Open '0' 弄1�7仄1�7
        //// THOST_FTDC_OF_Close '1' 平仓
        //// THOST_FTDC_OF_ForceClose '2' 强平
        //// THOST_FTDC_OF_CloseToday '3' 平今
        //// THOST_FTDC_OF_CloseYesterday '4' 平昨
        //// THOST_FTDC_OF_ForceOff '5' 强减
        //// THOST_FTDC_OF_LocalForceClose '6' 本地强平
        //strcpy(order.CombOffsetFlag[0], THOST_FTDC_OF_Open);
        order.CombOffsetFlag[0] = THOST_FTDC_OF_Open;

        ///组合投机套保标志 TThostFtdcCombHedgeFlagType char[5]
        //// THOST_FTDC_HF_Speculation '1' 投机
        //// THOST_FTDC_HF_Arbitrage '2' 套利
        //// THOST_FTDC_HF_Hedge '3' 套保
        strcpy(order.CombHedgeFlag, "1");


        ///有效期类垄1�7 TThostFtdcTimeConditionType char
        //// THOST_FTDC_TC_IOC '1' 立即完成，否则撤锄1�7
        //// THOST_FTDC_TC_GFS '2' 本节有效
        //// THOST_FTDC_TC_GFD '3' 当日有效
        //// THOST_FTDC_TC_GTD '4' 指定日期前有敄1�7
        //// THOST_FTDC_TC_GTC '5' 撤销前有敄1�7
        //// THOST_FTDC_TC_GFA '6' 集合竞价有效
        order.TimeCondition = THOST_FTDC_TC_GFS;

        ///GTD日期 TThostFtdcDateType char[9]
        strcpy(order.GTDDate, "");

        ///成交量类垄1�7 TThostFtdcVolumeConditionType char
        //// THOST_FTDC_VC_AV '1' 任何数量
        //// THOST_FTDC_VC_MV '2' 朄1�7小数釄1�7
        //// THOST_FTDC_VC_CV '3' 全部数量
        order.VolumeCondition = THOST_FTDC_VC_CV;

        ///朄1�7小成交量 TThostFtdcVolumeType int
        order.MinVolume = 1;

        ///触发条件 TThostFtdcContingentConditionType char
        //// THOST_FTDC_CC_Immediately '1' 立即
        //// THOST_FTDC_CC_Touch '2' 止损
        //// THOST_FTDC_CC_TouchProfit '3' 止赢
        //// THOST_FTDC_CC_ParkedOrder '4' 预埋卄1�7
        //// THOST_FTDC_CC_LastPriceGreaterThanStopPrice '5' 朄1�7新价大于条件仄1�7
        //// THOST_FTDC_CC_LastPriceGreaterEqualStopPrice '6' 朄1�7新价大于等于条件仄1�7
        //// THOST_FTDC_CC_LastPriceLesserThanStopPrice '7' 朄1�7新价小于条件仄1�7
        //// THOST_FTDC_CC_LastPriceLesserEqualStopPrice '8' 朄1�7新价小于等于条件仄1�7
        //// THOST_FTDC_CC_AskPriceGreaterThanStopPrice '9' 卖一价大于条件价
        //// THOST_FTDC_CC_AskPriceGreaterEqualStopPrice 'A' 卖一价大于等于条件价
        //// THOST_FTDC_CC_AskPriceLesserThanStopPrice 'B' 卖一价小于条件价
        //// THOST_FTDC_CC_AskPriceLesserEqualStopPrice 'C' 卖一价小于等于条件价
        //// THOST_FTDC_CC_BidPriceGreaterThanStopPrice 'D' 买一价大于条件价
        //// THOST_FTDC_CC_BidPriceGreaterEqualStopPrice 'E' 买一价大于等于条件价
        //// THOST_FTDC_CC_BidPriceLesserThanStopPrice 'F' 买一价小于条件价
        //// THOST_FTDC_CC_BidPriceLesserEqualStopPrice 'H' 买一价小于等于条件价
        order.ContingentCondition = THOST_FTDC_CC_Immediately;

        ///止损仄1�7 TThostFtdcPriceType double
        order.StopPrice = 0;

        ///强平原因 TThostFtdcForceCloseReasonType char
        //// THOST_FTDC_FCC_NotForceClose '0' 非强幄1�7
        //// THOST_FTDC_FCC_LackDeposit '1' 资金不足
        //// THOST_FTDC_FCC_ClientOverPositionLimit '2' 客户超仓
        //// THOST_FTDC_FCC_MemberOverPositionLimit '3' 会员超仓
        //// THOST_FTDC_FCC_NotMultiple '4' 持仓非整数�1�7�1�7
        //// THOST_FTDC_FCC_Violation '5' 违规
        //// THOST_FTDC_FCC_Other '6' 其它
        //// THOST_FTDC_FCC_PersonDeliv '7' 自然人临近交剄1�7
        order.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;

        ///自动挂起标志 TThostFtdcBoolType int
        order.IsAutoSuspend = 0;

        ///业务单元 TThostFtdcBusinessUnitType char[21]
        strcpy(order.BusinessUnit, "");

        ///请求编号 TThostFtdcRequestIDType int
        //order.RequestID = ++requestID;

        ///用户强评标志 TThostFtdcBoolType int
        order.UserForceClose = 0;

        ///互换单标忄1�7 TThostFtdcBoolType int
        order.IsSwapOrder = 0;

        return true;
}

bool OrderManage::fillOrderByJsonFile()
{
    static size_t requestID;
    json orderCfg;
    string cfgFileName = "../../project/projroot/orders.json";
    ifstream istrm(cfgFileName,ios::binary);
    if (!istrm.is_open())
    {
        printf("failed to open %s",cfgFileName.c_str());
    }
    istrm >> orderCfg;
    istrm.close();

    JsonPrint(orderCfg);
    ///经纪公司代码 TThostFtdcBrokerIDType char[11]
    INFO_LOG("begin to fill order"); // @suppress("Invalid arguments")
    string brokerId = orderCfg["BrokerID"].get<string>();
    strcpy(order.BrokerID, brokerId.c_str());
    printf("BrokerID fill ok!\n");

    ///投资者代砄1�7 TThostFtdcInvestorIDType char[13]
    string investorId = orderCfg["InvestorID"].get<string>();
    strcpy(order.InvestorID, investorId.c_str());
    printf("InvestorID fill ok!\n");

    ///报单引用 TThostFtdcOrderRefType char[13]

    //strcpy(order.OrderRef,"000000000001");
    string orderRef = genOrderRef();
    strcpy(order.OrderRef, orderRef.c_str());
    printf("OrderRef fill ok!\n");

    ///用户代码 TThostFtdcUserIDType char[16]
    string userId = orderCfg["UserID"].get<string>();
    strcpy(order.UserID, userId.c_str());
    printf("UserID fill ok!\n");

    string exchangeId = orderCfg["ExchangeID"].get<string>();
    strcpy(order.ExchangeID, exchangeId.c_str());
    printf("ExchangeID fill ok!\n");

    string instrumentID = orderCfg["InstrumentID"].get<string>();
    strcpy(order.InstrumentID, instrumentID.c_str());
    printf("InstrumentID fill ok!\n");

    string direction = orderCfg["Direction"].get<string>();
    printf("%s", direction.c_str());
    order.Direction = direction[0];
    printf("Direction fill ok!\n");

    order.LimitPrice = orderCfg["LimitPrice"].get<double>();
    printf("LimitPrice fill ok!\n");

    order.VolumeTotalOriginal = orderCfg["VolumeTotalOriginal"].get<int>();
    printf("VolumeTotalOriginal fill ok!\n");
    ///报单价格条件 TThostFtdcOrderPriceTypeType char
    //// THOST_FTDC_OPT_AnyPrice '1' 任意仄1�7
    //// THOST_FTDC_OPT_LimitPrice '2' 限价
    //// THOST_FTDC_OPT_BestPrice '3' 朄1�7优价
    //// THOST_FTDC_OPT_LastPrice '4' 朄1�7新价
    //// THOST_FTDC_OPT_LastPricePlusOneTicks '5' 朄1�7新价浮动上浮1个ticks
    //// THOST_FTDC_OPT_LastPricePlusTwoTicks '6' 朄1�7新价浮动上浮2个ticks
    //// THOST_FTDC_OPT_LastPricePlusThreeTicks '7' 朄1�7新价浮动上浮3个ticks
    //// THOST_FTDC_OPT_AskPrice1 '8' 卖一仄1�7
    //// THOST_FTDC_OPT_AskPrice1PlusOneTicks '9' 卖一价浮动上浄1�7�ticks
    //// THOST_FTDC_OPT_AskPrice1PlusTwoTicks 'A' 卖一价浮动上浄1�7�ticks
    //// THOST_FTDC_OPT_AskPrice1PlusThreeTicks 'B' 卖一价浮动上浄1�7�ticks
    //// THOST_FTDC_OPT_BidPrice1 'C' 买一仄1�7
    //// THOST_FTDC_OPT_BidPrice1PlusOneTicks 'D' 买一价浮动上浄1�7�ticks
    //// THOST_FTDC_OPT_BidPrice1PlusTwoTicks 'E' 买一价浮动上浄1�7�ticks
    //// THOST_FTDC_OPT_BidPrice1PlusThreeTicks 'F' 买一价浮动上浄1�7�ticks
    string orderPrinceType = orderCfg["OrderPriceType"].get<string>();
    order.OrderPriceType = orderPrinceType[0];
    printf("OrderPriceType fill ok!\n");

    ///组合弄1�7平标忄1�7 TThostFtdcCombOffsetFlagType char[5]
    //// THOST_FTDC_OF_Open '0' 弄1�7仄1�7
    //// THOST_FTDC_OF_Close '1' 平仓
    //// THOST_FTDC_OF_ForceClose '2' 强平
    //// THOST_FTDC_OF_CloseToday '3' 平今
    //// THOST_FTDC_OF_CloseYesterday '4' 平昨
    //// THOST_FTDC_OF_ForceOff '5' 强减
    //// THOST_FTDC_OF_LocalForceClose '6' 本地强平
    //strcpy(order.CombOffsetFlag[0], THOST_FTDC_OF_Open);
    string combOffsetFlag = orderCfg["CombOffsetFlag"].get<string>();
    order.CombOffsetFlag[0] = combOffsetFlag[0];
    printf("CombOffsetFlag fill ok!\n");

    ///组合投机套保标志 TThostFtdcCombHedgeFlagType char[5]
    //// THOST_FTDC_HF_Speculation '1' 投机
    //// THOST_FTDC_HF_Arbitrage '2' 套利
    //// THOST_FTDC_HF_Hedge '3' 套保
    string combHedgeFlag = orderCfg["CombHedgeFlag"].get<string>();
    strcpy(order.CombHedgeFlag, combHedgeFlag.c_str());
    printf("CombHedgeFlag fill ok!\n");


    ///有效期类垄1�7 TThostFtdcTimeConditionType char
    //// THOST_FTDC_TC_IOC '1' 立即完成，否则撤锄1�7
    //// THOST_FTDC_TC_GFS '2' 本节有效
    //// THOST_FTDC_TC_GFD '3' 当日有效
    //// THOST_FTDC_TC_GTD '4' 指定日期前有敄1�7
    //// THOST_FTDC_TC_GTC '5' 撤销前有敄1�7
    //// THOST_FTDC_TC_GFA '6' 集合竞价有效
    string timeCondition = orderCfg["TimeCondition"].get<string>();
    order.TimeCondition = timeCondition[0];
    printf("TimeCondition fill ok!\n");

    ///GTD日期 TThostFtdcDateType char[9]
    string gTDDate = orderCfg["GTDDate"].get<string>();
    strcpy(order.GTDDate, gTDDate.c_str());
    printf("GTDDate fill ok!\n");

    ///成交量类垄1�7 TThostFtdcVolumeConditionType char
    //// THOST_FTDC_VC_AV '1' 任何数量
    //// THOST_FTDC_VC_MV '2' 朄1�7小数釄1�7
    //// THOST_FTDC_VC_CV '3' 全部数量
    string volumeCondition = orderCfg["VolumeCondition"].get<string>();
    order.VolumeCondition = volumeCondition[0];
    printf("VolumeCondition fill ok!\n");

    ///朄1�7小成交量 TThostFtdcVolumeType int
    order.MinVolume = orderCfg["MinVolume"].get<int>();
    printf("MinVolume fill ok!\n");

    ///触发条件 TThostFtdcContingentConditionType char
    //// THOST_FTDC_CC_Immediately '1' 立即
    //// THOST_FTDC_CC_Touch '2' 止损
    //// THOST_FTDC_CC_TouchProfit '3' 止赢
    //// THOST_FTDC_CC_ParkedOrder '4' 预埋卄1�7
    //// THOST_FTDC_CC_LastPriceGreaterThanStopPrice '5' 朄1�7新价大于条件仄1�7
    //// THOST_FTDC_CC_LastPriceGreaterEqualStopPrice '6' 朄1�7新价大于等于条件仄1�7
    //// THOST_FTDC_CC_LastPriceLesserThanStopPrice '7' 朄1�7新价小于条件仄1�7
    //// THOST_FTDC_CC_LastPriceLesserEqualStopPrice '8' 朄1�7新价小于等于条件仄1�7
    //// THOST_FTDC_CC_AskPriceGreaterThanStopPrice '9' 卖一价大于条件价
    //// THOST_FTDC_CC_AskPriceGreaterEqualStopPrice 'A' 卖一价大于等于条件价
    //// THOST_FTDC_CC_AskPriceLesserThanStopPrice 'B' 卖一价小于条件价
    //// THOST_FTDC_CC_AskPriceLesserEqualStopPrice 'C' 卖一价小于等于条件价
    //// THOST_FTDC_CC_BidPriceGreaterThanStopPrice 'D' 买一价大于条件价
    //// THOST_FTDC_CC_BidPriceGreaterEqualStopPrice 'E' 买一价大于等于条件价
    //// THOST_FTDC_CC_BidPriceLesserThanStopPrice 'F' 买一价小于条件价
    //// THOST_FTDC_CC_BidPriceLesserEqualStopPrice 'H' 买一价小于等于条件价
    string contingentCondition = orderCfg["ContingentCondition"].get<string>();
    order.ContingentCondition = contingentCondition[0];
    printf("ContingentCondition fill ok!\n");

    ///止损仄1�7 TThostFtdcPriceType double
    order.StopPrice = orderCfg["StopPrice"].get<double>();
    printf("StopPrice fill ok!\n");

    ///强平原因 TThostFtdcForceCloseReasonType char
    //// THOST_FTDC_FCC_NotForceClose '0' 非强幄1�7
    //// THOST_FTDC_FCC_LackDeposit '1' 资金不足
    //// THOST_FTDC_FCC_ClientOverPositionLimit '2' 客户超仓
    //// THOST_FTDC_FCC_MemberOverPositionLimit '3' 会员超仓
    //// THOST_FTDC_FCC_NotMultiple '4' 持仓非整数�1�7�1�7
    //// THOST_FTDC_FCC_Violation '5' 违规
    //// THOST_FTDC_FCC_Other '6' 其它
    //// THOST_FTDC_FCC_PersonDeliv '7' 自然人临近交剄1�7
    string forceCloseReason = orderCfg["ForceCloseReason"].get<string>();
    order.ForceCloseReason = forceCloseReason[0];
    printf("ForceCloseReason fill ok!\n");

    ///自动挂起标志 TThostFtdcBoolType int
    order.IsAutoSuspend = orderCfg["IsAutoSuspend"].get<int>();
    printf("IsAutoSuspend fill ok!\n");

    ///业务单元 TThostFtdcBusinessUnitType char[21]
    string businessUnit = orderCfg["BusinessUnit"].get<string>();
    strcpy(order.BusinessUnit, businessUnit.c_str());
    printf("BusinessUnit fill ok!\n");

    ///请求编号 TThostFtdcRequestIDType int
    order.RequestID = ++requestID;
    printf("RequestID fill ok!\n");

    ///用户强评标志 TThostFtdcBoolType int
    order.UserForceClose = orderCfg["UserForceClose"].get<int>();
    printf("UserForceClose fill ok!\n");

    ///互换单标忄1�7 TThostFtdcBoolType int
    order.IsSwapOrder = orderCfg["IsSwapOrder"].get<int>();
    printf("IsSwapOrder fill ok!\n");

    string mac;
    if(!get_local_mac(mac))
    {
        ERROR_LOG("get_local_mac error!"); // @suppress("Invalid arguments")
    }
    INFO_LOG("mac address:%s",mac.c_str()); // @suppress("Invalid arguments")
    strcpy(order.MacAddress,mac.c_str());

    string Ip;
    string hostName;
    if(!GetHostInfo(hostName,Ip))
    {
        ERROR_LOG("GetHostInfo error!"); // @suppress("Invalid arguments")
    }
    strcpy(order.IPAddress,Ip.c_str());
        return true;
}

bool OrderManage::fillOrderByJsonString(const json& orderData)
{
//    if(orderMsg == nullptr)
//    {
//        ERROR_LOG("the pointer orderMsg is nullptr!"); // @suppress("Invalid arguments")
//        return false;
//    }
//    printf("%s\n",orderMsg);
//    orderData = json::parse(string(orderMsg));
//    if(! orderDataCheck(orderData))
//    {
//        ERROR_LOG("orderDataCheck failed");
//        return false;
//    }
    JsonPrint(orderData);
    char bySellDirections[2] = {'0','0'};
    INFO_LOG("begin to determineBuyAndSaleDirection"); // @suppress("Invalid arguments")
    if(!determineBuyAndSaleDirection(orderData, bySellDirections))
    {
        ERROR_LOG("determine BuyAnd Sale Direction error!"); // @suppress("Invalid arguments")
    }

    INFO_LOG("begin to buildFirstOrder");
    if(!buildFirstOrder(orderData, bySellDirections[0]))
    {
        ERROR_LOG("fill first order error!"); // @suppress("Invalid arguments")
        return false;
    }
    INFO_LOG("buildFirstOrder OK");

    INFO_LOG("begin to buildSecondOrder");
    if(!buildSecondOrder(orderData, bySellDirections[1]))
    {
        ERROR_LOG("fill second order error!"); // @suppress("Invalid arguments")
        return false;
    }
    INFO_LOG("buildSecondOrder OK");
    INFO_LOG("fillOrderByJsonString ok"); // @suppress("Invalid arguments")
    return true;
}

bool OrderManage::buildFirstOrder(const json& orderData, const char& direction)
{
    static size_t requestID;
    json orderCfg;
    string cfgFileName = "../../project/projroot/commonorder.json";
    ifstream istrm(cfgFileName,ios::binary);
    if (!istrm.is_open())
    {
        printf("failed to open %s",cfgFileName.c_str());
    }
    istrm >> orderCfg;
    istrm.close();

    INFO_LOG("begin to fill order"); // @suppress("Invalid arguments")
    string brokerId = orderCfg["BrokerID"].get<string>();
    strcpy(order1.BrokerID, brokerId.c_str());
//    printf("BrokerID fill ok!\n");

    string investorId = orderCfg["InvestorID"].get<string>();
    strcpy(order1.InvestorID, investorId.c_str());
//    printf("InvestorID fill ok!\n");

    string orderRef = genOrderRef();
    strcpy(order1.OrderRef, orderRef.c_str());
//    printf("OrderRef fill ok!\n");

    string userId = orderCfg["UserID"].get<string>();
    strcpy(order1.UserID, userId.c_str());
//    printf("UserID fill ok!\n");

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

    ///组合投机套保标志
    string combHedgeFlag = orderCfg["CombHedgeFlag"].get<string>();
    strcpy(order1.CombHedgeFlag, combHedgeFlag.c_str());
//    printf("CombHedgeFlag fill ok!\n");

    string timeCondition = orderCfg["TimeCondition"].get<string>();
    order1.TimeCondition = timeCondition[0];
//    printf("TimeCondition fill ok!\n");

    string gTDDate = orderCfg["GTDDate"].get<string>();
    strcpy(order1.GTDDate, gTDDate.c_str());
//    printf("GTDDate fill ok!\n");

    ///成交量类垄1�7
    string volumeCondition = orderCfg["VolumeCondition"].get<string>();
    order1.VolumeCondition = volumeCondition[0];
//    printf("VolumeCondition fill ok!\n");

    order1.MinVolume = orderCfg["MinVolume"].get<int>();
//    printf("MinVolume fill ok!\n");

    ///触发条件
    string contingentCondition = orderCfg["ContingentCondition"].get<string>();
    order1.ContingentCondition = contingentCondition[0];
//    printf("ContingentCondition fill ok!\n");

    ///止损仄1�7 TThostFtdcPriceType double
    order1.StopPrice = orderCfg["StopPrice"].get<double>();
//    printf("StopPrice fill ok!\n");

    ///强平原因 TThostFtdcForceCloseReasonType char
    string forceCloseReason = orderCfg["ForceCloseReason"].get<string>();
    order1.ForceCloseReason = forceCloseReason[0];
//    printf("ForceCloseReason fill ok!\n");

    ///自动挂起标志 TThostFtdcBoolType int
    order1.IsAutoSuspend = orderCfg["IsAutoSuspend"].get<int>();
//    printf("IsAutoSuspend fill ok!\n");

    ///业务单元 TThostFtdcBusinessUnitType char[21]
    string businessUnit = orderCfg["BusinessUnit"].get<string>();
    strcpy(order1.BusinessUnit, businessUnit.c_str());
//    printf("BusinessUnit fill ok!\n");

    ///请求编号 TThostFtdcRequestIDType int
    order1.RequestID = ++requestID;
//    printf("RequestID fill ok!\n");

    ///用户强评标志 TThostFtdcBoolType int
    order1.UserForceClose = orderCfg["UserForceClose"].get<int>();
//    printf("UserForceClose fill ok!\n");

    ///互换单标忄1�7 TThostFtdcBoolType int
    order1.IsSwapOrder = orderCfg["IsSwapOrder"].get<int>();
//    printf("IsSwapOrder fill ok!\n");

    string mac;
    if(!get_local_mac(mac))
    {
        ERROR_LOG("get_local_mac error!"); // @suppress("Invalid arguments")
    }
//    INFO_LOG("mac address:%s",mac); // @suppress("Invalid arguments")
    strcpy(order1.MacAddress,mac.c_str());

    string Ip;
    string hostName;
    if(!GetHostInfo(hostName,Ip))
    {
        ERROR_LOG("GetHostInfo error!"); // @suppress("Invalid arguments")
    }
    strcpy(order1.IPAddress,Ip.c_str());
    INFO_LOG("fill order1 over! order1 is:");
    ROLE(PintCheck).printOrderInfo(order1,"order1");
    return true;
}

bool OrderManage::buildSecondOrder(const json& orderData, const char& direction)
{
    static size_t requestID2;
    json orderCfg;
    string cfgFileName = "../../project/projroot/commonorder.json";
    ifstream istrm(cfgFileName,ios::binary);
    if (!istrm.is_open())
    {
        ERROR_LOG("failed to open %s",cfgFileName.c_str());
    }
    istrm >> orderCfg;
    istrm.close();

    INFO_LOG("begin to fill order"); // @suppress("Invalid arguments")
    string brokerId = orderCfg["BrokerID"].get<string>();
    strcpy(order2.BrokerID, brokerId.c_str());
//    printf("BrokerID fill ok!\n");

    string investorId = orderCfg["InvestorID"].get<string>();
    strcpy(order2.InvestorID, investorId.c_str());
//    printf("InvestorID fill ok!\n");

    string orderRef = genOrderRef();
    strcpy(order2.OrderRef, orderRef.c_str());
//    printf("OrderRef fill ok!\n");

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

    ///组合投机套保标志
    string combHedgeFlag = orderCfg["CombHedgeFlag"].get<string>();
    strcpy(order2.CombHedgeFlag, combHedgeFlag.c_str());
//    printf("CombHedgeFlag fill ok!\n");

    string timeCondition = orderCfg["TimeCondition"].get<string>();
    order2.TimeCondition = timeCondition[0];
//    printf("TimeCondition fill ok!\n");

    string gTDDate = orderCfg["GTDDate"].get<string>();
    strcpy(order2.GTDDate, gTDDate.c_str());
//    printf("GTDDate fill ok!\n");

    ///成交量类垄1�7
    string volumeCondition = orderCfg["VolumeCondition"].get<string>();
    order2.VolumeCondition = volumeCondition[0];
//    printf("VolumeCondition fill ok!\n");

    order2.MinVolume = orderCfg["MinVolume"].get<int>();
//    printf("MinVolume fill ok!\n");

    ///触发条件
    string contingentCondition = orderCfg["ContingentCondition"].get<string>();
    order2.ContingentCondition = contingentCondition[0];
//    printf("ContingentCondition fill ok!\n");

    ///止损仄1�7 TThostFtdcPriceType double
    order2.StopPrice = orderCfg["StopPrice"].get<double>();
//    printf("StopPrice fill ok!\n");

    ///强平原因 TThostFtdcForceCloseReasonType char
    string forceCloseReason = orderCfg["ForceCloseReason"].get<string>();
    order2.ForceCloseReason = forceCloseReason[0];
//    printf("ForceCloseReason fill ok!\n");

    ///自动挂起标志 TThostFtdcBoolType int
    order2.IsAutoSuspend = orderCfg["IsAutoSuspend"].get<int>();
//    printf("IsAutoSuspend fill ok!\n");

    ///业务单元 TThostFtdcBusinessUnitType char[21]
    string businessUnit = orderCfg["BusinessUnit"].get<string>();
    strcpy(order2.BusinessUnit, businessUnit.c_str());
//    printf("BusinessUnit fill ok!\n");

    ///请求编号 TThostFtdcRequestIDType int
    order2.RequestID = ++requestID2;
//    printf("RequestID fill ok!\n");

    ///用户强评标志 TThostFtdcBoolType int
    order2.UserForceClose = orderCfg["UserForceClose"].get<int>();
//    printf("UserForceClose fill ok!\n");

    ///互换单标忄1�7 TThostFtdcBoolType int
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
        ERROR_LOG("Direction from strategy error!"); // @suppress("Invalid arguments")
        return false;
    }
    if (Direction == BUY1_SELL2) {           //鍚堢宄1�7�帮紝鍚堢害2鍗�
    //  direction_flag = {'0','1'};
        direction_flag[0] = BUY_;
        direction_flag[1] = SELL_;
    }
    else if (Direction == SELL1_BUY2) {  //鍚堢宄1�7�栵紝鍚堢害2涔�
   //    direction_flag = {'1','0'};
        direction_flag[0] = SELL_;
        direction_flag[1] = BUY_;
    }
    return true;
}
