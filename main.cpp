//#include "stdafx.h"
#include <list>
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
//#include <windows.h>
#include <time.h>
#include "main.h"
#include <semaphore.h>
//#include <conio.h>
#include "getconfig.h"
#include "define.h"
#include <vector>
#include <map>
#include <time.h>
#include<limits>
#include "utils.h"
#include <fstream>

#include "log.h"
#include "trader.h"
#include "json.h"
#include "semaphorePart.h"
#include "trade_components/orderstates.h"
#include "trade_components/InfoShow.h"
using namespace std;

//FILE *logfile;
char str[300];
//sem_t sem;
//sem_t sem2;
//sem_t sem_login;
extern InsertResult repResult;
extern GlobalSem globalSem;
using json = nlohmann::json;
extern json AllInstruments;
extern CThostFtdcInstrumentField InstrumentInfo;
extern FillFlag fillFlag;
extern InfoShowFlag infoShowFlag;
std::ofstream outfile;
// 报单录入操作是否完成的标忄1�7
// Create a manual reset event with no signal
//HANDLE g_hEvent = CreateEvent(NULL, false, false, NULL);

/// 会员代码
TThostFtdcBrokerIDType g_chBrokerID;
/// 交易用户代码
TThostFtdcUserIDType g_chUserID;
/// 交易用户密码
TThostFtdcPasswordType g_chPassword;
/// 交易扄1�7代码
TThostFtdcExchangeIDType g_chExchangeID;
///合约代码
TThostFtdcInstrumentIDType	g_chInstrumentID;
///投资者代砄1�7
TThostFtdcInvestorIDType g_chInvestorID;
///预埋撤单编号
TThostFtdcParkedOrderActionIDType	g_chParkedOrderActionID1;
///预埋报单编号
TThostFtdcParkedOrderIDType	g_chParkedOrderID1;
///报单引用
TThostFtdcOrderRefType	g_chOrderRef;
///前置编号
TThostFtdcFrontIDType	g_chFrontID;
///会话编号
TThostFtdcSessionIDType	g_chSessionID;
///报单编号
TThostFtdcOrderSysIDType	g_chOrderSysID;
///止损仄1�7
TThostFtdcPriceType	g_chStopPrice;
///报价引用
TThostFtdcOrderRefType	g_chQuoteRef;
int FrontID = 0;
int SessionID = 0;
int Limitprice = 0;
int nRequestID = 0;
int chioce_action = 0;//丄1�7�全部报

vector<string> vector_OrderSysID;
vector<string> vector_ExchangeID;
vector<string> vector_InstrumentID;
vector<string> md_InstrumentID;
int action_number;

///执行宣告引用
TThostFtdcOrderRefType	g_NewExecOrderRef;
///执行宣告编号
TThostFtdcExecOrderSysIDType	g_NewExecOrderSysID;
///前置编号
TThostFtdcFrontIDType	g_NewFrontID;
///会话编号
TThostFtdcSessionIDType	g_NewSessionID;

//期权自对冲响应�1�7�知
///期权自对冲编叄1�7
TThostFtdcOrderSysIDType	g_chOptionSelfCloseSysID;
///期权自对冲引甄1�7
TThostFtdcOrderRefType	g_chOptionSelfCloseRef;
///用户端产品信恄1�7
TThostFtdcProductInfoType	g_chUserProductInfo;
///认证砄1�7
TThostFtdcAuthCodeType	g_chAuthCode;
///App代码
TThostFtdcAppIDType	g_chAppID;

//HANDLE xinhao = CreateEvent(NULL, false, false, NULL);

//CTraderApi *pUserApi = new CTraderApi;

//行情籄1�7

// 当客户端与交易托管系统建立起通信连接，客户端霄1�7要进行登彄1�7
void CSimpleMdHandler::OnFrontConnected()
{
    /*strcpy(g_chBrokerID, getConfig("config", "BrokerID").c_str());
    strcpy(g_chUserID, getConfig("config", "UserID").c_str());
    strcpy(g_chPassword, getConfig("config", "Password").c_str());*/
    ReqUserLogin();
}

void CSimpleMdHandler::ReqUserLogin()
{
    CThostFtdcReqUserLoginField reqUserLogin;
    int num = m_pUserMdApi->ReqUserLogin(&reqUserLogin, 0);
    LOGDEMO("\tlogin num = %d\n", num);
}

// 当客户端与交易托管系统�1�7�信连接断开时，该方法被调用
void CSimpleMdHandler::OnFrontDisconnected(int nReason)
{
    // 当发生这个情况后，API会自动重新连接，客户端可不做处理
    WARNING_LOG("<OnFrontDisconnected>");
    WARNING_LOG("\tnReason= = [%d]", nReason);
    WARNING_LOG("</OnFrontDisconnected>");
}

	// 当客户端发出登录请求之后，该方法会被调用，�1�7�知客户端登录是否成劄1�7
void CSimpleMdHandler::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    LOGDEMO("OnRspUserLogin:\n");
    LOGDEMO("\tErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,
        pRspInfo->ErrorMsg);
    LOGDEMO("\tRequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
    if (pRspInfo->ErrorID != 0) {
        // 端登失败，客户端霄1�7进行错误处理
        LOGDEMO("\tFailed to login, errorcode=%d errormsg=%s requestid=%d chain = %d",
            pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
        exit(-1);
    }
    //SetEvent(xinhao);
    sem_post(&globalSem.sem_login);
    //SubscribeMarketData();//订阅行情
    //SubscribeForQuoteRsp();//询价请求
}

	void CSimpleMdHandler::SubscribeMarketData()//收行惄1�7
	{
		int md_num = 0;
		char **ppInstrumentID = new char*[5000];
		for (int count1 = 0; count1 <= md_InstrumentID.size() / 500; count1++)
		{
			if (count1 < md_InstrumentID.size() / 500)
			{
				int a = 0;
				for (a; a < 500; a++)
				{
					ppInstrumentID[a] = const_cast<char *>(md_InstrumentID.at(md_num).c_str());
					md_num++;
				}
				int result = m_pUserMdApi->SubscribeMarketData(ppInstrumentID, a);
				LOGDEMO((result == 0) ? "订阅行情请求1......发�1�7�成功\n" : "订阅行情请求1......发�1�7�失败，错误序号=[%d]\n", result);
			}
			else if (count1 = md_InstrumentID.size() / 500)
			{
				int count2 = 0;
				for (count2; count2 < md_InstrumentID.size() % 500; count2++)
				{
					ppInstrumentID[count2] = const_cast<char *>(md_InstrumentID.at(md_num).c_str());
					md_num++;
				}
				int result = m_pUserMdApi->SubscribeMarketData(ppInstrumentID, count2);
				LOGDEMO((result == 0) ? "订阅行情请求2......发�1�7�成功\n" : "订阅行情请求2......发�1�7�失败，错误序号=[%d]\n", result);
			}
		}
	}

	///订阅行情应答
	void CSimpleMdHandler::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		LOGDEMO("<OnRspSubMarketData>\n");
		if (pSpecificInstrument)
		{
			LOGDEMO("\tInstrumentID = [%s]\n", pSpecificInstrument->InstrumentID);
		}
		if (pRspInfo)
		{
			LOGDEMO("\tErrorMsg = [%s]\n", pRspInfo->ErrorMsg);
			LOGDEMO("\tErrorID = [%d]\n", pRspInfo->ErrorID);
		}
		LOGDEMO("\tnRequestID = [%d]\n", nRequestID);
		LOGDEMO("\tbIsLast = [%d]\n", bIsLast);
		LOGDEMO("</OnRspSubMarketData>\n");
	}

	///深度行情通知
	void CSimpleMdHandler::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
	{
		LOGDEMO("<OnRtnDepthMarketData>\n");
		if (pDepthMarketData)
		{
			LOGDEMO("\tInstrumentID = [%s]\n", pDepthMarketData->InstrumentID);
			LOGDEMO("\tExchangeID = [%s]\n", pDepthMarketData->ExchangeID);
			LOGDEMO("\tLastPrice = [%.8lf]\n", pDepthMarketData->LastPrice);
			LOGDEMO("\tPreSettlementPrice = [%.8lf]\n", pDepthMarketData->PreSettlementPrice);
			LOGDEMO("\tOpenPrice = [%.8lf]\n", pDepthMarketData->OpenPrice);
			LOGDEMO("\tVolume = [%d]\n", pDepthMarketData->Volume);
			LOGDEMO("\tTurnover = [%.8lf]\n", pDepthMarketData->Turnover);
			LOGDEMO("\tOpenInterest = [%d]\n", pDepthMarketData->OpenInterest);
		}
		LOGDEMO("</OnRtnDepthMarketData>\n");
	}

	///订阅询价请求
	void CSimpleMdHandler::SubscribeForQuoteRsp()
	{
		LOGDEMO("行情中订阅询价请求\n");
		char **ppInstrumentID = new char*[50];
		string g_chInstrumentID = getConfig("market", "InstrumentID");
		ppInstrumentID[0] = const_cast<char *>(g_chInstrumentID.c_str());
		int result = m_pUserMdApi->SubscribeForQuoteRsp(ppInstrumentID, 1);
	}

	///订阅询价应答
	void CSimpleMdHandler::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast)
	{
		LOGDEMO("<OnRspSubForQuoteRsp>\n");
		if (pSpecificInstrument)
		{
			LOGDEMO("\tInstrumentID = [%s]\n", pSpecificInstrument->InstrumentID);
		}
		if (pRspInfo)
		{
			LOGDEMO("\tErrorMsg = [%s]\n", pRspInfo->ErrorMsg);
			LOGDEMO("\tErrorID = [%d]\n", pRspInfo->ErrorID);
		}
		LOGDEMO("\tnRequestID = [%d]\n", nRequestID);
		LOGDEMO("\tbIsLast = [%d]\n", bIsLast);
		LOGDEMO("</OnRspSubForQuoteRsp>\n");
		//SetEvent(xinhao);
        sem_post(&globalSem.sem_query);
	}


	///询价通知
	void CSimpleMdHandler::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
	{
		LOGDEMO("<OnRtnForQuoteRsp>\n");
		if (pForQuoteRsp)
		{
			LOGDEMO("\tTradingDay = [%s]\n", pForQuoteRsp->TradingDay);
			LOGDEMO("\tInstrumentID = [%s]\n", pForQuoteRsp->InstrumentID);
			LOGDEMO("\tForQuoteSysID = [%s]\n", pForQuoteRsp->ForQuoteSysID);
			LOGDEMO("\tForQuoteTime = [%s]\n", pForQuoteRsp->ForQuoteTime);
			LOGDEMO("\tActionDay = [%s]\n", pForQuoteRsp->ActionDay);
			LOGDEMO("\tExchangeID = [%s]\n", pForQuoteRsp->ExchangeID);
		}
		LOGDEMO("</OnRtnForQuoteRsp>\n");
		//SetEvent(xinhao);
        sem_post(&globalSem.sem_query);
	}
////*******************************************************************************************************************//////
//交易籄1�7

void CSimpleHandler::OnFrontConnected()
{
    INFO_LOG("reach onfrontconnected!\n");
    strcpy(g_chBrokerID, getConfig("trade", "BrokerID").c_str());
    strcpy(g_chUserID, getConfig("trade", "UserID").c_str());
    strcpy(g_chPassword, getConfig("trade", "Password").c_str());
    strcpy(g_chInvestorID, getConfig("trade", "InvestorID").c_str());
    strcpy(g_chAuthCode, getConfig("trade", "AuthCode").c_str());
    strcpy(g_chAppID, getConfig("trade", "AppID").c_str());

    //strcpy(g_NewExecOrderRef, "");
    //strcpy(g_NewExecOrderSysID, "");
    g_NewFrontID = 0;
    g_NewSessionID = 0;
    //ReqUserLogin();
    //ReqAuthenticate();
    //SetEvent(g_hEvent);
    sem_post(&globalSem.sem_login);
}

//客户端认评1�7
void CSimpleHandler::ReqAuthenticate()
{
    //strcpy(g_chUserProductInfo, getConfig("config", "UserProductInfo").c_str());
    strcpy(g_chAuthCode, getConfig("trade", "AuthCode").c_str());
    strcpy(g_chAppID, getConfig("trade", "AppID").c_str());
    CThostFtdcReqAuthenticateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.UserID, g_chUserID);
    //strcpy(a.UserProductInfo, "");
    strcpy(a.AuthCode, g_chAuthCode);
    strcpy(a.AppID, g_chAppID);
    int b = m_pUserApi->ReqAuthenticate(&a, 1);
    INFO_LOG("\t客户端认评1�7 = [%d]\n", b);
}

///客户端认证响庄1�7
void CSimpleHandler::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo,
    int nRequestID, bool bIsLast)
{
    CTraderSpi::OnRspAuthenticate(pRspAuthenticateField, pRspInfo, nRequestID, bIsLast);
    //SetEvent(g_hEvent);
    sem_post(&globalSem.sem_login);
}

