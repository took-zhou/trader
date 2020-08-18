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
// 閹躲儱宕熻ぐ鏇炲弳閹垮秳缍旈弰顖氭儊鐎瑰本鍨氶惃鍕垼韫囷拷1閿燂拷7
// Create a manual reset event with no signal
//HANDLE g_hEvent = CreateEvent(NULL, false, false, NULL);

/// 娴兼艾鎲虫禒锝囩垳
TThostFtdcBrokerIDType g_chBrokerID;
/// 娴溿倖妲楅悽銊﹀煕娴狅絿鐖�
TThostFtdcUserIDType g_chUserID;
/// 娴溿倖妲楅悽銊﹀煕鐎靛棛鐖�
TThostFtdcPasswordType g_chPassword;
/// 娴溿倖妲楅幍锟�1閿燂拷7娴狅絿鐖�
TThostFtdcExchangeIDType g_chExchangeID;
///閸氬牏瀹虫禒锝囩垳
TThostFtdcInstrumentIDType	g_chInstrumentID;
///閹舵洝绁懓鍛敩閻拷1閿燂拷7
TThostFtdcInvestorIDType g_chInvestorID;
///妫板嫬鐓侀幘銈呭礋缂傛牕褰�
TThostFtdcParkedOrderActionIDType	g_chParkedOrderActionID1;
///妫板嫬鐓侀幎銉ュ礋缂傛牕褰�
TThostFtdcParkedOrderIDType	g_chParkedOrderID1;
///閹躲儱宕熷鏇犳暏
TThostFtdcOrderRefType	g_chOrderRef;
///閸撳秶鐤嗙紓鏍у娇
TThostFtdcFrontIDType	g_chFrontID;
///娴兼俺鐦界紓鏍у娇
TThostFtdcSessionIDType	g_chSessionID;
///閹躲儱宕熺紓鏍у娇
TThostFtdcOrderSysIDType	g_chOrderSysID;
///濮濄垺宕禒锟�1閿燂拷7
TThostFtdcPriceType	g_chStopPrice;
///閹躲儰鐜鏇犳暏
TThostFtdcOrderRefType	g_chQuoteRef;
int FrontID = 0;
int SessionID = 0;
int Limitprice = 0;
int nRequestID = 0;
int chioce_action = 0;//娑擄拷1閿燂拷7閿熻棄鍙忛柈銊﹀Г

vector<string> vector_OrderSysID;
vector<string> vector_ExchangeID;
vector<string> vector_InstrumentID;
vector<string> md_InstrumentID;
int action_number;

///閹笛嗩攽鐎癸絽鎲″鏇犳暏
TThostFtdcOrderRefType	g_NewExecOrderRef;
///閹笛嗩攽鐎癸絽鎲＄紓鏍у娇
TThostFtdcExecOrderSysIDType	g_NewExecOrderSysID;
///閸撳秶鐤嗙紓鏍у娇
TThostFtdcFrontIDType	g_NewFrontID;
///娴兼俺鐦界紓鏍у娇
TThostFtdcSessionIDType	g_NewSessionID;

//閺堢喐娼堥懛顏勵嚠閸愭彃鎼锋惔鏃撴嫹1閿燂拷7閿熺晫鐓�
///閺堢喐娼堥懛顏勵嚠閸愯尙绱崣锟�1閿燂拷7
TThostFtdcOrderSysIDType	g_chOptionSelfCloseSysID;
///閺堢喐娼堥懛顏勵嚠閸愭彃绱╅悽锟�1閿燂拷7
TThostFtdcOrderRefType	g_chOptionSelfCloseRef;
///閻€劍鍩涚粩顖欓獓閸濅椒淇婇幁锟�1閿燂拷7
TThostFtdcProductInfoType	g_chUserProductInfo;
///鐠併倛鐦夐惍锟�1閿燂拷7
TThostFtdcAuthCodeType	g_chAuthCode;
///App娴狅絿鐖�
TThostFtdcAppIDType	g_chAppID;

//HANDLE xinhao = CreateEvent(NULL, false, false, NULL);

//CTraderApi *pUserApi = new CTraderApi;

//鐞涘本鍎忕猾锟�1閿燂拷7

// 瑜版挸顓归幋椋庮伂娑撳簼姘﹂弰鎾村缁狅紕閮寸紒鐔风紦缁斿鎹ｉ柅姘繆鏉╃偞甯撮敍灞筋吂閹撮顏棁锟�1閿燂拷7鐟曚浇绻樼悰宀�娅ヨぐ锟�1閿燂拷7
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

// 瑜版挸顓归幋椋庮伂娑撳簼姘﹂弰鎾村缁狅紕閮寸紒鐕傛嫹1閿燂拷7閿熸垝淇婃潻鐐村复閺傤厼绱戦弮璁圭礉鐠囥儲鏌熷▔鏇☆潶鐠嬪啰鏁�
void CSimpleMdHandler::OnFrontDisconnected(int nReason)
{
    // 瑜版挸褰傞悽鐔荤箹娑擃亝鍎忛崘闈涙倵閿涘瓑PI娴兼俺鍤滈崝銊╁櫢閺傛媽绻涢幒銉礉鐎广垺鍩涚粩顖氬讲娑撳秴浠涙径鍕倞
    WARNING_LOG("<OnFrontDisconnected>");
    WARNING_LOG("\tnReason= = [%d]", nReason);
    WARNING_LOG("</OnFrontDisconnected>");
}

	// 瑜版挸顓归幋椋庮伂閸欐垵鍤惂璇茬秿鐠囬攱鐪版稊瀣倵閿涘矁顕氶弬瑙勭《娴兼俺顫︾拫鍐暏閿涘矉鎷�1閿燂拷7閿熺晫鐓＄�广垺鍩涚粩顖滄瑜版洘妲搁崥锔藉灇閸旓拷1閿燂拷7
void CSimpleMdHandler::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    LOGDEMO("OnRspUserLogin:\n");
    LOGDEMO("\tErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,
        pRspInfo->ErrorMsg);
    LOGDEMO("\tRequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
    if (pRspInfo->ErrorID != 0) {
        // 缁旑垳娅ユ径杈Е閿涘苯顓归幋椋庮伂闂囷拷1閿燂拷7鏉╂稖顢戦柨娆掝嚖婢跺嫮鎮�
        LOGDEMO("\tFailed to login, errorcode=%d errormsg=%s requestid=%d chain = %d",
            pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
        exit(-1);
    }
    //SetEvent(xinhao);
    sem_post(&globalSem.sem_login);
    //SubscribeMarketData();//鐠併垽妲勭悰灞惧剰
    //SubscribeForQuoteRsp();//鐠囶澀鐜拠閿嬬湴
}

	void CSimpleMdHandler::SubscribeMarketData()//閺�鎯邦攽閹拷1閿燂拷7
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
				LOGDEMO((result == 0) ? "鐠併垽妲勭悰灞惧剰鐠囬攱鐪�1......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠併垽妲勭悰灞惧剰鐠囬攱鐪�1......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", result);
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
				LOGDEMO((result == 0) ? "鐠併垽妲勭悰灞惧剰鐠囬攱鐪�2......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠併垽妲勭悰灞惧剰鐠囬攱鐪�2......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", result);
			}
		}
	}

	///鐠併垽妲勭悰灞惧剰鎼存梻鐡�
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

	///濞ｅ崬瀹崇悰灞惧剰闁氨鐓�
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

	///鐠併垽妲勭拠顫幆鐠囬攱鐪�
	void CSimpleMdHandler::SubscribeForQuoteRsp()
	{
		LOGDEMO("鐞涘本鍎忔稉顓☆吂闂冨懓顕楁禒鐤嚞濮逛繐n");
		char **ppInstrumentID = new char*[50];
		string g_chInstrumentID = getConfig("market", "InstrumentID");
		ppInstrumentID[0] = const_cast<char *>(g_chInstrumentID.c_str());
		int result = m_pUserMdApi->SubscribeForQuoteRsp(ppInstrumentID, 1);
	}

	///鐠併垽妲勭拠顫幆鎼存梻鐡�
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


	///鐠囶澀鐜柅姘辩叀
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
//娴溿倖妲楃猾锟�1閿燂拷7

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

//鐎广垺鍩涚粩顖濐吇鐠囷拷1閿燂拷7
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
    INFO_LOG("ReqAuthenticate b = [%d]", b);
}

///鐎广垺鍩涚粩顖濐吇鐠囦礁鎼锋惔锟�1閿燂拷7
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
    // 閸欐垵鍤惂濠氭鐠囬攱鐪�
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

///閻ц鍤拠閿嬬湴閸濆秴绨�
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

///鐠囬攱鐪扮涵顔款吇缂佹挾鐣婚崡锟�1閿燂拷7
void CSimpleHandler::ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *Confirm, int nRequestID)
{
//		CThostFtdcSettlementInfoConfirmField Confirm = { 0 };
//		///缂佸繒閭崗顒�寰冩禒锝囩垳
//		strcpy(Confirm.BrokerID, g_chBrokerID);
//		///閹舵洝绁懓鍛敩閻拷1閿燂拷7
//		strcpy(Confirm.InvestorID, g_chUserID);
    m_pUserApi->ReqSettlementInfoConfirm(Confirm, nRequestID);
}

///閹舵洝绁懓鍛波缁犳绮ㄩ弸婊呪�樼拋銈呮惙鎼达拷1閿燂拷7
void CSimpleHandler::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CTraderSpi::OnRspSettlementInfoConfirm(pSettlementInfoConfirm, pRspInfo, nRequestID, bIsLast);
    //SetEvent(g_hEvent);
    sem_post(&globalSem.sem_settlement);
}

///閻€劍鍩涢崣锝勬姢閺囧瓨鏌婄拠閿嬬湴
void CSimpleHandler::ReqUserPasswordUpdate()
{
    string newpassword;
    LOGDEMO("鐠囩柉绶崗銉︽煀閻ц缍嶇�靛棛鐖滈敍姝昻");
    cin >> newpassword;
    CThostFtdcUserPasswordUpdateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.UserID, g_chUserID);
    strcpy(a.OldPassword, g_chPassword);
    strcpy(a.NewPassword, newpassword.c_str());
    int b = m_pUserApi->ReqUserPasswordUpdate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "閻€劍鍩涢崣锝勬姢閺囧瓨鏌婄拠閿嬬湴......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閻€劍鍩涢崣锝勬姢閺囧瓨鏌婄拠閿嬬湴......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", b);
}

///閻€劍鍩涢崣锝勬姢閺囧瓨鏌婄拠閿嬬湴閸濆秴绨�
void CSimpleHandler::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CTraderSpi::OnRspUserPasswordUpdate(pUserPasswordUpdate, pRspInfo, nRequestID, bIsLast);
    //SetEvent(g_hEvent);
    sem_post(&globalSem.sem);
}

///鐠у嫰鍣剧拹锔藉煕閸欙絼鎶ら弴瀛樻煀鐠囬攱鐪�
void CSimpleHandler::ReqTradingAccountPasswordUpdate()
{
    string newpassword;
    LOGDEMO("鐠囩柉绶崗銉︽煀鐠у嫰鍣剧�靛棛鐖滈敍姝昻");
    cin >> newpassword;
    CThostFtdcTradingAccountPasswordUpdateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.AccountID, g_chInvestorID);
    strcpy(a.OldPassword, g_chPassword);
    strcpy(a.NewPassword, newpassword.c_str());
    strcpy(a.CurrencyID, "CNY");
    int b = m_pUserApi->ReqTradingAccountPasswordUpdate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠у嫰鍣剧拹锔藉煕閸欙絼鎶ら弴瀛樻煀鐠囬攱鐪�......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠у嫰鍣剧拹锔藉煕閸欙絼鎶ら弴瀛樻煀鐠囬攱鐪�......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", b);
}

