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
// 鎶ュ崟褰曞叆鎿嶄綔鏄惁瀹屾垚鐨勬爣蹇�1锟�7
// Create a manual reset event with no signal
//HANDLE g_hEvent = CreateEvent(NULL, false, false, NULL);

/// 浼氬憳浠ｇ爜
TThostFtdcBrokerIDType g_chBrokerID;
/// 浜ゆ槗鐢ㄦ埛浠ｇ爜
TThostFtdcUserIDType g_chUserID;
/// 浜ゆ槗鐢ㄦ埛瀵嗙爜
TThostFtdcPasswordType g_chPassword;
/// 浜ゆ槗鎵�1锟�7浠ｇ爜
TThostFtdcExchangeIDType g_chExchangeID;
///鍚堢害浠ｇ爜
TThostFtdcInstrumentIDType	g_chInstrumentID;
///鎶曡祫鑰呬唬鐮�1锟�7
TThostFtdcInvestorIDType g_chInvestorID;
///棰勫煁鎾ゅ崟缂栧彿
TThostFtdcParkedOrderActionIDType	g_chParkedOrderActionID1;
///棰勫煁鎶ュ崟缂栧彿
TThostFtdcParkedOrderIDType	g_chParkedOrderID1;
///鎶ュ崟寮曠敤
TThostFtdcOrderRefType	g_chOrderRef;
///鍓嶇疆缂栧彿
TThostFtdcFrontIDType	g_chFrontID;
///浼氳瘽缂栧彿
TThostFtdcSessionIDType	g_chSessionID;
///鎶ュ崟缂栧彿
TThostFtdcOrderSysIDType	g_chOrderSysID;
///姝㈡崯浠�1锟�7
TThostFtdcPriceType	g_chStopPrice;
///鎶ヤ环寮曠敤
TThostFtdcOrderRefType	g_chQuoteRef;
int FrontID = 0;
int SessionID = 0;
int Limitprice = 0;
int nRequestID = 0;
int chioce_action = 0;//涓�1锟�7锟藉叏閮ㄦ姤

vector<string> vector_OrderSysID;
vector<string> vector_ExchangeID;
vector<string> vector_InstrumentID;
vector<string> md_InstrumentID;
int action_number;

///鎵ц瀹ｅ憡寮曠敤
TThostFtdcOrderRefType	g_NewExecOrderRef;
///鎵ц瀹ｅ憡缂栧彿
TThostFtdcExecOrderSysIDType	g_NewExecOrderSysID;
///鍓嶇疆缂栧彿
TThostFtdcFrontIDType	g_NewFrontID;
///浼氳瘽缂栧彿
TThostFtdcSessionIDType	g_NewSessionID;

//鏈熸潈鑷鍐插搷搴旓拷1锟�7锟界煡
///鏈熸潈鑷鍐茬紪鍙�1锟�7
TThostFtdcOrderSysIDType	g_chOptionSelfCloseSysID;
///鏈熸潈鑷鍐插紩鐢�1锟�7
TThostFtdcOrderRefType	g_chOptionSelfCloseRef;
///鐢ㄦ埛绔骇鍝佷俊鎭�1锟�7
TThostFtdcProductInfoType	g_chUserProductInfo;
///璁よ瘉鐮�1锟�7
TThostFtdcAuthCodeType	g_chAuthCode;
///App浠ｇ爜
TThostFtdcAppIDType	g_chAppID;

//HANDLE xinhao = CreateEvent(NULL, false, false, NULL);

//CTraderApi *pUserApi = new CTraderApi;

//琛屾儏绫�1锟�7

// 褰撳鎴风涓庝氦鏄撴墭绠＄郴缁熷缓绔嬭捣閫氫俊杩炴帴锛屽鎴风闇�1锟�7瑕佽繘琛岀櫥褰�1锟�7
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

// 褰撳鎴风涓庝氦鏄撴墭绠＄郴缁燂拷1锟�7锟戒俊杩炴帴鏂紑鏃讹紝璇ユ柟娉曡璋冪敤
void CSimpleMdHandler::OnFrontDisconnected(int nReason)
{
    // 褰撳彂鐢熻繖涓儏鍐靛悗锛孉PI浼氳嚜鍔ㄩ噸鏂拌繛鎺ワ紝瀹㈡埛绔彲涓嶅仛澶勭悊
    WARNING_LOG("<OnFrontDisconnected>");
    WARNING_LOG("\tnReason= = [%d]", nReason);
    WARNING_LOG("</OnFrontDisconnected>");
}

	// 褰撳鎴风鍙戝嚭鐧诲綍璇锋眰涔嬪悗锛岃鏂规硶浼氳璋冪敤锛岋拷1锟�7锟界煡瀹㈡埛绔櫥褰曟槸鍚︽垚鍔�1锟�7
void CSimpleMdHandler::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    LOGDEMO("OnRspUserLogin:\n");
    LOGDEMO("\tErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,
        pRspInfo->ErrorMsg);
    LOGDEMO("\tRequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
    if (pRspInfo->ErrorID != 0) {
        // 绔櫥澶辫触锛屽鎴风闇�1锟�7杩涜閿欒澶勭悊
        LOGDEMO("\tFailed to login, errorcode=%d errormsg=%s requestid=%d chain = %d",
            pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
        exit(-1);
    }
    //SetEvent(xinhao);
    sem_post(&globalSem.sem_login);
    //SubscribeMarketData();//璁㈤槄琛屾儏
    //SubscribeForQuoteRsp();//璇环璇锋眰
}

	void CSimpleMdHandler::SubscribeMarketData()//鏀惰鎯�1锟�7
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
				LOGDEMO((result == 0) ? "璁㈤槄琛屾儏璇锋眰1......鍙戯拷1锟�7锟芥垚鍔焅n" : "璁㈤槄琛屾儏璇锋眰1......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", result);
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
				LOGDEMO((result == 0) ? "璁㈤槄琛屾儏璇锋眰2......鍙戯拷1锟�7锟芥垚鍔焅n" : "璁㈤槄琛屾儏璇锋眰2......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", result);
			}
		}
	}

	///璁㈤槄琛屾儏搴旂瓟
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

	///娣卞害琛屾儏閫氱煡
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

	///璁㈤槄璇环璇锋眰
	void CSimpleMdHandler::SubscribeForQuoteRsp()
	{
		LOGDEMO("琛屾儏涓闃呰浠疯姹俓n");
		char **ppInstrumentID = new char*[50];
		string g_chInstrumentID = getConfig("market", "InstrumentID");
		ppInstrumentID[0] = const_cast<char *>(g_chInstrumentID.c_str());
		int result = m_pUserMdApi->SubscribeForQuoteRsp(ppInstrumentID, 1);
	}

	///璁㈤槄璇环搴旂瓟
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


	///璇环閫氱煡
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
//浜ゆ槗绫�1锟�7

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

//瀹㈡埛绔璇�1锟�7
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
    INFO_LOG("\t瀹㈡埛绔璇�1锟�7 = [%d]\n", b);
}

///瀹㈡埛绔璇佸搷搴�1锟�7
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
    // 鍙戝嚭鐧婚檰璇锋眰
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

///鐧诲嚭璇锋眰鍝嶅簲
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

///璇锋眰纭缁撶畻鍗�1锟�7
void CSimpleHandler::ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *Confirm, int nRequestID)
{
//		CThostFtdcSettlementInfoConfirmField Confirm = { 0 };
//		///缁忕邯鍏徃浠ｇ爜
//		strcpy(Confirm.BrokerID, g_chBrokerID);
//		///鎶曡祫鑰呬唬鐮�1锟�7
//		strcpy(Confirm.InvestorID, g_chUserID);
    m_pUserApi->ReqSettlementInfoConfirm(Confirm, nRequestID);
}

///鎶曡祫鑰呯粨绠楃粨鏋滅‘璁ゅ搷搴�1锟�7
void CSimpleHandler::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CTraderSpi::OnRspSettlementInfoConfirm(pSettlementInfoConfirm, pRspInfo, nRequestID, bIsLast);
    //SetEvent(g_hEvent);
    sem_post(&globalSem.sem_settlement);
}

///鐢ㄦ埛鍙ｄ护鏇存柊璇锋眰
void CSimpleHandler::ReqUserPasswordUpdate()
{
    string newpassword;
    LOGDEMO("璇疯緭鍏ユ柊鐧诲綍瀵嗙爜锛歕n");
    cin >> newpassword;
    CThostFtdcUserPasswordUpdateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.UserID, g_chUserID);
    strcpy(a.OldPassword, g_chPassword);
    strcpy(a.NewPassword, newpassword.c_str());
    int b = m_pUserApi->ReqUserPasswordUpdate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐢ㄦ埛鍙ｄ护鏇存柊璇锋眰......鍙戯拷1锟�7锟芥垚鍔焅n" : "鐢ㄦ埛鍙ｄ护鏇存柊璇锋眰......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", b);
}

///鐢ㄦ埛鍙ｄ护鏇存柊璇锋眰鍝嶅簲
void CSimpleHandler::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CTraderSpi::OnRspUserPasswordUpdate(pUserPasswordUpdate, pRspInfo, nRequestID, bIsLast);
    //SetEvent(g_hEvent);
    sem_post(&globalSem.sem);
}

///璧勯噾璐︽埛鍙ｄ护鏇存柊璇锋眰
void CSimpleHandler::ReqTradingAccountPasswordUpdate()
{
    string newpassword;
    LOGDEMO("璇疯緭鍏ユ柊璧勯噾瀵嗙爜锛歕n");
    cin >> newpassword;
    CThostFtdcTradingAccountPasswordUpdateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.AccountID, g_chInvestorID);
    strcpy(a.OldPassword, g_chPassword);
    strcpy(a.NewPassword, newpassword.c_str());
    strcpy(a.CurrencyID, "CNY");
    int b = m_pUserApi->ReqTradingAccountPasswordUpdate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璧勯噾璐︽埛鍙ｄ护鏇存柊璇锋眰......鍙戯拷1锟�7锟芥垚鍔焅n" : "璧勯噾璐︽埛鍙ｄ护鏇存柊璇锋眰......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", b);
}