void CSimpleHandler::RegisterFensUserInfo()
{
    CThostFtdcFensUserInfoField pFensUserInfo = { 0 };
    strcpy(pFensUserInfo.BrokerID, g_chBrokerID);
    strcpy(pFensUserInfo.UserID, g_chUserID);
    pFensUserInfo.LoginMode = THOST_FTDC_LM_Trade;
    m_pUserApi->RegisterFensUserInfo(&pFensUserInfo);
}

/*virtual void OnFrontDisconnected(int nReason)
{
    LOGDEMO("<OnFrontDisconnected>\n");
    LOGDEMO("\tnReason = %d\n", nReason);
    LOGDEMO("</OnFrontDisconnected>\n");
}*/

void CSimpleHandler::ReqUserLogin( CThostFtdcReqUserLoginField& reqUserLogin, int  requestId)
{
//    CThostFtdcReqUserLoginField reqUserLogin = { 0 };
//    strcpy(reqUserLogin.BrokerID, g_chBrokerID);
//    strcpy(reqUserLogin.UserID, g_chUserID);
//    strcpy(reqUserLogin.Password, g_chPassword);

    //strcpy(reqUserLogin.ClientIPAddress, "::c0a8:0101");
    //strcpy(reqUserLogin.UserProductInfo, "123");
    // 发出登陆请求
    m_pUserApi->ReqUserLogin(&reqUserLogin, requestId);
}

void CSimpleHandler::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    FrontID = pRspUserLogin->FrontID;
    SessionID = pRspUserLogin->SessionID;
    CTraderSpi::OnRspUserLogin(pRspUserLogin, pRspInfo, nRequestID, bIsLast);
    if (pRspInfo->ErrorID != 0)
        //if (pRspInfo)
    {
        LOGDEMO("\tFailed to login, errorcode=[%d]\n \terrormsg=[%s]\n \trequestid = [%d]\n \tchain = [%d]\n",
            pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
        //exit(-1);
    }
    //SetEvent(g_hEvent);
    sem_post(&globalSem.sem_login);
}

void CSimpleHandler::ReqUserLogout(CThostFtdcUserLogoutField& logOutFIeld, int requestId )
{
//    CThostFtdcUserLogoutField a = { 0 };
//    strcpy(a.BrokerID, g_chBrokerID);
//    strcpy(a.UserID, g_chUserID);
    m_pUserApi->ReqUserLogout(&logOutFIeld, requestId);
}

///登出请求响应
void CSimpleHandler::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    INFO_LOG("<OnRspUserLogout>");
    if (pUserLogout)
    {
        INFO_LOG("\tBrokerID [%s]", pUserLogout->BrokerID);
        INFO_LOG("\tUserID [%s]", pUserLogout->UserID);
    }
    if (pRspInfo)
    {
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        INFO_LOG("\tErrorMsg [%s]", msg);
        INFO_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
    }
    INFO_LOG("\tnRequestID [%d]", nRequestID);
    INFO_LOG("\tbIsLast [%d]", bIsLast);
    INFO_LOG("</OnRspUserLogout>");
    sem_post(&globalSem.sem_logout);

}

///请求确认结算卄1�7
void CSimpleHandler::ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *Confirm, int nRequestID)
{
//		CThostFtdcSettlementInfoConfirmField Confirm = { 0 };
//		///经纪公司代码
//		strcpy(Confirm.BrokerID, g_chBrokerID);
//		///投资者代砄1�7
//		strcpy(Confirm.InvestorID, g_chUserID);
    m_pUserApi->ReqSettlementInfoConfirm(Confirm, nRequestID);
}

///投资者结算结果确认响庄1�7
void CSimpleHandler::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CTraderSpi::OnRspSettlementInfoConfirm(pSettlementInfoConfirm, pRspInfo, nRequestID, bIsLast);
    //SetEvent(g_hEvent);
    sem_post(&globalSem.sem);
}

///用户口令更新请求
void CSimpleHandler::ReqUserPasswordUpdate()
{
    string newpassword;
    LOGDEMO("请输入新登录密码：\n");
    cin >> newpassword;
    CThostFtdcUserPasswordUpdateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.UserID, g_chUserID);
    strcpy(a.OldPassword, g_chPassword);
    strcpy(a.NewPassword, newpassword.c_str());
    int b = m_pUserApi->ReqUserPasswordUpdate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "用户口令更新请求......发�1�7�成功\n" : "用户口令更新请求......发�1�7�失败，序号=[%d]\n", b);
}

///用户口令更新请求响应
void CSimpleHandler::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CTraderSpi::OnRspUserPasswordUpdate(pUserPasswordUpdate, pRspInfo, nRequestID, bIsLast);
    //SetEvent(g_hEvent);
    sem_post(&globalSem.sem);
}

///资金账户口令更新请求
void CSimpleHandler::ReqTradingAccountPasswordUpdate()
{
    string newpassword;
    LOGDEMO("请输入新资金密码：\n");
    cin >> newpassword;
    CThostFtdcTradingAccountPasswordUpdateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.AccountID, g_chInvestorID);
    strcpy(a.OldPassword, g_chPassword);
    strcpy(a.NewPassword, newpassword.c_str());
    strcpy(a.CurrencyID, "CNY");
    int b = m_pUserApi->ReqTradingAccountPasswordUpdate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "资金账户口令更新请求......发�1�7�成功\n" : "资金账户口令更新请求......发�1�7�失败，序号=[%d]\n", b);
}

///资金账户口令更新请求响应
void CSimpleHandler::OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CTraderSpi::OnRspTradingAccountPasswordUpdate(pTradingAccountPasswordUpdate, pRspInfo, nRequestID, bIsLast);
    //SetEvent(g_hEvent);
    sem_post(&globalSem.sem);
}

///预埋单录兄1�7//限价卄1�7
void CSimpleHandler::ReqParkedOrderInsert()
{
    int limitprice = 0;
    LOGDEMO("请输入限价单价格＄1�7(默认0)\n");
    cin >> limitprice;
    CThostFtdcParkedOrderField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    strcpy(a.UserID, g_chUserID);
    a.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    a.Direction = THOST_FTDC_D_Buy;
    a.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    a.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    a.LimitPrice = limitprice;
    a.VolumeTotalOriginal = 1;
    a.TimeCondition = THOST_FTDC_TC_GFD;
    a.VolumeCondition = THOST_FTDC_VC_AV;
    a.MinVolume = 1;
    a.ContingentCondition = THOST_FTDC_CC_Immediately;
    a.StopPrice = 0;
    a.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    a.IsAutoSuspend = 0;
    strcpy(a.ExchangeID, g_chExchangeID);
    int b = m_pUserApi->ReqParkedOrderInsert(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求录入预埋卄1�7......发�1�7�成功\n" : "请求录入预埋卄1�7......发�1�7�失败，序号=[%d]\n", b);
}

///预埋撤单录入请求
void CSimpleHandler::ReqParkedOrderAction()
{
    CThostFtdcParkedOrderActionField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    //strcpy(a.OrderRef, "          15");
    strcpy(a.ExchangeID, g_chExchangeID);
    /*a.FrontID = 1;
    a.SessionID = -287506422;*/
    strcpy(a.OrderSysID, g_chOrderSysID);
    strcpy(a.UserID, g_chUserID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    a.ActionFlag = THOST_FTDC_AF_Delete;
    int b = m_pUserApi->ReqParkedOrderAction(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求录入预埋撤单......发�1�7�成功\n" : "请求录入预埋撤单......发�1�7�失败，序号=[%d]\n", b);
}

///请求删除预埋卄1�7
void CSimpleHandler::ReqRemoveParkedOrder()
{
    CThostFtdcRemoveParkedOrderField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.ParkedOrderID, g_chParkedOrderID1);
    int b = m_pUserApi->ReqRemoveParkedOrder(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求删除预埋卄1�7......发�1�7�成功\n" : "请求删除预埋卄1�7......发�1�7�失败，序号=[%d]\n", b);
}

///请求删除预埋撤单
void CSimpleHandler::ReqRemoveParkedOrderAction()
{
    CThostFtdcRemoveParkedOrderActionField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.ParkedOrderActionID, g_chParkedOrderActionID1);
    int b = m_pUserApi->ReqRemoveParkedOrderAction(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求删除预埋撤单......发�1�7�成功\n" : "请求删除预埋撤单......发�1�7�失败，序号=[%d]\n", b);
}

///报单录入请求
void CSimpleHandler::ReqOrderInsert_Ordinary()
{
    system("clear");
    int volume;
    string instrument_id;
    std::cout << "请输入合约名＄1�7" << std::endl;
    cin >> instrument_id;
    string new_limitprice;
    LOGDEMO("请输入指定价格：\n");
    cin >> new_limitprice;
    std::cout << "请输入数量：" << std::endl;
    cin >> volume;
    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    ///strcpy(ord.InstrumentID, g_chInstrumentID);

    strcpy(ord.InstrumentID, instrument_id.c_str());
    strcpy(ord.UserID, g_chUserID);
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;   //THOST_FTDC_OPT_LimitPrice
    //ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;

    int num1;
Direction:LOGDEMO("请�1�7�择买卖方向\t1.买\t2.卖\n");
    cin >> num1;
    if (num1 == 1) {
        ord.Direction = THOST_FTDC_D_Buy;//乄1�7
    }
    else if (num1 == 2) {
        ord.Direction = THOST_FTDC_D_Sell;//卄1�7
    }
    else {
        LOGDEMO("输入错误请重新输入\n");
        system("stty -echo");
        getchar();
        system("stty echo");

        goto Direction;
    }

    int num2;
CombOffsetFlag:LOGDEMO("请输入开平方向\t1.弄1�7仓\t2.平仓\t3.强平\t4.平今\t5.平昨\t6.强减\t7.本地强平\n");
    cin >> num2;
    if (num2 == 1) {
        ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    }
    else if (num2 == 2) {
        ord.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
    }
    else if (num2 == 3) {
        ord.CombOffsetFlag[0] = THOST_FTDC_OF_ForceClose;
    }
    else if (num2 == 4) {
        ord.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
    }
    else if (num2 == 5) {
        ord.CombOffsetFlag[0] = THOST_FTDC_OF_CloseYesterday;
    }
    else if (num2 == 6) {
        ord.CombOffsetFlag[0] = THOST_FTDC_OF_ForceOff;
    }
    else if (num2 == 7) {
        ord.CombOffsetFlag[0] = THOST_FTDC_OF_LocalForceClose;
    }
    else {
        LOGDEMO("输入错误请重新输入\n");
        system("stty -echo");
        getchar();
        system("stty echo");;
        goto CombOffsetFlag;
    }

    //ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    strcpy(ord.CombHedgeFlag, "1");
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    ord.LimitPrice = atoi(new_limitprice.c_str());
    ord.VolumeTotalOriginal = volume;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///当日有效
    ord.VolumeCondition = THOST_FTDC_VC_CV;///全部数量
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "报单录入请求限价卄1�7......发�1�7�成功\n" : "报单录入请求限价卄1�7......发�1�7�失败，序号=[%d]\n", a);
}
void CSimpleHandler::ReqOrderInsert_Ordinary_hai(CThostFtdcInputOrderField &ord, int nRequestID)
{
    int a = m_pUserApi->ReqOrderInsert(&ord, nRequestID);
    INFO_LOG("%s",(a == 0) ? "Order insert request send succ, waiting for response from exchange ......\n" : "Order insert request send failed\n"); // @suppress("Invalid arguments")
}
///大商扄1�7止损卄1�7
void CSimpleHandler::ReqOrderInsert_Touch()
{
    int new_limitprice;
    LOGDEMO("请输入指定价格limitprice：\n");
    cin >> new_limitprice;

    int new_StopPrice;
    LOGDEMO("请输入指定价格stopprice：\n");
    cin >> new_StopPrice;

    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ord.Direction = THOST_FTDC_D_Buy;//乄1�7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = 1;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///当日有效
    ord.VolumeCondition = THOST_FTDC_VC_AV;///任何数量
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Touch;
    ord.StopPrice = new_StopPrice;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "报单录入请求限价卄1�7......发�1�7�成功\n" : "报单录入请求限价卄1�7......发�1�7�失败，序号=[%d]\n", a);
}

///大商扄1�7止盈卄1�7
void CSimpleHandler::ReqOrderInsert_TouchProfit()
{
    int new_limitprice;
    LOGDEMO("请输入指定价格limitprice：\n");
    cin >> new_limitprice;

    int new_StopPrice;
    LOGDEMO("请输入指定价格stopprice：\n");
    cin >> new_StopPrice;

    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ord.Direction = THOST_FTDC_D_Buy;//乄1�7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = 1;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///当日有效
    ord.VolumeCondition = THOST_FTDC_VC_AV;///全部数量
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_TouchProfit;
    ord.StopPrice = new_StopPrice;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "报单录入请求限价卄1�7......发�1�7�成功\n" : "报单录入请求限价卄1�7......发�1�7�失败，序号=[%d]\n", a);
}