///鐠у嫰鍣剧拹锔藉煕閸欙絼鎶ら弴瀛樻煀鐠囬攱鐪伴崫宥呯安
void CSimpleHandler::OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CTraderSpi::OnRspTradingAccountPasswordUpdate(pTradingAccountPasswordUpdate, pRspInfo, nRequestID, bIsLast);
    //SetEvent(g_hEvent);
    sem_post(&globalSem.sem);
}

///妫板嫬鐓侀崡鏇炵秿閸忥拷1閿燂拷7//闂勬劒鐜崡锟�1閿燂拷7
void CSimpleHandler::ReqParkedOrderInsert()
{
    int limitprice = 0;
    LOGDEMO("鐠囩柉绶崗銉╂娴犲嘲宕熸禒閿嬬壐閿涳拷1閿燂拷7(姒涙顓�0)\n");
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
    LOGDEMO((b == 0) ? "鐠囬攱鐪拌ぐ鏇炲弳妫板嫬鐓侀崡锟�1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪拌ぐ鏇炲弳妫板嫬鐓侀崡锟�1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", b);
}

///妫板嫬鐓侀幘銈呭礋瑜版洖鍙嗙拠閿嬬湴
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
    LOGDEMO((b == 0) ? "鐠囬攱鐪拌ぐ鏇炲弳妫板嫬鐓侀幘銈呭礋......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪拌ぐ鏇炲弳妫板嫬鐓侀幘銈呭礋......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", b);
}

///鐠囬攱鐪伴崚鐘绘珟妫板嫬鐓侀崡锟�1閿燂拷7
void CSimpleHandler::ReqRemoveParkedOrder()
{
    CThostFtdcRemoveParkedOrderField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.ParkedOrderID, g_chParkedOrderID1);
    int b = m_pUserApi->ReqRemoveParkedOrder(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴崚鐘绘珟妫板嫬鐓侀崡锟�1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴崚鐘绘珟妫板嫬鐓侀崡锟�1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", b);
}

///鐠囬攱鐪伴崚鐘绘珟妫板嫬鐓侀幘銈呭礋
void CSimpleHandler::ReqRemoveParkedOrderAction()
{
    CThostFtdcRemoveParkedOrderActionField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.ParkedOrderActionID, g_chParkedOrderActionID1);
    int b = m_pUserApi->ReqRemoveParkedOrderAction(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴崚鐘绘珟妫板嫬鐓侀幘銈呭礋......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴崚鐘绘珟妫板嫬鐓侀幘銈呭礋......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", b);
}

///閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪�
void CSimpleHandler::ReqOrderInsert_Ordinary()
{
    system("clear");
    int volume;
    string instrument_id;
    std::cout << "鐠囩柉绶崗銉ユ値缁撅箑鎮曢敍锟�1閿燂拷7" << std::endl;
    cin >> instrument_id;
    string new_limitprice;
    LOGDEMO("鐠囩柉绶崗銉﹀瘹鐎规矮鐜弽纭风窗\n");
    cin >> new_limitprice;
    std::cout << "鐠囩柉绶崗銉︽殶闁插骏绱�" << std::endl;
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
Direction:LOGDEMO("鐠囧嚖鎷�1閿燂拷7閿熻姤瀚ㄦ稊鏉垮礌閺傜懓鎮淺t1.娑旂櫒t2.閸楁湺n");
    cin >> num1;
    if (num1 == 1) {
        ord.Direction = THOST_FTDC_D_Buy;//娑旓拷1閿燂拷7
    }
    else if (num1 == 2) {
        ord.Direction = THOST_FTDC_D_Sell;//閸楋拷1閿燂拷7
    }
    else {
        LOGDEMO("鏉堟挸鍙嗛柨娆掝嚖鐠囩兘鍣搁弬鎷岀翻閸忣櫌n");
        system("stty -echo");
        getchar();
        system("stty echo");

        goto Direction;
    }

    int num2;
CombOffsetFlag:LOGDEMO("鐠囩柉绶崗銉ョ磻楠炶櫕鏌熼崥鎱璽1.瀵拷1閿燂拷7娴犳彫t2.楠炲厖绮╘t3.瀵搫閽‐t4.楠炲厖绮朶t5.楠炶櫕妲癨t6.瀵搫鍣篭t7.閺堫剙婀村鍝勯挬\n");
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
        LOGDEMO("鏉堟挸鍙嗛柨娆掝嚖鐠囩兘鍣搁弬鎷岀翻閸忣櫌n");
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
    ord.TimeCondition = THOST_FTDC_TC_GFD;///瑜版挻妫╅張澶嬫櫏
    ord.VolumeCondition = THOST_FTDC_VC_CV;///閸忋劑鍎撮弫浼村櫤
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪伴梽鎰幆閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪伴梽鎰幆閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", a);
}
void CSimpleHandler::ReqOrderInsert_Ordinary_hai(CThostFtdcInputOrderField &ord, int nRequestID)
{
    int a = m_pUserApi->ReqOrderInsert(&ord, nRequestID);
    INFO_LOG("%s",(a == 0) ? "Order insert request send succ, waiting for response from exchange ......\n" : "Order insert request send failed\n"); // @suppress("Invalid arguments")
}
///婢堆冩櫌閹碉拷1閿燂拷7濮濄垺宕崡锟�1閿燂拷7
void CSimpleHandler::ReqOrderInsert_Touch()
{
    int new_limitprice;
    LOGDEMO("鐠囩柉绶崗銉﹀瘹鐎规矮鐜弽绯絠mitprice閿涙瓡n");
    cin >> new_limitprice;

    int new_StopPrice;
    LOGDEMO("鐠囩柉绶崗銉﹀瘹鐎规矮鐜弽绱則opprice閿涙瓡n");
    cin >> new_StopPrice;

    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ord.Direction = THOST_FTDC_D_Buy;//娑旓拷1閿燂拷7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = 1;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///瑜版挻妫╅張澶嬫櫏
    ord.VolumeCondition = THOST_FTDC_VC_AV;///娴犺缍嶉弫浼村櫤
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Touch;
    ord.StopPrice = new_StopPrice;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪伴梽鎰幆閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪伴梽鎰幆閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", a);
}

///婢堆冩櫌閹碉拷1閿燂拷7濮濄垻娉╅崡锟�1閿燂拷7
void CSimpleHandler::ReqOrderInsert_TouchProfit()
{
    int new_limitprice;
    LOGDEMO("鐠囩柉绶崗銉﹀瘹鐎规矮鐜弽绯絠mitprice閿涙瓡n");
    cin >> new_limitprice;

    int new_StopPrice;
    LOGDEMO("鐠囩柉绶崗銉﹀瘹鐎规矮鐜弽绱則opprice閿涙瓡n");
    cin >> new_StopPrice;

    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ord.Direction = THOST_FTDC_D_Buy;//娑旓拷1閿燂拷7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = 1;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///瑜版挻妫╅張澶嬫櫏
    ord.VolumeCondition = THOST_FTDC_VC_AV;///閸忋劑鍎撮弫浼村櫤
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_TouchProfit;
    ord.StopPrice = new_StopPrice;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪伴梽鎰幆閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪伴梽鎰幆閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", a);
}

//閸忋劍鍨氶崗銊︽寵
void CSimpleHandler::ReqOrderInsert_VC_CV()
{
    int new_limitprice;
    LOGDEMO("鐠囩柉绶崗銉﹀瘹鐎规矮鐜弽纭风窗\n");
    cin >> new_limitprice;

    int insert_num;
    LOGDEMO("鐠囩柉绶崗銉ょ瑓閸楁洘鏆熼柌蹇ョ窗\n");
    cin >> insert_num;

    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ord.Direction = THOST_FTDC_D_Buy;//娑旓拷1閿燂拷7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = insert_num;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///瑜版挻妫╅張澶嬫櫏
    ord.VolumeCondition = THOST_FTDC_VC_CV;///閸忋劑鍎撮弫浼村櫤
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪伴梽鎰幆閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪伴梽鎰幆閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", a);
}

//闁劍鍨氶柈銊︽寵
void CSimpleHandler::ReqOrderInsert_VC_AV()
{
    int new_limitprice;
    LOGDEMO("鐠囩柉绶崗銉﹀瘹鐎规矮鐜弽纭风窗\n");
    cin >> new_limitprice;

    int insert_num;
    LOGDEMO("鐠囩柉绶崗銉ょ瑓閸楁洘鏆熼柌蹇ョ窗\n");
    cin >> insert_num;

    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ord.Direction = THOST_FTDC_D_Buy;//娑旓拷1閿燂拷7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = insert_num;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///瑜版挻妫╅張澶嬫櫏
    ord.VolumeCondition = THOST_FTDC_VC_AV;///娴犺缍嶉弫浼村櫤
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪伴梽鎰幆閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪伴梽鎰幆閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", a);
}

//鐢倷鐜崡锟�1閿燂拷7
void CSimpleHandler::ReqOrderInsert_AnyPrice()
{
    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
    ord.Direction = THOST_FTDC_D_Buy;//娑旓拷1閿燂拷7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    //ord.LimitPrice = new_limitprice;
    ord.LimitPrice = 0;
    ord.VolumeTotalOriginal = 1;
    ord.TimeCondition = THOST_FTDC_TC_IOC;///缁斿宓嗙�瑰本鍨氶敍灞芥儊閸掓瑦鎸欓柨锟�1閿燂拷7
    ord.VolumeCondition = THOST_FTDC_VC_AV;///娴犺缍嶉弫浼村櫤
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    //ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪伴梽鎰幆閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪伴梽鎰幆閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", a);
}

//鐢倷鐜潪顒勬娴犲嘲宕�(娑擃參鍣鹃幍锟�1閿燂拷7)
void CSimpleHandler::ReqOrderInsert_BestPrice()
{
    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_BestPrice;
    ord.Direction = THOST_FTDC_D_Buy;//娑旓拷1閿燂拷7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    //ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = 1;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///瑜版挻妫╅張澶嬫櫏
    ord.VolumeCondition = THOST_FTDC_VC_AV;///娴犺缍嶉弫浼村櫤
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪伴梽鎰幆閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪伴梽鎰幆閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", a);
}

//婵傛鍩勯幐鍥︽姢
void CSimpleHandler::ReqOrderInsert_Arbitrage()
{
    int new_limitprice;
    LOGDEMO("鐠囩柉绶崗銉﹀瘹鐎规矮鐜弽纭风窗\n");
    cin >> new_limitprice;

    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ord.Direction = THOST_FTDC_D_Buy;//娑旓拷1閿燂拷7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = 1;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///瑜版挻妫╅張澶嬫櫏
    ord.VolumeCondition = THOST_FTDC_VC_AV;///娴犺缍嶉弫浼村櫤
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪伴梽鎰幆閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪伴梽鎰幆閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", a);
}

//娴滄帗宕查崡锟�1閿燂拷7
void CSimpleHandler::ReqOrderInsert_IsSwapOrder()
{
    int new_limitprice;
    LOGDEMO("鐠囩柉绶崗銉﹀瘹鐎规矮鐜弽纭风窗\n");
    cin >> new_limitprice;

    CThostFtdcInputOrderField ord = { 0 };
    strcpy(ord.BrokerID, g_chBrokerID);
    strcpy(ord.InvestorID, g_chInvestorID);
    strcpy(ord.InstrumentID, g_chInstrumentID);
    strcpy(ord.UserID, g_chUserID);
    //strcpy(ord.OrderRef, "");
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ord.Direction = THOST_FTDC_D_Buy;//娑旓拷1閿燂拷7
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    //ord.LimitPrice = atoi(getConfig("config", "LimitPrice").c_str());
    ord.LimitPrice = new_limitprice;
    ord.VolumeTotalOriginal = 1;
    ord.TimeCondition = THOST_FTDC_TC_GFD;///瑜版挻妫╅張澶嬫櫏
    ord.VolumeCondition = THOST_FTDC_VC_AV;///娴犺缍嶉弫浼村櫤
    ord.MinVolume = 1;
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    ord.StopPrice = 0;
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ord.IsAutoSuspend = 0;
    ord.IsSwapOrder = 1;//娴滄帗宕查崡鏇熺垼韫囷拷1閿燂拷7
    strcpy(ord.ExchangeID, g_chExchangeID);
    int a = m_pUserApi->ReqOrderInsert(&ord, 1);
    LOGDEMO((a == 0) ? "閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪伴梽鎰幆閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪伴梽鎰幆閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", a);
}