///璧勯噾璐︽埛鍙ｄ护鏇存柊璇锋眰鍝嶅簲
void CSimpleHandler::OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CTraderSpi::OnRspTradingAccountPasswordUpdate(pTradingAccountPasswordUpdate, pRspInfo, nRequestID, bIsLast);
    //SetEvent(g_hEvent);
    sem_post(&globalSem.sem);
}

///棰勫煁鍗曞綍鍏�1锟�7//闄愪环鍗�1锟�7
void CSimpleHandler::ReqParkedOrderInsert()
{
    int limitprice = 0;
    LOGDEMO("璇疯緭鍏ラ檺浠峰崟浠锋牸锛�1锟�7(榛樿0)\n");
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
    LOGDEMO((b == 0) ? "璇锋眰褰曞叆棰勫煁鍗�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰褰曞叆棰勫煁鍗�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", b);
}

///棰勫煁鎾ゅ崟褰曞叆璇锋眰
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
    LOGDEMO((b == 0) ? "璇锋眰褰曞叆棰勫煁鎾ゅ崟......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰褰曞叆棰勫煁鎾ゅ崟......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", b);
}

///璇锋眰鍒犻櫎棰勫煁鍗�1锟�7
void CSimpleHandler::ReqRemoveParkedOrder()
{
    CThostFtdcRemoveParkedOrderField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.ParkedOrderID, g_chParkedOrderID1);
    int b = m_pUserApi->ReqRemoveParkedOrder(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鍒犻櫎棰勫煁鍗�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鍒犻櫎棰勫煁鍗�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", b);
}

///璇锋眰鍒犻櫎棰勫煁鎾ゅ崟
void CSimpleHandler::ReqRemoveParkedOrderAction()
{
    CThostFtdcRemoveParkedOrderActionField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.ParkedOrderActionID, g_chParkedOrderActionID1);
    int b = m_pUserApi->ReqRemoveParkedOrderAction(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鍒犻櫎棰勫煁鎾ゅ崟......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鍒犻櫎棰勫煁鎾ゅ崟......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", b);
}

///鎶ュ崟褰曞叆璇锋眰
void CSimpleHandler::ReqOrderInsert_Ordinary()
{
    system("clear");
    int volume;
    string instrument_id;
    std::cout << "璇疯緭鍏ュ悎绾﹀悕锛�1锟�7" << std::endl;
    cin >> instrument_id;
    string new_limitprice;
    LOGDEMO("璇疯緭鍏ユ寚瀹氫环鏍硷細\n");
    cin >> new_limitprice;
    std::cout << "璇疯緭鍏ユ暟閲忥細" << std::endl;
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
Direction:LOGDEMO("璇凤拷1锟�7锟芥嫨涔板崠鏂瑰悜\t1.涔癨t2.鍗朶n");
    cin >> num1;
    if (num1 == 1) {
        ord.Direction = THOST_FTDC_D_Buy;//涔�1锟�7
    }
    else if (num1 == 2) {
        ord.Direction = THOST_FTDC_D_Sell;//鍗�1锟�7
    }
    else {
        LOGDEMO("杈撳叆閿欒璇烽噸鏂拌緭鍏n");
        system("stty -echo");
        getchar();
        system("stty echo");

        goto Direction;
    }

    int num2;
CombOffsetFlag:LOGDEMO("璇疯緭鍏ュ紑骞虫柟鍚慭t1.寮�1锟�7浠揬t2.骞充粨\t3.寮哄钩\t4.骞充粖\t5.骞虫槰\t6.寮哄噺\t7.鏈湴寮哄钩\n");
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
        LOGDEMO("杈撳叆閿欒璇烽噸鏂拌緭鍏n");
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
    ord.TimeCondition = THOST_FTDC_TC_GFD;///褰撴棩鏈夋晥
    ord.VolumeCondition = THOST_FTDC_VC_CV;///鍏ㄩ儴鏁伴噺
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "鎶ュ崟褰曞叆璇锋眰闄愪环鍗�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "鎶ュ崟褰曞叆璇锋眰闄愪环鍗�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", a);
}
void CSimpleHandler::ReqOrderInsert_Ordinary_hai(CThostFtdcInputOrderField &ord, int nRequestID)
{
    int a = m_pUserApi->ReqOrderInsert(&ord, nRequestID);
    INFO_LOG("%s",(a == 0) ? "Order insert request send succ, waiting for response from exchange ......\n" : "Order insert request send failed\n"); // @suppress("Invalid arguments")
}
///澶у晢鎵�1锟�7姝㈡崯鍗�1锟�7
void CSimpleHandler::ReqOrderInsert_Touch()
{
    int new_limitprice;
    LOGDEMO("璇疯緭鍏ユ寚瀹氫环鏍糽imitprice锛歕n");
    cin >> new_limitprice;

    int new_StopPrice;
    LOGDEMO("璇疯緭鍏ユ寚瀹氫环鏍約topprice锛歕n");
    cin >> new_StopPrice;

    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ord.Direction = THOST_FTDC_D_Buy;//涔�1锟�7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = 1;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///褰撴棩鏈夋晥
    ord.VolumeCondition = THOST_FTDC_VC_AV;///浠讳綍鏁伴噺
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Touch;
    ord.StopPrice = new_StopPrice;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "鎶ュ崟褰曞叆璇锋眰闄愪环鍗�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "鎶ュ崟褰曞叆璇锋眰闄愪环鍗�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", a);
}

///澶у晢鎵�1锟�7姝㈢泩鍗�1锟�7
void CSimpleHandler::ReqOrderInsert_TouchProfit()
{
    int new_limitprice;
    LOGDEMO("璇疯緭鍏ユ寚瀹氫环鏍糽imitprice锛歕n");
    cin >> new_limitprice;

    int new_StopPrice;
    LOGDEMO("璇疯緭鍏ユ寚瀹氫环鏍約topprice锛歕n");
    cin >> new_StopPrice;

    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ord.Direction = THOST_FTDC_D_Buy;//涔�1锟�7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = 1;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///褰撴棩鏈夋晥
    ord.VolumeCondition = THOST_FTDC_VC_AV;///鍏ㄩ儴鏁伴噺
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_TouchProfit;
    ord.StopPrice = new_StopPrice;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "鎶ュ崟褰曞叆璇锋眰闄愪环鍗�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "鎶ュ崟褰曞叆璇锋眰闄愪环鍗�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", a);
}

//鍏ㄦ垚鍏ㄦ挙
void CSimpleHandler::ReqOrderInsert_VC_CV()
{
    int new_limitprice;
    LOGDEMO("璇疯緭鍏ユ寚瀹氫环鏍硷細\n");
    cin >> new_limitprice;

    int insert_num;
    LOGDEMO("璇疯緭鍏ヤ笅鍗曟暟閲忥細\n");
    cin >> insert_num;

    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ord.Direction = THOST_FTDC_D_Buy;//涔�1锟�7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = insert_num;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///褰撴棩鏈夋晥
    ord.VolumeCondition = THOST_FTDC_VC_CV;///鍏ㄩ儴鏁伴噺
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "鎶ュ崟褰曞叆璇锋眰闄愪环鍗�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "鎶ュ崟褰曞叆璇锋眰闄愪环鍗�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", a);
}

//閮ㄦ垚閮ㄦ挙
void CSimpleHandler::ReqOrderInsert_VC_AV()
{
    int new_limitprice;
    LOGDEMO("璇疯緭鍏ユ寚瀹氫环鏍硷細\n");
    cin >> new_limitprice;

    int insert_num;
    LOGDEMO("璇疯緭鍏ヤ笅鍗曟暟閲忥細\n");
    cin >> insert_num;

    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ord.Direction = THOST_FTDC_D_Buy;//涔�1锟�7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = insert_num;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///褰撴棩鏈夋晥
    ord.VolumeCondition = THOST_FTDC_VC_AV;///浠讳綍鏁伴噺
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "鎶ュ崟褰曞叆璇锋眰闄愪环鍗�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "鎶ュ崟褰曞叆璇锋眰闄愪环鍗�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", a);
}

//甯備环鍗�1锟�7
void CSimpleHandler::ReqOrderInsert_AnyPrice()
{
    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
    ord.Direction = THOST_FTDC_D_Buy;//涔�1锟�7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    //ord.LimitPrice = new_limitprice;
    ord.LimitPrice = 0;
    ord.VolumeTotalOriginal = 1;
    ord.TimeCondition = THOST_FTDC_TC_IOC;///绔嬪嵆瀹屾垚锛屽惁鍒欐挙閿�1锟�7
    ord.VolumeCondition = THOST_FTDC_VC_AV;///浠讳綍鏁伴噺
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    //ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "鎶ュ崟褰曞叆璇锋眰闄愪环鍗�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "鎶ュ崟褰曞叆璇锋眰闄愪环鍗�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", a);
}

//甯備环杞檺浠峰崟(涓噾鎵�1锟�7)
void CSimpleHandler::ReqOrderInsert_BestPrice()
{
    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_BestPrice;
    ord.Direction = THOST_FTDC_D_Buy;//涔�1锟�7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    //ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = 1;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///褰撴棩鏈夋晥
    ord.VolumeCondition = THOST_FTDC_VC_AV;///浠讳綍鏁伴噺
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "鎶ュ崟褰曞叆璇锋眰闄愪环鍗�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "鎶ュ崟褰曞叆璇锋眰闄愪环鍗�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", a);
}

//濂楀埄鎸囦护
void CSimpleHandler::ReqOrderInsert_Arbitrage()
{
    int new_limitprice;
    LOGDEMO("璇疯緭鍏ユ寚瀹氫环鏍硷細\n");
    cin >> new_limitprice;

    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ord.Direction = THOST_FTDC_D_Buy;//涔�1锟�7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = 1;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///褰撴棩鏈夋晥
    ord.VolumeCondition = THOST_FTDC_VC_AV;///浠讳綍鏁伴噺
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "鎶ュ崟褰曞叆璇锋眰闄愪环鍗�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "鎶ュ崟褰曞叆璇锋眰闄愪环鍗�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", a);
}