//全成全撤
void CSimpleHandler::ReqOrderInsert_VC_CV()
{
    int new_limitprice;
    LOGDEMO("请输入指定价格：\n");
    cin >> new_limitprice;

    int insert_num;
    LOGDEMO("请输入下单数量：\n");
    cin >> insert_num;

    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ord.Direction = THOST_FTDC_D_Buy;//乄1�7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = insert_num;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///当日有效
    ord.VolumeCondition = THOST_FTDC_VC_CV;///全部数量
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "报单录入请求限价卄1�7......发�1�7�成功\n" : "报单录入请求限价卄1�7......发�1�7�失败，序号=[%d]\n", a);
}

//部成部撤
void CSimpleHandler::ReqOrderInsert_VC_AV()
{
    int new_limitprice;
    LOGDEMO("请输入指定价格：\n");
    cin >> new_limitprice;

    int insert_num;
    LOGDEMO("请输入下单数量：\n");
    cin >> insert_num;

    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ord.Direction = THOST_FTDC_D_Buy;//乄1�7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = insert_num;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///当日有效
    ord.VolumeCondition = THOST_FTDC_VC_AV;///任何数量
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "报单录入请求限价卄1�7......发�1�7�成功\n" : "报单录入请求限价卄1�7......发�1�7�失败，序号=[%d]\n", a);
}

//市价卄1�7
void CSimpleHandler::ReqOrderInsert_AnyPrice()
{
    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
    ord.Direction = THOST_FTDC_D_Buy;//乄1�7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    //ord.LimitPrice = new_limitprice;
    ord.LimitPrice = 0;
    ord.VolumeTotalOriginal = 1;
    ord.TimeCondition = THOST_FTDC_TC_IOC;///立即完成，否则撤锄1�7
    ord.VolumeCondition = THOST_FTDC_VC_AV;///任何数量
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    //ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "报单录入请求限价卄1�7......发�1�7�成功\n" : "报单录入请求限价卄1�7......发�1�7�失败，序号=[%d]\n", a);
}

//市价转限价单(中金扄1�7)
void CSimpleHandler::ReqOrderInsert_BestPrice()
{
    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_BestPrice;
    ord.Direction = THOST_FTDC_D_Buy;//乄1�7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    //ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = 1;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///当日有效
    ord.VolumeCondition = THOST_FTDC_VC_AV;///任何数量
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "报单录入请求限价卄1�7......发�1�7�成功\n" : "报单录入请求限价卄1�7......发�1�7�失败，序号=[%d]\n", a);
}

//套利指令
void CSimpleHandler::ReqOrderInsert_Arbitrage()
{
    int new_limitprice;
    LOGDEMO("请输入指定价格：\n");
    cin >> new_limitprice;

    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ord.Direction = THOST_FTDC_D_Buy;//乄1�7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = 1;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///当日有效
    ord.VolumeCondition = THOST_FTDC_VC_AV;///任何数量
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "报单录入请求限价卄1�7......发�1�7�成功\n" : "报单录入请求限价卄1�7......发�1�7�失败，序号=[%d]\n", a);
}

//互换卄1�7
void CSimpleHandler::ReqOrderInsert_IsSwapOrder()
{
    int new_limitprice;
    LOGDEMO("请输入指定价格：\n");
    cin >> new_limitprice;

    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ord.Direction = THOST_FTDC_D_Buy;//乄1�7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = 1;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///当日有效
    ord.VolumeCondition = THOST_FTDC_VC_AV;///任何数量
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    ord.IsSwapOrder = 1;//互换单标忄1�7
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "报单录入请求限价卄1�7......发�1�7�成功\n" : "报单录入请求限价卄1�7......发�1�7�失败，序号=[%d]\n", a);
}

///报单操作请求
void CSimpleHandler::ReqOrderAction_Ordinary()
{
    CThostFtdcInputOrderActionField a = { 0 };

    strcpy(a.BrokerID, g_chBrokerID);     //经济公司代码
    strcpy(a.InvestorID, g_chInvestorID);  //投资者代砄1�7
    strcpy(a.UserID, g_chUserID);        //用户代码
    //a.OrderActionRef = 1;

//      std::cout << "请输入报单引用：" << std::endl;
//      cin >> g_chOrderRef;
    //strcpy(a.OrderRef, g_chOrderRef);     //报单引用
    //std::cout << "g_chOrderRef is:" << g_chOrderRef << std::endl;

    //a.FrontID = g_chFrontID;   //前置编号
    //a.SessionID = g_chSessionID; //会话编号

    //a.LimitPrice =
    //a.VolumeChange =
    std::cout << "请输入交易所代码:\n";
    cin >> g_chExchangeID;
    strcpy(a.ExchangeID, g_chExchangeID);    //交易扄1�7代码

    std::cout << "请输入合约号＄1�7" << std::endl;
    cin >> g_chInstrumentID;                   //合约代码
    strcpy(a.InstrumentID, g_chInstrumentID);

    std::cout << "请输入报单编号：" << std::endl;
    //cin >> g_chOrderSysID;
    cin.ignore(numeric_limits <streamsize> ::max(), '\n');
    cin.getline(g_chOrderSysID, sizeof(g_chOrderSysID));
    strcpy(a.OrderSysID, g_chOrderSysID);         //报单编号

    a.ActionFlag = THOST_FTDC_AF_Delete;    //操作标志

    int ab = m_pUserApi->ReqOrderAction(&a, nRequestID++);
    LOGDEMO((ab == 0) ? "报单操作请求......发�1�7�成功\n" : "报单操作请求......发�1�7�失败，序号=[%d]\n", ab);
}

///执行宣告录入请求
void CSimpleHandler::ReqExecOrderInsert(int a)
{
    CThostFtdcInputExecOrderField OrderInsert = { 0 };
    strcpy(OrderInsert.BrokerID, g_chBrokerID);
    strcpy(OrderInsert.InvestorID, g_chInvestorID);
    strcpy(OrderInsert.InstrumentID, g_chInstrumentID);
    strcpy(OrderInsert.ExchangeID, g_chExchangeID);
    //strcpy(OrderInsert.ExecOrderRef, "00001");
    strcpy(OrderInsert.UserID, g_chUserID);
    OrderInsert.Volume = 1;
    OrderInsert.RequestID = 1;
    OrderInsert.OffsetFlag = THOST_FTDC_OF_Close;//弄1�7平标忄1�7
    OrderInsert.HedgeFlag = THOST_FTDC_HF_Speculation;//投机套保标志
    if (a == 0) {
        OrderInsert.ActionType = THOST_FTDC_ACTP_Exec;//执行类型类型
    }
    if (a == 1) {
        OrderInsert.ActionType = THOST_FTDC_ACTP_Abandon;//执行类型类型
    }
    OrderInsert.PosiDirection = THOST_FTDC_PD_Long;//持仓多空方向类型
    OrderInsert.ReservePositionFlag = THOST_FTDC_EOPF_Reserve;//期权行权后是否保留期货头寸的标记类型
    //OrderInsert.ReservePositionFlag = THOST_FTDC_EOPF_UnReserve;//不保留头寄1�7
    OrderInsert.CloseFlag = THOST_FTDC_EOCF_NotToClose;//期权行权后生成的头寸是否自动平仓类型
    //OrderInsert.CloseFlag = THOST_FTDC_EOCF_AutoClose;//自动平仓
    //strcpy(OrderInsert.InvestUnitID, "");AccountID
    //strcpy(OrderInsert.AccountID, "");
    //strcpy(OrderInsert.CurrencyID, "CNY");
    //strcpy(OrderInsert.ClientID, "");
    int b = m_pUserApi->ReqExecOrderInsert(&OrderInsert, 1);
    LOGDEMO((b == 0) ? "执行宣告录入请求......发�1�7�成功\n" : "执行宣告录入请求......发�1�7�失败，错误序号=[%d]\n", b);
}

///执行宣告操作请求
void CSimpleHandler::ReqExecOrderAction()
{
    CThostFtdcInputExecOrderActionField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    a.ExecOrderActionRef = 1;
    strcpy(a.ExecOrderRef, g_NewExecOrderRef);
    a.FrontID = g_NewFrontID;
    a.SessionID = g_NewSessionID;
    strcpy(a.ExchangeID, g_chExchangeID);
    strcpy(a.ExecOrderSysID, g_NewExecOrderSysID);
    a.ActionFlag = THOST_FTDC_AF_Delete;//删除
    strcpy(a.UserID, g_chUserID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    //strcpy(a.InvestUnitID, "");
    //strcpy(a.IPAddress, "");
    //strcpy(a.MacAddress, "");
    int b = m_pUserApi->ReqExecOrderAction(&a, 1);
    LOGDEMO((b == 0) ? "执行宣告操作请求......发�1�7�成功\n" : "执行宣告操作请求......发�1�7�失败，错误序号=[%d]\n", b);
}

//批量报单操作请求
void CSimpleHandler::ReqBatchOrderAction()
{
    CThostFtdcInputBatchOrderActionField a = { 0 };

}

///请求查询报单
void CSimpleHandler::ReqQryOrder()
{
    action_number = 0;
    vector_OrderSysID.clear();
    vector_ExchangeID.clear();
    vector_InstrumentID.clear();
    CThostFtdcQryOrderField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    //strcpy(a.InstrumentID, g_chInstrumentID);
    strcpy(a.ExchangeID, g_chExchangeID);
    int ab = m_pUserApi->ReqQryOrder(&a, nRequestID++);
    LOGDEMO((ab == 0) ? "请求查询报单......发�1�7�成功\n" : "请求查询报单......发�1�7�失败，序号=[%d]\n", ab);
}

///报单录入请求
void CSimpleHandler::ReqOrderInsert_Condition(int select_num)
{
    string limit_price;
    LOGDEMO("请输入指定价栄1�7(limitprice):\n");
    cin >> limit_price;

    string stop_price;
    LOGDEMO("请输入触发价栄1�7(stopprice):\n");
    cin >> stop_price;

    CThostFtdcInputOrderField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    strcpy(a.UserID, g_chUserID);
    a.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    a.Direction = THOST_FTDC_D_Buy;//乄1�7
    //a.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    a.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    strcpy(a.CombOffsetFlag, "0");
    strcpy(a.CombHedgeFlag, "1");
    a.LimitPrice = atoi(limit_price.c_str());
    a.VolumeTotalOriginal = 1;
    a.TimeCondition = THOST_FTDC_TC_GFD;
    a.VolumeCondition = THOST_FTDC_VC_AV;
    a.MinVolume = 0;
    if (select_num == 1)
    {
        a.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterThanStopPrice;
    }
    else if (select_num == 2)
    {
        a.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterEqualStopPrice;
    }
    else if (select_num == 3)
    {
        a.ContingentCondition = THOST_FTDC_CC_LastPriceLesserThanStopPrice;
    }
    else if (select_num == 4)
    {
        a.ContingentCondition = THOST_FTDC_CC_LastPriceLesserEqualStopPrice;
    }
    else if (select_num == 5)
    {
        a.ContingentCondition = THOST_FTDC_CC_AskPriceGreaterThanStopPrice;
    }
    else if (select_num == 6)
    {
        a.ContingentCondition = THOST_FTDC_CC_AskPriceGreaterEqualStopPrice;
    }
    else if (select_num == 7)
    {
        a.ContingentCondition = THOST_FTDC_CC_AskPriceLesserThanStopPrice;
    }
    else if (select_num == 8)
    {
        a.ContingentCondition = THOST_FTDC_CC_AskPriceLesserEqualStopPrice;
    }
    else if (select_num == 9)
    {
        a.ContingentCondition = THOST_FTDC_CC_BidPriceGreaterThanStopPrice;
    }
    else if (select_num == 10)
    {
        a.ContingentCondition = THOST_FTDC_CC_BidPriceGreaterEqualStopPrice;
    }
    else if (select_num == 11)
    {
        a.ContingentCondition = THOST_FTDC_CC_BidPriceLesserThanStopPrice;
    }
    else if (select_num == 12)
    {
        a.ContingentCondition = THOST_FTDC_CC_BidPriceLesserEqualStopPrice;
    }
    a.StopPrice = atoi(stop_price.c_str());
    //itoa(a.StopPrice, const_cast<char *>(getConfig("config", "StopPrice").c_str()), 10);
    a.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    a.IsAutoSuspend = 0;
    strcpy(a.ExchangeID, g_chExchangeID);
    int ab = m_pUserApi->ReqOrderInsert(&a, nRequestID++);
    LOGDEMO((ab == 0) ? "请求报入条件卄1�7......发�1�7�成功\n" : "请求报入条件卄1�7......发�1�7�失败，序号=[%d]\n", ab);
}

///报单操作请求
void CSimpleHandler::ReqOrderAction_Condition()
{
    CThostFtdcInputOrderActionField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    strcpy(a.UserID, g_chUserID);
    if (chioce_action == 0)
    {
        a.FrontID = g_chFrontID;
        a.SessionID = g_chSessionID;
        strcpy(a.OrderRef, g_chOrderRef);
    }
    if (chioce_action == 1)
    {
        strcpy(a.OrderSysID, g_chOrderSysID);
    }
    strcpy(a.ExchangeID, g_chExchangeID);
    a.ActionFlag = THOST_FTDC_AF_Delete;
    int ab = m_pUserApi->ReqOrderAction(&a, nRequestID++);
    LOGDEMO((ab == 0) ? "请求撤销条件卄1�7......发�1�7�成功\n" : "请求撤销条件卄1�7......发�1�7�失败，序号=[%d]\n", ab);
}

//撤销查询的报卄1�7
void CSimpleHandler::ReqOrderAction_forqry(int action_num)
{
    CThostFtdcInputOrderActionField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.UserID, g_chUserID);

    strcpy(a.OrderSysID, vector_OrderSysID.at(action_num - 1).c_str());
    strcpy(a.ExchangeID, vector_ExchangeID.at(action_num - 1).c_str());
    strcpy(a.InstrumentID, vector_InstrumentID.at(action_num - 1).c_str());

    a.ActionFlag = THOST_FTDC_AF_Delete;
    int ab = m_pUserApi->ReqOrderAction(&a, nRequestID++);
    LOGDEMO((ab == 0) ? "请求撤销条件卄1�7......发�1�7�成功\n" : "请求撤销条件卄1�7......发�1�7�失败，错误序号=[%d]\n", ab);
}