///閹躲儱宕熼幙宥勭稊鐠囬攱鐪�
void CSimpleHandler::ReqOrderAction_Ordinary()
{
    CThostFtdcInputOrderActionField a = { 0 };

    strcpy(a.BrokerID, g_chBrokerID);     //缂佸繑绁归崗顒�寰冩禒锝囩垳
    strcpy(a.InvestorID, g_chInvestorID);  //閹舵洝绁懓鍛敩閻拷1閿燂拷7
    strcpy(a.UserID, g_chUserID);        //閻€劍鍩涙禒锝囩垳
    //a.OrderActionRef = 1;

//      std::cout << "鐠囩柉绶崗銉﹀Г閸楁洖绱╅悽顭掔窗" << std::endl;
//      cin >> g_chOrderRef;
    //strcpy(a.OrderRef, g_chOrderRef);     //閹躲儱宕熷鏇犳暏
    //std::cout << "g_chOrderRef is:" << g_chOrderRef << std::endl;

    //a.FrontID = g_chFrontID;   //閸撳秶鐤嗙紓鏍у娇
    //a.SessionID = g_chSessionID; //娴兼俺鐦界紓鏍у娇

    //a.LimitPrice =
    //a.VolumeChange =
    std::cout << "鐠囩柉绶崗銉ゆ唉閺勬挻澧嶆禒锝囩垳:\n";
    cin >> g_chExchangeID;
    strcpy(a.ExchangeID, g_chExchangeID);    //娴溿倖妲楅幍锟�1閿燂拷7娴狅絿鐖�

    std::cout << "鐠囩柉绶崗銉ユ値缁撅箑褰块敍锟�1閿燂拷7" << std::endl;
    cin >> g_chInstrumentID;                   //閸氬牏瀹虫禒锝囩垳
    strcpy(a.InstrumentID, g_chInstrumentID);

    std::cout << "鐠囩柉绶崗銉﹀Г閸楁洜绱崣鍑ょ窗" << std::endl;
    //cin >> g_chOrderSysID;
    cin.ignore(numeric_limits <streamsize> ::max(), '\n');
    cin.getline(g_chOrderSysID, sizeof(g_chOrderSysID));
    strcpy(a.OrderSysID, g_chOrderSysID);         //閹躲儱宕熺紓鏍у娇

    a.ActionFlag = THOST_FTDC_AF_Delete;    //閹垮秳缍旈弽鍥х箶

    int ab = m_pUserApi->ReqOrderAction(&a, nRequestID++);
    LOGDEMO((ab == 0) ? "閹躲儱宕熼幙宥勭稊鐠囬攱鐪�......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閹躲儱宕熼幙宥勭稊鐠囬攱鐪�......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", ab);
}

///閹笛嗩攽鐎癸絽鎲¤ぐ鏇炲弳鐠囬攱鐪�
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
    OrderInsert.OffsetFlag = THOST_FTDC_OF_Close;//瀵拷1閿燂拷7楠炶櫕鐖ｈ箛锟�1閿燂拷7
    OrderInsert.HedgeFlag = THOST_FTDC_HF_Speculation;//閹舵洘婧�婵傛ぞ绻氶弽鍥х箶
    if (a == 0) {
        OrderInsert.ActionType = THOST_FTDC_ACTP_Exec;//閹笛嗩攽缁鐎风猾璇茬��
    }
    if (a == 1) {
        OrderInsert.ActionType = THOST_FTDC_ACTP_Abandon;//閹笛嗩攽缁鐎风猾璇茬��
    }
    OrderInsert.PosiDirection = THOST_FTDC_PD_Long;//閹镐椒绮ㄦ径姘扁敄閺傜懓鎮滅猾璇茬��
    OrderInsert.ReservePositionFlag = THOST_FTDC_EOPF_Reserve;//閺堢喐娼堢悰灞炬綀閸氬孩妲搁崥锔跨箽閻ｆ瑦婀＄拹褍銇旂�靛摜娈戦弽鍥唶缁鐎�
    //OrderInsert.ReservePositionFlag = THOST_FTDC_EOPF_UnReserve;//娑撳秳绻氶悾娆忋仈鐎碉拷1閿燂拷7
    OrderInsert.CloseFlag = THOST_FTDC_EOCF_NotToClose;//閺堢喐娼堢悰灞炬綀閸氬海鏁撻幋鎰畱婢舵潙顕弰顖氭儊閼奉亜濮╅獮鍏呯波缁鐎�
    //OrderInsert.CloseFlag = THOST_FTDC_EOCF_AutoClose;//閼奉亜濮╅獮鍏呯波
    //strcpy(OrderInsert.InvestUnitID, "");AccountID
    //strcpy(OrderInsert.AccountID, "");
    //strcpy(OrderInsert.CurrencyID, "CNY");
    //strcpy(OrderInsert.ClientID, "");
    int b = m_pUserApi->ReqExecOrderInsert(&OrderInsert, 1);
    LOGDEMO((b == 0) ? "閹笛嗩攽鐎癸絽鎲¤ぐ鏇炲弳鐠囬攱鐪�......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閹笛嗩攽鐎癸絽鎲¤ぐ鏇炲弳鐠囬攱鐪�......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

///閹笛嗩攽鐎癸絽鎲￠幙宥勭稊鐠囬攱鐪�
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
    a.ActionFlag = THOST_FTDC_AF_Delete;//閸掔娀娅�
    strcpy(a.UserID, g_chUserID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    //strcpy(a.InvestUnitID, "");
    //strcpy(a.IPAddress, "");
    //strcpy(a.MacAddress, "");
    int b = m_pUserApi->ReqExecOrderAction(&a, 1);
    LOGDEMO((b == 0) ? "閹笛嗩攽鐎癸絽鎲￠幙宥勭稊鐠囬攱鐪�......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閹笛嗩攽鐎癸絽鎲￠幙宥勭稊鐠囬攱鐪�......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//閹靛綊鍣洪幎銉ュ礋閹垮秳缍旂拠閿嬬湴
void CSimpleHandler::ReqBatchOrderAction()
{
    CThostFtdcInputBatchOrderActionField a = { 0 };

}

///鐠囬攱鐪伴弻銉嚄閹躲儱宕�
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
    LOGDEMO((ab == 0) ? "鐠囬攱鐪伴弻銉嚄閹躲儱宕�......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄閹躲儱宕�......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", ab);
}

///閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪�
void CSimpleHandler::ReqOrderInsert_Condition(int select_num)
{
    string limit_price;
    LOGDEMO("鐠囩柉绶崗銉﹀瘹鐎规矮鐜弽锟�1閿燂拷7(limitprice):\n");
    cin >> limit_price;

    string stop_price;
    LOGDEMO("鐠囩柉绶崗銉ㄐ曢崣鎴滅幆閺嶏拷1閿燂拷7(stopprice):\n");
    cin >> stop_price;

    CThostFtdcInputOrderField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    strcpy(a.UserID, g_chUserID);
    a.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    a.Direction = THOST_FTDC_D_Buy;//娑旓拷1閿燂拷7
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
    LOGDEMO((ab == 0) ? "鐠囬攱鐪伴幎銉ュ弳閺夆�叉閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴幎銉ュ弳閺夆�叉閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", ab);
}

///閹躲儱宕熼幙宥勭稊鐠囬攱鐪�
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
    LOGDEMO((ab == 0) ? "鐠囬攱鐪伴幘銈夋敘閺夆�叉閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴幘銈夋敘閺夆�叉閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", ab);
}

//閹俱倝鏀㈤弻銉嚄閻ㄥ嫭濮ら崡锟�1閿燂拷7
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
    LOGDEMO((ab == 0) ? "鐠囬攱鐪伴幘銈夋敘閺夆�叉閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴幘銈夋敘閺夆�叉閸楋拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", ab);
}

