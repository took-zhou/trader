//#include "utils.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <cstring>
//#include <unistd.h>
//#include "getconfig.h"
//#include <string>
//#include "PrintStruct.h"
//#include "define.h"
//#include <semaphore.h>
//// #include "ThostFtdcTraderApi.h"
//#include "trade2strategy_if_ipc.h"
//#include "trade2market_if_ipc.h"
//#include "main.h"
//extern sem_t sem;
//
//void error(const char *msg)
//{
//    perror(msg);
//    exit(1);
//}
//bool buildCommonOrder(CThostFtdcInputOrderField *requestData, TThostFtdcOrderRefType jjj){
//    //static int requestID;
//    /***********************/
//    ///缁忕邯鍏徃浠ｇ爜 TThostFtdcBrokerIDType char[11]
//    strcpy(requestData->BrokerID, getConfig("trade", "BrokerID").c_str());
//
//    ///鎶曡祫鑰呬唬鐮� TThostFtdcInvestorIDType char[13]
//    strcpy(requestData->InvestorID, getConfig("trade", "InvestorID").c_str());
//
//
//    ///鎶ュ崟寮曠敤 TThostFtdcOrderRefType char[13]
//    //strcpy(requestData->OrderRef,"000000000001");
//    strcpy(requestData->OrderRef, jjj);
//
//    ///鐢ㄦ埛浠ｇ爜 TThostFtdcUserIDType char[16]
//    strcpy(requestData->UserID, getConfig("trade", "UserID").c_str());
//
//	strcpy(requestData->ExchangeID, "DCE");
//    ///鎶ュ崟浠锋牸鏉′欢 TThostFtdcOrderPriceTypeType char
//    //// THOST_FTDC_OPT_AnyPrice '1' 浠绘剰浠�
//    //// THOST_FTDC_OPT_LimitPrice '2' 闄愪环
//    //// THOST_FTDC_OPT_BestPrice '3' 鏈�浼樹环
//    //// THOST_FTDC_OPT_LastPrice '4' 鏈�鏂颁环
//    //// THOST_FTDC_OPT_LastPricePlusOneTicks '5' 鏈�鏂颁环娴姩涓婃诞1涓猼icks
//    //// THOST_FTDC_OPT_LastPricePlusTwoTicks '6' 鏈�鏂颁环娴姩涓婃诞2涓猼icks
//    //// THOST_FTDC_OPT_LastPricePlusThreeTicks '7' 鏈�鏂颁环娴姩涓婃诞3涓猼icks
//    //// THOST_FTDC_OPT_AskPrice1 '8' 鍗栦竴浠�
//    //// THOST_FTDC_OPT_AskPrice1PlusOneTicks '9' 鍗栦竴浠锋诞鍔ㄤ笂娴�猼icks
//    //// THOST_FTDC_OPT_AskPrice1PlusTwoTicks 'A' 鍗栦竴浠锋诞鍔ㄤ笂娴�猼icks
//    //// THOST_FTDC_OPT_AskPrice1PlusThreeTicks 'B' 鍗栦竴浠锋诞鍔ㄤ笂娴�猼icks
//    //// THOST_FTDC_OPT_BidPrice1 'C' 涔颁竴浠�
//    //// THOST_FTDC_OPT_BidPrice1PlusOneTicks 'D' 涔颁竴浠锋诞鍔ㄤ笂娴�猼icks
//    //// THOST_FTDC_OPT_BidPrice1PlusTwoTicks 'E' 涔颁竴浠锋诞鍔ㄤ笂娴�猼icks
//    //// THOST_FTDC_OPT_BidPrice1PlusThreeTicks 'F' 涔颁竴浠锋诞鍔ㄤ笂娴�猼icks
//    requestData->OrderPriceType = '2';
//
//    ///缁勫悎寮�骞虫爣蹇� TThostFtdcCombOffsetFlagType char[5]
//    //// THOST_FTDC_OF_Open '0' 寮�浠�
//    //// THOST_FTDC_OF_Close '1' 骞充粨
//    //// THOST_FTDC_OF_ForceClose '2' 寮哄钩
//    //// THOST_FTDC_OF_CloseToday '3' 骞充粖
//    //// THOST_FTDC_OF_CloseYesterday '4' 骞虫槰
//    //// THOST_FTDC_OF_ForceOff '5' 寮哄噺
//    //// THOST_FTDC_OF_LocalForceClose '6' 鏈湴寮哄钩
//    //strcpy(requestData->CombOffsetFlag[0], THOST_FTDC_OF_Open);
//	requestData->CombOffsetFlag[0] = THOST_FTDC_OF_Open;
//
//    ///缁勫悎鎶曟満濂椾繚鏍囧織 TThostFtdcCombHedgeFlagType char[5]
//    //// THOST_FTDC_HF_Speculation '1' 鎶曟満
//    //// THOST_FTDC_HF_Arbitrage '2' 濂楀埄
//    //// THOST_FTDC_HF_Hedge '3' 濂椾繚
//    strcpy(requestData->CombHedgeFlag,"1");
//
//
//    ///鏈夋晥鏈熺被鍨� TThostFtdcTimeConditionType char
//    //// THOST_FTDC_TC_IOC '1' 绔嬪嵆瀹屾垚锛屽惁鍒欐挙閿�
//    //// THOST_FTDC_TC_GFS '2' 鏈妭鏈夋晥
//    //// THOST_FTDC_TC_GFD '3' 褰撴棩鏈夋晥
//    //// THOST_FTDC_TC_GTD '4' 鎸囧畾鏃ユ湡鍓嶆湁鏁�
//    //// THOST_FTDC_TC_GTC '5' 鎾ら攢鍓嶆湁鏁�
//    //// THOST_FTDC_TC_GFA '6' 闆嗗悎绔炰环鏈夋晥
//    requestData->TimeCondition = THOST_FTDC_TC_GFS;
//
//    ///GTD鏃ユ湡 TThostFtdcDateType char[9]
//    strcpy(requestData->GTDDate,"");
//
//    ///鎴愪氦閲忕被鍨� TThostFtdcVolumeConditionType char
//    //// THOST_FTDC_VC_AV '1' 浠讳綍鏁伴噺
//    //// THOST_FTDC_VC_MV '2' 鏈�灏忔暟閲�
//    //// THOST_FTDC_VC_CV '3' 鍏ㄩ儴鏁伴噺
//    requestData->VolumeCondition = THOST_FTDC_VC_CV;
//
//    ///鏈�灏忔垚浜ら噺 TThostFtdcVolumeType int
//    requestData->MinVolume = 1;
//
//    ///瑙﹀彂鏉′欢 TThostFtdcContingentConditionType char
//    //// THOST_FTDC_CC_Immediately '1' 绔嬪嵆
//    //// THOST_FTDC_CC_Touch '2' 姝㈡崯
//    //// THOST_FTDC_CC_TouchProfit '3' 姝㈣耽
//    //// THOST_FTDC_CC_ParkedOrder '4' 棰勫煁鍗�
//    //// THOST_FTDC_CC_LastPriceGreaterThanStopPrice '5' 鏈�鏂颁环澶т簬鏉′欢浠�
//    //// THOST_FTDC_CC_LastPriceGreaterEqualStopPrice '6' 鏈�鏂颁环澶т簬绛変簬鏉′欢浠�
//    //// THOST_FTDC_CC_LastPriceLesserThanStopPrice '7' 鏈�鏂颁环灏忎簬鏉′欢浠�
//    //// THOST_FTDC_CC_LastPriceLesserEqualStopPrice '8' 鏈�鏂颁环灏忎簬绛変簬鏉′欢浠�
//    //// THOST_FTDC_CC_AskPriceGreaterThanStopPrice '9' 鍗栦竴浠峰ぇ浜庢潯浠朵环
//    //// THOST_FTDC_CC_AskPriceGreaterEqualStopPrice 'A' 鍗栦竴浠峰ぇ浜庣瓑浜庢潯浠朵环
//    //// THOST_FTDC_CC_AskPriceLesserThanStopPrice 'B' 鍗栦竴浠峰皬浜庢潯浠朵环
//    //// THOST_FTDC_CC_AskPriceLesserEqualStopPrice 'C' 鍗栦竴浠峰皬浜庣瓑浜庢潯浠朵环
//    //// THOST_FTDC_CC_BidPriceGreaterThanStopPrice 'D' 涔颁竴浠峰ぇ浜庢潯浠朵环
//    //// THOST_FTDC_CC_BidPriceGreaterEqualStopPrice 'E' 涔颁竴浠峰ぇ浜庣瓑浜庢潯浠朵环
//    //// THOST_FTDC_CC_BidPriceLesserThanStopPrice 'F' 涔颁竴浠峰皬浜庢潯浠朵环
//    //// THOST_FTDC_CC_BidPriceLesserEqualStopPrice 'H' 涔颁竴浠峰皬浜庣瓑浜庢潯浠朵环
//    requestData->ContingentCondition = THOST_FTDC_CC_Immediately;
//
//    ///姝㈡崯浠� TThostFtdcPriceType double
//    requestData->StopPrice = 0;
//
//    ///寮哄钩鍘熷洜 TThostFtdcForceCloseReasonType char
//    //// THOST_FTDC_FCC_NotForceClose '0' 闈炲己骞�
//    //// THOST_FTDC_FCC_LackDeposit '1' 璧勯噾涓嶈冻
//    //// THOST_FTDC_FCC_ClientOverPositionLimit '2' 瀹㈡埛瓒呬粨
//    //// THOST_FTDC_FCC_MemberOverPositionLimit '3' 浼氬憳瓒呬粨
//    //// THOST_FTDC_FCC_NotMultiple '4' 鎸佷粨闈炴暣鏁板��
//    //// THOST_FTDC_FCC_Violation '5' 杩濊
//    //// THOST_FTDC_FCC_Other '6' 鍏跺畠
//    //// THOST_FTDC_FCC_PersonDeliv '7' 鑷劧浜轰复杩戜氦鍓�
//    requestData->ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
//
//    ///鑷姩鎸傝捣鏍囧織 TThostFtdcBoolType int
//    requestData->IsAutoSuspend = 0;
//
//    ///涓氬姟鍗曞厓 TThostFtdcBusinessUnitType char[21]
//    strcpy(requestData->BusinessUnit,"");
//
//    ///璇锋眰缂栧彿 TThostFtdcRequestIDType int
//    //requestData->RequestID = ++requestID;
//
//    ///鐢ㄦ埛寮鸿瘎鏍囧織 TThostFtdcBoolType int
//    requestData->UserForceClose = 0;
//
//    ///浜掓崲鍗曟爣蹇� TThostFtdcBoolType int
//    requestData->IsSwapOrder = 0;
//
//	return true;
//}
//bool build_setting_part_order(CThostFtdcInputOrderField *request_order,
//     Contract *setting_order, char direction
// ){
//    ///涔板崠鏂瑰悜 TThostFtdcDirectionType char
//    //// THOST_FTDC_D_Buy '0' 涔�
//    //// THOST_FTDC_D_Sell '1' 鍗�
//
//    request_order->Direction = direction;
//
//    ///鍚堢害浠ｇ爜 TThostFtdcInstrumentIDType char[31]
//    char * instrumentID = setting_order->InstrumentID;
//    strcpy(request_order->InstrumentID,instrumentID);
//    ///浠锋牸 TThostFtdcPriceType double
//    request_order->LimitPrice = setting_order->LimitPrice;
//    ///鏁伴噺 TThostFtdcVolumeType int
//    request_order->VolumeTotalOriginal = setting_order->VolumeTotalOriginal;
//	return true;
//}
//
//void dealOrder_SA_TEST(CTraderApi *pTraderApi)
//{
//    ARBITRAGE_ORDER *part_order = new(ARBITRAGE_ORDER);
//    memset(part_order,0,sizeof(ARBITRAGE_ORDER));
//	IPCStream_Read_ARBITRAGE_ORDER_t2S(part_order);
//    printARBITRAGE_ORDER(part_order);
//}
//bool determineBuyAndSaleDirection(const ARBITRAGE_ORDER &part_order,char *direction_flag)
//{
//	bool error_flag = false;
//	if (part_order.Direction == ARBITRAGEDIRECTION_BUY1_SELL2) {           //鍚堢害1涔帮紝鍚堢害2鍗�
//	//  direction_flag = {'0','1'};
//		direction_flag[0] = BUY;
//		direction_flag[1] = SELL;
//	}
//	else if (part_order.Direction == ARBITRAGEDIRECTION_SELL1_BUY2) {  //鍚堢害1鍗栵紝鍚堢害2涔�
//   //    direction_flag = {'1','0'};
//		direction_flag[0] = SELL;
//		direction_flag[1] = BUY;
//	}
//	else
//	{
//		error_flag = true;
//	}
//	return !error_flag;
//}
//bool dealOrder_SA(int sock, CSimpleHandler& pTraderApi){
//    int n;
//    char direction_flag[2] = {'0','0'};
//    static int RequestID = 0;
//    /*****************鏂板缓涓�涓粨鏋勪綋绌洪棿*************************/
//
//
//    //ARBITRAGE_ORDER *part_order = new(ARBITRAGE_ORDER);
//    //CThostFtdcInputOrderField *inputOrder_st_contract1= new(CThostFtdcInputOrderField);
//    //CThostFtdcInputOrderField *inputOrder_st_contract2= new(CThostFtdcInputOrderField);
//
//	ARBITRAGE_ORDER part_order;
//	CThostFtdcInputOrderField inputOrder_st_contract1;
//	CThostFtdcInputOrderField inputOrder_st_contract2;
//
//
//    Contract SettingPart;
//
//    memset(&part_order,0,sizeof(ARBITRAGE_ORDER));
//    memset(&inputOrder_st_contract1, 0, sizeof(CThostFtdcInputOrderField));
//    memset(&inputOrder_st_contract2, 0, sizeof(CThostFtdcInputOrderField));
//    memset(&SettingPart,0,sizeof(Contract));
//    printf("memset ok!\n");
//    /****************************************************************/
//    /******************浠巗ocket璇诲彇鏁版嵁骞惰В鏋愬埌鐩稿簲鐨勭粨鏋勪綋*************/
//
//    n = read(sock,&part_order,sizeof(ARBITRAGE_ORDER));
//
//    /****************缁檚ocket鍥炲簲********************/
//  /*  n = write(sock, "I got your message", 18);
//    if (n < 0)
//        error("ERROR writing to socket");
//        */
//    #ifdef DEBUG
//        printARBITRAGE_ORDER(&part_order);
//    #endif
//    std::cout<<"here1"<<std::endl;
//    /**********************鏍规嵁鎺ユ敹鍒扮殑鏁版嵁杩涜瀵瑰簲鐨勬搷浣�**************************/
//	if (!determineBuyAndSaleDirection(part_order, direction_flag))
//	{
//		std::cout << "determineBuyAndSaleDirection error!" << std::endl;
//	}
//	std::cout << "here1-2" << std::endl;
//	if (!buildCommonOrder(&inputOrder_st_contract1,"000000000001"))
//	{
//		std::cout << "buildCommonOrder of inputOrder_st_contract1 error!" << std::endl;
//	}
//	std::cout << "here1-3" << std::endl;
//	if (!buildCommonOrder(&inputOrder_st_contract2,"000000000002"))
//	{
//		std::cout << "buildCommonOrder of inputOrder_st_contract2 error!" << std::endl;
//	}
//	std::cout << "here1-4" << std::endl;
//    /*******鍚堢害1鎿嶄綔******/
//	std::cout << "here2" << std::endl;
//    strcpy(SettingPart.InstrumentID,part_order.InstrumentID1);
//	std::cout << "here3" << std::endl;
//    SettingPart.LimitPrice = part_order.LimitPrice1;
//    SettingPart.VolumeTotalOriginal= part_order.VolumeTotalOriginal1;
//    build_setting_part_order(&inputOrder_st_contract1,&SettingPart,direction_flag[0]);
//
//    #ifdef DEBUG
//        printCThostFtdcInputOrderField(&inputOrder_st_contract1);
//    #endif
//
//    #ifdef TRADE
//        //result = pTraderApi->ReqOrderInsert(&inputOrder_st_contract1,RequestID);
//        pTraderApi.ReqOrderInsert_Ordinary_hai(inputOrder_st_contract1,RequestID);
//        sem_wait(&sem);
//        RequestID++;
//    #endif
//
//    /*******鍚堢害2鎿嶄綔*******/
//    strcpy(SettingPart.InstrumentID,part_order.InstrumentID2);
//    SettingPart.LimitPrice = part_order.LimitPrice2;
//    SettingPart.VolumeTotalOriginal = part_order.VolumeTotalOriginal2;
//    build_setting_part_order(&inputOrder_st_contract2,&SettingPart,direction_flag[1]);
//
//    #ifdef DEBUG
//        printCThostFtdcInputOrderField(&inputOrder_st_contract2);
//    #endif
//
//    #ifdef TRADE
//        pTraderApi.ReqOrderInsert_Ordinary_hai(inputOrder_st_contract2,RequestID);
//        sem_wait(&sem);
//        RequestID++;
//    #endif
//    std::cout<<"Order insert ok!" << std::endl;
//   /******************閲婃斁绌洪棿*************************/
//    //delete part_order;
//    //delete inputOrder_st_contract1;
//    //delete inputOrder_st_contract2;
//	return true;
//}
//
//void dealOrder_GDF(int sock, CSimpleHandler &pTraderApi) {
//	int n;
//	GDF_STRUCT infor_from_socket = { 0 };
//	memset(&infor_from_socket, 0, sizeof(GDF_STRUCT));
//	printf("sizeof(infor_from_socket) is %d\n",sizeof(GDF_STRUCT));
//	//printGDF_STRUCT(&infor_from_socket);
//
//	n = read(sock, &infor_from_socket, sizeof(GDF_STRUCT));
//	n = write(sock, "I got your message", 18);
//	if (n < 0)
//		error("ERROR writing to socket");
//
//	printGDF_STRUCT(&infor_from_socket);
//
//}
//
//
///***************************淇℃伅鏌ヨ***********************************/
//void ShowCommonInfor(CThostFtdcTraderApi *pTraderApi){
//    const char *version = pTraderApi->GetApiVersion();
//    const char *tradeday = pTraderApi->GetTradingDay();
//    std::cout<<tradeday<<std::endl;
//
//}
//
//// //鏌ヨ处鎴烽噷鐨勯挶
//// void QueryAccount(CThostFtdcTraderApi *pTraderApi){
////     int result = pTraderApi->ReqQryTradingAccount(&requestData,RequestId++);
//// }
//
//
//// //鏌ユ姇璧勮�呬俊鎭�
//// void QueryInvestor(CThostFtdcTraderApi *pTraderApi,int RequestId){
////     pTraderApi->ReqQryInvestor(,RequestId);
//// }
//
//
///**************************************************************/
//
//
//
//
//
///*****************************************************/