///请求查询成交
void CSimpleHandler::ReqQryTrade()
{
    CThostFtdcQryTradeField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    string instr;
    instr.clear();
    LOGDEMO("请输入合约代砄1�7(不输入则为空)\n");
    cin.ignore();
    getline(cin, instr);
    strcpy(a.InstrumentID, instr.c_str());

    string Exch;
    Exch.clear();
    LOGDEMO("请输入交易所代码(不输入则为空)\n");
    //cin.ignore();
    getline(cin, Exch);
    strcpy(a.ExchangeID, Exch.c_str());
    /*strcpy(a.TradeID, "");
    strcpy(a.TradeTimeStart, "");
    strcpy(a.TradeTimeEnd, "");*/
    int b = m_pUserApi->ReqQryTrade(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询成交......发�1�7�成功\n" : "请求查询成交......发�1�7�失败，错误序号=[%d]\n", b);
}

///请求查询预埋卄1�7
void CSimpleHandler::ReqQryParkedOrder()
{
    CThostFtdcQryParkedOrderField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    //strcpy(a.InstrumentID, g_chInstrumentID);
    strcpy(a.ExchangeID, g_chExchangeID);
    int ab = m_pUserApi->ReqQryParkedOrder(&a, nRequestID++);
    LOGDEMO((ab == 0) ? "请求查询预埋卄1�7......发�1�7�成功\n" : "请求查询预埋卄1�7......发�1�7�失败，序号=[%d]\n", ab);
}

//请求查询服务器预埋撤卄1�7
void CSimpleHandler::ReqQryParkedOrderAction()
{
    CThostFtdcQryParkedOrderActionField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    strcpy(a.ExchangeID, g_chExchangeID);
    int ab = m_pUserApi->ReqQryParkedOrderAction(&a, nRequestID++);
    LOGDEMO((ab == 0) ? "请求查询服务器预埋撤卄1�7......发�1�7�成功\n" : "请求查询服务器预埋撤卄1�7......发�1�7�失败，序号=[%d]\n", ab);
}

//请求查询资金账户
void CSimpleHandler::ReqQryTradingAccount()
{
    CThostFtdcQryTradingAccountField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.CurrencyID, "CNY");
    int ab = m_pUserApi->ReqQryTradingAccount(&a, nRequestID++);
    LOGDEMO((ab == 0) ? "请求查询资金账户......发�1�7�成功\n" : "请求查询资金账户......发�1�7�失败，序号=[%d]\n", ab);
}