//浜掓崲鍗�1锟�7
void CSimpleHandler::ReqOrderInsert_IsSwapOrder()
{
    int new_limitprice;
    LOGDEMO("璇疯緭鍏ユ寚瀹氫环鏍硷細\n");
    cin >> new_limitprice;

    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ord.Direction = THOST_FTDC_D_Buy;//涔�1锟�7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = 1;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///褰撴棩鏈夋晥
    ord.VolumeCondition = THOST_FTDC_VC_AV;///浠讳綍鏁伴噺
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    ord.IsSwapOrder = 1;//浜掓崲鍗曟爣蹇�1锟�7
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "鎶ュ崟褰曞叆璇锋眰闄愪环鍗�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "鎶ュ崟褰曞叆璇锋眰闄愪环鍗�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", a);
}

///鎶ュ崟鎿嶄綔璇锋眰
void CSimpleHandler::ReqOrderAction_Ordinary()
{
    CThostFtdcInputOrderActionField a = { 0 };

    strcpy(a.BrokerID, g_chBrokerID);     //缁忔祹鍏徃浠ｇ爜
    strcpy(a.InvestorID, g_chInvestorID);  //鎶曡祫鑰呬唬鐮�1锟�7
    strcpy(a.UserID, g_chUserID);        //鐢ㄦ埛浠ｇ爜
    //a.OrderActionRef = 1;

//      std::cout << "璇疯緭鍏ユ姤鍗曞紩鐢細" << std::endl;
//      cin >> g_chOrderRef;
    //strcpy(a.OrderRef, g_chOrderRef);     //鎶ュ崟寮曠敤
    //std::cout << "g_chOrderRef is:" << g_chOrderRef << std::endl;

    //a.FrontID = g_chFrontID;   //鍓嶇疆缂栧彿
    //a.SessionID = g_chSessionID; //浼氳瘽缂栧彿

    //a.LimitPrice =
    //a.VolumeChange =
    std::cout << "璇疯緭鍏ヤ氦鏄撴墍浠ｇ爜:\n";
    cin >> g_chExchangeID;
    strcpy(a.ExchangeID, g_chExchangeID);    //浜ゆ槗鎵�1锟�7浠ｇ爜

    std::cout << "璇疯緭鍏ュ悎绾﹀彿锛�1锟�7" << std::endl;
    cin >> g_chInstrumentID;                   //鍚堢害浠ｇ爜
    strcpy(a.InstrumentID, g_chInstrumentID);

    std::cout << "璇疯緭鍏ユ姤鍗曠紪鍙凤細" << std::endl;
    //cin >> g_chOrderSysID;
    cin.ignore(numeric_limits <streamsize> ::max(), '\n');
    cin.getline(g_chOrderSysID, sizeof(g_chOrderSysID));
    strcpy(a.OrderSysID, g_chOrderSysID);         //鎶ュ崟缂栧彿

    a.ActionFlag = THOST_FTDC_AF_Delete;    //鎿嶄綔鏍囧織

    int ab = m_pUserApi->ReqOrderAction(&a, nRequestID++);
    LOGDEMO((ab == 0) ? "鎶ュ崟鎿嶄綔璇锋眰......鍙戯拷1锟�7锟芥垚鍔焅n" : "鎶ュ崟鎿嶄綔璇锋眰......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", ab);
}

///鎵ц瀹ｅ憡褰曞叆璇锋眰
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
    OrderInsert.OffsetFlag = THOST_FTDC_OF_Close;//寮�1锟�7骞虫爣蹇�1锟�7
    OrderInsert.HedgeFlag = THOST_FTDC_HF_Speculation;//鎶曟満濂椾繚鏍囧織
    if (a == 0) {
        OrderInsert.ActionType = THOST_FTDC_ACTP_Exec;//鎵ц绫诲瀷绫诲瀷
    }
    if (a == 1) {
        OrderInsert.ActionType = THOST_FTDC_ACTP_Abandon;//鎵ц绫诲瀷绫诲瀷
    }
    OrderInsert.PosiDirection = THOST_FTDC_PD_Long;//鎸佷粨澶氱┖鏂瑰悜绫诲瀷
    OrderInsert.ReservePositionFlag = THOST_FTDC_EOPF_Reserve;//鏈熸潈琛屾潈鍚庢槸鍚︿繚鐣欐湡璐уご瀵哥殑鏍囪绫诲瀷
    //OrderInsert.ReservePositionFlag = THOST_FTDC_EOPF_UnReserve;//涓嶄繚鐣欏ご瀵�1锟�7
    OrderInsert.CloseFlag = THOST_FTDC_EOCF_NotToClose;//鏈熸潈琛屾潈鍚庣敓鎴愮殑澶村鏄惁鑷姩骞充粨绫诲瀷
    //OrderInsert.CloseFlag = THOST_FTDC_EOCF_AutoClose;//鑷姩骞充粨
    //strcpy(OrderInsert.InvestUnitID, "");AccountID
    //strcpy(OrderInsert.AccountID, "");
    //strcpy(OrderInsert.CurrencyID, "CNY");
    //strcpy(OrderInsert.ClientID, "");
    int b = m_pUserApi->ReqExecOrderInsert(&OrderInsert, 1);
    LOGDEMO((b == 0) ? "鎵ц瀹ｅ憡褰曞叆璇锋眰......鍙戯拷1锟�7锟芥垚鍔焅n" : "鎵ц瀹ｅ憡褰曞叆璇锋眰......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

///鎵ц瀹ｅ憡鎿嶄綔璇锋眰
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
    a.ActionFlag = THOST_FTDC_AF_Delete;//鍒犻櫎
    strcpy(a.UserID, g_chUserID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    //strcpy(a.InvestUnitID, "");
    //strcpy(a.IPAddress, "");
    //strcpy(a.MacAddress, "");
    int b = m_pUserApi->ReqExecOrderAction(&a, 1);
    LOGDEMO((b == 0) ? "鎵ц瀹ｅ憡鎿嶄綔璇锋眰......鍙戯拷1锟�7锟芥垚鍔焅n" : "鎵ц瀹ｅ憡鎿嶄綔璇锋眰......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//鎵归噺鎶ュ崟鎿嶄綔璇锋眰
void CSimpleHandler::ReqBatchOrderAction()
{
    CThostFtdcInputBatchOrderActionField a = { 0 };

}

///璇锋眰鏌ヨ鎶ュ崟
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
    LOGDEMO((ab == 0) ? "璇锋眰鏌ヨ鎶ュ崟......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ鎶ュ崟......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", ab);
}

///鎶ュ崟褰曞叆璇锋眰
void CSimpleHandler::ReqOrderInsert_Condition(int select_num)
{
    string limit_price;
    LOGDEMO("璇疯緭鍏ユ寚瀹氫环鏍�1锟�7(limitprice):\n");
    cin >> limit_price;

    string stop_price;
    LOGDEMO("璇疯緭鍏ヨЕ鍙戜环鏍�1锟�7(stopprice):\n");
    cin >> stop_price;

    CThostFtdcInputOrderField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    strcpy(a.UserID, g_chUserID);
    a.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    a.Direction = THOST_FTDC_D_Buy;//涔�1锟�7
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
    LOGDEMO((ab == 0) ? "璇锋眰鎶ュ叆鏉′欢鍗�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鎶ュ叆鏉′欢鍗�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", ab);
}

///鎶ュ崟鎿嶄綔璇锋眰
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
    LOGDEMO((ab == 0) ? "璇锋眰鎾ら攢鏉′欢鍗�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鎾ら攢鏉′欢鍗�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", ab);
}

//鎾ら攢鏌ヨ鐨勬姤鍗�1锟�7
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
    LOGDEMO((ab == 0) ? "璇锋眰鎾ら攢鏉′欢鍗�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鎾ら攢鏉′欢鍗�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", ab);
}