///鐠囬攱鐪伴弻銉嚄閹存劒姘�
void CSimpleHandler::ReqQryTrade()
{
    CThostFtdcQryTradeField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    string instr;
    instr.clear();
    LOGDEMO("鐠囩柉绶崗銉ユ値缁撅缚鍞惍锟�1閿燂拷7(娑撳秷绶崗銉ュ灟娑撹櫣鈹�)\n");
    cin.ignore();
    getline(cin, instr);
    strcpy(a.InstrumentID, instr.c_str());

    string Exch;
    Exch.clear();
    LOGDEMO("鐠囩柉绶崗銉ゆ唉閺勬挻澧嶆禒锝囩垳(娑撳秷绶崗銉ュ灟娑撹櫣鈹�)\n");
    //cin.ignore();
    getline(cin, Exch);
    strcpy(a.ExchangeID, Exch.c_str());
    /*strcpy(a.TradeID, "");
    strcpy(a.TradeTimeStart, "");
    strcpy(a.TradeTimeEnd, "");*/
    int b = m_pUserApi->ReqQryTrade(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄閹存劒姘�......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄閹存劒姘�......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

///鐠囬攱鐪伴弻銉嚄妫板嫬鐓侀崡锟�1閿燂拷7
void CSimpleHandler::ReqQryParkedOrder()
{
    CThostFtdcQryParkedOrderField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    //strcpy(a.InstrumentID, g_chInstrumentID);
    strcpy(a.ExchangeID, g_chExchangeID);
    int ab = m_pUserApi->ReqQryParkedOrder(&a, nRequestID++);
    LOGDEMO((ab == 0) ? "鐠囬攱鐪伴弻銉嚄妫板嫬鐓侀崡锟�1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄妫板嫬鐓侀崡锟�1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", ab);
}

//鐠囬攱鐪伴弻銉嚄閺堝秴濮熼崳銊╊暕閸╁鎸欓崡锟�1閿燂拷7
void CSimpleHandler::ReqQryParkedOrderAction()
{
    CThostFtdcQryParkedOrderActionField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    strcpy(a.ExchangeID, g_chExchangeID);
    int ab = m_pUserApi->ReqQryParkedOrderAction(&a, nRequestID++);
    LOGDEMO((ab == 0) ? "鐠囬攱鐪伴弻銉嚄閺堝秴濮熼崳銊╊暕閸╁鎸欓崡锟�1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄閺堝秴濮熼崳銊╊暕閸╁鎸欓崡锟�1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉鎼村繐褰�=[%d]\n", ab);
}

//鐠囬攱鐪伴弻銉嚄鐠у嫰鍣剧拹锔藉煕
void CSimpleHandler::ReqQryTradingAccount(CThostFtdcQryTradingAccountField& qryTradingAccount)
{
//    CThostFtdcQryTradingAccountField a = { 0 };
//    strcpy(a.BrokerID, g_chBrokerID);
//    strcpy(a.InvestorID, g_chInvestorID);
//    strcpy(a.CurrencyID, "CNY");
    int result = m_pUserApi->ReqQryTradingAccount(&qryTradingAccount, nRequestID++);
    INFO_LOG("ReqQryTradingAccount request send ok! result is [%d]",result);
}

//鐠囬攱鐪伴弻銉嚄閹舵洝绁懓鍛瘮娴狅拷1閿燂拷7
void CSimpleHandler::ReqQryInvestorPosition()
{
    CThostFtdcQryInvestorPositionField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    string instr;
    instr.clear();
    cin.ignore();
    LOGDEMO("鐠囩柉绶崗銉ユ値缁撅缚鍞惍锟�1閿燂拷7(娑撳秷绶崗銉ュ灟娑撹櫣鈹�)閿涙瓡n");
    getline(cin, instr);
    strcpy(a.InstrumentID, instr.c_str());

    string exch;
    exch.clear();
    cin.ignore();
    LOGDEMO("鐠囩柉绶崗銉ゆ唉閺勬挻澧嶆禒锝囩垳(娑撳秷绶崗銉ュ灟娑撹櫣鈹�)閿涙瓡n");
    getline(cin, exch);
    strcpy(a.ExchangeID, exch.c_str());
    //strcpy(a.InstrumentID, "SPD");
    int b = m_pUserApi->ReqQryInvestorPosition(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄閹舵洝绁懓鍛瘮娴狅拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄閹舵洝绁懓鍛瘮娴狅拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//鐠囬攱鐪伴弻銉嚄閹舵洝绁懓鍛瘮娴犳挻妲戠紒锟�1閿燂拷7
void CSimpleHandler::ReqQryInvestorPositionDetail()
{
    CThostFtdcQryInvestorPositionDetailField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    string instr;
    instr.clear();
    cin.ignore();
    LOGDEMO("鐠囩柉绶崗銉ユ値缁撅缚鍞惍锟�1閿燂拷7(娑撳秷绶崗銉ュ灟娑撹櫣鈹�)\n");
    getline(cin, instr);
    strcpy(a.InstrumentID, instr.c_str());
    string exch;
    exch.clear();
    cin.ignore();
    LOGDEMO("鐠囩柉绶崗銉ゆ唉閺勬挻澧嶆禒锝囩垳(娑撳秷绶崗銉ュ灟娑撹櫣鈹�)閿涙瓡n");
    getline(cin, exch);
    strcpy(a.ExchangeID, exch.c_str());
    //strcpy(a.InstrumentID, g_chInstrumentID);
    int b = m_pUserApi->ReqQryInvestorPositionDetail(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄閹舵洝绁懓鍛瘮娴犳挻妲戠紒锟�1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄閹舵洝绁懓鍛瘮娴犳挻妲戠紒锟�1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//鐠囬攱鐪伴弻銉嚄娴溿倖妲楅幍锟�1閿燂拷7娣囨繆鐦夐柌鎴犲芳
void CSimpleHandler::ReqQryExchangeMarginRate()
{
    CThostFtdcQryExchangeMarginRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    a.HedgeFlag = THOST_FTDC_HF_Speculation;//閹舵洘婧�
    int b = m_pUserApi->ReqQryExchangeMarginRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄娴溿倖妲楅幍锟�1閿燂拷7娣囨繆鐦夐柌鎴犲芳......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄娴溿倖妲楅幍锟�1閿燂拷7娣囨繆鐦夐柌鎴犲芳......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//鐠囬攱鐪伴弻銉嚄閸氬牏瀹虫穱婵婄槈闁叉垹宸�
void CSimpleHandler::ReqQryInstrumentMarginRate()
{
    CThostFtdcQryInstrumentMarginRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    a.HedgeFlag = THOST_FTDC_HF_Speculation;//閹舵洘婧�
    int b = m_pUserApi->ReqQryInstrumentMarginRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄閸氬牏瀹虫穱婵婄槈闁叉垹宸�......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄閸氬牏瀹虫穱婵婄槈闁叉垹宸�......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//鐠囬攱鐪伴弻銉嚄閸氬牏瀹抽幍瀣敾鐠愬湱宸�
void CSimpleHandler::ReqQryInstrumentCommissionRate()
{
    CThostFtdcQryInstrumentCommissionRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    int b = m_pUserApi->ReqQryInstrumentCommissionRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄閸氬牏瀹抽幍瀣敾鐠愬湱宸�......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄閸氬牏瀹抽幍瀣敾鐠愬湱宸�......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//鐠囬攱鐪伴弻銉嚄閸嬫艾绔堕崯鍡楁値缁撅附澧滅紒顓″瀭閻滐拷1閿燂拷7
void CSimpleHandler::ReqQryMMInstrumentCommissionRate()
{
    CThostFtdcQryMMInstrumentCommissionRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    int b = m_pUserApi->ReqQryMMInstrumentCommissionRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄閸嬫艾绔堕崯鍡楁値缁撅附澧滅紒顓″瀭閻滐拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄閸嬫艾绔堕崯鍡楁値缁撅附澧滅紒顓″瀭閻滐拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//鐠囬攱鐪伴弻銉嚄閸嬫艾绔堕崯鍡樻埂閺夊啫鎮庣痪锔藉缂侇叀鍨�
void CSimpleHandler::ReqQryMMOptionInstrCommRate()
{
    CThostFtdcQryMMOptionInstrCommRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    int b = m_pUserApi->ReqQryMMOptionInstrCommRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄閸嬫艾绔堕崯鍡樻埂閺夊啫鎮庣痪锔藉缂侇叀鍨�......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄閸嬫艾绔堕崯鍡樻埂閺夊啫鎮庣痪锔藉缂侇叀鍨�......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//鐠囬攱鐪伴弻銉嚄閹躲儱宕熼幍瀣敾鐠愶拷1閿燂拷7
void CSimpleHandler::ReqQryInstrumentOrderCommRate()
{
    CThostFtdcQryInstrumentOrderCommRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    int b = m_pUserApi->ReqQryInstrumentOrderCommRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄閹躲儱宕熼幍瀣敾鐠愶拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄閹躲儱宕熼幍瀣敾鐠愶拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//鐠囬攱鐪伴弻銉嚄閺堢喐娼堥崥鍫㈠閹靛鐢荤拹锟�1閿燂拷7
void CSimpleHandler::ReqQryOptionInstrCommRate()
{
    CThostFtdcQryOptionInstrCommRateField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    string Inst;
    string Exch;
    string InvestUnit;
    LOGDEMO("鐠囩柉绶崗銉ユ値缁撅缚鍞惍锟�1閿燂拷7(娑撳秴锝為崚娆庤礋缁岋拷1閿燂拷7)");
    cin >> Inst;
    LOGDEMO("鐠囩柉绶崗銉ゆ唉閺勬挻澧嶆禒锝囩垳:(娑撳秴锝為崚娆庤礋缁岋拷1閿燂拷7)");
    cin >> Exch;
    LOGDEMO("鐠囩柉绶崗銉﹀鐠у嫸鎷�1閿燂拷7閿熻棄宕熼崗鍐у敩閻拷1閿燂拷7(娑撳秴锝為崚娆庤礋缁岋拷1閿燂拷7)");
    cin >> InvestUnit;
    strcpy(a.InstrumentID, Inst.c_str());
    strcpy(a.ExchangeID, Exch.c_str());
    strcpy(a.InvestUnitID, InvestUnit.c_str());
    int b = m_pUserApi->ReqQryOptionInstrCommRate(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄閺堢喐娼堥崥鍫㈠閹靛鐢荤拹锟�1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄閺堢喐娼堥崥鍫㈠閹靛鐢荤拹锟�1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//鐠囬攱鐪伴弻銉嚄閸氬牏瀹�
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
        INFO_LOG("鐠囬攱鐪伴弻銉嚄閸氬牏瀹�......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝锟�1閿燂拷7");
        return;
    }

    INFO_LOG("鐠囬攱鐪伴弻銉嚄閸氬牏瀹�......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
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
///鐠囬攱鐪伴弻銉嚄閸氬牏瀹抽崫宥呯安
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

//鐠囬攱鐪伴弻銉嚄閹舵洝绁懓鍛波缁犳绮ㄩ弸锟�1閿燂拷7
void CSimpleHandler::ReqQrySettlementInfo()
{
    CThostFtdcQrySettlementInfoField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    string Traday;
    LOGDEMO("鐠囩柉绶崗銉ゆ唉閺勬挻妫╅張鐔稿灗閼板懍姘﹂弰鎾存箑娴狅拷1閿燂拷7(娓氬顩�:20180101,閺堝牅鍞ら崚锟�1閿燂拷71801):");
    cin >> Traday;
    strcpy(a.TradingDay, Traday.c_str());
    int b = m_pUserApi->ReqQrySettlementInfo(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄閹舵洝绁懓鍛波缁犳绮ㄩ弸锟�1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄閹舵洝绁懓鍛波缁犳绮ㄩ弸锟�1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//鐠囬攱鐪伴弻銉嚄鏉烆剙绗庡ù浣规寜
void CSimpleHandler::ReqQryTransferSerial()
{
    CThostFtdcQryTransferSerialField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.AccountID, g_chInvestorID);
cir1:int bankid;
    LOGDEMO("鐠囩柉绶崗銉ょ稑闂囷拷1閿燂拷7鐟曚胶娈戦弻銉嚄閻ㄥ嫰鎽辩悰瀛縩");
    LOGDEMO("1.瀹搞儱鏅㈤柧鎯邦攽\n");
    LOGDEMO("2.閸愭粈绗熼柧鎯邦攽\n");
    LOGDEMO("3.娑擃厼娴楅柧鎯邦攽\n");
    LOGDEMO("5.娴溿倧鎷�1閿燂拷7閿熶粙鎽辩悰瀛縩");
    LOGDEMO("6.閹锋稑鏅㈤柧鎯邦攽\n");
    LOGDEMO("7.閸忕繝绗熼柧鎯邦攽\n");
    LOGDEMO("8.濞达箑褰傞柧鎯邦攽\n");
    LOGDEMO("9.濮樻垹鏁撻柧鎯邦攽\n");
    LOGDEMO("10.閸忓銇囬柧鎯邦攽\n");
    LOGDEMO("11.娑擃厺淇婇柧鎯邦攽\n");
    LOGDEMO("12.濮瑰洣璧撮柧鎯邦攽\n");
    LOGDEMO("13.楠炲啿鐣ㄩ柧鎯邦攽\n");
    LOGDEMO("14.閸愭粌褰傞柧鎯邦攽\n");
    LOGDEMO("15.閺勭喎鐫嶉柧鎯邦攽\n");
    LOGDEMO("16.楠炲灝褰傞柧鎯邦攽\n");
    cin >> bankid;
    if (bankid == 1 | 2 | 3 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16)
    {
        //strcpy(a.BankID, itoa(bankid, a.BankID, 10));///闁炬儼顢戞禒锝囩垳
        //itoa(bankid, a.BankID, 10);
        sprintf(a.BankID, "%d", bankid);
    }
    else
    {
        LOGDEMO("鐠囩兘鍣搁弬鎷岀翻閸忋儵鎽辩悰灞煎敩閻緤鎷�1閿燂拷7閿熺祿n");
        goto cir1;
    }
    int choos;
curr:LOGDEMO("鐠囩柉绶崗銉ョ缁夊秳鍞惍涔梩1.CNY\t2.USD\n");
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
        LOGDEMO("鐠囩柉绶崗銉︻劀绾喚娈戞惔蹇撳娇\n");
        system("stty -echo");
        getchar();
        system("stty echo");
        goto curr;
    }
    int b = m_pUserApi->ReqQryTransferSerial(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄鏉烆剙绗庡ù浣规寜......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄鏉烆剙绗庡ù浣规寜......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//鐠囬攱鐪伴弻銉嚄娴溠冩惂
void CSimpleHandler::ReqQryProduct()
{
    CThostFtdcQryProductField a = { 0 };
    strcpy(a.ProductID, "sc");
    a.ProductClass = THOST_FTDC_PC_Futures;
    strcpy(a.ExchangeID, g_chExchangeID);
    m_pUserApi->ReqQryProduct(&a, nRequestID++);
}

//鐠囬攱鐪伴弻銉嚄鏉烆剙绗庨柧鎯邦攽
void CSimpleHandler::ReqQryTransferBank()
{
    CThostFtdcQryTransferBankField a = { 0 };
    strcpy(a.BankID, "3");
    int b = m_pUserApi->ReqQryTransferBank(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄鏉烆剙绗庨柧鎯邦攽......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄鏉烆剙绗庨柧鎯邦攽......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//鐠囬攱鐪伴弻銉嚄娴溿倖妲楅柅姘辩叀
void CSimpleHandler::ReqQryTradingNotice()
{
    CThostFtdcQryTradingNoticeField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    int b = m_pUserApi->ReqQryTradingNotice(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄娴溿倖妲楅柅姘辩叀......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄娴溿倖妲楅柅姘辩叀......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//鐠囬攱鐪伴弻銉嚄娴溿倖妲楃紓鏍垳
void CSimpleHandler::ReqQryTradingCode()
{
    CThostFtdcQryTradingCodeField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.ExchangeID, g_chExchangeID);
    a.ClientIDType = THOST_FTDC_CIDT_Speculation;
    int b = m_pUserApi->ReqQryTradingCode(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄娴溿倖妲楃紓鏍垳......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄娴溿倖妲楃紓鏍垳......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//鐠囬攱鐪伴弻銉嚄缂佹挾鐣绘穱鈩冧紖绾喛顓�
void CSimpleHandler::ReqQrySettlementInfoConfirm()
{
    CThostFtdcQrySettlementInfoConfirmField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    //strcpy(a.AccountID, g_chInvestorID);
    strcpy(a.CurrencyID, "CNY");
    int b = m_pUserApi->ReqQrySettlementInfoConfirm(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄缂佹挾鐣绘穱鈩冧紖绾喛顓�......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄缂佹挾鐣绘穱鈩冧紖绾喛顓�......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//鐠囬攱鐪伴弻銉嚄娴溠冩惂缂侊拷1閿燂拷7
void CSimpleHandler::ReqQryProductGroup()
{
    CThostFtdcQryProductGroupField a = { 0 };

}

//鐠囬攱鐪伴弻銉嚄閹舵洝绁懓鍛礋閸忥拷1閿燂拷7
void CSimpleHandler::ReqQryInvestUnit()
{
    CThostFtdcQryInvestUnitField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    //strcpy(a.InvestorID, "00402");
    //strcpy(a.InvestorID, g_chInvestorID);
    int b = m_pUserApi->ReqQryInvestUnit(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄閹舵洝绁懓鍛礋閸忥拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄閹舵洝绁懓鍛礋閸忥拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//鐠囬攱鐪伴弻銉嚄缂佸繒閭崗顒�寰冩禍銈嗘閸欏倹鏆�
void CSimpleHandler::ReqQryBrokerTradingParams()
{
    CThostFtdcQryBrokerTradingParamsField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.CurrencyID, "CNY");
    int b = m_pUserApi->ReqQryBrokerTradingParams(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄缂佸繒閭崗顒�寰冩禍銈嗘閸欏倹鏆�......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄缂佸繒閭崗顒�寰冩禍銈嗘閸欏倹鏆�......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//鐠囬攱鐪伴弻銉嚄鐠囶澀鐜�
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
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄鐠囶澀鐜�......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄鐠囶澀鐜�......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//鐠囬攱鐪伴弻銉嚄閹躲儰鐜�
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
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄鐠囶澀鐜�......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄鐠囶澀鐜�......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

///鐠囶澀鐜ぐ鏇炲弳鐠囬攱鐪�
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
    LOGDEMO((b == 0) ? "鐠囶澀鐜ぐ鏇炲弳鐠囬攱鐪�......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囶澀鐜ぐ鏇炲弳鐠囬攱鐪�......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

///閸嬫艾绔堕崯鍡樺Г娴犲嘲缍嶉崗銉嚞濮癸拷1閿燂拷7
void CSimpleHandler::ReqQuoteInsert()
{
choose:int choose_Flag;
    LOGDEMO("鐠囬鈥樼拋銈呯磻楠炶櫕鐖ｈ箛姊妕1.瀵拷1閿燂拷7娴犳彫t2.楠炲厖绮╘n");
    cin >> choose_Flag;

    if (choose_Flag != 1 && choose_Flag != 2)
    {
        LOGDEMO("鐠囩兘鍣搁弬甯嫹1閿燂拷7閿熻姤瀚ㄥ锟�1閿燂拷7楠炶櫕鐖ｈ箛姊妌");
        system("stty -echo");
        getchar();
        system("stty echo");
        choose_Flag = 0; //NULL;
        goto choose;
    }

    int price_bid;
    LOGDEMO("鐠囩柉绶崗銉ゆ嫳閺傜懓鎮滄禒閿嬬壐閿涙瓡n");
    cin >> price_bid;

    int price_ask;
    LOGDEMO("鐠囩柉绶崗銉ュ礌閺傜懓鎮滄禒閿嬬壐閿涙瓡n");
    cin >> price_ask;
    LOGDEMO("娑旀澘宕犻弫浼村櫤姒涙顓婚弰锟�1閿燂拷7閿熺祿n");
    string quoteref;
    LOGDEMO("鐠囩柉绶崗顧穟oteref鎼村繐褰块敍姝昻");
    cin >> quoteref;
    string AskOrderRef;
    string BidOrderRef;
    LOGDEMO("鐠囩柉绶崗顧噑kOrderRef鎼村繐褰�:\n");
    cin >> AskOrderRef;
    LOGDEMO("鐠囩柉绶崗顧坕dOrderRef鎼村繐褰�:\n");
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
        t.AskOffsetFlag = THOST_FTDC_OF_Open;///閸楁牕绱戦獮铏垼韫囷拷1閿燂拷7
        t.BidOffsetFlag = THOST_FTDC_OF_Open;///娑旀澘绱戦獮铏垼韫囷拷1閿燂拷7
    }
    else if (choose_Flag == 2)
    {
        t.AskOffsetFlag = THOST_FTDC_OF_Close;///閸楁牕绱戦獮铏垼韫囷拷1閿燂拷7
        t.BidOffsetFlag = THOST_FTDC_OF_Close;///娑旀澘绱戦獮铏垼韫囷拷1閿燂拷7
    }
    t.AskHedgeFlag = THOST_FTDC_HF_Speculation;///閸楁牗濮囬張鍝勵殰娣囨繃鐖ｈ箛锟�1閿燂拷7
    t.BidHedgeFlag = THOST_FTDC_HF_Speculation;///娑旂増濮囬張鍝勵殰娣囨繃鐖ｈ箛锟�1閿燂拷7

    strcpy(t.AskOrderRef, AskOrderRef.c_str());///鐞涘秶鏁撻崡鏍ㄥГ閸楁洖绱╅悽锟�1閿燂拷7
    strcpy(t.BidOrderRef, BidOrderRef.c_str());///鐞涘秶鏁撴稊鐗堝Г閸楁洖绱╅悽锟�1閿燂拷7
    //strcpy(t.ForQuoteSysID, "");///鎼存柧鐜紓鏍у娇
    //strcpy(t.InvestUnitID, "1");///閹舵洝绁崡鏇炲帗娴狅絿鐖�
    int a = m_pUserApi->ReqQuoteInsert(&t, 1);
    LOGDEMO((a == 0) ? "閸嬫艾绔堕崯鍡樺Г娴犲嘲缍嶉崗銉嚞濮癸拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閸嬫艾绔堕崯鍡樺Г娴犲嘲缍嶉崗銉嚞濮癸拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", a);
}

///閹躲儰鐜柅姘辩叀
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

//閹躲儰鐜幘銈夋敘
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

//閺屻儴顕楅張锟�1閿燂拷7婢堆勫Г閸楁洘鏆熼柌蹇氼嚞濮癸拷1閿燂拷7
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
    LOGDEMO((b == 0) ? "閺屻儴顕楅張锟�1閿燂拷7婢堆勫Г閸楁洘鏆熼柌蹇氼嚞濮癸拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閺屻儴顕楅張锟�1閿燂拷7婢堆勫Г閸楁洘鏆熼柌蹇氼嚞濮癸拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//鐠囬攱鐪伴弻銉嚄閻╂垶甯舵稉顓炵妇閻€劍鍩涙禒銈囧
void CSimpleHandler::ReqQueryCFMMCTradingAccountToken()
{
    CThostFtdcQueryCFMMCTradingAccountTokenField a = { 0 };

}
///閹躲儱宕熼幙宥勭稊闁挎瑨顕ら崶鐐村Г
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

///閹躲儱宕熻ぐ鏇炲弳鐠囬攱鐪伴崫宥呯安
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

///閹躲儱宕熻ぐ鏇炲弳闁挎瑨顕ら崶鐐村Г
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

///閹躲儱宕熼柅姘辩叀
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
        if (pOrder->OrderStatus == THOST_FTDC_OST_AllTraded)///閸忋劑鍎撮幋鎰唉
        {
            INFO_LOG("閹躲儱宕熼崗銊╁劥閹存劒姘�");
            //SetEvent(g_hEvent);
        }
        else
        {
            if (pOrder->OrderStatus == THOST_FTDC_OST_PartTradedQueueing)///闁劌鍨庨幋鎰唉鏉╂ê婀梼鐔峰灙娑擄拷1閿燂拷7
            {
                INFO_LOG("闁劌鍨庨幋鎰唉鏉╂ê婀梼鐔峰灙娑擄拷");
            }
            if (pOrder->OrderStatus == THOST_FTDC_OST_PartTradedNotQueueing)///闁劌鍨庨幋鎰唉娑撳秴婀梼鐔峰灙娑擄拷1閿燂拷7
            {
                INFO_LOG("闁劌鍨庨幋鎰唉娑撳秴婀梼鐔峰灙娑擄拷");
            }
            if (pOrder->OrderStatus == THOST_FTDC_OST_NoTradeQueueing)///閺堫亝鍨氭禍銈堢箷閸︺劑妲﹂崚妞捐厬
            {
                chioce_action = 0;
                INFO_LOG("閺堫亝鍨氭禍銈堢箷閸︺劑妲﹂崚妞捐厬");
            }
            if (pOrder->OrderStatus == THOST_FTDC_OST_NoTradeNotQueueing)///閺堫亝鍨氭禍銈勭瑝閸︺劑妲﹂崚妞捐厬
            {
                INFO_LOG("閺堫亝鍨氭禍銈勭瑝閸︺劑妲﹂崚妞捐厬");
            }
            if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)///閹俱倕宕�
            {
                repResult = InsertResult::Failed;
                INFO_LOG("閹俱倕宕�");
                string orderRef = pOrder->OrderRef;
                DEBUG_LOG("orderRef:%s",orderRef.c_str());
//                DEBUG_LOG("globalSem.semOrder1.orderRef:%s; ",globalSem.semOrder1.orderRef.c_str());
//                DEBUG_LOG("globalSem.semOrder2.orderRef:%s; ",globalSem.semOrder2.orderRef.c_str());

                globalSem.postSemByOrderRef(orderRef);
                INFO_LOG("post globalSem %s, 閹俱倕宕�",orderRef.c_str());
//                if(postSemByOrderRef.semOrder1.orderRef == orderRef)
//                {
//                    DEBUG_LOG("semOrder1");
//                    INFO_LOG("%s","post globalSem.semOrder1.sem, 閹俱倕宕�");
//                    sem_post(&globalSem.semOrder1.sem);
//                }
//                if(globalSem.semOrder2.orderRef == orderRef)
//                {
//                    DEBUG_LOG("semOrder2");
//                    INFO_LOG("%s","post globalSem.semOrder2.sem, 閹俱倕宕�");
//                    sem_post(&globalSem.semOrder2.sem);
//                }
            }if (pOrder->OrderStatus == THOST_FTDC_OST_Unknown)///閺堫亞鐓�
            {
                INFO_LOG("閺堫亞鐓�");
            }
            if (pOrder->OrderStatus == THOST_FTDC_OST_NotTouched)///鐏忔碍婀憴锕�褰�
            {
                chioce_action = 1;
                INFO_LOG("鐏忔碍婀憴锕�褰�");
            }
            if (pOrder->OrderStatus == THOST_FTDC_OST_Touched)///瀹歌尪袝閸欙拷1閿燂拷7
            {
                INFO_LOG("瀹歌尪袝閸欙拷1閿燂拷7");
            }
        }

    }


}

///閸掔娀娅庢０鍕厑閸楁洖鎼锋惔锟�1閿燂拷7
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

///閸掔娀娅庢０鍕厑閹俱倕宕熼崫宥呯安
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

///妫板嫬鐓侀崡鏇炵秿閸忋儴顕Ч鍌氭惙鎼达拷1閿燂拷7
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

///妫板嫬鐓侀幘銈呭礋瑜版洖鍙嗙拠閿嬬湴閸濆秴绨�
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

///鐠囬攱鐪伴弻銉嚄妫板嫬鐓侀幘銈呭礋閸濆秴绨�
void CSimpleHandler::OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CTraderSpi::OnRspQryParkedOrderAction(pParkedOrderAction, pRspInfo, nRequestID, bIsLast);
}

///鐠囬攱鐪伴弻銉嚄妫板嫬鐓侀崡鏇炴惙鎼达拷1閿燂拷7
void CSimpleHandler::OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CTraderSpi::OnRspQryParkedOrder(pParkedOrder, pRspInfo, nRequestID, bIsLast);
}

///鐠囬攱鐪伴弻銉嚄閹躲儱宕熼崫宥呯安
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

///閹笛嗩攽鐎癸絽鎲￠柅姘辩叀
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

//閺堢喕鎻ｉ崣鎴ｆ崳閺屻儴顕楅柧鎯邦攽娴ｆ瑩顤傜拠閿嬬湴
void CSimpleHandler::ReqQueryBankAccountMoneyByFuture()
{
    CThostFtdcReqQueryAccountField a = { 0 };
    int b = m_pUserApi->ReqQueryBankAccountMoneyByFuture(&a, nRequestID++);
    LOGDEMO((b == 0) ? "閺堢喕鎻ｉ崣鎴ｆ崳閺屻儴顕楅柧鎯邦攽娴ｆ瑩顤傜拠閿嬬湴......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閺堢喕鎻ｉ崣鎴ｆ崳閺屻儴顕楅柧鎯邦攽娴ｆ瑩顤傜拠閿嬬湴......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//閺堢喕鎻ｉ崣鎴ｆ崳闁炬儼顢戠挧鍕櫨鏉烆剚婀＄拹褑顕Ч锟�1閿燂拷7
void CSimpleHandler::ReqFromBankToFutureByFuture()
{
    int output_num;
    LOGDEMO("鐠囩柉绶崗銉ㄦ祮鐠愶箓鍣炬０锟�1閿燂拷7");
    cin >> output_num;

    CThostFtdcReqTransferField a = { 0 };
    strcpy(a.TradeCode, "202001");///娑撴艾濮熼崝鐔诲厴閻拷1閿燂拷7
    int bankid = 0;
    while (bankid != 1 & 2 & 3 & 5 & 6 & 7 & 8 & 9 & 10 & 11 & 12 & 13 & 14 & 15 & 16) {
        LOGDEMO("鐠囩柉绶崗銉ょ稑闂囷拷1閿燂拷7鐟曚胶娈戞潪顒冨閻ㄥ嫰鎽辩悰瀛縩");
        LOGDEMO("1.瀹搞儱鏅㈤柧鎯邦攽\n");
        LOGDEMO("2.閸愭粈绗熼柧鎯邦攽\n");
        LOGDEMO("3.娑擃厼娴楅柧鎯邦攽\n");
        LOGDEMO("5.娴溿倧鎷�1閿燂拷7閿熶粙鎽辩悰瀛縩");
        LOGDEMO("6.閹锋稑鏅㈤柧鎯邦攽\n");
        LOGDEMO("7.閸忕繝绗熼柧鎯邦攽\n");
        LOGDEMO("8.濞达箑褰傞柧鎯邦攽\n");
        LOGDEMO("9.濮樻垹鏁撻柧鎯邦攽\n");
        LOGDEMO("10.閸忓銇囬柧鎯邦攽\n");
        LOGDEMO("11.娑擃厺淇婇柧鎯邦攽\n");
        LOGDEMO("12.濮瑰洣璧撮柧鎯邦攽\n");
        LOGDEMO("13.楠炲啿鐣ㄩ柧鎯邦攽\n");
        LOGDEMO("14.閸愭粌褰傞柧鎯邦攽\n");
        LOGDEMO("15.閺勭喎鐫嶉柧鎯邦攽\n");
        LOGDEMO("16.楠炲灝褰傞柧鎯邦攽\n");
        cin >> bankid;
        if (bankid == 1 | 2 | 3 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16)
        {
            //strcpy(a.BankID, itoa(bankid, a.BankID, 10));///闁炬儼顢戞禒锝囩垳
            //itoa(bankid, a.BankID, 10);
            sprintf(a.BankID, "%d", bankid);
        }
        else
        {
            LOGDEMO("鐠囩兘鍣搁弬鎷岀翻閸忋儵鎽辩悰灞煎敩閻緤鎷�1閿燂拷7閿熺祿n");
            system("stty -echo");
            getchar();
            system("stty echo");
        }
    }


    strcpy(a.BankBranchID, "0000");///閺堢喎鏅㈡禒锝囩垳
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.TradeDate, "20170829");///娴溿倖妲楅弮銉︽埂
    strcpy(a.TradeTime, "09:00:00");
    strcpy(a.BankSerial, "6889");///闁炬儼顢戝ù浣规寜閸欙拷1閿燂拷7
    strcpy(a.TradingDay, "20170829");///娴溿倖妲楃化鑽ょ埠閺冦儲婀�
    a.PlateSerial = 5;///闁捐埖婀￠獮鍐插酱濞戝牊浼呭ù浣规寜閸欙拷1閿燂拷7
    a.LastFragment = THOST_FTDC_LF_Yes;///閺堬拷1閿燂拷7閸氬骸鍨庨悧鍥ㄧ垼韫囷拷1閿燂拷7 '0'=閺勵垱娓堕崥搴″瀻閻楋拷1閿燂拷7
    a.SessionID = SessionID;
    //strcpy(a.CustomerName, "");///鐎广垺鍩涙慨鎾虫倳
    a.IdCardType = THOST_FTDC_ICT_IDCard;///鐠囦椒娆㈢猾璇茬��
    a.CustType = THOST_FTDC_CUSTT_Person;///鐎广垺鍩涚猾璇茬��
    //strcpy(a.IdentifiedCardNo, "310115198706241914");///鐠囦椒娆㈤崣椋庣垳
    /*strcpy(a.BankAccount, "123456789");
    strcpy(a.BankPassWord, "123456");///闁炬儼顢戠�靛棛鐖�*/
    strcpy(a.BankAccount, "621485212110187");
    //strcpy(a.BankPassWord, "092812");///闁炬儼顢戠�靛棛鐖�--娑撳秹娓剁憰渚�鎽辩悰灞藉幢鐎靛棛鐖�
    strcpy(a.AccountID, g_chInvestorID);///閹舵洝绁懓鍛瑤閸欙拷1閿燂拷7
    //strcpy(a.Password, "092812");///閺堢喕鎻ｇ�靛棛鐖�--鐠у嫰鍣剧�靛棛鐖�
    strcpy(a.Password, "123456");///閺堢喕鎻ｇ�靛棛鐖�--鐠у嫰鍣剧�靛棛鐖�
    a.InstallID = 1;///鐎瑰顥婄紓鏍у娇
    a.FutureSerial = 0;///閺堢喕鎻ｉ崗顒�寰冨ù浣规寜閸欙拷1閿燂拷7
    a.VerifyCertNoFlag = THOST_FTDC_YNI_No;///妤犲矁鐦夌�广垺鍩涚拠浣锋閸欓鐖滈弽鍥х箶
    strcpy(a.CurrencyID, "CNY");///鐢胶顫掓禒锝囩垳
    a.TradeAmount = output_num;///鏉烆剙绗庨柌鎴︻杺
    a.FutureFetchAmount = 0;///閺堢喕鎻ｉ崣顖氬絿闁叉垿顤�
    a.CustFee = 0;///鎼存梹鏁圭�广垺鍩涚拹鍦暏
    a.BrokerFee = 0;///鎼存梹鏁归張鐔绘彛閸忣剙寰冪拹鍦暏
    a.SecuPwdFlag = THOST_FTDC_BPWDF_BlankCheck;///閺堢喕鎻ｇ挧鍕櫨鐎靛棛鐖滈弽绋款嚠閺嶅洤绻�
    a.RequestID = 0;///鐠囬攱鐪扮紓鏍у娇
    a.TID = 0;///娴溿倖妲桰D
    int b = m_pUserApi->ReqFromBankToFutureByFuture(&a, 1);
    LOGDEMO((b == 0) ? "閺堢喕鎻ｉ崣鎴ｆ崳闁炬儼顢戠挧鍕櫨鏉烆剚婀＄拹褑顕Ч锟�1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閺堢喕鎻ｉ崣鎴ｆ崳闁炬儼顢戠挧鍕櫨鏉烆剚婀＄拹褑顕Ч锟�1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//閺堢喕鎻ｉ崣鎴ｆ崳閺堢喕鎻ｇ挧鍕櫨鏉烆剟鎽辩悰宀冾嚞濮癸拷1閿燂拷7
void CSimpleHandler::ReqFromFutureToBankByFuture()
{
    int output_num;
    LOGDEMO("鐠囩柉绶崗銉ㄦ祮鐠愶箓鍣炬０锟�1閿燂拷7");
    cin >> output_num;

    CThostFtdcReqTransferField a = { 0 };
    strcpy(a.TradeCode, "202002");///娑撴艾濮熼崝鐔诲厴閻拷1閿燂拷7
bankid_new:int bankid = 0;
    LOGDEMO("鐠囩柉绶崗銉ょ稑闂囷拷1閿燂拷7鐟曚胶娈戞潪顒冨閻ㄥ嫰鎽辩悰瀛縩");
    LOGDEMO("1.瀹搞儱鏅㈤柧鎯邦攽\n");
    LOGDEMO("2.閸愭粈绗熼柧鎯邦攽\n");
    LOGDEMO("3.娑擃厼娴楅柧鎯邦攽\n");
    LOGDEMO("5.娴溿倧鎷�1閿燂拷7閿熶粙鎽辩悰瀛縩");
    LOGDEMO("6.閹锋稑鏅㈤柧鎯邦攽\n");
    LOGDEMO("7.閸忕繝绗熼柧鎯邦攽\n");
    LOGDEMO("8.濞达箑褰傞柧鎯邦攽\n");
    LOGDEMO("9.濮樻垹鏁撻柧鎯邦攽\n");
    LOGDEMO("10.閸忓銇囬柧鎯邦攽\n");
    LOGDEMO("11.娑擃厺淇婇柧鎯邦攽\n");
    LOGDEMO("12.濮瑰洣璧撮柧鎯邦攽\n");
    LOGDEMO("13.楠炲啿鐣ㄩ柧鎯邦攽\n");
    LOGDEMO("14.閸愭粌褰傞柧鎯邦攽\n");
    LOGDEMO("15.閺勭喎鐫嶉柧鎯邦攽\n");
    LOGDEMO("16.楠炲灝褰傞柧鎯邦攽\n");
    cin >> bankid;
    if (bankid == 1 | 2 | 3 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16)
    {
        //strcpy(a.BankID, itoa(bankid, a.BankID, 10));///闁炬儼顢戞禒锝囩垳
        //itoa(bankid, a.BankID, 10);
        sprintf(a.BankID, "%d", bankid);
    }
    else {
        LOGDEMO("鐠囩柉绶崗銉︻劀绾喚娈戠紓鏍у娇\n");
        system("stty -echo");
        getchar();
        system("stty echo");
        goto bankid_new;
    }
    strcpy(a.BankBranchID, "0000");///閺堢喎鏅㈡禒锝囩垳
    strcpy(a.BrokerID, g_chBrokerID);
    //strcpy(a.BankBranchID, "0000");///闁炬儼顢戦崚鍡樻暜閺堢儤鐎禒锝囩垳
    //strcpy(a.TradeDate, "20170829");///娴溿倖妲楅弮銉︽埂
    //strcpy(a.TradeTime, "09:00:00");
    //strcpy(a.BankSerial, "");///闁炬儼顢戝ù浣规寜閸欙拷1閿燂拷7
    //strcpy(a.TradingDay, "20170829");///娴溿倖妲楃化鑽ょ埠閺冦儲婀�
    //a.PlateSerial= 0;///闁捐埖婀￠獮鍐插酱濞戝牊浼呭ù浣规寜閸欙拷1閿燂拷7
    a.LastFragment = THOST_FTDC_LF_Yes;///閺堬拷1閿燂拷7閸氬骸鍨庨悧鍥ㄧ垼韫囷拷1閿燂拷7 '0'=閺勵垱娓堕崥搴″瀻閻楋拷1閿燂拷7
    a.SessionID = SessionID;
    //strcpy(a.CustomerName, "");///鐎广垺鍩涙慨鎾虫倳
    a.IdCardType = THOST_FTDC_ICT_IDCard;///鐠囦椒娆㈢猾璇茬��
    strcpy(a.IdentifiedCardNo, "310115198706241914");///鐠囦椒娆㈤崣椋庣垳
    strcpy(a.BankAccount, "123456789");///闁炬儼顢戠敮鎰娇
    //strcpy(a.BankPassWord, "123456");///闁炬儼顢戠�靛棛鐖�
    strcpy(a.AccountID, g_chInvestorID);///閹舵洝绁懓鍛瑤閸欙拷1閿燂拷7
    strcpy(a.Password, "123456");///閺堢喕鎻ｇ�靛棛鐖�
    a.InstallID = 1;///鐎瑰顥婄紓鏍у娇
    a.CustType = THOST_FTDC_CUSTT_Person;
    //a.FutureSerial = 0;///閺堢喕鎻ｉ崗顒�寰冨ù浣规寜閸欙拷1閿燂拷7
    a.VerifyCertNoFlag = THOST_FTDC_YNI_No;///妤犲矁鐦夌�广垺鍩涚拠浣锋閸欓鐖滈弽鍥х箶
    strcpy(a.CurrencyID, "CNY");///鐢胶顫掓禒锝囩垳
    a.TradeAmount = output_num;///鏉烆剙绗庨柌鎴︻杺
    a.FutureFetchAmount = 0;///閺堢喕鎻ｉ崣顖氬絿闁叉垿顤�
    a.CustFee = 0;///鎼存梹鏁圭�广垺鍩涚拹鍦暏
    a.BrokerFee = 0;///鎼存梹鏁归張鐔绘彛閸忣剙寰冪拹鍦暏
    //a.SecuPwdFlag = THOST_FTDC_BPWDF_BlankCheck;///閺堢喕鎻ｇ挧鍕櫨鐎靛棛鐖滈弽绋款嚠閺嶅洤绻�
    a.RequestID = 0;///鐠囬攱鐪扮紓鏍у娇
    a.TID = 0;///娴溿倖妲桰D
    int b = m_pUserApi->ReqFromFutureToBankByFuture(&a, 1);
    LOGDEMO((b == 0) ? "閺堢喕鎻ｉ崣鎴ｆ崳閺堢喕鎻ｇ挧鍕櫨鏉烆剟鎽辩悰宀冾嚞濮癸拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閺堢喕鎻ｉ崣鎴ｆ崳閺堢喕鎻ｇ挧鍕櫨鏉烆剟鎽辩悰宀冾嚞濮癸拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//閺堢喐娼堥懛顏勵嚠閸愭彃缍嶉崗銉嚞濮癸拷1閿燂拷7
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
        LOGDEMO("鐠囧嚖鎷�1閿燂拷7閿熻姤瀚ㄩ幎鏇熸簚婵傛ぞ绻氶弽鍥х箶\n1.閹舵洘婧�\t2.婵傛鍩刓t3.婵傛ぞ绻歕t4.閸嬫艾绔堕崯鍝眓");
        cin >> choose_1;
        if (choose_1 == 1) { a.HedgeFlag = THOST_FTDC_HF_Speculation; }
        else if (choose_1 == 2) { a.HedgeFlag = THOST_FTDC_HF_Arbitrage; }
        else if (choose_1 == 3) { a.HedgeFlag = THOST_FTDC_HF_Hedge; }
        else if (choose_1 == 4) { a.HedgeFlag = THOST_FTDC_HF_MarketMaker; }
        else {
            LOGDEMO("闁銆嶉柨娆掝嚖閿涘矁顕柌宥嗘煀闁瀚ㄩ妴淇搉");
            system("stty -echo");
            getchar();
            system("stty echo");
        }
    }

    int choose_2 = 0;
    while (choose_2 != 1 && choose_2 != 2 && choose_2 != 3) {
        LOGDEMO("鐠囧嚖鎷�1閿燂拷7閿熻姤瀚ㄩ張鐔告綀鐞涘本娼堥惃鍕仈鐎靛憡妲搁崥锕佸殰鐎电懓鍟块弽鍥х箶\n1.閼奉亜顕崘鍙夋埂閺夊啩绮ㄦ担宄攖2.娣囨繄鏆�閺堢喐娼堟禒鎾茬秴\t3.閼奉亜顕崘鎻掑礌閺傜懓楗辩痪锕�鎮楅惃鍕埂鐠愌傜波娴ｅ硵n");
        cin >> choose_2;
        if (choose_2 == 1) { a.OptSelfCloseFlag = THOST_FTDC_OSCF_CloseSelfOptionPosition; }
        else if (choose_2 == 2) { a.OptSelfCloseFlag = THOST_FTDC_OSCF_ReserveOptionPosition; }
        else if (choose_2 == 3) { a.OptSelfCloseFlag = THOST_FTDC_OSCF_SellCloseSelfFuturePosition; }
        else {
            LOGDEMO("闁銆嶉柨娆掝嚖閿涘矁顕柌宥嗘煀闁瀚ㄩ妴淇搉");
            system("stty -echo");
            getchar();
            system("stty echo");
            continue;
        }
    }

    strcpy(a.ExchangeID, g_chExchangeID);
    string accountid_new;
    LOGDEMO("鐠囩柉绶崗銉ㄧカ闁叉垼澶勯崣锟�1閿燂拷7\n");
    cin >> accountid_new;
    strcpy(a.AccountID, accountid_new.c_str());
    strcpy(a.CurrencyID, "CNY");
    int b = m_pUserApi->ReqOptionSelfCloseInsert(&a, 1);
    LOGDEMO((b == 0) ? "閺堢喐娼堥懛顏勵嚠閸愭彃缍嶉崗銉嚞濮癸拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閺堢喐娼堥懛顏勵嚠閸愭彃缍嶉崗銉嚞濮癸拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

///閺堢喐娼堥懛顏勵嚠閸愯鎷�1閿燂拷7閿熺晫鐓�
void CSimpleHandler::OnRtnOptionSelfClose(CThostFtdcOptionSelfCloseField *pOptionSelfClose)
{
    if (pOptionSelfClose) {
        g_chFrontID = pOptionSelfClose->FrontID;
        g_chSessionID = pOptionSelfClose->SessionID;
        strcpy(g_chOptionSelfCloseSysID, pOptionSelfClose->OptionSelfCloseSysID);//閺堢喐娼堥懛顏勵嚠閸愯尙绱崣锟�1閿燂拷7
        strcpy(g_chOptionSelfCloseRef, pOptionSelfClose->OptionSelfCloseRef);//閺堢喐娼堥懛顏勵嚠閸愭彃绱╅悽锟�1閿燂拷7
    }
    CTraderSpi::OnRtnOptionSelfClose(pOptionSelfClose);
}

//閺堢喐娼堥懛顏勵嚠閸愬弶鎼锋担婊嗩嚞濮癸拷1閿燂拷7
void CSimpleHandler::ReqOptionSelfCloseAction()
{
    CThostFtdcInputOptionSelfCloseActionField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    //strcpy(a.OptionSelfCloseSysID, g_chOptionSelfCloseSysID);//閺堢喐娼堥懛顏勵嚠閸愯尙绱崣锟�1閿燂拷7
    strcpy(a.OptionSelfCloseRef, g_chOptionSelfCloseRef);//閺堢喐娼堥懛顏勵嚠閸愭彃绱╅悽锟�1閿燂拷7
    //a.FrontID = g_chFrontID;
    //a.SessionID = g_chSessionID;
    strcpy(a.ExchangeID, g_chExchangeID);
    a.ActionFlag = THOST_FTDC_AF_Delete;
    strcpy(a.UserID, g_chUserID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    int b = m_pUserApi->ReqOptionSelfCloseAction(&a, 1);
    LOGDEMO((b == 0) ? "閺堢喐娼堥懛顏勵嚠閸愬弶鎼锋担婊嗩嚞濮癸拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閺堢喐娼堥懛顏勵嚠閸愬弶鎼锋担婊嗩嚞濮癸拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

//鐠囬攱鐪伴弻銉嚄閺堢喐娼堥懛顏勵嚠閸愶拷1閿燂拷7
void CSimpleHandler::ReqQryOptionSelfClose()
{
    CThostFtdcQryOptionSelfCloseField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.InstrumentID, g_chInstrumentID);
    strcpy(a.ExchangeID, g_chExchangeID);
    int b = m_pUserApi->ReqQryOptionSelfClose(&a, 1);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄閺堢喐娼堥懛顏勵嚠閸愶拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄閺堢喐娼堥懛顏勵嚠閸愶拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

///鐠囬攱鐪伴弻銉嚄閺堢喐娼堥懛顏勵嚠閸愭彃鎼锋惔锟�1閿燂拷7
void CSimpleHandler::OnRspQryOptionSelfClose(CThostFtdcOptionSelfCloseField *pOptionSelfClose, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (pOptionSelfClose) {
        g_chFrontID = pOptionSelfClose->FrontID;
        g_chSessionID = pOptionSelfClose->SessionID;
        strcpy(g_chOptionSelfCloseSysID, pOptionSelfClose->OptionSelfCloseSysID);//閺堢喐娼堥懛顏勵嚠閸愯尙绱崣锟�1閿燂拷7
        strcpy(g_chOptionSelfCloseRef, pOptionSelfClose->OptionSelfCloseRef);//閺堢喐娼堥懛顏勵嚠閸愭彃绱╅悽锟�1閿燂拷7
    }
    CTraderSpi::OnRspQryOptionSelfClose(pOptionSelfClose, pRspInfo, nRequestID, bIsLast);
}

///鐠囬攱鐪伴弻銉嚄閹笛嗩攽鐎癸絽鎲�
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
    LOGDEMO((b == 0) ? "閹笛嗩攽鐎癸絽鎲￠弻銉嚄......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閹笛嗩攽鐎癸絽鎲￠弻銉嚄......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

///閺屻儴顕楁禍灞煎敩鐠у嫰鍣剧拹锔藉煕
void CSimpleHandler::ReqQrySecAgentTradingAccount()
{
    CThostFtdcQryTradingAccountField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    strcpy(a.CurrencyID, "CNY");
    a.BizType = THOST_FTDC_BZTP_Future;
    strcpy(a.AccountID, g_chInvestorID);
    int b = m_pUserApi->ReqQrySecAgentTradingAccount(&a, 1);
    LOGDEMO((b == 0) ? "閺屻儴顕楁禍灞煎敩鐠у嫰鍣剧拹锔藉煕......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閺屻儴顕楁禍灞煎敩鐠у嫰鍣剧拹锔藉煕......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

///鐠囬攱鐪伴弻銉嚄娴滃瞼楠囨禒锝囨倞閸熷棜绁柌鎴炵墡妤犲本膩瀵拷1閿燂拷7
void CSimpleHandler::ReqQrySecAgentCheckMode()
{
    CThostFtdcQrySecAgentCheckModeField a = { 0 };
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.InvestorID, g_chInvestorID);
    int b = m_pUserApi->ReqQrySecAgentCheckMode(&a, 1);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄娴滃瞼楠囨禒锝囨倞閸熷棜绁柌鎴炵墡妤犲本膩瀵拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄娴滃瞼楠囨禒锝囨倞閸熷棜绁柌鎴炵墡妤犲本膩瀵拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

///濞夈劌鍞介悽銊﹀煕缂佸牏顏穱鈩冧紖閿涘瞼鏁ゆ禍搴濊厬缂佈勬箛閸斺�虫珤婢舵俺绻涢幒銉δ佸锟�1閿燂拷7
///闂囷拷1閿燂拷7鐟曚礁婀紒鍫㈩伂鐠併倛鐦夐幋鎰閸氬函绱濋悽銊﹀煕閻ц缍嶉崜宥堢殶閻€劏顕氶幒銉ュ經
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
    LOGDEMO("鐠囩柉绶崗顧痯閸︽澘娼�(娑撳秷绶崗銉ュ灟娑撹櫣鈹�)\n");
    getline(cin, ip);
    strcpy(a.ClientPublicIP, ip.c_str());*/
    strcpy(a.ClientPublicIP, "192.168.0.1");//ip閸︽澘娼�

    //int Port;
    //Port = 0;
    //cin.ignore();
    //LOGDEMO("鐠囩柉绶崗銉ь伂閸欙絽褰縗n");
    //cin >> Port;
    //a.ClientIPPort = Port;//缁旑垰褰涢崣锟�1閿燂拷7
    a.ClientIPPort = 51305;//缁旑垰褰涢崣锟�1閿燂拷7

    /*string LoginTime;
    LoginTime.clear();
    cin.ignore();
    LOGDEMO("鐠囩柉绶崗銉ф瑜版洘妞傞梻锟�1閿燂拷7(娑撳秷绶崗銉ュ灟娑撹櫣鈹�)\n");
    getline(cin, LoginTime);
    strcpy(a.ClientPublicIP, LoginTime.c_str());*/
    strcpy(a.ClientLoginTime, "20190121");
    strcpy(a.ClientAppID, g_chAppID);
    int b = m_pUserApi->RegisterUserSystemInfo(&a);
    LOGDEMO((b == 0) ? "濞夈劌鍞介悽銊﹀煕缂佸牏顏穱鈩冧紖......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "濞夈劌鍞介悽銊﹀煕缂佸牏顏穱鈩冧紖......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

///娑撳﹥濮ら悽銊﹀煕缂佸牏顏穱鈩冧紖閿涘瞼鏁ゆ禍搴濊厬缂佈勬箛閸斺�虫珤閹垮秳缍旈崨妯兼瑜版洘膩瀵拷1閿燂拷7
///閹垮秳缍旈崨妯兼瑜版洖鎮楅敍灞藉讲娴犮儱顦垮▎陇鐨熼悽銊嚉閹恒儱褰涙稉濠冨Г鐎广垺鍩涙穱鈩冧紖
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
    LOGDEMO("鐠囩柉绶崗顧痯閸︽澘娼�(娑撳秷绶崗銉ュ灟娑撹櫣鈹�)\n");
    getline(cin, ip);
    strcpy(a.ClientPublicIP, ip.c_str());*/
    strcpy(a.ClientPublicIP, "192.168.0.1");//ip閸︽澘娼�

    //int Port;
    //Port = 0;
    //cin.ignore();
    //LOGDEMO("鐠囩柉绶崗銉ь伂閸欙絽褰縗n");
    //cin >> Port;
    //a.ClientIPPort = Port;//缁旑垰褰涢崣锟�1閿燂拷7
    a.ClientIPPort = 51305;//缁旑垰褰涢崣锟�1閿燂拷7

    /*string LoginTime;
    LoginTime.clear();
    cin.ignore();
    LOGDEMO("鐠囩柉绶崗銉ф瑜版洘妞傞梻锟�1閿燂拷7(娑撳秷绶崗銉ュ灟娑撹櫣鈹�)\n");
    getline(cin, LoginTime);
    strcpy(a.ClientPublicIP, LoginTime.c_str());*/
    strcpy(a.ClientLoginTime, "20190121");
    strcpy(a.ClientAppID, g_chAppID);
    int b = m_pUserApi->SubmitUserSystemInfo(&a);
    LOGDEMO((b == 0) ? "濞夈劌鍞介悽銊﹀煕缂佸牏顏穱鈩冧紖......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "濞夈劌鍞介悽銊﹀煕缂佸牏顏穱鈩冧紖......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

///閺屻儴顕楅悽銊﹀煕瑜版挸澧犻弨顖涘瘮閻ㄥ嫯顓荤拠浣鼓佸锟�1閿燂拷7
void CSimpleHandler::ReqUserAuthMethod()
{
    CThostFtdcReqUserAuthMethodField a = { 0 };
    strcpy(a.TradingDay, "20190308");
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.UserID, g_chUserID);
    int b = m_pUserApi->ReqUserAuthMethod(&a, nRequestID++);
    LOGDEMO((b == 0) ? "閺屻儴顕楅悽銊﹀煕瑜版挸澧犻弨顖涘瘮閻ㄥ嫯顓荤拠浣鼓佸锟�1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閺屻儴顕楅悽銊﹀煕瑜版挸澧犻弨顖涘瘮閻ㄥ嫯顓荤拠浣鼓佸锟�1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

///閻€劍鍩涢崣鎴濆毉閼惧嘲褰囬崶鎯ц埌妤犲矁鐦夐惍浣筋嚞濮癸拷1閿燂拷7
void CSimpleHandler::ReqGenUserCaptcha()
{
    CThostFtdcReqGenUserCaptchaField a = { 0 };
    strcpy(a.TradingDay, "");
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.UserID, g_chUserID);
    int b = m_pUserApi->ReqGenUserCaptcha(&a, nRequestID++);
    LOGDEMO((b == 0) ? "閻€劍鍩涢崣鎴濆毉閼惧嘲褰囬崶鎯ц埌妤犲矁鐦夐惍浣筋嚞濮癸拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閻€劍鍩涢崣鎴濆毉閼惧嘲褰囬崶鎯ц埌妤犲矁鐦夐惍浣筋嚞濮癸拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

///閻€劍鍩涢崣鎴濆毉閼惧嘲褰囬惌顓濅繆妤犲矁鐦夐惍浣筋嚞濮癸拷1閿燂拷7
void CSimpleHandler::ReqGenUserText()
{
    CThostFtdcReqGenUserTextField a = { 0 };
    strcpy(a.TradingDay, "");
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.UserID, g_chUserID);
    int b = m_pUserApi->ReqGenUserText(&a, nRequestID++);
    LOGDEMO((b == 0) ? "閻€劍鍩涢崣鎴濆毉閼惧嘲褰囬惌顓濅繆妤犲矁鐦夐惍浣筋嚞濮癸拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閻€劍鍩涢崣鎴濆毉閼惧嘲褰囬惌顓濅繆妤犲矁鐦夐惍浣筋嚞濮癸拷1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

///閻€劍鍩涢崣鎴濆毉鐢附婀侀崶鍓у妤犲矁鐦夐惍浣烘畱閻у妾扮拠閿嬬湴
void CSimpleHandler::ReqUserLoginWithCaptcha()
{
    CThostFtdcReqUserLoginWithCaptchaField a = { 0 };
    strcpy(a.TradingDay, "");
    strcpy(a.BrokerID, g_chBrokerID);
    strcpy(a.UserID, g_chUserID);
    strcpy(a.Password, g_chPassword);
    strcpy(a.UserProductInfo, "");
    strcpy(a.InterfaceProductInfo, "");
    strcpy(a.ProtocolInfo, "");//閸楀繗顔呮穱鈩冧紖
    strcpy(a.MacAddress, "");//Mac閸︽澘娼�
    strcpy(a.ClientIPAddress, "");//缂佸牏顏琁P閸︽澘娼�
    strcpy(a.LoginRemark, "");//閻ц缍嶆稉璇差槵
    strcpy(a.Captcha, "");//閸ユ儳鑸版宀冪槈閻胶娈戦弬鍥х摟閸愬懎顔�
    a.ClientIPPort = 10203;
    int b = m_pUserApi->ReqUserLoginWithCaptcha(&a, nRequestID++);
    LOGDEMO((b == 0) ? "閻€劍鍩涢崣鎴濆毉鐢附婀侀崶鍓у妤犲矁鐦夐惍浣烘畱閻у妾扮拠閿嬬湴......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閻€劍鍩涢崣鎴濆毉鐢附婀侀崶鍓у妤犲矁鐦夐惍浣烘畱閻у妾扮拠閿嬬湴......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

///閻€劍鍩涢崣鎴濆毉鐢附婀侀惌顓濅繆妤犲矁鐦夐惍浣烘畱閻у妾扮拠閿嬬湴
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
    LOGDEMO((b == 0) ? "閻€劍鍩涢崣鎴濆毉鐢附婀侀惌顓濅繆妤犲矁鐦夐惍浣烘畱閻у妾扮拠閿嬬湴......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" :
        "閻€劍鍩涢崣鎴濆毉鐢附婀侀惌顓濅繆妤犲矁鐦夐惍浣烘畱閻у妾扮拠閿嬬湴......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

///閻€劍鍩涢崣鎴濆毉鐢附婀侀崝顭掓嫹1閿燂拷7閿熻棄褰涙禒銈囨畱閻у妾扮拠閿嬬湴
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
    LOGDEMO((b == 0) ? "閻€劍鍩涢崣鎴濆毉鐢附婀侀崝顭掓嫹1閿燂拷7閿熻棄褰涙禒銈囨畱閻у妾扮拠閿嬬湴......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "閻€劍鍩涢崣鎴濆毉鐢附婀侀崝顭掓嫹1閿燂拷7閿熻棄褰涙禒銈囨畱閻у妾扮拠閿嬬湴......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

///鐠囬攱鐪伴弻銉嚄娴滃瞼楠囨禒锝囨倞閸熷棔淇婇幁锟�1閿燂拷7
void CSimpleHandler::ReqQrySecAgentTradeInfo()
{
    CThostFtdcQrySecAgentTradeInfoField a = { 0 };
    strcpy(a.BrokerID, "");
    strcpy(a.BrokerSecAgentID, "");
    int b = m_pUserApi->ReqQrySecAgentTradeInfo(&a, nRequestID++);
    LOGDEMO((b == 0) ? "鐠囬攱鐪伴弻銉嚄娴滃瞼楠囨禒锝囨倞閸熷棔淇婇幁锟�1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻姤鍨氶崝鐒卬" : "鐠囬攱鐪伴弻銉嚄娴滃瞼楠囨禒锝囨倞閸熷棔淇婇幁锟�1閿燂拷7......閸欐埊鎷�1閿燂拷7閿熻棄銇戠拹銉礉闁挎瑨顕ゆ惔蹇撳娇=[%d]\n", b);
}