//请求查询投资者持仄1�7
void CSimpleHandler::ReqQryInvestorPosition()
{
    CThostFtdcQryInvestorPositionField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    string instr;
    instr.clear();
    cin.ignore();
    LOGDEMO("请输入合约代砄1�7(不输入则为空)：\n");
    getline(cin, instr);
    strcpy(a.InstrumentID, instr.c_str());

    string exch;
    exch.clear();
    cin.ignore();
    LOGDEMO("请输入交易所代码(不输入则为空)：\n");
    getline(cin, exch);
    strcpy(a.ExchangeID, exch.c_str());
    //strcpy(a.InstrumentID, "SPD");
    int b = m_pUserApi->ReqQryInvestorPosition(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询投资者持仄1�7......发�1�7�成功\n" : "请求查询投资者持仄1�7......发�1�7�失败，错误序号=[%d]\n", b);
}

//请求查询投资者持仓明组1�7
void CSimpleHandler::ReqQryInvestorPositionDetail()
{
    CThostFtdcQryInvestorPositionDetailField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    string instr;
    instr.clear();
    cin.ignore();
    LOGDEMO("请输入合约代砄1�7(不输入则为空)\n");
    getline(cin, instr);
    strcpy(a.InstrumentID, instr.c_str());
    string exch;
    exch.clear();
    cin.ignore();
    LOGDEMO("请输入交易所代码(不输入则为空)：\n");
    getline(cin, exch);
    strcpy(a.ExchangeID, exch.c_str());
    //strcpy(a.InstrumentID, g_chInstrumentID);
    int b = m_pUserApi->ReqQryInvestorPositionDetail(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询投资者持仓明组1�7......发�1�7�成功\n" : "请求查询投资者持仓明组1�7......发�1�7�失败，错误序号=[%d]\n", b);
}

//请求查询交易扄1�7保证金率
void CSimpleHandler::ReqQryExchangeMarginRate()
{
    CThostFtdcQryExchangeMarginRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    a.HedgeFlag = THOST_FTDC_HF_Speculation;//投机
    int b = m_pUserApi->ReqQryExchangeMarginRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询交易扄1�7保证金率......发�1�7�成功\n" : "请求查询交易扄1�7保证金率......发�1�7�失败，错误序号=[%d]\n", b);
}

//请求查询合约保证金率
void CSimpleHandler::ReqQryInstrumentMarginRate()
{
    CThostFtdcQryInstrumentMarginRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    a.HedgeFlag = THOST_FTDC_HF_Speculation;//投机
    int b = m_pUserApi->ReqQryInstrumentMarginRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询合约保证金率......发�1�7�成功\n" : "请求查询合约保证金率......发�1�7�失败，错误序号=[%d]\n", b);
}

//请求查询合约手续费率
void CSimpleHandler::ReqQryInstrumentCommissionRate()
{
    CThostFtdcQryInstrumentCommissionRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    int b = m_pUserApi->ReqQryInstrumentCommissionRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询合约手续费率......发�1�7�成功\n" : "请求查询合约手续费率......发�1�7�失败，错误序号=[%d]\n", b);
}

//请求查询做市商合约手续费玄1�7
void CSimpleHandler::ReqQryMMInstrumentCommissionRate()
{
    CThostFtdcQryMMInstrumentCommissionRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    int b = m_pUserApi->ReqQryMMInstrumentCommissionRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询做市商合约手续费玄1�7......发�1�7�成功\n" : "请求查询做市商合约手续费玄1�7......发�1�7�失败，错误序号=[%d]\n", b);
}

//请求查询做市商期权合约手续费
void CSimpleHandler::ReqQryMMOptionInstrCommRate()
{
    CThostFtdcQryMMOptionInstrCommRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    int b = m_pUserApi->ReqQryMMOptionInstrCommRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询做市商期权合约手续费......发�1�7�成功\n" : "请求查询做市商期权合约手续费......发�1�7�失败，错误序号=[%d]\n", b);
}

//请求查询报单手续贄1�7
void CSimpleHandler::ReqQryInstrumentOrderCommRate()
{
    CThostFtdcQryInstrumentOrderCommRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    int b = m_pUserApi->ReqQryInstrumentOrderCommRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询报单手续贄1�7......发�1�7�成功\n" : "请求查询报单手续贄1�7......发�1�7�失败，错误序号=[%d]\n", b);
}

//请求查询期权合约手续贄1�7
void CSimpleHandler::ReqQryOptionInstrCommRate()
{
    CThostFtdcQryOptionInstrCommRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    string Inst;
    string Exch;
    string InvestUnit;
    LOGDEMO("请输入合约代砄1�7(不填则为穄1�7)");
    cin >> Inst;
    LOGDEMO("请输入交易所代码:(不填则为穄1�7)");
    cin >> Exch;
    LOGDEMO("请输入投资�1�7�单元代砄1�7(不填则为穄1�7)");
    cin >> InvestUnit;
    strcpy(a.InstrumentID, Inst.c_str());
    strcpy(a.ExchangeID, Exch.c_str());
    strcpy(a.InvestUnitID, InvestUnit.c_str());
    int b = m_pUserApi->ReqQryOptionInstrCommRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询期权合约手续贄1�7......发�1�7�成功\n" : "请求查询期权合约手续贄1�7......发�1�7�失败，错误序号=[%d]\n", b);
}

//请求查询合约
void CSimpleHandler::ReqQryInstrument()
{
    CThostFtdcQryInstrumentField a = { 0 };
    strcpy(a.ExchangeID, g_chExchangeID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    //strcpy(a.ExchangeInstID,"");
    //strcpy(a.ProductID, "m");
    int b = m_pUserApi->ReqQryInstrument(&a, nRequestID++);
    if(b == 0)
    {
        INFO_LOG("请求查询合约......发�1�7�成劄1�7");
        return;
    }

    INFO_LOG("请求查询合约......发�1�7�失败，错误序号=[%d]\n", b);
}
void CSimpleHandler::ReqQryInstrument_hai(CThostFtdcQryInstrumentField& qryFields)
{
//        CThostFtdcQryInstrumentField a = { 0 };
//        strcpy(a.ExchangeID, g_chExchangeID);
//        strcpy(a.InstrumentID, g_chInstrumentID);
    //strcpy(a.ExchangeInstID,"");
    //strcpy(a.ProductID, "m");
    int b = m_pUserApi->ReqQryInstrument(&qryFields, nRequestID++);
    if(b == 0)
    {
        INFO_LOG("ReqQryInstrument send ok!");
        return;
    }

    ERROR_LOG("ReqQryInstrument send failed result b =[%d]\n", b);
    sem_post(&globalSem.sem_query);

}
///请求查询合约响应
void CSimpleHandler::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    LOGDEMO("<OnRspQryInstrument>\n");
    if (pInstrument)
    {
        md_InstrumentID.push_back(pInstrument->InstrumentID);
        //std::cout << pInstrument->InstrumentID<<" "<<strlen(pInstrument->InstrumentID) << std::endl;
        LOGDEMO("\tInstrumentID [%s]\n", pInstrument->InstrumentID);
        //outfile << "InstrumentID:" << pInstrument->InstrumentID << "\t";
        LOGDEMO("\tExchangeID [%s]\n", pInstrument->ExchangeID);
        //outfile << "ExchangeID:" << pInstrument->ExchangeID << "\t";

        TThostFtdcInstrumentNameType name;
        gbk2utf8(pInstrument->InstrumentName, name, sizeof(name));
        LOGDEMO("\tInstrumentName [%s]\n", name);
        //outfile << "InstrumentName:" << name << "\t";
        TThostFtdcExchangeInstIDType id;
        gbk2utf8(pInstrument->ExchangeInstID, id, sizeof(id));
        LOGDEMO("\tExchangeInstID [%s]\n", id);
        //outfile << "ExchangeInstID:" << id << "\t";

        LOGDEMO("\tProductID [%s]\n", pInstrument->ProductID);
        //outfile << "ProductID:" << pInstrument->ProductID << "\t";
        LOGDEMO("\tCreateDate [%s]\n", pInstrument->CreateDate);
        //outfile << "CreateDate:" << pInstrument->CreateDate << "\t";

        LOGDEMO("\tOpenDate [%s]\n", pInstrument->OpenDate);
        //outfile << "OpenDate:" << pInstrument->OpenDate << "\t";
        LOGDEMO("\tExpireDate [%s]\n", pInstrument->ExpireDate);
        //outfile << "ExpireDate:" << pInstrument->ExpireDate << "\t";
        LOGDEMO("\tStartDelivDate [%s]\n", pInstrument->StartDelivDate);
        //outfile << "StartDelivDate:" << pInstrument->StartDelivDate << "\t";
        LOGDEMO("\tEndDelivDate [%s]\n", pInstrument->EndDelivDate);
        //outfile << "EndDelivDate:" << pInstrument->EndDelivDate << "\t";
        LOGDEMO("\tUnderlyingInstrID [%s]\n", pInstrument->UnderlyingInstrID);
        //outfile << "UnderlyingInstrID:" << pInstrument->UnderlyingInstrID << "\t";
        LOGDEMO("\tDeliveryYear [%d]\n", pInstrument->DeliveryYear);
        //outfile << "DeliveryYear:" << pInstrument->DeliveryYear << "\t";
        LOGDEMO("\tDeliveryMonth [%d]\n", pInstrument->DeliveryMonth);
        //outfile << "DeliveryMonth:" << pInstrument->DeliveryMonth << "\t";
        LOGDEMO("\tMaxMarketOrderVolume [%d]\n", pInstrument->MaxMarketOrderVolume);
        //outfile << "MaxMarketOrderVolume:" << pInstrument->MaxMarketOrderVolume << "\t";
        LOGDEMO("\tMinMarketOrderVolume [%d]\n", pInstrument->MinMarketOrderVolume);
        //outfile << "MinMarketOrderVolume:" << pInstrument->MinMarketOrderVolume << "\t";
        LOGDEMO("\tMaxLimitOrderVolume [%d]\n", pInstrument->MaxLimitOrderVolume);
        //outfile << "MaxLimitOrderVolume:" << pInstrument->MaxLimitOrderVolume << "\t";
        LOGDEMO("\tMinLimitOrderVolume [%d]\n", pInstrument->MinLimitOrderVolume);
        //outfile << "MinLimitOrderVolume:" << pInstrument->MinLimitOrderVolume << "\t";
        LOGDEMO("\tVolumeMultiple [%d]\n", pInstrument->VolumeMultiple);
        //outfile << "VolumeMultiple:" << pInstrument->VolumeMultiple << "\t";
        LOGDEMO("\tIsTrading [%d]\n", pInstrument->IsTrading);
        //outfile << "IsTrading:" << pInstrument->IsTrading << "\t";
        LOGDEMO("\tProductClass [%c]\n", pInstrument->ProductClass);
        //outfile << "ProductClass:" << pInstrument->ProductClass << "\t";
        LOGDEMO("\tInstLifePhase [%c]\n", pInstrument->InstLifePhase);
        //outfile << "InstLifePhase:" << pInstrument->InstLifePhase << "\t";
        LOGDEMO("\tPositionType [%c]\n", pInstrument->PositionType);
        //outfile << "PositionType:" << pInstrument->PositionType << "\t";
        LOGDEMO("\tPositionDateType [%c]\n", pInstrument->PositionDateType);
        //outfile << "PositionDateType:" << pInstrument->PositionDateType << "\t";
        LOGDEMO("\tMaxMarginSideAlgorithm [%c]\n", pInstrument->MaxMarginSideAlgorithm);
        //outfile << "MaxMarginSideAlgorithm:" << pInstrument->MaxMarginSideAlgorithm << "\t";
        LOGDEMO("\tOptionsType [%c]\n", pInstrument->OptionsType);
        //outfile << "OptionsType:" << pInstrument->OptionsType << "\t";
        LOGDEMO("\tCombinationType [%c]\n", pInstrument->CombinationType);
        //outfile << "CombinationType:" << pInstrument->CombinationType << "\t";
        LOGDEMO("\tPriceTick [%.8lf]\n", pInstrument->PriceTick);
        //outfile << "PriceTick:" << pInstrument->PriceTick << "\t";
        /*LOGDEMO("\tLongMarginRatio [%.8lf]\n", pInstrument->LongMarginRatio);
        LOGDEMO("\tShortMarginRatio [%.8lf]\n", pInstrument->ShortMarginRatio);*/
        LOGDEMO("\tStrikePrice [%.8lf]\n", pInstrument->StrikePrice);
        //outfile << "StrikePrice:" << pInstrument->StrikePrice << "\t";
        LOGDEMO("\tUnderlyingMultiple [%.8lf]\n", pInstrument->UnderlyingMultiple);
        //outfile << "UnderlyingMultiple:" << pInstrument->UnderlyingMultiple << "\n";
    }
    if (pRspInfo)
    {
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        LOGDEMO("\tErrorMsg [%s]\n", msg);
        //outfile << "ErrorMsg:" << msg << "\t";
        LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
        //outfile << "ErrorID:" << pRspInfo->ErrorID << "\n";
    }
    LOGDEMO("\tnRequestID [%d]\n", nRequestID);
    LOGDEMO("\tbIsLast [%d]\n", bIsLast);
    LOGDEMO("</OnRspQryInstrument>\n");

    /******write the instrumentID to queryInstrumentIdCnt for socket transmission******/
//		if (strlen(pInstrument->InstrumentID)<=6 && queryInstrumentIdCnt.cnt< MAXNUM_OF_INSTRUMENTID)
//		{
//			strcpy(queryInstrumentIdCnt.queryInstrumentId.InstrumentID[queryInstrumentIdCnt.cnt], \
//				pInstrument->InstrumentID);
//			(queryInstrumentIdCnt.cnt)++;
//		}
    if(strlen(pInstrument->InstrumentID)<=6)
    {
        json tmpInstrument = {
                {
                    {"instrumentId",pInstrument->InstrumentID},
                    {"exchangeID",pInstrument->ExchangeID},
                    {"IsTrading",pInstrument->IsTrading},
                }
        };
        AllInstruments["instruments"].push_back(tmpInstrument);
    }
    if(infoShowFlag.SingleInstrumentQuery)
    {
        InstrumentInfo = *pInstrument;
    }
    if(fillFlag.SingleInstrumentQuery)
    {
        InstrumentInfo = *pInstrument;
        INFO_LOG("%s","get InstrumentInfo!");
    }
    if (bIsLast)
    {
        sem_post(&globalSem.sem_query);
    }
}

//请求查询投资者结算结构1�7
void CSimpleHandler::ReqQrySettlementInfo()
{
    CThostFtdcQrySettlementInfoField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    string Traday;
    LOGDEMO("请输入交易日期或者交易月仄1�7(例如:20180101,月份刄1�71801):");
    cin >> Traday;
    strcpy(a.TradingDay, Traday.c_str());
    int b = m_pUserApi->ReqQrySettlementInfo(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询投资者结算结构1�7......发�1�7�成功\n" : "请求查询投资者结算结构1�7......发�1�7�失败，错误序号=[%d]\n", b);
}

//请求查询转帐流水
void CSimpleHandler::ReqQryTransferSerial()
{
    CThostFtdcQryTransferSerialField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.AccountID, g_chInvestorID);
cir1:int bankid;
    LOGDEMO("请输入你霄1�7要的查询的银行\n");
    LOGDEMO("1.工商银行\n");
    LOGDEMO("2.农业银行\n");
    LOGDEMO("3.中国银行\n");
    LOGDEMO("5.交�1�7�银行\n");
    LOGDEMO("6.招商银行\n");
    LOGDEMO("7.兴业银行\n");
    LOGDEMO("8.浦发银行\n");
    LOGDEMO("9.民生银行\n");
    LOGDEMO("10.光大银行\n");
    LOGDEMO("11.中信银行\n");
    LOGDEMO("12.汇丰银行\n");
    LOGDEMO("13.平安银行\n");
    LOGDEMO("14.农发银行\n");
    LOGDEMO("15.星展银行\n");
    LOGDEMO("16.广发银行\n");
    cin >> bankid;
    if (bankid == 1 | 2 | 3 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16)
    {
        //strcpy(a.BankID, itoa(bankid, a.BankID, 10));///银行代码
        //itoa(bankid, a.BankID, 10);
        sprintf(a.BankID, "%d", bankid);
    }
    else
    {
        LOGDEMO("请重新输入银行代码�1�7�\n");
        goto cir1;
    }
    int choos;
curr:LOGDEMO("请输入币种代码\t1.CNY\t2.USD\n");
    cin >> choos;
    switch (choos)
    {
    case 1:
        strcpy(a.CurrencyID, "CNY");
        break;
    case 2:
        strcpy(a.CurrencyID, "USD");
        break;
    default:
        LOGDEMO("请输入正确的序号\n");
        system("stty -echo");
        getchar();
        system("stty echo");
        goto curr;
    }
    int b = m_pUserApi->ReqQryTransferSerial(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询转帐流水......发�1�7�成功\n" : "请求查询转帐流水......发�1�7�失败，错误序号=[%d]\n", b);
}

//请求查询产品
void CSimpleHandler::ReqQryProduct()
{
    CThostFtdcQryProductField a = { 0 };
    strcpy(a.ProductID, "sc");
    a.ProductClass = THOST_FTDC_PC_Futures;
    strcpy(a.ExchangeID, g_chExchangeID);
    m_pUserApi->ReqQryProduct(&a, nRequestID++);
}

//请求查询转帐银行
void CSimpleHandler::ReqQryTransferBank()
{
    CThostFtdcQryTransferBankField a = { 0 };
    strcpy(a.BankID, "3");
    int b = m_pUserApi->ReqQryTransferBank(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询转帐银行......发�1�7�成功\n" : "请求查询转帐银行......发�1�7�失败，错误序号=[%d]\n", b);
}

//请求查询交易通知
void CSimpleHandler::ReqQryTradingNotice()
{
    CThostFtdcQryTradingNoticeField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    int b = m_pUserApi->ReqQryTradingNotice(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询交易通知......发�1�7�成功\n" : "请求查询交易通知......发�1�7�失败，错误序号=[%d]\n", b);
}

//请求查询交易编码
void CSimpleHandler::ReqQryTradingCode()
{
    CThostFtdcQryTradingCodeField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.ExchangeID, g_chExchangeID);
    a.ClientIDType = THOST_FTDC_CIDT_Speculation;
    int b = m_pUserApi->ReqQryTradingCode(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询交易编码......发�1�7�成功\n" : "请求查询交易编码......发�1�7�失败，错误序号=[%d]\n", b);
}

//请求查询结算信息确认
void CSimpleHandler::ReqQrySettlementInfoConfirm()
{
    CThostFtdcQrySettlementInfoConfirmField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    //strcpy(a.AccountID, g_chInvestorID);
    strcpy(a.CurrencyID, "CNY");
    int b = m_pUserApi->ReqQrySettlementInfoConfirm(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询结算信息确认......发�1�7�成功\n" : "请求查询结算信息确认......发�1�7�失败，错误序号=[%d]\n", b);
}

//请求查询产品组1�7
void CSimpleHandler::ReqQryProductGroup()
{
    CThostFtdcQryProductGroupField a = { 0 };

}

//请求查询投资者单兄1�7
void CSimpleHandler::ReqQryInvestUnit()
{
    CThostFtdcQryInvestUnitField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    //strcpy(a.InvestorID, "00402");
    //strcpy(a.InvestorID, g_chInvestorID);
    int b = m_pUserApi->ReqQryInvestUnit(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询投资者单兄1�7......发�1�7�成功\n" : "请求查询投资者单兄1�7......发�1�7�失败，错误序号=[%d]\n", b);
}

//请求查询经纪公司交易参数
void CSimpleHandler::ReqQryBrokerTradingParams()
{
    CThostFtdcQryBrokerTradingParamsField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.CurrencyID, "CNY");
    int b = m_pUserApi->ReqQryBrokerTradingParams(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询经纪公司交易参数......发�1�7�成功\n" : "请求查询经纪公司交易参数......发�1�7�失败，错误序号=[%d]\n", b);
}

//请求查询询价
void CSimpleHandler::ReqQryForQuote()
{
    CThostFtdcQryForQuoteField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    strcpy(a.ExchangeID, g_chExchangeID);
    strcpy(a.InsertTimeStart, "");
    strcpy(a.InsertTimeEnd, "");
    strcpy(a.InvestUnitID, "");
    int b = m_pUserApi->ReqQryForQuote(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询询价......发�1�7�成功\n" : "请求查询询价......发�1�7�失败，错误序号=[%d]\n", b);
}

//请求查询报价
void CSimpleHandler::ReqQryQuote()
{
    CThostFtdcQryQuoteField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    strcpy(a.ExchangeID, g_chExchangeID);
    strcpy(a.QuoteSysID, "");
    strcpy(a.InsertTimeStart, "");
    strcpy(a.InsertTimeEnd, "");
    strcpy(a.InvestUnitID, "");
    int b = m_pUserApi->ReqQryQuote(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询询价......发�1�7�成功\n" : "请求查询询价......发�1�7�失败，错误序号=[%d]\n", b);
}

///询价录入请求
void CSimpleHandler::ReqForQuoteInsert()
{
    CThostFtdcInputForQuoteField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    //strcpy(a.ForQuoteRef, "");
    strcpy(a.UserID, g_chUserID);
    strcpy(a.ExchangeID, g_chExchangeID);
    //strcpy(a.InvestUnitID, "");
    //strcpy(a.IPAddress, "");
    //strcpy(a.MacAddress, "");
    int b = m_pUserApi->ReqForQuoteInsert(&a, nRequestID++);
    LOGDEMO((b == 0) ? "询价录入请求......发�1�7�成功\n" : "询价录入请求......发�1�7�失败，错误序号=[%d]\n", b);
}

///做市商报价录入请汄1�7
void CSimpleHandler::ReqQuoteInsert()
{
choose:int choose_Flag;
    LOGDEMO("请确认开平标志\t1.弄1�7仓\t2.平仓\n");
    cin >> choose_Flag;

    if (choose_Flag != 1 && choose_Flag != 2)
    {
        LOGDEMO("请重新�1�7�择弄1�7平标志\n");
        system("stty -echo");
        getchar();
        system("stty echo");
        choose_Flag = 0; //NULL;
        goto choose;
    }

    int price_bid;
    LOGDEMO("请输入买方向价格：\n");
    cin >> price_bid;

    int price_ask;
    LOGDEMO("请输入卖方向价格：\n");
    cin >> price_ask;
    LOGDEMO("买卖数量默认昄1�7�\n");
    string quoteref;
    LOGDEMO("请输入quoteref序号：\n");
    cin >> quoteref;
    string AskOrderRef;
    string BidOrderRef;
    LOGDEMO("请输入AskOrderRef序号:\n");
    cin >> AskOrderRef;
    LOGDEMO("请输入BidOrderRef序号:\n");
    cin >> BidOrderRef;
    system("stty -echo");
    getchar();
    system("stty echo");
    CThostFtdcInputQuoteField t = { 0 };
    strcpy(t.BrokerID, g_chBrokerID);
    strcpy(t.InvestorID, g_chInvestorID);
    strcpy(t.InstrumentID, g_chInstrumentID);
    strcpy(t.ExchangeID, g_chExchangeID);

    strcpy(t.QuoteRef, quoteref.c_str());
    strcpy(t.UserID, g_chUserID);
    t.AskPrice = price_ask;
    t.BidPrice = price_bid;
    t.AskVolume = 1;
    t.BidVolume = 1;
    if (choose_Flag == 1)
    {
        t.AskOffsetFlag = THOST_FTDC_OF_Open;///卖开平标忄1�7
        t.BidOffsetFlag = THOST_FTDC_OF_Open;///买开平标忄1�7
    }
    else if (choose_Flag == 2)
    {
        t.AskOffsetFlag = THOST_FTDC_OF_Close;///卖开平标忄1�7
        t.BidOffsetFlag = THOST_FTDC_OF_Close;///买开平标忄1�7
    }
    t.AskHedgeFlag = THOST_FTDC_HF_Speculation;///卖投机套保标忄1�7
    t.BidHedgeFlag = THOST_FTDC_HF_Speculation;///买投机套保标忄1�7

    strcpy(t.AskOrderRef, AskOrderRef.c_str());///衍生卖报单引甄1�7
    strcpy(t.BidOrderRef, BidOrderRef.c_str());///衍生买报单引甄1�7
    //strcpy(t.ForQuoteSysID, "");///应价编号
    //strcpy(t.InvestUnitID, "1");///投资单元代码
    int a = m_pUserApi->ReqQuoteInsert(&t, 1);
    LOGDEMO((a == 0) ? "做市商报价录入请汄1�7......发�1�7�成功\n" : "做市商报价录入请汄1�7......发�1�7�失败，错误序号=[%d]\n", a);
}

///报价通知
void CSimpleHandler::OnRtnQuote(CThostFtdcQuoteField *pQuote)
{
    if (pQuote && strcmp(pQuote->InvestorID, g_chInvestorID) != 0)
    {
        return;
    }
    else
    {
        CTraderSpi::OnRtnQuote(pQuote);
        //SetEvent(g_hEvent);
    }
}

//报价撤销
void CSimpleHandler::ReqQuoteAction()
{
    CThostFtdcInputQuoteActionField t = { 0 };
    strcpy(t.BrokerID, g_chBrokerID);
    strcpy(t.InvestorID, "00404");
    //strcpy(t.UserID, g_chUserID);
    strcpy(t.ExchangeID, "SHFE");
    strcpy(t.QuoteRef, "           8");
    t.FrontID = 7;
    t.SessionID = 1879781311;
    t.ActionFlag = THOST_FTDC_AF_Delete;
    strcpy(t.InstrumentID, "cu1905C55000");
    int a = m_pUserApi->ReqQuoteAction(&t, 1);
    printf("m_pUserApi->ReqQuoteAction = [%d]", a);
}

//查询朄1�7大报单数量请汄1�7
void CSimpleHandler::ReqQueryMaxOrderVolume()
{
    CThostFtdcQueryMaxOrderVolumeField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    a.Direction = THOST_FTDC_D_Buy;
    a.OffsetFlag = THOST_FTDC_OF_Open;
    a.HedgeFlag = THOST_FTDC_HF_Speculation;
    a.MaxVolume = 1;
    strcpy(a.BrokerID, g_chBrokerID);
    int b = m_pUserApi->ReqQueryMaxOrderVolume(&a, nRequestID++);
    LOGDEMO((b == 0) ? "查询朄1�7大报单数量请汄1�7......发�1�7�成功\n" : "查询朄1�7大报单数量请汄1�7......发�1�7�失败，错误序号=[%d]\n", b);
}

//请求查询监控中心用户令牌
void CSimpleHandler::ReqQueryCFMMCTradingAccountToken()
{
    CThostFtdcQueryCFMMCTradingAccountTokenField a = { 0 };

}
///报单操作错误回报
void CSimpleHandler::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
    if (pOrderAction && strcmp(pOrderAction->InvestorID, g_chInvestorID) != 0)
    {
        return;
    }
    else
    {
        CTraderSpi::OnErrRtnOrderAction(pOrderAction, pRspInfo);
        //SetEvent(g_hEvent);
        sem_post(&globalSem.sem);
    }
}

///报单录入请求响应
void CSimpleHandler::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo,
    int nRequestID, bool bIsLast)
{
    repResult = InsertResult::Failed;
    if (pInputOrder && strcmp(pInputOrder->InvestorID, g_chInvestorID) != 0)
    {
        return;
    }
    else
    {
        CTraderSpi::OnRspOrderInsert(pInputOrder, pRspInfo, nRequestID, bIsLast);
    }
    ERROR_LOG("insert order error!");
    string orderRef = pInputOrder->OrderRef;
    globalSem.postSemByOrderRef(orderRef);
    DEBUG_LOG("post globalSem.sem, orderRef:%s",orderRef.c_str());
//    if(globalSem.semOrder1.orderRef == orderRef)
//    {
//       DEBUG_LOG("post globalSem.semOrder1.sem, orderRef:%s",orderRef.c_str());
//       sem_post(&globalSem.semOrder1.sem);
//    }
//    if(globalSem.semOrder2.orderRef == orderRef)
//    {
//        DEBUG_LOG("post globalSem.semOrder2.sem, orderRef:%s",orderRef.c_str());
//       sem_post(&globalSem.semOrder2.sem);
//    }
}

///报单录入错误回报
void CSimpleHandler::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
    repResult = InsertResult::Failed;
    if (pInputOrder && strcmp(pInputOrder->InvestorID, g_chInvestorID) != 0)
    {
        return;
    }
    else
    {
        CTraderSpi::OnErrRtnOrderInsert(pInputOrder, pRspInfo);
    }
    if(pRspInfo)
    {
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        auto& orderStates = OrderStates::getInstance();
        string orderKey = string(pInputOrder->OrderRef);
        if(! orderStates.insertState(orderKey, 'E'))
        {
            ERROR_LOG("insertState ERROR!");
        }
        PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
    }
    ERROR_LOG("insert order error!");
    string orderRef = pInputOrder->OrderRef;
    globalSem.postSemByOrderRef(orderRef);
    DEBUG_LOG("post globalSem.sem, orderRef:%s",orderRef.c_str());
//    if(globalSem.semOrder1.orderRef == orderRef)
//    {
//       DEBUG_LOG("post globalSem.semOrder1.sem, orderRef:%s",orderRef.c_str());
//       sem_post(&globalSem.semOrder1.sem);
//    }
//    if(globalSem.semOrder2.orderRef == orderRef)
//    {
//       DEBUG_LOG("post globalSem.semOrder2.sem, orderRef:%s",orderRef.c_str());
//       sem_post(&globalSem.semOrder2.sem);
//    }
}

///报单通知
void CSimpleHandler::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
    INFO_LOG("got insert order response!");
    if (pOrder && strcmp(pOrder->InvestorID, g_chInvestorID) != 0)
    {
        return;
    }  
    else
    {
        auto& orderStates = OrderStates::getInstance();
        string orderKey = orderkey::getOrderKey(pOrder);
        if(! orderStates.insertState(orderKey, pOrder->OrderStatus))
        {
            ERROR_LOG("insertState ERROR!");
        }
        INFO_LOG("continue OnRtnOrder log");
        CTraderSpi::OnRtnOrder(pOrder);
        strcpy(g_chOrderSysID, pOrder->OrderSysID);
        g_chFrontID = pOrder->FrontID;
        g_chSessionID = pOrder->SessionID;
        strcpy(g_chOrderRef, pOrder->OrderRef);
        strcpy(g_chExchangeID, pOrder->ExchangeID);
        if (pOrder->OrderStatus == THOST_FTDC_OST_AllTraded)///全部成交
        {
            INFO_LOG("报单全部成交");
            //SetEvent(g_hEvent);
        }
        else
        {
            if (pOrder->OrderStatus == THOST_FTDC_OST_PartTradedQueueing)///部分成交还在队列丄1�7
            {
                INFO_LOG("部分成交还在队列中");
            }
            if (pOrder->OrderStatus == THOST_FTDC_OST_PartTradedNotQueueing)///部分成交不在队列丄1�7
            {
                INFO_LOG("部分成交不在队列中");
            }
            if (pOrder->OrderStatus == THOST_FTDC_OST_NoTradeQueueing)///未成交还在队列中
            {
                chioce_action = 0;
                INFO_LOG("未成交还在队列中");
            }
            if (pOrder->OrderStatus == THOST_FTDC_OST_NoTradeNotQueueing)///未成交不在队列中
            {
                INFO_LOG("未成交不在队列中");
            }
            if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)///撤单
            {
                repResult = InsertResult::Failed;
                INFO_LOG("撤单");
                string orderRef = pOrder->OrderRef;
                DEBUG_LOG("orderRef:%s",orderRef.c_str());
//                DEBUG_LOG("globalSem.semOrder1.orderRef:%s; ",globalSem.semOrder1.orderRef.c_str());
//                DEBUG_LOG("globalSem.semOrder2.orderRef:%s; ",globalSem.semOrder2.orderRef.c_str());

                globalSem.postSemByOrderRef(orderRef);
                INFO_LOG("post globalSem %s, 撤单",orderRef.c_str());
//                if(postSemByOrderRef.semOrder1.orderRef == orderRef)
//                {
//                    DEBUG_LOG("semOrder1");
//                    INFO_LOG("%s","post globalSem.semOrder1.sem, 撤单");
//                    sem_post(&globalSem.semOrder1.sem);
//                }
//                if(globalSem.semOrder2.orderRef == orderRef)
//                {
//                    DEBUG_LOG("semOrder2");
//                    INFO_LOG("%s","post globalSem.semOrder2.sem, 撤单");
//                    sem_post(&globalSem.semOrder2.sem);
//                }
            }if (pOrder->OrderStatus == THOST_FTDC_OST_Unknown)///未知
            {
                INFO_LOG("未知");
            }
            if (pOrder->OrderStatus == THOST_FTDC_OST_NotTouched)///尚未触发
            {
                chioce_action = 1;
                INFO_LOG("尚未触发");
            }
            if (pOrder->OrderStatus == THOST_FTDC_OST_Touched)///已触叄1�7
            {
                INFO_LOG("已触叄1�7");
            }
        }

    }


}

///删除预埋单响庄1�7
void CSimpleHandler::OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo,
    int nRequestID, bool bIsLast)
{
    if (pRemoveParkedOrder && strcmp(pRemoveParkedOrder->InvestorID, g_chInvestorID) != 0)
    {
        return;
    }
    else
    {
        strcpy(g_chParkedOrderID1, pRemoveParkedOrder->ParkedOrderID);
        CTraderSpi::OnRspRemoveParkedOrder(pRemoveParkedOrder, pRspInfo, nRequestID, bIsLast);
        //SetEvent(g_hEvent);
        sem_post(&globalSem.sem);
    }
}

///删除预埋撤单响应
void CSimpleHandler::OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo,
    int nRequestID, bool bIsLast)
{
    if (pRemoveParkedOrderAction && strcmp(pRemoveParkedOrderAction->InvestorID, g_chInvestorID) != 0)
    {
        return;
    }
    else
    {
        strcpy(g_chParkedOrderActionID1, pRemoveParkedOrderAction->ParkedOrderActionID);
        CTraderSpi::OnRspRemoveParkedOrderAction(pRemoveParkedOrderAction, pRspInfo, nRequestID, bIsLast);
        ////SetEvent(g_hEvent);
        sem_post(&globalSem.sem);

    }
}

///预埋单录入请求响庄1�7
void CSimpleHandler::OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo,
    int nRequestID, bool bIsLast)
{
    if (pParkedOrder && strcmp(pParkedOrder->InvestorID, g_chInvestorID) != 0)
    {
        return;
    }
    else
    {
        strcpy(g_chParkedOrderID1, pParkedOrder->ParkedOrderID);
        CTraderSpi::OnRspParkedOrderInsert(pParkedOrder, pRspInfo, nRequestID, bIsLast);
        //SetEvent(g_hEvent);
        sem_post(&globalSem.sem);
    }
}

///预埋撤单录入请求响应
void CSimpleHandler::OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo,
    int nRequestID, bool bIsLast)
{
    if (pParkedOrderAction && strcmp(pParkedOrderAction->InvestorID, g_chInvestorID) != 0)
    {
        return;
    }
    else
    {
        strcpy(g_chParkedOrderActionID1, pParkedOrderAction->ParkedOrderActionID);
        CTraderSpi::OnRspParkedOrderAction(pParkedOrderAction, pRspInfo, nRequestID, bIsLast);
        //SetEvent(g_hEvent);
        sem_post(&globalSem.sem);
    }
}