///璇锋眰鏌ヨ鎴愪氦
void CSimpleHandler::ReqQryTrade()
{
    CThostFtdcQryTradeField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    string instr;
    instr.clear();
    LOGDEMO("璇疯緭鍏ュ悎绾︿唬鐮�1锟�7(涓嶈緭鍏ュ垯涓虹┖)\n");
    cin.ignore();
    getline(cin, instr);
    strcpy(a.InstrumentID, instr.c_str());

    string Exch;
    Exch.clear();
    LOGDEMO("璇疯緭鍏ヤ氦鏄撴墍浠ｇ爜(涓嶈緭鍏ュ垯涓虹┖)\n");
    //cin.ignore();
    getline(cin, Exch);
    strcpy(a.ExchangeID, Exch.c_str());
    /*strcpy(a.TradeID, "");
    strcpy(a.TradeTimeStart, "");
    strcpy(a.TradeTimeEnd, "");*/
    int b = m_pUserApi->ReqQryTrade(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ鎴愪氦......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ鎴愪氦......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

///璇锋眰鏌ヨ棰勫煁鍗�1锟�7
void CSimpleHandler::ReqQryParkedOrder()
{
    CThostFtdcQryParkedOrderField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    //strcpy(a.InstrumentID, g_chInstrumentID);
    strcpy(a.ExchangeID, g_chExchangeID);
    int ab = m_pUserApi->ReqQryParkedOrder(&a, nRequestID++);
    LOGDEMO((ab == 0) ? "璇锋眰鏌ヨ棰勫煁鍗�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ棰勫煁鍗�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", ab);
}

//璇锋眰鏌ヨ鏈嶅姟鍣ㄩ鍩嬫挙鍗�1锟�7
void CSimpleHandler::ReqQryParkedOrderAction()
{
    CThostFtdcQryParkedOrderActionField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    strcpy(a.ExchangeID, g_chExchangeID);
    int ab = m_pUserApi->ReqQryParkedOrderAction(&a, nRequestID++);
    LOGDEMO((ab == 0) ? "璇锋眰鏌ヨ鏈嶅姟鍣ㄩ鍩嬫挙鍗�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ鏈嶅姟鍣ㄩ鍩嬫挙鍗�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", ab);
}

//璇锋眰鏌ヨ璧勯噾璐︽埛
void CSimpleHandler::ReqQryTradingAccount()
{
    CThostFtdcQryTradingAccountField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.CurrencyID, "CNY");
    int ab = m_pUserApi->ReqQryTradingAccount(&a, nRequestID++);
    LOGDEMO((ab == 0) ? "璇锋眰鏌ヨ璧勯噾璐︽埛......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ璧勯噾璐︽埛......鍙戯拷1锟�7锟藉け璐ワ紝搴忓彿=[%d]\n", ab);
}

//璇锋眰鏌ヨ鎶曡祫鑰呮寔浠�1锟�7
void CSimpleHandler::ReqQryInvestorPosition()
{
    CThostFtdcQryInvestorPositionField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    string instr;
    instr.clear();
    cin.ignore();
    LOGDEMO("璇疯緭鍏ュ悎绾︿唬鐮�1锟�7(涓嶈緭鍏ュ垯涓虹┖)锛歕n");
    getline(cin, instr);
    strcpy(a.InstrumentID, instr.c_str());

    string exch;
    exch.clear();
    cin.ignore();
    LOGDEMO("璇疯緭鍏ヤ氦鏄撴墍浠ｇ爜(涓嶈緭鍏ュ垯涓虹┖)锛歕n");
    getline(cin, exch);
    strcpy(a.ExchangeID, exch.c_str());
    //strcpy(a.InstrumentID, "SPD");
    int b = m_pUserApi->ReqQryInvestorPosition(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ鎶曡祫鑰呮寔浠�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ鎶曡祫鑰呮寔浠�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//璇锋眰鏌ヨ鎶曡祫鑰呮寔浠撴槑缁�1锟�7
void CSimpleHandler::ReqQryInvestorPositionDetail()
{
    CThostFtdcQryInvestorPositionDetailField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    string instr;
    instr.clear();
    cin.ignore();
    LOGDEMO("璇疯緭鍏ュ悎绾︿唬鐮�1锟�7(涓嶈緭鍏ュ垯涓虹┖)\n");
    getline(cin, instr);
    strcpy(a.InstrumentID, instr.c_str());
    string exch;
    exch.clear();
    cin.ignore();
    LOGDEMO("璇疯緭鍏ヤ氦鏄撴墍浠ｇ爜(涓嶈緭鍏ュ垯涓虹┖)锛歕n");
    getline(cin, exch);
    strcpy(a.ExchangeID, exch.c_str());
    //strcpy(a.InstrumentID, g_chInstrumentID);
    int b = m_pUserApi->ReqQryInvestorPositionDetail(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ鎶曡祫鑰呮寔浠撴槑缁�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ鎶曡祫鑰呮寔浠撴槑缁�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//璇锋眰鏌ヨ浜ゆ槗鎵�1锟�7淇濊瘉閲戠巼
void CSimpleHandler::ReqQryExchangeMarginRate()
{
    CThostFtdcQryExchangeMarginRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    a.HedgeFlag = THOST_FTDC_HF_Speculation;//鎶曟満
    int b = m_pUserApi->ReqQryExchangeMarginRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ浜ゆ槗鎵�1锟�7淇濊瘉閲戠巼......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ浜ゆ槗鎵�1锟�7淇濊瘉閲戠巼......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//璇锋眰鏌ヨ鍚堢害淇濊瘉閲戠巼
void CSimpleHandler::ReqQryInstrumentMarginRate()
{
    CThostFtdcQryInstrumentMarginRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    a.HedgeFlag = THOST_FTDC_HF_Speculation;//鎶曟満
    int b = m_pUserApi->ReqQryInstrumentMarginRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ鍚堢害淇濊瘉閲戠巼......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ鍚堢害淇濊瘉閲戠巼......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//璇锋眰鏌ヨ鍚堢害鎵嬬画璐圭巼
void CSimpleHandler::ReqQryInstrumentCommissionRate()
{
    CThostFtdcQryInstrumentCommissionRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    int b = m_pUserApi->ReqQryInstrumentCommissionRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ鍚堢害鎵嬬画璐圭巼......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ鍚堢害鎵嬬画璐圭巼......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//璇锋眰鏌ヨ鍋氬競鍟嗗悎绾︽墜缁垂鐜�1锟�7
void CSimpleHandler::ReqQryMMInstrumentCommissionRate()
{
    CThostFtdcQryMMInstrumentCommissionRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    int b = m_pUserApi->ReqQryMMInstrumentCommissionRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ鍋氬競鍟嗗悎绾︽墜缁垂鐜�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ鍋氬競鍟嗗悎绾︽墜缁垂鐜�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//璇锋眰鏌ヨ鍋氬競鍟嗘湡鏉冨悎绾︽墜缁垂
void CSimpleHandler::ReqQryMMOptionInstrCommRate()
{
    CThostFtdcQryMMOptionInstrCommRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    int b = m_pUserApi->ReqQryMMOptionInstrCommRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ鍋氬競鍟嗘湡鏉冨悎绾︽墜缁垂......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ鍋氬競鍟嗘湡鏉冨悎绾︽墜缁垂......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//璇锋眰鏌ヨ鎶ュ崟鎵嬬画璐�1锟�7
void CSimpleHandler::ReqQryInstrumentOrderCommRate()
{
    CThostFtdcQryInstrumentOrderCommRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    int b = m_pUserApi->ReqQryInstrumentOrderCommRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ鎶ュ崟鎵嬬画璐�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ鎶ュ崟鎵嬬画璐�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//璇锋眰鏌ヨ鏈熸潈鍚堢害鎵嬬画璐�1锟�7
void CSimpleHandler::ReqQryOptionInstrCommRate()
{
    CThostFtdcQryOptionInstrCommRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    string Inst;
    string Exch;
    string InvestUnit;
    LOGDEMO("璇疯緭鍏ュ悎绾︿唬鐮�1锟�7(涓嶅～鍒欎负绌�1锟�7)");
    cin >> Inst;
    LOGDEMO("璇疯緭鍏ヤ氦鏄撴墍浠ｇ爜:(涓嶅～鍒欎负绌�1锟�7)");
    cin >> Exch;
    LOGDEMO("璇疯緭鍏ユ姇璧勶拷1锟�7锟藉崟鍏冧唬鐮�1锟�7(涓嶅～鍒欎负绌�1锟�7)");
    cin >> InvestUnit;
    strcpy(a.InstrumentID, Inst.c_str());
    strcpy(a.ExchangeID, Exch.c_str());
    strcpy(a.InvestUnitID, InvestUnit.c_str());
    int b = m_pUserApi->ReqQryOptionInstrCommRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ鏈熸潈鍚堢害鎵嬬画璐�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ鏈熸潈鍚堢害鎵嬬画璐�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//璇锋眰鏌ヨ鍚堢害
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
        INFO_LOG("璇锋眰鏌ヨ鍚堢害......鍙戯拷1锟�7锟芥垚鍔�1锟�7");
        return;
    }

    INFO_LOG("璇锋眰鏌ヨ鍚堢害......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
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
///璇锋眰鏌ヨ鍚堢害鍝嶅簲
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
                    {"CreateDate",pInstrument->CreateDate},
                    {"OpenDate",pInstrument->OpenDate},
                    {"PriceTick",pInstrument->PriceTick}
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

//璇锋眰鏌ヨ鎶曡祫鑰呯粨绠楃粨鏋�1锟�7
void CSimpleHandler::ReqQrySettlementInfo()
{
    CThostFtdcQrySettlementInfoField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    string Traday;
    LOGDEMO("璇疯緭鍏ヤ氦鏄撴棩鏈熸垨鑰呬氦鏄撴湀浠�1锟�7(渚嬪:20180101,鏈堜唤鍒�1锟�71801):");
    cin >> Traday;
    strcpy(a.TradingDay, Traday.c_str());
    int b = m_pUserApi->ReqQrySettlementInfo(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ鎶曡祫鑰呯粨绠楃粨鏋�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ鎶曡祫鑰呯粨绠楃粨鏋�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//璇锋眰鏌ヨ杞笎娴佹按
void CSimpleHandler::ReqQryTransferSerial()
{
    CThostFtdcQryTransferSerialField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.AccountID, g_chInvestorID);
cir1:int bankid;
    LOGDEMO("璇疯緭鍏ヤ綘闇�1锟�7瑕佺殑鏌ヨ鐨勯摱琛孿n");
    LOGDEMO("1.宸ュ晢閾惰\n");
    LOGDEMO("2.鍐滀笟閾惰\n");
    LOGDEMO("3.涓浗閾惰\n");
    LOGDEMO("5.浜わ拷1锟�7锟介摱琛孿n");
    LOGDEMO("6.鎷涘晢閾惰\n");
    LOGDEMO("7.鍏翠笟閾惰\n");
    LOGDEMO("8.娴﹀彂閾惰\n");
    LOGDEMO("9.姘戠敓閾惰\n");
    LOGDEMO("10.鍏夊ぇ閾惰\n");
    LOGDEMO("11.涓俊閾惰\n");
    LOGDEMO("12.姹囦赴閾惰\n");
    LOGDEMO("13.骞冲畨閾惰\n");
    LOGDEMO("14.鍐滃彂閾惰\n");
    LOGDEMO("15.鏄熷睍閾惰\n");
    LOGDEMO("16.骞垮彂閾惰\n");
    cin >> bankid;
    if (bankid == 1 | 2 | 3 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16)
    {
        //strcpy(a.BankID, itoa(bankid, a.BankID, 10));///閾惰浠ｇ爜
        //itoa(bankid, a.BankID, 10);
        sprintf(a.BankID, "%d", bankid);
    }
    else
    {
        LOGDEMO("璇烽噸鏂拌緭鍏ラ摱琛屼唬鐮侊拷1锟�7锟絓n");
        goto cir1;
    }
    int choos;
curr:LOGDEMO("璇疯緭鍏ュ竵绉嶄唬鐮乗t1.CNY\t2.USD\n");
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
        LOGDEMO("璇疯緭鍏ユ纭殑搴忓彿\n");
        system("stty -echo");
        getchar();
        system("stty echo");
        goto curr;
    }
    int b = m_pUserApi->ReqQryTransferSerial(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ杞笎娴佹按......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ杞笎娴佹按......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//璇锋眰鏌ヨ浜у搧
void CSimpleHandler::ReqQryProduct()
{
    CThostFtdcQryProductField a = { 0 };
    strcpy(a.ProductID, "sc");
    a.ProductClass = THOST_FTDC_PC_Futures;
    strcpy(a.ExchangeID, g_chExchangeID);
    m_pUserApi->ReqQryProduct(&a, nRequestID++);
}

//璇锋眰鏌ヨ杞笎閾惰
void CSimpleHandler::ReqQryTransferBank()
{
    CThostFtdcQryTransferBankField a = { 0 };
    strcpy(a.BankID, "3");
    int b = m_pUserApi->ReqQryTransferBank(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ杞笎閾惰......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ杞笎閾惰......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//璇锋眰鏌ヨ浜ゆ槗閫氱煡
void CSimpleHandler::ReqQryTradingNotice()
{
    CThostFtdcQryTradingNoticeField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    int b = m_pUserApi->ReqQryTradingNotice(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ浜ゆ槗閫氱煡......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ浜ゆ槗閫氱煡......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//璇锋眰鏌ヨ浜ゆ槗缂栫爜
void CSimpleHandler::ReqQryTradingCode()
{
    CThostFtdcQryTradingCodeField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.ExchangeID, g_chExchangeID);
    a.ClientIDType = THOST_FTDC_CIDT_Speculation;
    int b = m_pUserApi->ReqQryTradingCode(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ浜ゆ槗缂栫爜......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ浜ゆ槗缂栫爜......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//璇锋眰鏌ヨ缁撶畻淇℃伅纭
void CSimpleHandler::ReqQrySettlementInfoConfirm()
{
    CThostFtdcQrySettlementInfoConfirmField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    //strcpy(a.AccountID, g_chInvestorID);
    strcpy(a.CurrencyID, "CNY");
    int b = m_pUserApi->ReqQrySettlementInfoConfirm(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ缁撶畻淇℃伅纭......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ缁撶畻淇℃伅纭......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//璇锋眰鏌ヨ浜у搧缁�1锟�7
void CSimpleHandler::ReqQryProductGroup()
{
    CThostFtdcQryProductGroupField a = { 0 };

}

//璇锋眰鏌ヨ鎶曡祫鑰呭崟鍏�1锟�7
void CSimpleHandler::ReqQryInvestUnit()
{
    CThostFtdcQryInvestUnitField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    //strcpy(a.InvestorID, "00402");
    //strcpy(a.InvestorID, g_chInvestorID);
    int b = m_pUserApi->ReqQryInvestUnit(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ鎶曡祫鑰呭崟鍏�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ鎶曡祫鑰呭崟鍏�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//璇锋眰鏌ヨ缁忕邯鍏徃浜ゆ槗鍙傛暟
void CSimpleHandler::ReqQryBrokerTradingParams()
{
    CThostFtdcQryBrokerTradingParamsField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.CurrencyID, "CNY");
    int b = m_pUserApi->ReqQryBrokerTradingParams(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ缁忕邯鍏徃浜ゆ槗鍙傛暟......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ缁忕邯鍏徃浜ゆ槗鍙傛暟......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//璇锋眰鏌ヨ璇环
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
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ璇环......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ璇环......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//璇锋眰鏌ヨ鎶ヤ环
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
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ璇环......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ璇环......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

///璇环褰曞叆璇锋眰
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
    LOGDEMO((b == 0) ? "璇环褰曞叆璇锋眰......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇环褰曞叆璇锋眰......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

///鍋氬競鍟嗘姤浠峰綍鍏ヨ姹�1锟�7
void CSimpleHandler::ReqQuoteInsert()
{
choose:int choose_Flag;
    LOGDEMO("璇风‘璁ゅ紑骞虫爣蹇梊t1.寮�1锟�7浠揬t2.骞充粨\n");
    cin >> choose_Flag;

    if (choose_Flag != 1 && choose_Flag != 2)
    {
        LOGDEMO("璇烽噸鏂帮拷1锟�7锟芥嫨寮�1锟�7骞虫爣蹇梊n");
        system("stty -echo");
        getchar();
        system("stty echo");
        choose_Flag = 0; //NULL;
        goto choose;
    }

    int price_bid;
    LOGDEMO("璇疯緭鍏ヤ拱鏂瑰悜浠锋牸锛歕n");
    cin >> price_bid;

    int price_ask;
    LOGDEMO("璇疯緭鍏ュ崠鏂瑰悜浠锋牸锛歕n");
    cin >> price_ask;
    LOGDEMO("涔板崠鏁伴噺榛樿鏄�1锟�7锟絓n");
    string quoteref;
    LOGDEMO("璇疯緭鍏uoteref搴忓彿锛歕n");
    cin >> quoteref;
    string AskOrderRef;
    string BidOrderRef;
    LOGDEMO("璇疯緭鍏skOrderRef搴忓彿:\n");
    cin >> AskOrderRef;
    LOGDEMO("璇疯緭鍏idOrderRef搴忓彿:\n");
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
        t.AskOffsetFlag = THOST_FTDC_OF_Open;///鍗栧紑骞虫爣蹇�1锟�7
        t.BidOffsetFlag = THOST_FTDC_OF_Open;///涔板紑骞虫爣蹇�1锟�7
    }
    else if (choose_Flag == 2)
    {
        t.AskOffsetFlag = THOST_FTDC_OF_Close;///鍗栧紑骞虫爣蹇�1锟�7
        t.BidOffsetFlag = THOST_FTDC_OF_Close;///涔板紑骞虫爣蹇�1锟�7
    }
    t.AskHedgeFlag = THOST_FTDC_HF_Speculation;///鍗栨姇鏈哄淇濇爣蹇�1锟�7
    t.BidHedgeFlag = THOST_FTDC_HF_Speculation;///涔版姇鏈哄淇濇爣蹇�1锟�7

    strcpy(t.AskOrderRef, AskOrderRef.c_str());///琛嶇敓鍗栨姤鍗曞紩鐢�1锟�7
    strcpy(t.BidOrderRef, BidOrderRef.c_str());///琛嶇敓涔版姤鍗曞紩鐢�1锟�7
    //strcpy(t.ForQuoteSysID, "");///搴斾环缂栧彿
    //strcpy(t.InvestUnitID, "1");///鎶曡祫鍗曞厓浠ｇ爜
    int a = m_pUserApi->ReqQuoteInsert(&t, 1);
    LOGDEMO((a == 0) ? "鍋氬競鍟嗘姤浠峰綍鍏ヨ姹�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "鍋氬競鍟嗘姤浠峰綍鍏ヨ姹�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", a);
}

///鎶ヤ环閫氱煡
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

//鎶ヤ环鎾ら攢
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

//鏌ヨ鏈�1锟�7澶ф姤鍗曟暟閲忚姹�1锟�7
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
    LOGDEMO((b == 0) ? "鏌ヨ鏈�1锟�7澶ф姤鍗曟暟閲忚姹�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "鏌ヨ鏈�1锟�7澶ф姤鍗曟暟閲忚姹�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//璇锋眰鏌ヨ鐩戞帶涓績鐢ㄦ埛浠ょ墝
void CSimpleHandler::ReqQueryCFMMCTradingAccountToken()
{
    CThostFtdcQueryCFMMCTradingAccountTokenField a = { 0 };

}
///鎶ュ崟鎿嶄綔閿欒鍥炴姤
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

///鎶ュ崟褰曞叆璇锋眰鍝嶅簲
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

///鎶ュ崟褰曞叆閿欒鍥炴姤
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

///鎶ュ崟閫氱煡
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
        if (pOrder->OrderStatus == THOST_FTDC_OST_AllTraded)///鍏ㄩ儴鎴愪氦
        {
            INFO_LOG("鎶ュ崟鍏ㄩ儴鎴愪氦");
            //SetEvent(g_hEvent);
        }
        else
        {
            if (pOrder->OrderStatus == THOST_FTDC_OST_PartTradedQueueing)///閮ㄥ垎鎴愪氦杩樺湪闃熷垪涓�1锟�7
            {
                INFO_LOG("閮ㄥ垎鎴愪氦杩樺湪闃熷垪涓�");
            }
            if (pOrder->OrderStatus == THOST_FTDC_OST_PartTradedNotQueueing)///閮ㄥ垎鎴愪氦涓嶅湪闃熷垪涓�1锟�7
            {
                INFO_LOG("閮ㄥ垎鎴愪氦涓嶅湪闃熷垪涓�");
            }
            if (pOrder->OrderStatus == THOST_FTDC_OST_NoTradeQueueing)///鏈垚浜よ繕鍦ㄩ槦鍒椾腑
            {
                chioce_action = 0;
                INFO_LOG("鏈垚浜よ繕鍦ㄩ槦鍒椾腑");
            }
            if (pOrder->OrderStatus == THOST_FTDC_OST_NoTradeNotQueueing)///鏈垚浜や笉鍦ㄩ槦鍒椾腑
            {
                INFO_LOG("鏈垚浜や笉鍦ㄩ槦鍒椾腑");
            }
            if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)///鎾ゅ崟
            {
                repResult = InsertResult::Failed;
                INFO_LOG("鎾ゅ崟");
                string orderRef = pOrder->OrderRef;
                DEBUG_LOG("orderRef:%s",orderRef.c_str());
//                DEBUG_LOG("globalSem.semOrder1.orderRef:%s; ",globalSem.semOrder1.orderRef.c_str());
//                DEBUG_LOG("globalSem.semOrder2.orderRef:%s; ",globalSem.semOrder2.orderRef.c_str());

                globalSem.postSemByOrderRef(orderRef);
                INFO_LOG("post globalSem %s, 鎾ゅ崟",orderRef.c_str());
//                if(postSemByOrderRef.semOrder1.orderRef == orderRef)
//                {
//                    DEBUG_LOG("semOrder1");
//                    INFO_LOG("%s","post globalSem.semOrder1.sem, 鎾ゅ崟");
//                    sem_post(&globalSem.semOrder1.sem);
//                }
//                if(globalSem.semOrder2.orderRef == orderRef)
//                {
//                    DEBUG_LOG("semOrder2");
//                    INFO_LOG("%s","post globalSem.semOrder2.sem, 鎾ゅ崟");
//                    sem_post(&globalSem.semOrder2.sem);
//                }
            }if (pOrder->OrderStatus == THOST_FTDC_OST_Unknown)///鏈煡
            {
                INFO_LOG("鏈煡");
            }
            if (pOrder->OrderStatus == THOST_FTDC_OST_NotTouched)///灏氭湭瑙﹀彂
            {
                chioce_action = 1;
                INFO_LOG("灏氭湭瑙﹀彂");
            }
            if (pOrder->OrderStatus == THOST_FTDC_OST_Touched)///宸茶Е鍙�1锟�7
            {
                INFO_LOG("宸茶Е鍙�1锟�7");
            }
        }

    }


}

///鍒犻櫎棰勫煁鍗曞搷搴�1锟�7
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

///鍒犻櫎棰勫煁鎾ゅ崟鍝嶅簲
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

///棰勫煁鍗曞綍鍏ヨ姹傚搷搴�1锟�7
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

///棰勫煁鎾ゅ崟褰曞叆璇锋眰鍝嶅簲
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

///璇锋眰鏌ヨ棰勫煁鎾ゅ崟鍝嶅簲
void CSimpleHandler::OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CTraderSpi::OnRspQryParkedOrderAction(pParkedOrderAction, pRspInfo, nRequestID, bIsLast);
}

///璇锋眰鏌ヨ棰勫煁鍗曞搷搴�1锟�7
void CSimpleHandler::OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CTraderSpi::OnRspQryParkedOrder(pParkedOrder, pRspInfo, nRequestID, bIsLast);
}

///璇锋眰鏌ヨ鎶ュ崟鍝嶅簲
void CSimpleHandler::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (pOrder) {
        vector_OrderSysID.push_back(pOrder->OrderSysID);
        vector_ExchangeID.push_back(pOrder->ExchangeID);
        vector_InstrumentID.push_back(pOrder->InstrumentID);
    }
    CTraderSpi::OnRspQryOrder(pOrder, pRspInfo, nRequestID, bIsLast);
    action_number++;

}

///鎵ц瀹ｅ憡閫氱煡
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

//鏈熻揣鍙戣捣鏌ヨ閾惰浣欓璇锋眰
void CSimpleHandler::ReqQueryBankAccountMoneyByFuture()
{
    CThostFtdcReqQueryAccountField a = { 0 };
    int b = m_pUserApi->ReqQueryBankAccountMoneyByFuture(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鏈熻揣鍙戣捣鏌ヨ閾惰浣欓璇锋眰......鍙戯拷1锟�7锟芥垚鍔焅n" : "鏈熻揣鍙戣捣鏌ヨ閾惰浣欓璇锋眰......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//鏈熻揣鍙戣捣閾惰璧勯噾杞湡璐ц姹�1锟�7
void CSimpleHandler::ReqFromBankToFutureByFuture()
{
    int output_num;
    LOGDEMO("璇疯緭鍏ヨ浆璐﹂噾棰�1锟�7");
    cin >> output_num;

    CThostFtdcReqTransferField a = { 0 };
    strcpy(a.TradeCode, "202001");///涓氬姟鍔熻兘鐮�1锟�7
    int bankid = 0;
    while (bankid != 1 & 2 & 3 & 5 & 6 & 7 & 8 & 9 & 10 & 11 & 12 & 13 & 14 & 15 & 16) {
        LOGDEMO("璇疯緭鍏ヤ綘闇�1锟�7瑕佺殑杞处鐨勯摱琛孿n");
        LOGDEMO("1.宸ュ晢閾惰\n");
        LOGDEMO("2.鍐滀笟閾惰\n");
        LOGDEMO("3.涓浗閾惰\n");
        LOGDEMO("5.浜わ拷1锟�7锟介摱琛孿n");
        LOGDEMO("6.鎷涘晢閾惰\n");
        LOGDEMO("7.鍏翠笟閾惰\n");
        LOGDEMO("8.娴﹀彂閾惰\n");
        LOGDEMO("9.姘戠敓閾惰\n");
        LOGDEMO("10.鍏夊ぇ閾惰\n");
        LOGDEMO("11.涓俊閾惰\n");
        LOGDEMO("12.姹囦赴閾惰\n");
        LOGDEMO("13.骞冲畨閾惰\n");
        LOGDEMO("14.鍐滃彂閾惰\n");
        LOGDEMO("15.鏄熷睍閾惰\n");
        LOGDEMO("16.骞垮彂閾惰\n");
        cin >> bankid;
        if (bankid == 1 | 2 | 3 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16)
        {
            //strcpy(a.BankID, itoa(bankid, a.BankID, 10));///閾惰浠ｇ爜
            //itoa(bankid, a.BankID, 10);
            sprintf(a.BankID, "%d", bankid);
        }
        else
        {
            LOGDEMO("璇烽噸鏂拌緭鍏ラ摱琛屼唬鐮侊拷1锟�7锟絓n");
            system("stty -echo");
            getchar();
            system("stty echo");
        }
    }


    strcpy(a.BankBranchID, "0000");///鏈熷晢浠ｇ爜
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.TradeDate, "20170829");///浜ゆ槗鏃ユ湡
    strcpy(a.TradeTime, "09:00:00");
    strcpy(a.BankSerial, "6889");///閾惰娴佹按鍙�1锟�7
    strcpy(a.TradingDay, "20170829");///浜ゆ槗绯荤粺鏃ユ湡
    a.PlateSerial = 5;///閾舵湡骞冲彴娑堟伅娴佹按鍙�1锟�7
    a.LastFragment = THOST_FTDC_LF_Yes;///鏈�1锟�7鍚庡垎鐗囨爣蹇�1锟�7 '0'=鏄渶鍚庡垎鐗�1锟�7
    a.SessionID = SessionID;
    //strcpy(a.CustomerName, "");///瀹㈡埛濮撳悕
    a.IdCardType = THOST_FTDC_ICT_IDCard;///璇佷欢绫诲瀷
    a.CustType = THOST_FTDC_CUSTT_Person;///瀹㈡埛绫诲瀷
    //strcpy(a.IdentifiedCardNo, "310115198706241914");///璇佷欢鍙风爜
    /*strcpy(a.BankAccount, "123456789");
    strcpy(a.BankPassWord, "123456");///閾惰瀵嗙爜*/
    strcpy(a.BankAccount, "621485212110187");
    //strcpy(a.BankPassWord, "092812");///閾惰瀵嗙爜--涓嶉渶瑕侀摱琛屽崱瀵嗙爜
    strcpy(a.AccountID, g_chInvestorID);///鎶曡祫鑰呭笎鍙�1锟�7
    //strcpy(a.Password, "092812");///鏈熻揣瀵嗙爜--璧勯噾瀵嗙爜
    strcpy(a.Password, "123456");///鏈熻揣瀵嗙爜--璧勯噾瀵嗙爜
    a.InstallID = 1;///瀹夎缂栧彿
    a.FutureSerial = 0;///鏈熻揣鍏徃娴佹按鍙�1锟�7
    a.VerifyCertNoFlag = THOST_FTDC_YNI_No;///楠岃瘉瀹㈡埛璇佷欢鍙风爜鏍囧織
    strcpy(a.CurrencyID, "CNY");///甯佺浠ｇ爜
    a.TradeAmount = output_num;///杞笎閲戦
    a.FutureFetchAmount = 0;///鏈熻揣鍙彇閲戦
    a.CustFee = 0;///搴旀敹瀹㈡埛璐圭敤
    a.BrokerFee = 0;///搴旀敹鏈熻揣鍏徃璐圭敤
    a.SecuPwdFlag = THOST_FTDC_BPWDF_BlankCheck;///鏈熻揣璧勯噾瀵嗙爜鏍稿鏍囧織
    a.RequestID = 0;///璇锋眰缂栧彿
    a.TID = 0;///浜ゆ槗ID
    int b = m_pUserApi->ReqFromBankToFutureByFuture(&a, 1);
    LOGDEMO((b == 0) ? "鏈熻揣鍙戣捣閾惰璧勯噾杞湡璐ц姹�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "鏈熻揣鍙戣捣閾惰璧勯噾杞湡璐ц姹�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//鏈熻揣鍙戣捣鏈熻揣璧勯噾杞摱琛岃姹�1锟�7
void CSimpleHandler::ReqFromFutureToBankByFuture()
{
    int output_num;
    LOGDEMO("璇疯緭鍏ヨ浆璐﹂噾棰�1锟�7");
    cin >> output_num;

    CThostFtdcReqTransferField a = { 0 };
    strcpy(a.TradeCode, "202002");///涓氬姟鍔熻兘鐮�1锟�7
bankid_new:int bankid = 0;
    LOGDEMO("璇疯緭鍏ヤ綘闇�1锟�7瑕佺殑杞处鐨勯摱琛孿n");
    LOGDEMO("1.宸ュ晢閾惰\n");
    LOGDEMO("2.鍐滀笟閾惰\n");
    LOGDEMO("3.涓浗閾惰\n");
    LOGDEMO("5.浜わ拷1锟�7锟介摱琛孿n");
    LOGDEMO("6.鎷涘晢閾惰\n");
    LOGDEMO("7.鍏翠笟閾惰\n");
    LOGDEMO("8.娴﹀彂閾惰\n");
    LOGDEMO("9.姘戠敓閾惰\n");
    LOGDEMO("10.鍏夊ぇ閾惰\n");
    LOGDEMO("11.涓俊閾惰\n");
    LOGDEMO("12.姹囦赴閾惰\n");
    LOGDEMO("13.骞冲畨閾惰\n");
    LOGDEMO("14.鍐滃彂閾惰\n");
    LOGDEMO("15.鏄熷睍閾惰\n");
    LOGDEMO("16.骞垮彂閾惰\n");
    cin >> bankid;
    if (bankid == 1 | 2 | 3 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16)
    {
        //strcpy(a.BankID, itoa(bankid, a.BankID, 10));///閾惰浠ｇ爜
        //itoa(bankid, a.BankID, 10);
        sprintf(a.BankID, "%d", bankid);
    }
    else {
        LOGDEMO("璇疯緭鍏ユ纭殑缂栧彿\n");
        system("stty -echo");
        getchar();
        system("stty echo");
        goto bankid_new;
    }
    strcpy(a.BankBranchID, "0000");///鏈熷晢浠ｇ爜
    strcpy(a.BrokerID, g_chBrokerID);
    //strcpy(a.BankBranchID, "0000");///閾惰鍒嗘敮鏈烘瀯浠ｇ爜
    //strcpy(a.TradeDate, "20170829");///浜ゆ槗鏃ユ湡
    //strcpy(a.TradeTime, "09:00:00");
    //strcpy(a.BankSerial, "");///閾惰娴佹按鍙�1锟�7
    //strcpy(a.TradingDay, "20170829");///浜ゆ槗绯荤粺鏃ユ湡
    //a.PlateSerial= 0;///閾舵湡骞冲彴娑堟伅娴佹按鍙�1锟�7
    a.LastFragment = THOST_FTDC_LF_Yes;///鏈�1锟�7鍚庡垎鐗囨爣蹇�1锟�7 '0'=鏄渶鍚庡垎鐗�1锟�7
    a.SessionID = SessionID;
    //strcpy(a.CustomerName, "");///瀹㈡埛濮撳悕
    a.IdCardType = THOST_FTDC_ICT_IDCard;///璇佷欢绫诲瀷
    strcpy(a.IdentifiedCardNo, "310115198706241914");///璇佷欢鍙风爜
    strcpy(a.BankAccount, "123456789");///閾惰甯愬彿
    //strcpy(a.BankPassWord, "123456");///閾惰瀵嗙爜
    strcpy(a.AccountID, g_chInvestorID);///鎶曡祫鑰呭笎鍙�1锟�7
    strcpy(a.Password, "123456");///鏈熻揣瀵嗙爜
    a.InstallID = 1;///瀹夎缂栧彿
    a.CustType = THOST_FTDC_CUSTT_Person;
    //a.FutureSerial = 0;///鏈熻揣鍏徃娴佹按鍙�1锟�7
    a.VerifyCertNoFlag = THOST_FTDC_YNI_No;///楠岃瘉瀹㈡埛璇佷欢鍙风爜鏍囧織
    strcpy(a.CurrencyID, "CNY");///甯佺浠ｇ爜
    a.TradeAmount = output_num;///杞笎閲戦
    a.FutureFetchAmount = 0;///鏈熻揣鍙彇閲戦
    a.CustFee = 0;///搴旀敹瀹㈡埛璐圭敤
    a.BrokerFee = 0;///搴旀敹鏈熻揣鍏徃璐圭敤
    //a.SecuPwdFlag = THOST_FTDC_BPWDF_BlankCheck;///鏈熻揣璧勯噾瀵嗙爜鏍稿鏍囧織
    a.RequestID = 0;///璇锋眰缂栧彿
    a.TID = 0;///浜ゆ槗ID
    int b = m_pUserApi->ReqFromFutureToBankByFuture(&a, 1);
    LOGDEMO((b == 0) ? "鏈熻揣鍙戣捣鏈熻揣璧勯噾杞摱琛岃姹�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "鏈熻揣鍙戣捣鏈熻揣璧勯噾杞摱琛岃姹�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//鏈熸潈鑷鍐插綍鍏ヨ姹�1锟�7
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
        LOGDEMO("璇凤拷1锟�7锟芥嫨鎶曟満濂椾繚鏍囧織\n1.鎶曟満\t2.濂楀埄\t3.濂椾繚\t4.鍋氬競鍟哱n");
        cin >> choose_1;
        if (choose_1 == 1) { a.HedgeFlag = THOST_FTDC_HF_Speculation; }
        else if (choose_1 == 2) { a.HedgeFlag = THOST_FTDC_HF_Arbitrage; }
        else if (choose_1 == 3) { a.HedgeFlag = THOST_FTDC_HF_Hedge; }
        else if (choose_1 == 4) { a.HedgeFlag = THOST_FTDC_HF_MarketMaker; }
        else {
            LOGDEMO("閫夐」閿欒锛岃閲嶆柊閫夋嫨銆俓n");
            system("stty -echo");
            getchar();
            system("stty echo");
        }
    }

    int choose_2 = 0;
    while (choose_2 != 1 && choose_2 != 2 && choose_2 != 3) {
        LOGDEMO("璇凤拷1锟�7锟芥嫨鏈熸潈琛屾潈鐨勫ご瀵告槸鍚﹁嚜瀵瑰啿鏍囧織\n1.鑷鍐叉湡鏉冧粨浣峔t2.淇濈暀鏈熸潈浠撲綅\t3.鑷鍐插崠鏂瑰饱绾﹀悗鐨勬湡璐т粨浣峔n");
        cin >> choose_2;
        if (choose_2 == 1) { a.OptSelfCloseFlag = THOST_FTDC_OSCF_CloseSelfOptionPosition; }
        else if (choose_2 == 2) { a.OptSelfCloseFlag = THOST_FTDC_OSCF_ReserveOptionPosition; }
        else if (choose_2 == 3) { a.OptSelfCloseFlag = THOST_FTDC_OSCF_SellCloseSelfFuturePosition; }
        else {
            LOGDEMO("閫夐」閿欒锛岃閲嶆柊閫夋嫨銆俓n");
            system("stty -echo");
            getchar();
            system("stty echo");
            continue;
        }
    }

    strcpy(a.ExchangeID, g_chExchangeID);
    string accountid_new;
    LOGDEMO("璇疯緭鍏ヨ祫閲戣处鍙�1锟�7\n");
    cin >> accountid_new;
    strcpy(a.AccountID, accountid_new.c_str());
    strcpy(a.CurrencyID, "CNY");
    int b = m_pUserApi->ReqOptionSelfCloseInsert(&a, 1);
    LOGDEMO((b == 0) ? "鏈熸潈鑷鍐插綍鍏ヨ姹�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "鏈熸潈鑷鍐插綍鍏ヨ姹�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

///鏈熸潈鑷鍐诧拷1锟�7锟界煡
void CSimpleHandler::OnRtnOptionSelfClose(CThostFtdcOptionSelfCloseField *pOptionSelfClose)
{
    if (pOptionSelfClose) {
        g_chFrontID = pOptionSelfClose->FrontID;
        g_chSessionID = pOptionSelfClose->SessionID;
        strcpy(g_chOptionSelfCloseSysID, pOptionSelfClose->OptionSelfCloseSysID);//鏈熸潈鑷鍐茬紪鍙�1锟�7
        strcpy(g_chOptionSelfCloseRef, pOptionSelfClose->OptionSelfCloseRef);//鏈熸潈鑷鍐插紩鐢�1锟�7
    }
    CTraderSpi::OnRtnOptionSelfClose(pOptionSelfClose);
}

//鏈熸潈鑷鍐叉搷浣滆姹�1锟�7
void CSimpleHandler::ReqOptionSelfCloseAction()
{
    CThostFtdcInputOptionSelfCloseActionField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    //strcpy(a.OptionSelfCloseSysID, g_chOptionSelfCloseSysID);//鏈熸潈鑷鍐茬紪鍙�1锟�7
    strcpy(a.OptionSelfCloseRef, g_chOptionSelfCloseRef);//鏈熸潈鑷鍐插紩鐢�1锟�7
    //a.FrontID = g_chFrontID;
    //a.SessionID = g_chSessionID;
    strcpy(a.ExchangeID, g_chExchangeID);
    a.ActionFlag = THOST_FTDC_AF_Delete;
    strcpy(a.UserID, g_chUserID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    int b = m_pUserApi->ReqOptionSelfCloseAction(&a, 1);
    LOGDEMO((b == 0) ? "鏈熸潈鑷鍐叉搷浣滆姹�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "鏈熸潈鑷鍐叉搷浣滆姹�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

//璇锋眰鏌ヨ鏈熸潈鑷鍐�1锟�7
void CSimpleHandler::ReqQryOptionSelfClose()
{
    CThostFtdcQryOptionSelfCloseField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    strcpy(a.ExchangeID, g_chExchangeID);
    int b = m_pUserApi->ReqQryOptionSelfClose(&a, 1);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ鏈熸潈鑷鍐�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ鏈熸潈鑷鍐�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

///璇锋眰鏌ヨ鏈熸潈鑷鍐插搷搴�1锟�7
void CSimpleHandler::OnRspQryOptionSelfClose(CThostFtdcOptionSelfCloseField *pOptionSelfClose, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (pOptionSelfClose) {
        g_chFrontID = pOptionSelfClose->FrontID;
        g_chSessionID = pOptionSelfClose->SessionID;
        strcpy(g_chOptionSelfCloseSysID, pOptionSelfClose->OptionSelfCloseSysID);//鏈熸潈鑷鍐茬紪鍙�1锟�7
        strcpy(g_chOptionSelfCloseRef, pOptionSelfClose->OptionSelfCloseRef);//鏈熸潈鑷鍐插紩鐢�1锟�7
    }
    CTraderSpi::OnRspQryOptionSelfClose(pOptionSelfClose, pRspInfo, nRequestID, bIsLast);
}

///璇锋眰鏌ヨ鎵ц瀹ｅ憡
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
    LOGDEMO((b == 0) ? "鎵ц瀹ｅ憡鏌ヨ......鍙戯拷1锟�7锟芥垚鍔焅n" : "鎵ц瀹ｅ憡鏌ヨ......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

///鏌ヨ浜屼唬璧勯噾璐︽埛
void CSimpleHandler::ReqQrySecAgentTradingAccount()
{
    CThostFtdcQryTradingAccountField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.CurrencyID, "CNY");
    a.BizType = THOST_FTDC_BZTP_Future;
    strcpy(a.AccountID, g_chInvestorID);
    int b = m_pUserApi->ReqQrySecAgentTradingAccount(&a, 1);
    LOGDEMO((b == 0) ? "鏌ヨ浜屼唬璧勯噾璐︽埛......鍙戯拷1锟�7锟芥垚鍔焅n" : "鏌ヨ浜屼唬璧勯噾璐︽埛......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

///璇锋眰鏌ヨ浜岀骇浠ｇ悊鍟嗚祫閲戞牎楠屾ā寮�1锟�7
void CSimpleHandler::ReqQrySecAgentCheckMode()
{
    CThostFtdcQrySecAgentCheckModeField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    int b = m_pUserApi->ReqQrySecAgentCheckMode(&a, 1);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ浜岀骇浠ｇ悊鍟嗚祫閲戞牎楠屾ā寮�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ浜岀骇浠ｇ悊鍟嗚祫閲戞牎楠屾ā寮�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

///娉ㄥ唽鐢ㄦ埛缁堢淇℃伅锛岀敤浜庝腑缁ф湇鍔″櫒澶氳繛鎺ユā寮�1锟�7
///闇�1锟�7瑕佸湪缁堢璁よ瘉鎴愬姛鍚庯紝鐢ㄦ埛鐧诲綍鍓嶈皟鐢ㄨ鎺ュ彛
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
    LOGDEMO("璇疯緭鍏p鍦板潃(涓嶈緭鍏ュ垯涓虹┖)\n");
    getline(cin, ip);
    strcpy(a.ClientPublicIP, ip.c_str());*/
    strcpy(a.ClientPublicIP, "192.168.0.1");//ip鍦板潃

    //int Port;
    //Port = 0;
    //cin.ignore();
    //LOGDEMO("璇疯緭鍏ョ鍙ｅ彿\n");
    //cin >> Port;
    //a.ClientIPPort = Port;//绔彛鍙�1锟�7
    a.ClientIPPort = 51305;//绔彛鍙�1锟�7

    /*string LoginTime;
    LoginTime.clear();
    cin.ignore();
    LOGDEMO("璇疯緭鍏ョ櫥褰曟椂闂�1锟�7(涓嶈緭鍏ュ垯涓虹┖)\n");
    getline(cin, LoginTime);
    strcpy(a.ClientPublicIP, LoginTime.c_str());*/
    strcpy(a.ClientLoginTime, "20190121");
    strcpy(a.ClientAppID, g_chAppID);
    int b = m_pUserApi->RegisterUserSystemInfo(&a);
    LOGDEMO((b == 0) ? "娉ㄥ唽鐢ㄦ埛缁堢淇℃伅......鍙戯拷1锟�7锟芥垚鍔焅n" : "娉ㄥ唽鐢ㄦ埛缁堢淇℃伅......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

///涓婃姤鐢ㄦ埛缁堢淇℃伅锛岀敤浜庝腑缁ф湇鍔″櫒鎿嶄綔鍛樼櫥褰曟ā寮�1锟�7
///鎿嶄綔鍛樼櫥褰曞悗锛屽彲浠ュ娆¤皟鐢ㄨ鎺ュ彛涓婃姤瀹㈡埛淇℃伅
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
    LOGDEMO("璇疯緭鍏p鍦板潃(涓嶈緭鍏ュ垯涓虹┖)\n");
    getline(cin, ip);
    strcpy(a.ClientPublicIP, ip.c_str());*/
    strcpy(a.ClientPublicIP, "192.168.0.1");//ip鍦板潃

    //int Port;
    //Port = 0;
    //cin.ignore();
    //LOGDEMO("璇疯緭鍏ョ鍙ｅ彿\n");
    //cin >> Port;
    //a.ClientIPPort = Port;//绔彛鍙�1锟�7
    a.ClientIPPort = 51305;//绔彛鍙�1锟�7

    /*string LoginTime;
    LoginTime.clear();
    cin.ignore();
    LOGDEMO("璇疯緭鍏ョ櫥褰曟椂闂�1锟�7(涓嶈緭鍏ュ垯涓虹┖)\n");
    getline(cin, LoginTime);
    strcpy(a.ClientPublicIP, LoginTime.c_str());*/
    strcpy(a.ClientLoginTime, "20190121");
    strcpy(a.ClientAppID, g_chAppID);
    int b = m_pUserApi->SubmitUserSystemInfo(&a);
    LOGDEMO((b == 0) ? "娉ㄥ唽鐢ㄦ埛缁堢淇℃伅......鍙戯拷1锟�7锟芥垚鍔焅n" : "娉ㄥ唽鐢ㄦ埛缁堢淇℃伅......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

///鏌ヨ鐢ㄦ埛褰撳墠鏀寔鐨勮璇佹ā寮�1锟�7
void CSimpleHandler::ReqUserAuthMethod()
{
    CThostFtdcReqUserAuthMethodField a = { 0 };
    strcpy(a.TradingDay, "20190308");
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.UserID, g_chUserID);
    int b = m_pUserApi->ReqUserAuthMethod(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鏌ヨ鐢ㄦ埛褰撳墠鏀寔鐨勮璇佹ā寮�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "鏌ヨ鐢ㄦ埛褰撳墠鏀寔鐨勮璇佹ā寮�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

///鐢ㄦ埛鍙戝嚭鑾峰彇鍥惧舰楠岃瘉鐮佽姹�1锟�7
void CSimpleHandler::ReqGenUserCaptcha()
{
    CThostFtdcReqGenUserCaptchaField a = { 0 };
    strcpy(a.TradingDay, "");
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.UserID, g_chUserID);
    int b = m_pUserApi->ReqGenUserCaptcha(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐢ㄦ埛鍙戝嚭鑾峰彇鍥惧舰楠岃瘉鐮佽姹�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "鐢ㄦ埛鍙戝嚭鑾峰彇鍥惧舰楠岃瘉鐮佽姹�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

///鐢ㄦ埛鍙戝嚭鑾峰彇鐭俊楠岃瘉鐮佽姹�1锟�7
void CSimpleHandler::ReqGenUserText()
{
    CThostFtdcReqGenUserTextField a = { 0 };
    strcpy(a.TradingDay, "");
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.UserID, g_chUserID);
    int b = m_pUserApi->ReqGenUserText(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐢ㄦ埛鍙戝嚭鑾峰彇鐭俊楠岃瘉鐮佽姹�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "鐢ㄦ埛鍙戝嚭鑾峰彇鐭俊楠岃瘉鐮佽姹�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

///鐢ㄦ埛鍙戝嚭甯︽湁鍥剧墖楠岃瘉鐮佺殑鐧婚檰璇锋眰
void CSimpleHandler::ReqUserLoginWithCaptcha()
{
    CThostFtdcReqUserLoginWithCaptchaField a = { 0 };
    strcpy(a.TradingDay, "");
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.UserID, g_chUserID);
    strcpy(a.Password, g_chPassword);
    strcpy(a.UserProductInfo, "");
    strcpy(a.InterfaceProductInfo, "");
    strcpy(a.ProtocolInfo, "");//鍗忚淇℃伅
    strcpy(a.MacAddress, "");//Mac鍦板潃
    strcpy(a.ClientIPAddress, "");//缁堢IP鍦板潃
    strcpy(a.LoginRemark, "");//鐧诲綍涓诲
    strcpy(a.Captcha, "");//鍥惧舰楠岃瘉鐮佺殑鏂囧瓧鍐呭
    a.ClientIPPort = 10203;
    int b = m_pUserApi->ReqUserLoginWithCaptcha(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐢ㄦ埛鍙戝嚭甯︽湁鍥剧墖楠岃瘉鐮佺殑鐧婚檰璇锋眰......鍙戯拷1锟�7锟芥垚鍔焅n" : "鐢ㄦ埛鍙戝嚭甯︽湁鍥剧墖楠岃瘉鐮佺殑鐧婚檰璇锋眰......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

///鐢ㄦ埛鍙戝嚭甯︽湁鐭俊楠岃瘉鐮佺殑鐧婚檰璇锋眰
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
    LOGDEMO((b == 0) ? "鐢ㄦ埛鍙戝嚭甯︽湁鐭俊楠岃瘉鐮佺殑鐧婚檰璇锋眰......鍙戯拷1锟�7锟芥垚鍔焅n" :
        "鐢ㄦ埛鍙戝嚭甯︽湁鐭俊楠岃瘉鐮佺殑鐧婚檰璇锋眰......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

///鐢ㄦ埛鍙戝嚭甯︽湁鍔拷1锟�7锟藉彛浠ょ殑鐧婚檰璇锋眰
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
    LOGDEMO((b == 0) ? "鐢ㄦ埛鍙戝嚭甯︽湁鍔拷1锟�7锟藉彛浠ょ殑鐧婚檰璇锋眰......鍙戯拷1锟�7锟芥垚鍔焅n" : "鐢ㄦ埛鍙戝嚭甯︽湁鍔拷1锟�7锟藉彛浠ょ殑鐧婚檰璇锋眰......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

///璇锋眰鏌ヨ浜岀骇浠ｇ悊鍟嗕俊鎭�1锟�7
void CSimpleHandler::ReqQrySecAgentTradeInfo()
{
    CThostFtdcQrySecAgentTradeInfoField a = { 0 };
    strcpy(a.BrokerID, "");
    strcpy(a.BrokerSecAgentID, "");
    int b = m_pUserApi->ReqQrySecAgentTradeInfo(&a, nRequestID++);
    LOGDEMO((b == 0) ? "璇锋眰鏌ヨ浜岀骇浠ｇ悊鍟嗕俊鎭�1锟�7......鍙戯拷1锟�7锟芥垚鍔焅n" : "璇锋眰鏌ヨ浜岀骇浠ｇ悊鍟嗕俊鎭�1锟�7......鍙戯拷1锟�7锟藉け璐ワ紝閿欒搴忓彿=[%d]\n", b);
}

