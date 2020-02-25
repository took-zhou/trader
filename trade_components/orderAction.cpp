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

bool OrderManage::fillOrder()
{
        INFO_LOG("begin to fill order"); // @suppress("Invalid arguments")
        ///缁忕邯鍏徃浠ｇ爜 TThostFtdcBrokerIDType char[11]
        strcpy(order.BrokerID, getConfig("trade", "BrokerID").c_str());

        ///鎶曡祫鑰呬唬鐮� TThostFtdcInvestorIDType char[13]
        strcpy(order.InvestorID, getConfig("trade", "InvestorID").c_str());

        ///鎶ュ崟寮曠敤 TThostFtdcOrderRefType char[13]
        // strcpy(order.OrderRef,"000000000001");
        strcpy(order.OrderRef, "000000000001");

        ///鐢ㄦ埛浠ｇ爜 TThostFtdcUserIDType char[16]
        strcpy(order.UserID, getConfig("trade", "UserID").c_str());

        strcpy(order.ExchangeID, "DCE");
        ///鎶ュ崟浠锋牸鏉′欢 TThostFtdcOrderPriceTypeType char
        //// THOST_FTDC_OPT_AnyPrice '1' 浠绘剰浠�
        //// THOST_FTDC_OPT_LimitPrice '2' 闄愪环
        //// THOST_FTDC_OPT_BestPrice '3' 鏈�浼樹环
        //// THOST_FTDC_OPT_LastPrice '4' 鏈�鏂颁环
        //// THOST_FTDC_OPT_LastPricePlusOneTicks '5' 鏈�鏂颁环娴姩涓婃诞1涓猼icks
        //// THOST_FTDC_OPT_LastPricePlusTwoTicks '6' 鏈�鏂颁环娴姩涓婃诞2涓猼icks
        //// THOST_FTDC_OPT_LastPricePlusThreeTicks '7' 鏈�鏂颁环娴姩涓婃诞3涓猼icks
        //// THOST_FTDC_OPT_AskPrice1 '8' 鍗栦竴浠�
        //// THOST_FTDC_OPT_AskPrice1PlusOneTicks '9' 鍗栦竴浠锋诞鍔ㄤ笂娴�猼icks
        //// THOST_FTDC_OPT_AskPrice1PlusTwoTicks 'A' 鍗栦竴浠锋诞鍔ㄤ笂娴�猼icks
        //// THOST_FTDC_OPT_AskPrice1PlusThreeTicks 'B' 鍗栦竴浠锋诞鍔ㄤ笂娴�猼icks
        //// THOST_FTDC_OPT_BidPrice1 'C' 涔颁竴浠�
        //// THOST_FTDC_OPT_BidPrice1PlusOneTicks 'D' 涔颁竴浠锋诞鍔ㄤ笂娴�猼icks
        //// THOST_FTDC_OPT_BidPrice1PlusTwoTicks 'E' 涔颁竴浠锋诞鍔ㄤ笂娴�猼icks
        //// THOST_FTDC_OPT_BidPrice1PlusThreeTicks 'F' 涔颁竴浠锋诞鍔ㄤ笂娴�猼icks
        order.OrderPriceType = '2';

        ///缁勫悎寮�骞虫爣蹇� TThostFtdcCombOffsetFlagType char[5]
        //// THOST_FTDC_OF_Open '0' 寮�浠�
        //// THOST_FTDC_OF_Close '1' 骞充粨
        //// THOST_FTDC_OF_ForceClose '2' 寮哄钩
        //// THOST_FTDC_OF_CloseToday '3' 骞充粖
        //// THOST_FTDC_OF_CloseYesterday '4' 骞虫槰
        //// THOST_FTDC_OF_ForceOff '5' 寮哄噺
        //// THOST_FTDC_OF_LocalForceClose '6' 鏈湴寮哄钩
        //strcpy(order.CombOffsetFlag[0], THOST_FTDC_OF_Open);
        order.CombOffsetFlag[0] = THOST_FTDC_OF_Open;

        ///缁勫悎鎶曟満濂椾繚鏍囧織 TThostFtdcCombHedgeFlagType char[5]
        //// THOST_FTDC_HF_Speculation '1' 鎶曟満
        //// THOST_FTDC_HF_Arbitrage '2' 濂楀埄
        //// THOST_FTDC_HF_Hedge '3' 濂椾繚
        strcpy(order.CombHedgeFlag, "1");


        ///鏈夋晥鏈熺被鍨� TThostFtdcTimeConditionType char
        //// THOST_FTDC_TC_IOC '1' 绔嬪嵆瀹屾垚锛屽惁鍒欐挙閿�
        //// THOST_FTDC_TC_GFS '2' 鏈妭鏈夋晥
        //// THOST_FTDC_TC_GFD '3' 褰撴棩鏈夋晥
        //// THOST_FTDC_TC_GTD '4' 鎸囧畾鏃ユ湡鍓嶆湁鏁�
        //// THOST_FTDC_TC_GTC '5' 鎾ら攢鍓嶆湁鏁�
        //// THOST_FTDC_TC_GFA '6' 闆嗗悎绔炰环鏈夋晥
        order.TimeCondition = THOST_FTDC_TC_GFS;

        ///GTD鏃ユ湡 TThostFtdcDateType char[9]
        strcpy(order.GTDDate, "");

        ///鎴愪氦閲忕被鍨� TThostFtdcVolumeConditionType char
        //// THOST_FTDC_VC_AV '1' 浠讳綍鏁伴噺
        //// THOST_FTDC_VC_MV '2' 鏈�灏忔暟閲�
        //// THOST_FTDC_VC_CV '3' 鍏ㄩ儴鏁伴噺
        order.VolumeCondition = THOST_FTDC_VC_CV;

        ///鏈�灏忔垚浜ら噺 TThostFtdcVolumeType int
        order.MinVolume = 1;

        ///瑙﹀彂鏉′欢 TThostFtdcContingentConditionType char
        //// THOST_FTDC_CC_Immediately '1' 绔嬪嵆
        //// THOST_FTDC_CC_Touch '2' 姝㈡崯
        //// THOST_FTDC_CC_TouchProfit '3' 姝㈣耽
        //// THOST_FTDC_CC_ParkedOrder '4' 棰勫煁鍗�
        //// THOST_FTDC_CC_LastPriceGreaterThanStopPrice '5' 鏈�鏂颁环澶т簬鏉′欢浠�
        //// THOST_FTDC_CC_LastPriceGreaterEqualStopPrice '6' 鏈�鏂颁环澶т簬绛変簬鏉′欢浠�
        //// THOST_FTDC_CC_LastPriceLesserThanStopPrice '7' 鏈�鏂颁环灏忎簬鏉′欢浠�
        //// THOST_FTDC_CC_LastPriceLesserEqualStopPrice '8' 鏈�鏂颁环灏忎簬绛変簬鏉′欢浠�
        //// THOST_FTDC_CC_AskPriceGreaterThanStopPrice '9' 鍗栦竴浠峰ぇ浜庢潯浠朵环
        //// THOST_FTDC_CC_AskPriceGreaterEqualStopPrice 'A' 鍗栦竴浠峰ぇ浜庣瓑浜庢潯浠朵环
        //// THOST_FTDC_CC_AskPriceLesserThanStopPrice 'B' 鍗栦竴浠峰皬浜庢潯浠朵环
        //// THOST_FTDC_CC_AskPriceLesserEqualStopPrice 'C' 鍗栦竴浠峰皬浜庣瓑浜庢潯浠朵环
        //// THOST_FTDC_CC_BidPriceGreaterThanStopPrice 'D' 涔颁竴浠峰ぇ浜庢潯浠朵环
        //// THOST_FTDC_CC_BidPriceGreaterEqualStopPrice 'E' 涔颁竴浠峰ぇ浜庣瓑浜庢潯浠朵环
        //// THOST_FTDC_CC_BidPriceLesserThanStopPrice 'F' 涔颁竴浠峰皬浜庢潯浠朵环
        //// THOST_FTDC_CC_BidPriceLesserEqualStopPrice 'H' 涔颁竴浠峰皬浜庣瓑浜庢潯浠朵环
        order.ContingentCondition = THOST_FTDC_CC_Immediately;

        ///姝㈡崯浠� TThostFtdcPriceType double
        order.StopPrice = 0;

        ///寮哄钩鍘熷洜 TThostFtdcForceCloseReasonType char
        //// THOST_FTDC_FCC_NotForceClose '0' 闈炲己骞�
        //// THOST_FTDC_FCC_LackDeposit '1' 璧勯噾涓嶈冻
        //// THOST_FTDC_FCC_ClientOverPositionLimit '2' 瀹㈡埛瓒呬粨
        //// THOST_FTDC_FCC_MemberOverPositionLimit '3' 浼氬憳瓒呬粨
        //// THOST_FTDC_FCC_NotMultiple '4' 鎸佷粨闈炴暣鏁板��
        //// THOST_FTDC_FCC_Violation '5' 杩濊
        //// THOST_FTDC_FCC_Other '6' 鍏跺畠
        //// THOST_FTDC_FCC_PersonDeliv '7' 鑷劧浜轰复杩戜氦鍓�
        order.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;

        ///鑷姩鎸傝捣鏍囧織 TThostFtdcBoolType int
        order.IsAutoSuspend = 0;

        ///涓氬姟鍗曞厓 TThostFtdcBusinessUnitType char[21]
        strcpy(order.BusinessUnit, "");

        ///璇锋眰缂栧彿 TThostFtdcRequestIDType int
        //order.RequestID = ++requestID;

        ///鐢ㄦ埛寮鸿瘎鏍囧織 TThostFtdcBoolType int
        order.UserForceClose = 0;

        ///浜掓崲鍗曟爣蹇� TThostFtdcBoolType int
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
    ///缁忕邯鍏徃浠ｇ爜 TThostFtdcBrokerIDType char[11]
    INFO_LOG("begin to fill order"); // @suppress("Invalid arguments")
    string brokerId = orderCfg["BrokerID"].get<string>();
    strcpy(order.BrokerID, brokerId.c_str());
    printf("BrokerID fill ok!\n");

    ///鎶曡祫鑰呬唬鐮� TThostFtdcInvestorIDType char[13]
    string investorId = orderCfg["InvestorID"].get<string>();
    strcpy(order.InvestorID, investorId.c_str());
    printf("InvestorID fill ok!\n");

    ///鎶ュ崟寮曠敤 TThostFtdcOrderRefType char[13]

    //strcpy(order.OrderRef,"000000000001");
    string orderRef = genOrderRef();
    strcpy(order.OrderRef, orderRef.c_str());
    printf("OrderRef fill ok!\n");

    ///鐢ㄦ埛浠ｇ爜 TThostFtdcUserIDType char[16]
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
    ///鎶ュ崟浠锋牸鏉′欢 TThostFtdcOrderPriceTypeType char
    //// THOST_FTDC_OPT_AnyPrice '1' 浠绘剰浠�
    //// THOST_FTDC_OPT_LimitPrice '2' 闄愪环
    //// THOST_FTDC_OPT_BestPrice '3' 鏈�浼樹环
    //// THOST_FTDC_OPT_LastPrice '4' 鏈�鏂颁环
    //// THOST_FTDC_OPT_LastPricePlusOneTicks '5' 鏈�鏂颁环娴姩涓婃诞1涓猼icks
    //// THOST_FTDC_OPT_LastPricePlusTwoTicks '6' 鏈�鏂颁环娴姩涓婃诞2涓猼icks
    //// THOST_FTDC_OPT_LastPricePlusThreeTicks '7' 鏈�鏂颁环娴姩涓婃诞3涓猼icks
    //// THOST_FTDC_OPT_AskPrice1 '8' 鍗栦竴浠�
    //// THOST_FTDC_OPT_AskPrice1PlusOneTicks '9' 鍗栦竴浠锋诞鍔ㄤ笂娴�猼icks
    //// THOST_FTDC_OPT_AskPrice1PlusTwoTicks 'A' 鍗栦竴浠锋诞鍔ㄤ笂娴�猼icks
    //// THOST_FTDC_OPT_AskPrice1PlusThreeTicks 'B' 鍗栦竴浠锋诞鍔ㄤ笂娴�猼icks
    //// THOST_FTDC_OPT_BidPrice1 'C' 涔颁竴浠�
    //// THOST_FTDC_OPT_BidPrice1PlusOneTicks 'D' 涔颁竴浠锋诞鍔ㄤ笂娴�猼icks
    //// THOST_FTDC_OPT_BidPrice1PlusTwoTicks 'E' 涔颁竴浠锋诞鍔ㄤ笂娴�猼icks
    //// THOST_FTDC_OPT_BidPrice1PlusThreeTicks 'F' 涔颁竴浠锋诞鍔ㄤ笂娴�猼icks
    string orderPrinceType = orderCfg["OrderPriceType"].get<string>();
    order.OrderPriceType = orderPrinceType[0];
    printf("OrderPriceType fill ok!\n");

    ///缁勫悎寮�骞虫爣蹇� TThostFtdcCombOffsetFlagType char[5]
    //// THOST_FTDC_OF_Open '0' 寮�浠�
    //// THOST_FTDC_OF_Close '1' 骞充粨
    //// THOST_FTDC_OF_ForceClose '2' 寮哄钩
    //// THOST_FTDC_OF_CloseToday '3' 骞充粖
    //// THOST_FTDC_OF_CloseYesterday '4' 骞虫槰
    //// THOST_FTDC_OF_ForceOff '5' 寮哄噺
    //// THOST_FTDC_OF_LocalForceClose '6' 鏈湴寮哄钩
    //strcpy(order.CombOffsetFlag[0], THOST_FTDC_OF_Open);
    string combOffsetFlag = orderCfg["CombOffsetFlag"].get<string>();
    order.CombOffsetFlag[0] = combOffsetFlag[0];
    printf("CombOffsetFlag fill ok!\n");

    ///缁勫悎鎶曟満濂椾繚鏍囧織 TThostFtdcCombHedgeFlagType char[5]
    //// THOST_FTDC_HF_Speculation '1' 鎶曟満
    //// THOST_FTDC_HF_Arbitrage '2' 濂楀埄
    //// THOST_FTDC_HF_Hedge '3' 濂椾繚
    string combHedgeFlag = orderCfg["CombHedgeFlag"].get<string>();
    strcpy(order.CombHedgeFlag, combHedgeFlag.c_str());
    printf("CombHedgeFlag fill ok!\n");


    ///鏈夋晥鏈熺被鍨� TThostFtdcTimeConditionType char
    //// THOST_FTDC_TC_IOC '1' 绔嬪嵆瀹屾垚锛屽惁鍒欐挙閿�
    //// THOST_FTDC_TC_GFS '2' 鏈妭鏈夋晥
    //// THOST_FTDC_TC_GFD '3' 褰撴棩鏈夋晥
    //// THOST_FTDC_TC_GTD '4' 鎸囧畾鏃ユ湡鍓嶆湁鏁�
    //// THOST_FTDC_TC_GTC '5' 鎾ら攢鍓嶆湁鏁�
    //// THOST_FTDC_TC_GFA '6' 闆嗗悎绔炰环鏈夋晥
    string timeCondition = orderCfg["TimeCondition"].get<string>();
    order.TimeCondition = timeCondition[0];
    printf("TimeCondition fill ok!\n");

    ///GTD鏃ユ湡 TThostFtdcDateType char[9]
    string gTDDate = orderCfg["GTDDate"].get<string>();
    strcpy(order.GTDDate, gTDDate.c_str());
    printf("GTDDate fill ok!\n");

    ///鎴愪氦閲忕被鍨� TThostFtdcVolumeConditionType char
    //// THOST_FTDC_VC_AV '1' 浠讳綍鏁伴噺
    //// THOST_FTDC_VC_MV '2' 鏈�灏忔暟閲�
    //// THOST_FTDC_VC_CV '3' 鍏ㄩ儴鏁伴噺
    string volumeCondition = orderCfg["VolumeCondition"].get<string>();
    order.VolumeCondition = volumeCondition[0];
    printf("VolumeCondition fill ok!\n");

    ///鏈�灏忔垚浜ら噺 TThostFtdcVolumeType int
    order.MinVolume = orderCfg["MinVolume"].get<int>();
    printf("MinVolume fill ok!\n");

    ///瑙﹀彂鏉′欢 TThostFtdcContingentConditionType char
    //// THOST_FTDC_CC_Immediately '1' 绔嬪嵆
    //// THOST_FTDC_CC_Touch '2' 姝㈡崯
    //// THOST_FTDC_CC_TouchProfit '3' 姝㈣耽
    //// THOST_FTDC_CC_ParkedOrder '4' 棰勫煁鍗�
    //// THOST_FTDC_CC_LastPriceGreaterThanStopPrice '5' 鏈�鏂颁环澶т簬鏉′欢浠�
    //// THOST_FTDC_CC_LastPriceGreaterEqualStopPrice '6' 鏈�鏂颁环澶т簬绛変簬鏉′欢浠�
    //// THOST_FTDC_CC_LastPriceLesserThanStopPrice '7' 鏈�鏂颁环灏忎簬鏉′欢浠�
    //// THOST_FTDC_CC_LastPriceLesserEqualStopPrice '8' 鏈�鏂颁环灏忎簬绛変簬鏉′欢浠�
    //// THOST_FTDC_CC_AskPriceGreaterThanStopPrice '9' 鍗栦竴浠峰ぇ浜庢潯浠朵环
    //// THOST_FTDC_CC_AskPriceGreaterEqualStopPrice 'A' 鍗栦竴浠峰ぇ浜庣瓑浜庢潯浠朵环
    //// THOST_FTDC_CC_AskPriceLesserThanStopPrice 'B' 鍗栦竴浠峰皬浜庢潯浠朵环
    //// THOST_FTDC_CC_AskPriceLesserEqualStopPrice 'C' 鍗栦竴浠峰皬浜庣瓑浜庢潯浠朵环
    //// THOST_FTDC_CC_BidPriceGreaterThanStopPrice 'D' 涔颁竴浠峰ぇ浜庢潯浠朵环
    //// THOST_FTDC_CC_BidPriceGreaterEqualStopPrice 'E' 涔颁竴浠峰ぇ浜庣瓑浜庢潯浠朵环
    //// THOST_FTDC_CC_BidPriceLesserThanStopPrice 'F' 涔颁竴浠峰皬浜庢潯浠朵环
    //// THOST_FTDC_CC_BidPriceLesserEqualStopPrice 'H' 涔颁竴浠峰皬浜庣瓑浜庢潯浠朵环
    string contingentCondition = orderCfg["ContingentCondition"].get<string>();
    order.ContingentCondition = contingentCondition[0];
    printf("ContingentCondition fill ok!\n");

    ///姝㈡崯浠� TThostFtdcPriceType double
    order.StopPrice = orderCfg["StopPrice"].get<double>();
    printf("StopPrice fill ok!\n");

    ///寮哄钩鍘熷洜 TThostFtdcForceCloseReasonType char
    //// THOST_FTDC_FCC_NotForceClose '0' 闈炲己骞�
    //// THOST_FTDC_FCC_LackDeposit '1' 璧勯噾涓嶈冻
    //// THOST_FTDC_FCC_ClientOverPositionLimit '2' 瀹㈡埛瓒呬粨
    //// THOST_FTDC_FCC_MemberOverPositionLimit '3' 浼氬憳瓒呬粨
    //// THOST_FTDC_FCC_NotMultiple '4' 鎸佷粨闈炴暣鏁板��
    //// THOST_FTDC_FCC_Violation '5' 杩濊
    //// THOST_FTDC_FCC_Other '6' 鍏跺畠
    //// THOST_FTDC_FCC_PersonDeliv '7' 鑷劧浜轰复杩戜氦鍓�
    string forceCloseReason = orderCfg["ForceCloseReason"].get<string>();
    order.ForceCloseReason = forceCloseReason[0];
    printf("ForceCloseReason fill ok!\n");

    ///鑷姩鎸傝捣鏍囧織 TThostFtdcBoolType int
    order.IsAutoSuspend = orderCfg["IsAutoSuspend"].get<int>();
    printf("IsAutoSuspend fill ok!\n");

    ///涓氬姟鍗曞厓 TThostFtdcBusinessUnitType char[21]
    string businessUnit = orderCfg["BusinessUnit"].get<string>();
    strcpy(order.BusinessUnit, businessUnit.c_str());
    printf("BusinessUnit fill ok!\n");

    ///璇锋眰缂栧彿 TThostFtdcRequestIDType int
    order.RequestID = ++requestID;
    printf("RequestID fill ok!\n");

    ///鐢ㄦ埛寮鸿瘎鏍囧織 TThostFtdcBoolType int
    order.UserForceClose = orderCfg["UserForceClose"].get<int>();
    printf("UserForceClose fill ok!\n");

    ///浜掓崲鍗曟爣蹇� TThostFtdcBoolType int
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

    ///鎴愪氦閲忕被鍨�
    string volumeCondition = orderCfg["VolumeCondition"].get<string>();
    order1.VolumeCondition = volumeCondition[0];
//    printf("VolumeCondition fill ok!\n");

    order1.MinVolume = orderCfg["MinVolume"].get<int>();
//    printf("MinVolume fill ok!\n");

    ///瑙﹀彂鏉′欢
    string contingentCondition = orderCfg["ContingentCondition"].get<string>();
    order1.ContingentCondition = contingentCondition[0];
//    printf("ContingentCondition fill ok!\n");

    ///姝㈡崯浠� TThostFtdcPriceType double
    order1.StopPrice = orderCfg["StopPrice"].get<double>();
//    printf("StopPrice fill ok!\n");

    ///寮哄钩鍘熷洜 TThostFtdcForceCloseReasonType char
    string forceCloseReason = orderCfg["ForceCloseReason"].get<string>();
    order1.ForceCloseReason = forceCloseReason[0];
//    printf("ForceCloseReason fill ok!\n");

    ///鑷姩鎸傝捣鏍囧織 TThostFtdcBoolType int
    order1.IsAutoSuspend = orderCfg["IsAutoSuspend"].get<int>();
//    printf("IsAutoSuspend fill ok!\n");

    ///涓氬姟鍗曞厓 TThostFtdcBusinessUnitType char[21]
    string businessUnit = orderCfg["BusinessUnit"].get<string>();
    strcpy(order1.BusinessUnit, businessUnit.c_str());
//    printf("BusinessUnit fill ok!\n");

    ///璇锋眰缂栧彿 TThostFtdcRequestIDType int
    order1.RequestID = ++requestID;
//    printf("RequestID fill ok!\n");

    ///鐢ㄦ埛寮鸿瘎鏍囧織 TThostFtdcBoolType int
    order1.UserForceClose = orderCfg["UserForceClose"].get<int>();
//    printf("UserForceClose fill ok!\n");

    ///浜掓崲鍗曟爣蹇� TThostFtdcBoolType int
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

    ///鎴愪氦閲忕被鍨�
    string volumeCondition = orderCfg["VolumeCondition"].get<string>();
    order2.VolumeCondition = volumeCondition[0];
//    printf("VolumeCondition fill ok!\n");

    order2.MinVolume = orderCfg["MinVolume"].get<int>();
//    printf("MinVolume fill ok!\n");

    ///瑙﹀彂鏉′欢
    string contingentCondition = orderCfg["ContingentCondition"].get<string>();
    order2.ContingentCondition = contingentCondition[0];
//    printf("ContingentCondition fill ok!\n");

    ///姝㈡崯浠� TThostFtdcPriceType double
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

    ///浜掓崲鍗曟爣蹇� TThostFtdcBoolType int
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
    if (Direction == BUY1_SELL2) {           //閸氬牏瀹�斿府绱濋崥鍫㈠2閸楋拷
    //  direction_flag = {'0','1'};
        direction_flag[0] = BUY_;
        direction_flag[1] = SELL_;
    }
    else if (Direction == SELL1_BUY2) {  //閸氬牏瀹�楁牭绱濋崥鍫㈠2娑旓拷
   //    direction_flag = {'1','0'};
        direction_flag[0] = SELL_;
        direction_flag[1] = BUY_;
    }
    return true;
}