///请求查询预埋撤单响应
void CSimpleHandler::OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CTraderSpi::OnRspQryParkedOrderAction(pParkedOrderAction, pRspInfo, nRequestID, bIsLast);
}

///请求查询预埋单响庄1�7
void CSimpleHandler::OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CTraderSpi::OnRspQryParkedOrder(pParkedOrder, pRspInfo, nRequestID, bIsLast);
}

///请求查询报单响应
void CSimpleHandler::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (pOrder) {
        vector_OrderSysID.push_back(pOrder->OrderSysID);
        vector_ExchangeID.push_back(pOrder->ExchangeID);
        vector_InstrumentID.push_back(pOrder->InstrumentID);
    }
    CTraderSpi::OnRspQryOrder(pOrder, pRspInfo, nRequestID, bIsLast);
    action_number++;
    LOGDEMO("\n查询序号：\"%d\"\n\n", action_number);
}

///执行宣告通知
void CSimpleHandler::OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder)
{
    if (pExecOrder) {
        strcpy(g_NewExecOrderRef, pExecOrder->ExecOrderRef);
        strcpy(g_NewExecOrderSysID, pExecOrder->ExecOrderSysID);
        g_NewFrontID = pExecOrder->FrontID;
        g_NewSessionID = pExecOrder->SessionID;
    }
    CTraderSpi::OnRtnExecOrder(pExecOrder);
}

//期货发起查询银行余额请求
void CSimpleHandler::ReqQueryBankAccountMoneyByFuture()
{
    CThostFtdcReqQueryAccountField a = { 0 };
    int b = m_pUserApi->ReqQueryBankAccountMoneyByFuture(&a, nRequestID++);
    LOGDEMO((b == 0) ? "期货发起查询银行余额请求......发�1�7�成功\n" : "期货发起查询银行余额请求......发�1�7�失败，错误序号=[%d]\n", b);
}

//期货发起银行资金转期货请汄1�7
void CSimpleHandler::ReqFromBankToFutureByFuture()
{
    int output_num;
    LOGDEMO("请输入转账金预1�7");
    cin >> output_num;

    CThostFtdcReqTransferField a = { 0 };
    strcpy(a.TradeCode, "202001");///业务功能砄1�7
    int bankid = 0;
    while (bankid != 1 & 2 & 3 & 5 & 6 & 7 & 8 & 9 & 10 & 11 & 12 & 13 & 14 & 15 & 16) {
        LOGDEMO("请输入你霄1�7要的转账的银行\n");
        LOGDEMO("1.工商银行\n");
        LOGDEMO("2.农业银行\n");
        LOGDEMO("3.中国银行\n");
        LOGDEMO("5.交�1�7�银行\n");
        LOGDEMO("6.招商银行\n");
        LOGDEMO("7.兴业银行\n");
        LOGDEMO("8.浦发银行\n");
        LOGDEMO("9.民生银行\n");
        LOGDEMO("10.光大银行\n");
        LOGDEMO("11.中信银行\n");
        LOGDEMO("12.汇丰银行\n");
        LOGDEMO("13.平安银行\n");
        LOGDEMO("14.农发银行\n");
        LOGDEMO("15.星展银行\n");
        LOGDEMO("16.广发银行\n");
        cin >> bankid;
        if (bankid == 1 | 2 | 3 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16)
        {
            //strcpy(a.BankID, itoa(bankid, a.BankID, 10));///银行代码
            //itoa(bankid, a.BankID, 10);
            sprintf(a.BankID, "%d", bankid);
        }
        else
        {
            LOGDEMO("请重新输入银行代码�1�7�\n");
            system("stty -echo");
            getchar();
            system("stty echo");
        }
    }


    strcpy(a.BankBranchID, "0000");///期商代码
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.TradeDate, "20170829");///交易日期
    strcpy(a.TradeTime, "09:00:00");
    strcpy(a.BankSerial, "6889");///银行流水叄1�7
    strcpy(a.TradingDay, "20170829");///交易系统日期
    a.PlateSerial = 5;///银期平台消息流水叄1�7
    a.LastFragment = THOST_FTDC_LF_Yes;///朄1�7后分片标忄1�7 '0'=是最后分牄1�7
    a.SessionID = SessionID;
    //strcpy(a.CustomerName, "");///客户姓名
    a.IdCardType = THOST_FTDC_ICT_IDCard;///证件类型
    a.CustType = THOST_FTDC_CUSTT_Person;///客户类型
    //strcpy(a.IdentifiedCardNo, "310115198706241914");///证件号码
    /*strcpy(a.BankAccount, "123456789");
    strcpy(a.BankPassWord, "123456");///银行密码*/
    strcpy(a.BankAccount, "621485212110187");
    //strcpy(a.BankPassWord, "092812");///银行密码--不需要银行卡密码
    strcpy(a.AccountID, g_chInvestorID);///投资者帐叄1�7
    //strcpy(a.Password, "092812");///期货密码--资金密码
    strcpy(a.Password, "123456");///期货密码--资金密码
    a.InstallID = 1;///安装编号
    a.FutureSerial = 0;///期货公司流水叄1�7
    a.VerifyCertNoFlag = THOST_FTDC_YNI_No;///验证客户证件号码标志
    strcpy(a.CurrencyID, "CNY");///币种代码
    a.TradeAmount = output_num;///转帐金额
    a.FutureFetchAmount = 0;///期货可取金额
    a.CustFee = 0;///应收客户费用
    a.BrokerFee = 0;///应收期货公司费用
    a.SecuPwdFlag = THOST_FTDC_BPWDF_BlankCheck;///期货资金密码核对标志
    a.RequestID = 0;///请求编号
    a.TID = 0;///交易ID
    int b = m_pUserApi->ReqFromBankToFutureByFuture(&a, 1);
    LOGDEMO((b == 0) ? "期货发起银行资金转期货请汄1�7......发�1�7�成功\n" : "期货发起银行资金转期货请汄1�7......发�1�7�失败，错误序号=[%d]\n", b);
}

//期货发起期货资金转银行请汄1�7
void CSimpleHandler::ReqFromFutureToBankByFuture()
{
    int output_num;
    LOGDEMO("请输入转账金预1�7");
    cin >> output_num;

    CThostFtdcReqTransferField a = { 0 };
    strcpy(a.TradeCode, "202002");///业务功能砄1�7
bankid_new:int bankid = 0;
    LOGDEMO("请输入你霄1�7要的转账的银行\n");
    LOGDEMO("1.工商银行\n");
    LOGDEMO("2.农业银行\n");
    LOGDEMO("3.中国银行\n");
    LOGDEMO("5.交�1�7�银行\n");
    LOGDEMO("6.招商银行\n");
    LOGDEMO("7.兴业银行\n");
    LOGDEMO("8.浦发银行\n");
    LOGDEMO("9.民生银行\n");
    LOGDEMO("10.光大银行\n");
    LOGDEMO("11.中信银行\n");
    LOGDEMO("12.汇丰银行\n");
    LOGDEMO("13.平安银行\n");
    LOGDEMO("14.农发银行\n");
    LOGDEMO("15.星展银行\n");
    LOGDEMO("16.广发银行\n");
    cin >> bankid;
    if (bankid == 1 | 2 | 3 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16)
    {
        //strcpy(a.BankID, itoa(bankid, a.BankID, 10));///银行代码
        //itoa(bankid, a.BankID, 10);
        sprintf(a.BankID, "%d", bankid);
    }
    else {
        LOGDEMO("请输入正确的编号\n");
        system("stty -echo");
        getchar();
        system("stty echo");
        goto bankid_new;
    }
    strcpy(a.BankBranchID, "0000");///期商代码
    strcpy(a.BrokerID, g_chBrokerID);
    //strcpy(a.BankBranchID, "0000");///银行分支机构代码
    //strcpy(a.TradeDate, "20170829");///交易日期
    //strcpy(a.TradeTime, "09:00:00");
    //strcpy(a.BankSerial, "");///银行流水叄1�7
    //strcpy(a.TradingDay, "20170829");///交易系统日期
    //a.PlateSerial= 0;///银期平台消息流水叄1�7
    a.LastFragment = THOST_FTDC_LF_Yes;///朄1�7后分片标忄1�7 '0'=是最后分牄1�7
    a.SessionID = SessionID;
    //strcpy(a.CustomerName, "");///客户姓名
    a.IdCardType = THOST_FTDC_ICT_IDCard;///证件类型
    strcpy(a.IdentifiedCardNo, "310115198706241914");///证件号码
    strcpy(a.BankAccount, "123456789");///银行帐号
    //strcpy(a.BankPassWord, "123456");///银行密码
    strcpy(a.AccountID, g_chInvestorID);///投资者帐叄1�7
    strcpy(a.Password, "123456");///期货密码
    a.InstallID = 1;///安装编号
    a.CustType = THOST_FTDC_CUSTT_Person;
    //a.FutureSerial = 0;///期货公司流水叄1�7
    a.VerifyCertNoFlag = THOST_FTDC_YNI_No;///验证客户证件号码标志
    strcpy(a.CurrencyID, "CNY");///币种代码
    a.TradeAmount = output_num;///转帐金额
    a.FutureFetchAmount = 0;///期货可取金额
    a.CustFee = 0;///应收客户费用
    a.BrokerFee = 0;///应收期货公司费用
    //a.SecuPwdFlag = THOST_FTDC_BPWDF_BlankCheck;///期货资金密码核对标志
    a.RequestID = 0;///请求编号
    a.TID = 0;///交易ID
    int b = m_pUserApi->ReqFromFutureToBankByFuture(&a, 1);
    LOGDEMO((b == 0) ? "期货发起期货资金转银行请汄1�7......发�1�7�成功\n" : "期货发起期货资金转银行请汄1�7......发�1�7�失败，错误序号=[%d]\n", b);
}

//期权自对冲录入请汄1�7
void CSimpleHandler::ReqOptionSelfCloseInsert()
{
    CThostFtdcInputOptionSelfCloseField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    strcpy(a.OptionSelfCloseRef, "1");
    strcpy(a.UserID, g_chUserID);
    a.Volume = 1;

    int choose_1 = 0;
    while (choose_1 != 1 && choose_1 != 2 && choose_1 != 3 && choose_1 != 4) {
        LOGDEMO("请�1�7�择投机套保标志\n1.投机\t2.套利\t3.套保\t4.做市商\n");
        cin >> choose_1;
        if (choose_1 == 1) { a.HedgeFlag = THOST_FTDC_HF_Speculation; }
        else if (choose_1 == 2) { a.HedgeFlag = THOST_FTDC_HF_Arbitrage; }
        else if (choose_1 == 3) { a.HedgeFlag = THOST_FTDC_HF_Hedge; }
        else if (choose_1 == 4) { a.HedgeFlag = THOST_FTDC_HF_MarketMaker; }
        else {
            LOGDEMO("选项错误，请重新选择。\n");
            system("stty -echo");
            getchar();
            system("stty echo");
        }
    }

    int choose_2 = 0;
    while (choose_2 != 1 && choose_2 != 2 && choose_2 != 3) {
        LOGDEMO("请�1�7�择期权行权的头寸是否自对冲标志\n1.自对冲期权仓位\t2.保留期权仓位\t3.自对冲卖方履约后的期货仓位\n");
        cin >> choose_2;
        if (choose_2 == 1) { a.OptSelfCloseFlag = THOST_FTDC_OSCF_CloseSelfOptionPosition; }
        else if (choose_2 == 2) { a.OptSelfCloseFlag = THOST_FTDC_OSCF_ReserveOptionPosition; }
        else if (choose_2 == 3) { a.OptSelfCloseFlag = THOST_FTDC_OSCF_SellCloseSelfFuturePosition; }
        else {
            LOGDEMO("选项错误，请重新选择。\n");
            system("stty -echo");
            getchar();
            system("stty echo");
            continue;
        }
    }

    strcpy(a.ExchangeID, g_chExchangeID);
    string accountid_new;
    LOGDEMO("请输入资金账叄1�7\n");
    cin >> accountid_new;
    strcpy(a.AccountID, accountid_new.c_str());
    strcpy(a.CurrencyID, "CNY");
    int b = m_pUserApi->ReqOptionSelfCloseInsert(&a, 1);
    LOGDEMO((b == 0) ? "期权自对冲录入请汄1�7......发�1�7�成功\n" : "期权自对冲录入请汄1�7......发�1�7�失败，错误序号=[%d]\n", b);
}

///期权自对冲�1�7�知
void CSimpleHandler::OnRtnOptionSelfClose(CThostFtdcOptionSelfCloseField *pOptionSelfClose)
{
    if (pOptionSelfClose) {
        g_chFrontID = pOptionSelfClose->FrontID;
        g_chSessionID = pOptionSelfClose->SessionID;
        strcpy(g_chOptionSelfCloseSysID, pOptionSelfClose->OptionSelfCloseSysID);//期权自对冲编叄1�7
        strcpy(g_chOptionSelfCloseRef, pOptionSelfClose->OptionSelfCloseRef);//期权自对冲引甄1�7
    }
    CTraderSpi::OnRtnOptionSelfClose(pOptionSelfClose);
}

//期权自对冲操作请汄1�7
void CSimpleHandler::ReqOptionSelfCloseAction()
{
    CThostFtdcInputOptionSelfCloseActionField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    //strcpy(a.OptionSelfCloseSysID, g_chOptionSelfCloseSysID);//期权自对冲编叄1�7
    strcpy(a.OptionSelfCloseRef, g_chOptionSelfCloseRef);//期权自对冲引甄1�7
    //a.FrontID = g_chFrontID;
    //a.SessionID = g_chSessionID;
    strcpy(a.ExchangeID, g_chExchangeID);
    a.ActionFlag = THOST_FTDC_AF_Delete;
    strcpy(a.UserID, g_chUserID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    int b = m_pUserApi->ReqOptionSelfCloseAction(&a, 1);
    LOGDEMO((b == 0) ? "期权自对冲操作请汄1�7......发�1�7�成功\n" : "期权自对冲操作请汄1�7......发�1�7�失败，错误序号=[%d]\n", b);
}

//请求查询期权自对冄1�7
void CSimpleHandler::ReqQryOptionSelfClose()
{
    CThostFtdcQryOptionSelfCloseField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    strcpy(a.ExchangeID, g_chExchangeID);
    int b = m_pUserApi->ReqQryOptionSelfClose(&a, 1);
    LOGDEMO((b == 0) ? "请求查询期权自对冄1�7......发�1�7�成功\n" : "请求查询期权自对冄1�7......发�1�7�失败，错误序号=[%d]\n", b);
}

///请求查询期权自对冲响庄1�7
void CSimpleHandler::OnRspQryOptionSelfClose(CThostFtdcOptionSelfCloseField *pOptionSelfClose, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (pOptionSelfClose) {
        g_chFrontID = pOptionSelfClose->FrontID;
        g_chSessionID = pOptionSelfClose->SessionID;
        strcpy(g_chOptionSelfCloseSysID, pOptionSelfClose->OptionSelfCloseSysID);//期权自对冲编叄1�7
        strcpy(g_chOptionSelfCloseRef, pOptionSelfClose->OptionSelfCloseRef);//期权自对冲引甄1�7
    }
    CTraderSpi::OnRspQryOptionSelfClose(pOptionSelfClose, pRspInfo, nRequestID, bIsLast);
}

///请求查询执行宣告
void CSimpleHandler::ReqQryExecOrder()
{
    CThostFtdcQryExecOrderField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    strcpy(a.ExchangeID, g_chExchangeID);
    strcpy(a.ExecOrderSysID, "");
    strcpy(a.InsertTimeStart, "");
    strcpy(a.InsertTimeEnd, "");
    int b = m_pUserApi->ReqQryExecOrder(&a, 1);
    LOGDEMO((b == 0) ? "执行宣告查询......发�1�7�成功\n" : "执行宣告查询......发�1�7�失败，错误序号=[%d]\n", b);
}

///查询二代资金账户
void CSimpleHandler::ReqQrySecAgentTradingAccount()
{
    CThostFtdcQryTradingAccountField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.CurrencyID, "CNY");
    a.BizType = THOST_FTDC_BZTP_Future;
    strcpy(a.AccountID, g_chInvestorID);
    int b = m_pUserApi->ReqQrySecAgentTradingAccount(&a, 1);
    LOGDEMO((b == 0) ? "查询二代资金账户......发�1�7�成功\n" : "查询二代资金账户......发�1�7�失败，错误序号=[%d]\n", b);
}

///请求查询二级代理商资金校验模弄1�7
void CSimpleHandler::ReqQrySecAgentCheckMode()
{
    CThostFtdcQrySecAgentCheckModeField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    int b = m_pUserApi->ReqQrySecAgentCheckMode(&a, 1);
    LOGDEMO((b == 0) ? "请求查询二级代理商资金校验模弄1�7......发�1�7�成功\n" : "请求查询二级代理商资金校验模弄1�7......发�1�7�失败，错误序号=[%d]\n", b);
}

///注册用户终端信息，用于中继服务器多连接模弄1�7
///霄1�7要在终端认证成功后，用户登录前调用该接口
void CSimpleHandler::RegisterUserSystemInfo()
{
    char pSystemInfo[344];
    int len;
    CTP_GetSystemInfo(pSystemInfo, len);

    CThostFtdcUserSystemInfoField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.UserID, g_chUserID);
    memcpy(a.ClientSystemInfo, pSystemInfo, len);
    a.ClientSystemInfoLen = len;

    /*string ip;
    ip.clear();
    cin.ignore();
    LOGDEMO("请输入ip地址(不输入则为空)\n");
    getline(cin, ip);
    strcpy(a.ClientPublicIP, ip.c_str());*/
    strcpy(a.ClientPublicIP, "192.168.0.1");//ip地址

    //int Port;
    //Port = 0;
    //cin.ignore();
    //LOGDEMO("请输入端口号\n");
    //cin >> Port;
    //a.ClientIPPort = Port;//端口叄1�7
    a.ClientIPPort = 51305;//端口叄1�7

    /*string LoginTime;
    LoginTime.clear();
    cin.ignore();
    LOGDEMO("请输入登录时闄1�7(不输入则为空)\n");
    getline(cin, LoginTime);
    strcpy(a.ClientPublicIP, LoginTime.c_str());*/
    strcpy(a.ClientLoginTime, "20190121");
    strcpy(a.ClientAppID, g_chAppID);
    int b = m_pUserApi->RegisterUserSystemInfo(&a);
    LOGDEMO((b == 0) ? "注册用户终端信息......发�1�7�成功\n" : "注册用户终端信息......发�1�7�失败，错误序号=[%d]\n", b);
}

///上报用户终端信息，用于中继服务器操作员登录模弄1�7
///操作员登录后，可以多次调用该接口上报客户信息
void CSimpleHandler::SubmitUserSystemInfo()
{
    char pSystemInfo[344];
    int len;
    CTP_GetSystemInfo(pSystemInfo, len);

    CThostFtdcUserSystemInfoField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.UserID, g_chUserID);
    memcpy(a.ClientSystemInfo, pSystemInfo, len);
    a.ClientSystemInfoLen = len;

    /*string ip;
    ip.clear();
    cin.ignore();
    LOGDEMO("请输入ip地址(不输入则为空)\n");
    getline(cin, ip);
    strcpy(a.ClientPublicIP, ip.c_str());*/
    strcpy(a.ClientPublicIP, "192.168.0.1");//ip地址

    //int Port;
    //Port = 0;
    //cin.ignore();
    //LOGDEMO("请输入端口号\n");
    //cin >> Port;
    //a.ClientIPPort = Port;//端口叄1�7
    a.ClientIPPort = 51305;//端口叄1�7

    /*string LoginTime;
    LoginTime.clear();
    cin.ignore();
    LOGDEMO("请输入登录时闄1�7(不输入则为空)\n");
    getline(cin, LoginTime);
    strcpy(a.ClientPublicIP, LoginTime.c_str());*/
    strcpy(a.ClientLoginTime, "20190121");
    strcpy(a.ClientAppID, g_chAppID);
    int b = m_pUserApi->SubmitUserSystemInfo(&a);
    LOGDEMO((b == 0) ? "注册用户终端信息......发�1�7�成功\n" : "注册用户终端信息......发�1�7�失败，错误序号=[%d]\n", b);
}

///查询用户当前支持的认证模弄1�7
void CSimpleHandler::ReqUserAuthMethod()
{
    CThostFtdcReqUserAuthMethodField a = { 0 };
    strcpy(a.TradingDay, "20190308");
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.UserID, g_chUserID);
    int b = m_pUserApi->ReqUserAuthMethod(&a, nRequestID++);
    LOGDEMO((b == 0) ? "查询用户当前支持的认证模弄1�7......发�1�7�成功\n" : "查询用户当前支持的认证模弄1�7......发�1�7�失败，错误序号=[%d]\n", b);
}

///用户发出获取图形验证码请汄1�7
void CSimpleHandler::ReqGenUserCaptcha()
{
    CThostFtdcReqGenUserCaptchaField a = { 0 };
    strcpy(a.TradingDay, "");
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.UserID, g_chUserID);
    int b = m_pUserApi->ReqGenUserCaptcha(&a, nRequestID++);
    LOGDEMO((b == 0) ? "用户发出获取图形验证码请汄1�7......发�1�7�成功\n" : "用户发出获取图形验证码请汄1�7......发�1�7�失败，错误序号=[%d]\n", b);
}

///用户发出获取短信验证码请汄1�7
void CSimpleHandler::ReqGenUserText()
{
    CThostFtdcReqGenUserTextField a = { 0 };
    strcpy(a.TradingDay, "");
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.UserID, g_chUserID);
    int b = m_pUserApi->ReqGenUserText(&a, nRequestID++);
    LOGDEMO((b == 0) ? "用户发出获取短信验证码请汄1�7......发�1�7�成功\n" : "用户发出获取短信验证码请汄1�7......发�1�7�失败，错误序号=[%d]\n", b);
}

///用户发出带有图片验证码的登陆请求
void CSimpleHandler::ReqUserLoginWithCaptcha()
{
    CThostFtdcReqUserLoginWithCaptchaField a = { 0 };
    strcpy(a.TradingDay, "");
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.UserID, g_chUserID);
    strcpy(a.Password, g_chPassword);
    strcpy(a.UserProductInfo, "");
    strcpy(a.InterfaceProductInfo, "");
    strcpy(a.ProtocolInfo, "");//协议信息
    strcpy(a.MacAddress, "");//Mac地址
    strcpy(a.ClientIPAddress, "");//终端IP地址
    strcpy(a.LoginRemark, "");//登录主备
    strcpy(a.Captcha, "");//图形验证码的文字内容
    a.ClientIPPort = 10203;
    int b = m_pUserApi->ReqUserLoginWithCaptcha(&a, nRequestID++);
    LOGDEMO((b == 0) ? "用户发出带有图片验证码的登陆请求......发�1�7�成功\n" : "用户发出带有图片验证码的登陆请求......发�1�7�失败，错误序号=[%d]\n", b);
}

///用户发出带有短信验证码的登陆请求
void CSimpleHandler::ReqUserLoginWithText()
{
    CThostFtdcReqUserLoginWithTextField a = { 0 };
    strcpy(a.TradingDay, "");
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.UserID, g_chUserID);
    strcpy(a.Password, g_chPassword);
    strcpy(a.UserProductInfo, "");
    strcpy(a.InterfaceProductInfo, "");
    strcpy(a.MacAddress, "");
    strcpy(a.ClientIPAddress, "");
    strcpy(a.LoginRemark, "");
    strcpy(a.Text, "");
    a.ClientIPPort = 10000;
    int b = m_pUserApi->ReqUserLoginWithText(&a, nRequestID++);
    LOGDEMO((b == 0) ? "用户发出带有短信验证码的登陆请求......发�1�7�成功\n" :
        "用户发出带有短信验证码的登陆请求......发�1�7�失败，错误序号=[%d]\n", b);
}

///用户发出带有动�1�7�口令的登陆请求
void CSimpleHandler::ReqUserLoginWithOTP()
{
    CThostFtdcReqUserLoginWithOTPField a = { 0 };
    strcpy(a.TradingDay, "");
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.UserID, g_chUserID);
    strcpy(a.Password, g_chPassword);
    strcpy(a.UserProductInfo, "");
    strcpy(a.InterfaceProductInfo, "");
    strcpy(a.MacAddress, "");
    strcpy(a.ClientIPAddress, "");
    strcpy(a.LoginRemark, "");
    strcpy(a.OTPPassword, "");
    a.ClientIPPort = 10000;
    int b = m_pUserApi->ReqUserLoginWithOTP(&a, nRequestID++);
    LOGDEMO((b == 0) ? "用户发出带有动�1�7�口令的登陆请求......发�1�7�成功\n" : "用户发出带有动�1�7�口令的登陆请求......发�1�7�失败，错误序号=[%d]\n", b);
}

///请求查询二级代理商信恄1�7
void CSimpleHandler::ReqQrySecAgentTradeInfo()
{
    CThostFtdcQrySecAgentTradeInfoField a = { 0 };
    strcpy(a.BrokerID, "");
    strcpy(a.BrokerSecAgentID, "");
    int b = m_pUserApi->ReqQrySecAgentTradeInfo(&a, nRequestID++);
    LOGDEMO((b == 0) ? "请求查询二级代理商信恄1�7......发�1�7�成功\n" : "请求查询二级代理商信恄1�7......发�1�7�失败，错误序号=[%d]\n", b);
}

