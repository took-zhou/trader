//#include "stdafx.h"
#include "traderApi.h"
#include <stdio.h>
#include "define.h"
#include "log.h"


CThostFtdcTraderApi* CTraderApi::CreateFtdcTraderApi(const char *pszFlowPath /*= ""*/)
{
	return m_pApi = CThostFtdcTraderApi::CreateFtdcTraderApi(pszFlowPath);
}

const char * CTraderApi::GetApiVersion()
{
	return m_pApi->GetApiVersion();
}

//////////////////////////////////////////////////////////////////////////以下可替换模板

void CTraderApi::Release()
{
	INFO_LOG("<Release>\n");
	INFO_LOG("</Release>\n");

	m_pApi->Release();
};

void CTraderApi::Init()
{

	m_pApi->Init();
    INFO_LOG("Init trader");
};

int CTraderApi::Join()
{
	INFO_LOG("<Join>\n");
	INFO_LOG("</Join>\n");

	return m_pApi->Join();
};

const char *CTraderApi::GetTradingDay()
{
//	INFO_LOG("<GetTradingDay>\n");
//	INFO_LOG("</GetTradingDay>\n");
    return m_pApi->GetTradingDay();
};

void CTraderApi::RegisterFront(char *pszFrontAddress)
{
//	INFO_LOG("<RegisterFront>\n");
//	INFO_LOG("\tpszFrontAddress [%s]\n", pszFrontAddress);
//	INFO_LOG("</RegisterFront>\n");
    INFO_LOG("RegisterFront");
    m_pApi->RegisterFront(pszFrontAddress);
}

void CTraderApi::RegisterNameServer(char *pszNsAddress)
{
	INFO_LOG("<RegisterNameServer>\n");
	INFO_LOG("\tpszNsAddress [%s]\n", pszNsAddress);
	INFO_LOG("</RegisterNameServer>\n");

	m_pApi->RegisterNameServer(pszNsAddress);
}

void CTraderApi::RegisterFensUserInfo(CThostFtdcFensUserInfoField * pFensUserInfo)
{
	INFO_LOG("<RegisterFensUserInfo>\n");
	if (pFensUserInfo)
	{
		INFO_LOG("\tBrokerID [%s]\n", pFensUserInfo->BrokerID);
		INFO_LOG("\tUserID [%s]\n", pFensUserInfo->UserID);
		INFO_LOG("\tLoginMode [%c]\n", pFensUserInfo->LoginMode);
	}
	INFO_LOG("</RegisterFensUserInfo>\n");

	m_pApi->RegisterFensUserInfo(pFensUserInfo);
};

void CTraderApi::RegisterSpi(CThostFtdcTraderSpi *pSpi)
{
	if (pSpi)
	{
	}	

	m_pApi->RegisterSpi(pSpi);
    INFO_LOG("RegisterSpi ok");
};

void CTraderApi::SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType)
{
//	INFO_LOG("<SubscribePrivateTopic>\n");
//	INFO_LOG("\tnResumeType [%d]\n", nResumeType);
//	INFO_LOG("</SubscribePrivateTopic>\n");
    INFO_LOG("SubscribePrivateTopic");
	m_pApi->SubscribePrivateTopic(nResumeType);
}

void CTraderApi::SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType)
{
//	INFO_LOG("<SubscribePublicTopic>\n");
//	INFO_LOG("\tnResumeType [%d]\n", nResumeType);
//	INFO_LOG("</SubscribePublicTopic>\n");
    INFO_LOG("SubscribePublicTopic");
	m_pApi->SubscribePublicTopic(nResumeType);
}

int CTraderApi::ReqAuthenticate(CThostFtdcReqAuthenticateField *pReqAuthenticateField, int nRequestID)
{
    MSG_LOG("%s","<ReqAuthenticate>");
	if (pReqAuthenticateField)
	{
        PURE_LOG("\tBrokerID [%s]", pReqAuthenticateField->BrokerID);
        PURE_LOG("\tUserID [%s]", pReqAuthenticateField->UserID);
        PURE_LOG("\tUserProductInfo [%s]", pReqAuthenticateField->UserProductInfo);
        PURE_LOG("\tAuthCode [%s]", pReqAuthenticateField->AuthCode);
        PURE_LOG("\tAppID [%s]", pReqAuthenticateField->AppID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    MSG_LOG("%s","</ReqAuthenticate>\n");

    int result_n = m_pApi->ReqAuthenticate(pReqAuthenticateField, nRequestID);
    return result_n;
}


int CTraderApi::ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID)
{
//	INFO_LOG("<ReqUserLogin>\n");
//	if (pReqUserLoginField)
//	{
//		INFO_LOG("\tTradingDay [%s]\n", pReqUserLoginField->TradingDay);
//		INFO_LOG("\tBrokerID [%s]\n", pReqUserLoginField->BrokerID);
//		INFO_LOG("\tUserID [%s]\n", pReqUserLoginField->UserID);
//		INFO_LOG("\tPassword [%s]\n", pReqUserLoginField->Password);
//		INFO_LOG("\tUserProductInfo [%s]\n", pReqUserLoginField->UserProductInfo);
//		INFO_LOG("\tInterfaceProductInfo [%s]\n", pReqUserLoginField->InterfaceProductInfo);
//		INFO_LOG("\tProtocolInfo [%s]\n", pReqUserLoginField->ProtocolInfo);
//		INFO_LOG("\tMacAddress [%s]\n", pReqUserLoginField->MacAddress);
//		INFO_LOG("\tOneTimePassword [%s]\n", pReqUserLoginField->OneTimePassword);
//		INFO_LOG("\tClientIPAddress [%s]\n", pReqUserLoginField->ClientIPAddress);
//		INFO_LOG("\tLoginRemark [%s]\n", pReqUserLoginField->LoginRemark);
//	}
//	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
//	INFO_LOG("</ReqUserLogin>\n");
    INFO_LOG("begin to login exchange......"); // @suppress("Invalid arguments")
	return m_pApi->ReqUserLogin(pReqUserLoginField, nRequestID);
};


int CTraderApi::ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID)
{
	INFO_LOG("<ReqUserLogout>\n");
	if (pUserLogout)
	{
		INFO_LOG("\tBrokerID [%s]\n", pUserLogout->BrokerID);
		INFO_LOG("\tUserID [%s]\n", pUserLogout->UserID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqUserLogout>\n");

	return m_pApi->ReqUserLogout(pUserLogout, nRequestID);
};


int CTraderApi::ReqUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID)
{
	INFO_LOG("<ReqUserPasswordUpdate>\n");
	if (pUserPasswordUpdate)
	{
		INFO_LOG("\tBrokerID [%s]\n", pUserPasswordUpdate->BrokerID);
		INFO_LOG("\tUserID [%s]\n", pUserPasswordUpdate->UserID);
		INFO_LOG("\tOldPassword [%s]\n", pUserPasswordUpdate->OldPassword);
		INFO_LOG("\tNewPassword [%s]\n", pUserPasswordUpdate->NewPassword);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqUserPasswordUpdate>\n");

	return m_pApi->ReqUserPasswordUpdate(pUserPasswordUpdate, nRequestID);
};


int CTraderApi::ReqTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, int nRequestID)
{
	INFO_LOG("<ReqTradingAccountPasswordUpdate>\n");
	if (pTradingAccountPasswordUpdate)
	{
		INFO_LOG("\tBrokerID [%s]\n", pTradingAccountPasswordUpdate->BrokerID);
		INFO_LOG("\tAccountID [%s]\n", pTradingAccountPasswordUpdate->AccountID);
		INFO_LOG("\tOldPassword [%s]\n", pTradingAccountPasswordUpdate->OldPassword);
		INFO_LOG("\tNewPassword [%s]\n", pTradingAccountPasswordUpdate->NewPassword);
		INFO_LOG("\tCurrencyID [%s]\n", pTradingAccountPasswordUpdate->CurrencyID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqTradingAccountPasswordUpdate>\n");

	return m_pApi->ReqTradingAccountPasswordUpdate(pTradingAccountPasswordUpdate, nRequestID);
};


//int CTraderApi::ReqUserLogin2(CThostFtdcReqUserLoginField *pReqUserLogin, int nRequestID)
//{
//	INFO_LOG("<ReqUserLogin2>\n");
//	if (pReqUserLogin)
//	{
//		INFO_LOG("\tTradingDay [%s]\n", pReqUserLogin->TradingDay);
//		INFO_LOG("\tBrokerID [%s]\n", pReqUserLogin->BrokerID);
//		INFO_LOG("\tUserID [%s]\n", pReqUserLogin->UserID);
//		INFO_LOG("\tPassword [%s]\n", pReqUserLogin->Password);
//		INFO_LOG("\tUserProductInfo [%s]\n", pReqUserLogin->UserProductInfo);
//		INFO_LOG("\tInterfaceProductInfo [%s]\n", pReqUserLogin->InterfaceProductInfo);
//		INFO_LOG("\tProtocolInfo [%s]\n", pReqUserLogin->ProtocolInfo);
//		INFO_LOG("\tMacAddress [%s]\n", pReqUserLogin->MacAddress);
//		INFO_LOG("\tOneTimePassword [%s]\n", pReqUserLogin->OneTimePassword);
//		INFO_LOG("\tClientIPAddress [%s]\n", pReqUserLogin->ClientIPAddress);
//		INFO_LOG("\tLoginRemark [%s]\n", pReqUserLogin->LoginRemark);
//	}
//	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
//	INFO_LOG("</ReqUserLogin2>\n");
//
//	return m_pApi->ReqUserLogin2(pReqUserLogin, nRequestID);
//};


//int CTraderApi::ReqUserPasswordUpdate2(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID)
//{
//	INFO_LOG("<ReqUserPasswordUpdate2>\n");
//	if (pUserPasswordUpdate)
//	{
//		INFO_LOG("\tBrokerID [%s]\n", pUserPasswordUpdate->BrokerID);
//		INFO_LOG("\tUserID [%s]\n", pUserPasswordUpdate->UserID);
//		INFO_LOG("\tOldPassword [%s]\n", pUserPasswordUpdate->OldPassword);
//		INFO_LOG("\tNewPassword [%s]\n", pUserPasswordUpdate->NewPassword);
//	}
//	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
//	INFO_LOG("</ReqUserPasswordUpdate2>\n");
//
//	return m_pApi->ReqUserPasswordUpdate2(pUserPasswordUpdate, nRequestID);
//};


int CTraderApi::ReqOrderInsert(CThostFtdcInputOrderField *pInputOrder, int nRequestID)
{
	/*INFO_LOG("<ReqOrderInsert>\n");
	if (pInputOrder)
	{
		INFO_LOG("\tBrokerID [%s]\n", pInputOrder->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pInputOrder->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pInputOrder->InstrumentID);
		INFO_LOG("\tOrderRef [%s]\n", pInputOrder->OrderRef);
		INFO_LOG("\tUserID [%s]\n", pInputOrder->UserID);
		INFO_LOG("\tCombOffsetFlag [%s]\n", pInputOrder->CombOffsetFlag);
		INFO_LOG("\tCombHedgeFlag [%s]\n", pInputOrder->CombHedgeFlag);
		INFO_LOG("\tGTDDate [%s]\n", pInputOrder->GTDDate);
		INFO_LOG("\tBusinessUnit [%s]\n", pInputOrder->BusinessUnit);
		INFO_LOG("\tExchangeID [%s]\n", pInputOrder->ExchangeID);
		INFO_LOG("\tInvestUnitID [%s]\n", pInputOrder->InvestUnitID);
		INFO_LOG("\tAccountID [%s]\n", pInputOrder->AccountID);
		INFO_LOG("\tCurrencyID [%s]\n", pInputOrder->CurrencyID);
		INFO_LOG("\tClientID [%s]\n", pInputOrder->ClientID);
		INFO_LOG("\tIPAddress [%s]\n", pInputOrder->IPAddress);
		INFO_LOG("\tMacAddress [%s]\n", pInputOrder->MacAddress);
		INFO_LOG("\tVolumeTotalOriginal [%d]\n", pInputOrder->VolumeTotalOriginal);
		INFO_LOG("\tMinVolume [%d]\n", pInputOrder->MinVolume);
		INFO_LOG("\tIsAutoSuspend [%d]\n", pInputOrder->IsAutoSuspend);
		INFO_LOG("\tRequestID [%d]\n", pInputOrder->RequestID);
		INFO_LOG("\tUserForceClose [%d]\n", pInputOrder->UserForceClose);
		INFO_LOG("\tIsSwapOrder [%d]\n", pInputOrder->IsSwapOrder);
		INFO_LOG("\tOrderPriceType [%c]\n", pInputOrder->OrderPriceType);
		INFO_LOG("\tDirection [%c]\n", pInputOrder->Direction);
		INFO_LOG("\tTimeCondition [%c]\n", pInputOrder->TimeCondition);
		INFO_LOG("\tVolumeCondition [%c]\n", pInputOrder->VolumeCondition);
		INFO_LOG("\tContingentCondition [%c]\n", pInputOrder->ContingentCondition);
		INFO_LOG("\tForceCloseReason [%c]\n", pInputOrder->ForceCloseReason);
		INFO_LOG("\tLimitPrice [%.8lf]\n", pInputOrder->LimitPrice);
		INFO_LOG("\tStopPrice [%.8lf]\n", pInputOrder->StopPrice);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqOrderInsert>\n");*/
	
	INFO_LOG("begin to request order insert");
	return m_pApi->ReqOrderInsert(pInputOrder, nRequestID);
};


int CTraderApi::ReqParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, int nRequestID)
{
	INFO_LOG("<ReqParkedOrderInsert>\n");
	if (pParkedOrder)
	{
		INFO_LOG("\tBrokerID [%s]\n", pParkedOrder->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pParkedOrder->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pParkedOrder->InstrumentID);
		INFO_LOG("\tOrderRef [%s]\n", pParkedOrder->OrderRef);
		INFO_LOG("\tUserID [%s]\n", pParkedOrder->UserID);
		INFO_LOG("\tCombOffsetFlag [%s]\n", pParkedOrder->CombOffsetFlag);
		INFO_LOG("\tCombHedgeFlag [%s]\n", pParkedOrder->CombHedgeFlag);
		INFO_LOG("\tGTDDate [%s]\n", pParkedOrder->GTDDate);
		INFO_LOG("\tBusinessUnit [%s]\n", pParkedOrder->BusinessUnit);
		INFO_LOG("\tExchangeID [%s]\n", pParkedOrder->ExchangeID);
		INFO_LOG("\tParkedOrderID [%s]\n", pParkedOrder->ParkedOrderID);
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pParkedOrder->ErrorMsg, msg, sizeof(msg));
		INFO_LOG("\tErrorMsg [%s]\n", msg);
		INFO_LOG("\tAccountID [%s]\n", pParkedOrder->AccountID);
		INFO_LOG("\tCurrencyID [%s]\n", pParkedOrder->CurrencyID);
		INFO_LOG("\tClientID [%s]\n", pParkedOrder->ClientID);
		INFO_LOG("\tInvestUnitID [%s]\n", pParkedOrder->InvestUnitID);
		INFO_LOG("\tIPAddress [%s]\n", pParkedOrder->IPAddress);
		INFO_LOG("\tMacAddress [%s]\n", pParkedOrder->MacAddress);
		INFO_LOG("\tVolumeTotalOriginal [%d]\n", pParkedOrder->VolumeTotalOriginal);
		INFO_LOG("\tMinVolume [%d]\n", pParkedOrder->MinVolume);
		INFO_LOG("\tIsAutoSuspend [%d]\n", pParkedOrder->IsAutoSuspend);
		INFO_LOG("\tRequestID [%d]\n", pParkedOrder->RequestID);
		INFO_LOG("\tUserForceClose [%d]\n", pParkedOrder->UserForceClose);
		INFO_LOG("\tErrorID [%d]\n", pParkedOrder->ErrorID);
		INFO_LOG("\tIsSwapOrder [%d]\n", pParkedOrder->IsSwapOrder);
		INFO_LOG("\tOrderPriceType [%c]\n", pParkedOrder->OrderPriceType);
		INFO_LOG("\tDirection [%c]\n", pParkedOrder->Direction);
		INFO_LOG("\tTimeCondition [%c]\n", pParkedOrder->TimeCondition);
		INFO_LOG("\tVolumeCondition [%c]\n", pParkedOrder->VolumeCondition);
		INFO_LOG("\tContingentCondition [%c]\n", pParkedOrder->ContingentCondition);
		INFO_LOG("\tForceCloseReason [%c]\n", pParkedOrder->ForceCloseReason);
		INFO_LOG("\tUserType [%c]\n", pParkedOrder->UserType);
		INFO_LOG("\tStatus [%c]\n", pParkedOrder->Status);
		INFO_LOG("\tLimitPrice [%.8lf]\n", pParkedOrder->LimitPrice);
		INFO_LOG("\tStopPrice [%.8lf]\n", pParkedOrder->StopPrice);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqParkedOrderInsert>\n");

	return m_pApi->ReqParkedOrderInsert(pParkedOrder, nRequestID);
};


int CTraderApi::ReqParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, int nRequestID)
{
	INFO_LOG("<ReqParkedOrderAction>\n");
	if (pParkedOrderAction)
	{
		INFO_LOG("\tBrokerID [%s]\n", pParkedOrderAction->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pParkedOrderAction->InvestorID);
		INFO_LOG("\tOrderRef [%s]\n", pParkedOrderAction->OrderRef);
		INFO_LOG("\tExchangeID [%s]\n", pParkedOrderAction->ExchangeID);
		INFO_LOG("\tOrderSysID [%s]\n", pParkedOrderAction->OrderSysID);
		INFO_LOG("\tUserID [%s]\n", pParkedOrderAction->UserID);
		INFO_LOG("\tInstrumentID [%s]\n", pParkedOrderAction->InstrumentID);
		INFO_LOG("\tParkedOrderActionID [%s]\n", pParkedOrderAction->ParkedOrderActionID);

        TThostFtdcErrorMsgType msg;
        gbk2utf8(pParkedOrderAction->ErrorMsg, msg, sizeof(msg));
		INFO_LOG("\tErrorMsg [%s]\n", msg);
		INFO_LOG("\tInvestUnitID [%s]\n", pParkedOrderAction->InvestUnitID);
		INFO_LOG("\tIPAddress [%s]\n", pParkedOrderAction->IPAddress);
		INFO_LOG("\tMacAddress [%s]\n", pParkedOrderAction->MacAddress);
		INFO_LOG("\tOrderActionRef [%d]\n", pParkedOrderAction->OrderActionRef);
		INFO_LOG("\tRequestID [%d]\n", pParkedOrderAction->RequestID);
		INFO_LOG("\tFrontID [%d]\n", pParkedOrderAction->FrontID);
		INFO_LOG("\tSessionID [%d]\n", pParkedOrderAction->SessionID);
		INFO_LOG("\tVolumeChange [%d]\n", pParkedOrderAction->VolumeChange);
		INFO_LOG("\tErrorID [%d]\n", pParkedOrderAction->ErrorID);
		INFO_LOG("\tActionFlag [%c]\n", pParkedOrderAction->ActionFlag);
		INFO_LOG("\tUserType [%c]\n", pParkedOrderAction->UserType);
		INFO_LOG("\tStatus [%c]\n", pParkedOrderAction->Status);
		INFO_LOG("\tLimitPrice [%.8lf]\n", pParkedOrderAction->LimitPrice);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqParkedOrderAction>\n");

	return m_pApi->ReqParkedOrderAction(pParkedOrderAction, nRequestID);
};


int CTraderApi::ReqOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, int nRequestID)
{
	INFO_LOG("<ReqOrderAction>\n");
	if (pInputOrderAction)
	{
		INFO_LOG("\tBrokerID [%s]\n", pInputOrderAction->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pInputOrderAction->InvestorID);
		INFO_LOG("\tOrderRef [%s]\n", pInputOrderAction->OrderRef);
		INFO_LOG("\tExchangeID [%s]\n", pInputOrderAction->ExchangeID);
		INFO_LOG("\tOrderSysID [%s]\n", pInputOrderAction->OrderSysID);
		INFO_LOG("\tUserID [%s]\n", pInputOrderAction->UserID);
		INFO_LOG("\tInstrumentID [%s]\n", pInputOrderAction->InstrumentID);
		INFO_LOG("\tInvestUnitID [%s]\n", pInputOrderAction->InvestUnitID);
		INFO_LOG("\tIPAddress [%s]\n", pInputOrderAction->IPAddress);
		INFO_LOG("\tMacAddress [%s]\n", pInputOrderAction->MacAddress);
		INFO_LOG("\tOrderActionRef [%d]\n", pInputOrderAction->OrderActionRef);
		INFO_LOG("\tRequestID [%d]\n", pInputOrderAction->RequestID);
		INFO_LOG("\tFrontID [%d]\n", pInputOrderAction->FrontID);
		INFO_LOG("\tSessionID [%d]\n", pInputOrderAction->SessionID);
		INFO_LOG("\tVolumeChange [%d]\n", pInputOrderAction->VolumeChange);
		INFO_LOG("\tActionFlag [%c]\n", pInputOrderAction->ActionFlag);
		INFO_LOG("\tLimitPrice [%.8lf]\n", pInputOrderAction->LimitPrice);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqOrderAction>\n");

	return m_pApi->ReqOrderAction(pInputOrderAction, nRequestID);
};


int CTraderApi::ReqQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, int nRequestID)
{
	INFO_LOG("<ReqQueryMaxOrderVolume>\n");
	if (pQueryMaxOrderVolume)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQueryMaxOrderVolume->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQueryMaxOrderVolume->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pQueryMaxOrderVolume->InstrumentID);
		INFO_LOG("\tExchangeID [%s]\n", pQueryMaxOrderVolume->ExchangeID);
		INFO_LOG("\tInvestUnitID [%s]\n", pQueryMaxOrderVolume->InvestUnitID);
		INFO_LOG("\tMaxVolume [%d]\n", pQueryMaxOrderVolume->MaxVolume);
		INFO_LOG("\tDirection [%c]\n", pQueryMaxOrderVolume->Direction);
		INFO_LOG("\tOffsetFlag [%c]\n", pQueryMaxOrderVolume->OffsetFlag);
		INFO_LOG("\tHedgeFlag [%c]\n", pQueryMaxOrderVolume->HedgeFlag);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQueryMaxOrderVolume>\n");

	return m_pApi->ReqQueryMaxOrderVolume(pQueryMaxOrderVolume, nRequestID);
};


int CTraderApi::ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, int nRequestID)
{
	INFO_LOG("<ReqSettlementInfoConfirm>\n");
	if (pSettlementInfoConfirm)
	{
		INFO_LOG("\tBrokerID [%s]\n", pSettlementInfoConfirm->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pSettlementInfoConfirm->InvestorID);
		INFO_LOG("\tConfirmDate [%s]\n", pSettlementInfoConfirm->ConfirmDate);
		INFO_LOG("\tConfirmTime [%s]\n", pSettlementInfoConfirm->ConfirmTime);
		INFO_LOG("\tAccountID [%s]\n", pSettlementInfoConfirm->AccountID);
		INFO_LOG("\tCurrencyID [%s]\n", pSettlementInfoConfirm->CurrencyID);
		INFO_LOG("\tSettlementID [%d]\n", pSettlementInfoConfirm->SettlementID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqSettlementInfoConfirm>\n");

	return m_pApi->ReqSettlementInfoConfirm(pSettlementInfoConfirm, nRequestID);
};


int CTraderApi::ReqRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, int nRequestID)
{
	INFO_LOG("<ReqRemoveParkedOrder>\n");
	if (pRemoveParkedOrder)
	{
		INFO_LOG("\tBrokerID [%s]\n", pRemoveParkedOrder->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pRemoveParkedOrder->InvestorID);
		INFO_LOG("\tParkedOrderID [%s]\n", pRemoveParkedOrder->ParkedOrderID);
		INFO_LOG("\tInvestUnitID [%s]\n", pRemoveParkedOrder->InvestUnitID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqRemoveParkedOrder>\n");

	return m_pApi->ReqRemoveParkedOrder(pRemoveParkedOrder, nRequestID);
};


int CTraderApi::ReqRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, int nRequestID)
{
	INFO_LOG("<ReqRemoveParkedOrderAction>\n");
	if (pRemoveParkedOrderAction)
	{
		INFO_LOG("\tBrokerID [%s]\n", pRemoveParkedOrderAction->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pRemoveParkedOrderAction->InvestorID);
		INFO_LOG("\tParkedOrderActionID [%s]\n", pRemoveParkedOrderAction->ParkedOrderActionID);
		INFO_LOG("\tInvestUnitID [%s]\n", pRemoveParkedOrderAction->InvestUnitID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqRemoveParkedOrderAction>\n");

	return m_pApi->ReqRemoveParkedOrderAction(pRemoveParkedOrderAction, nRequestID);
};


int CTraderApi::ReqExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, int nRequestID)
{
	INFO_LOG("<ReqExecOrderInsert>\n");
	if (pInputExecOrder)
	{
		INFO_LOG("\tBrokerID [%s]\n", pInputExecOrder->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pInputExecOrder->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pInputExecOrder->InstrumentID);
		INFO_LOG("\tExecOrderRef [%s]\n", pInputExecOrder->ExecOrderRef);
		INFO_LOG("\tUserID [%s]\n", pInputExecOrder->UserID);
		INFO_LOG("\tBusinessUnit [%s]\n", pInputExecOrder->BusinessUnit);
		INFO_LOG("\tExchangeID [%s]\n", pInputExecOrder->ExchangeID);
		INFO_LOG("\tInvestUnitID [%s]\n", pInputExecOrder->InvestUnitID);
		INFO_LOG("\tAccountID [%s]\n", pInputExecOrder->AccountID);
		INFO_LOG("\tCurrencyID [%s]\n", pInputExecOrder->CurrencyID);
		INFO_LOG("\tClientID [%s]\n", pInputExecOrder->ClientID);
		INFO_LOG("\tIPAddress [%s]\n", pInputExecOrder->IPAddress);
		INFO_LOG("\tMacAddress [%s]\n", pInputExecOrder->MacAddress);
		INFO_LOG("\tVolume [%d]\n", pInputExecOrder->Volume);
		INFO_LOG("\tRequestID [%d]\n", pInputExecOrder->RequestID);
		INFO_LOG("\tOffsetFlag [%c]\n", pInputExecOrder->OffsetFlag);
		INFO_LOG("\tHedgeFlag [%c]\n", pInputExecOrder->HedgeFlag);
		INFO_LOG("\tActionType [%c]\n", pInputExecOrder->ActionType);
		INFO_LOG("\tPosiDirection [%c]\n", pInputExecOrder->PosiDirection);
		INFO_LOG("\tReservePositionFlag [%c]\n", pInputExecOrder->ReservePositionFlag);
		INFO_LOG("\tCloseFlag [%c]\n", pInputExecOrder->CloseFlag);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqExecOrderInsert>\n");

	return m_pApi->ReqExecOrderInsert(pInputExecOrder, nRequestID);
};


int CTraderApi::ReqExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, int nRequestID)
{
	INFO_LOG("<ReqExecOrderAction>\n");
	if (pInputExecOrderAction)
	{
		INFO_LOG("\tBrokerID [%s]\n", pInputExecOrderAction->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pInputExecOrderAction->InvestorID);
		INFO_LOG("\tExecOrderRef [%s]\n", pInputExecOrderAction->ExecOrderRef);
		INFO_LOG("\tExchangeID [%s]\n", pInputExecOrderAction->ExchangeID);
		INFO_LOG("\tExecOrderSysID [%s]\n", pInputExecOrderAction->ExecOrderSysID);
		INFO_LOG("\tUserID [%s]\n", pInputExecOrderAction->UserID);
		INFO_LOG("\tInstrumentID [%s]\n", pInputExecOrderAction->InstrumentID);
		INFO_LOG("\tInvestUnitID [%s]\n", pInputExecOrderAction->InvestUnitID);
		INFO_LOG("\tIPAddress [%s]\n", pInputExecOrderAction->IPAddress);
		INFO_LOG("\tMacAddress [%s]\n", pInputExecOrderAction->MacAddress);
		INFO_LOG("\tExecOrderActionRef [%d]\n", pInputExecOrderAction->ExecOrderActionRef);
		INFO_LOG("\tRequestID [%d]\n", pInputExecOrderAction->RequestID);
		INFO_LOG("\tFrontID [%d]\n", pInputExecOrderAction->FrontID);
		INFO_LOG("\tSessionID [%d]\n", pInputExecOrderAction->SessionID);
		INFO_LOG("\tActionFlag [%c]\n", pInputExecOrderAction->ActionFlag);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqExecOrderAction>\n");

	return m_pApi->ReqExecOrderAction(pInputExecOrderAction, nRequestID);
};


int CTraderApi::ReqForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, int nRequestID)
{
	/*INFO_LOG("<ReqForQuoteInsert>\n");
	if (pInputForQuote)
	{
		INFO_LOG("\tBrokerID [%s]\n", pInputForQuote->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pInputForQuote->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pInputForQuote->InstrumentID);
		INFO_LOG("\tForQuoteRef [%s]\n", pInputForQuote->ForQuoteRef);
		INFO_LOG("\tUserID [%s]\n", pInputForQuote->UserID);
		INFO_LOG("\tExchangeID [%s]\n", pInputForQuote->ExchangeID);
		INFO_LOG("\tInvestUnitID [%s]\n", pInputForQuote->InvestUnitID);
		INFO_LOG("\tIPAddress [%s]\n", pInputForQuote->IPAddress);
		INFO_LOG("\tMacAddress [%s]\n", pInputForQuote->MacAddress);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqForQuoteInsert>\n");

	return m_pApi->ReqForQuoteInsert(pInputForQuote, nRequestID);*/
	INFO_LOG("<ReqForQuoteInsert>\n"
		"\tBrokerID [%s]\n"
		"\tInvestorID [%s]\n"
		"\tInstrumentID [%s]\n"
		"\tForQuoteRef [%s]\n"
		"\tUserID [%s]\n"
		"\tExchangeID [%s]\n"
		"\tInvestUnitID [%s]\n"
		"\tIPAddress [%s]\n"
		"\tMacAddress [%s]\n"
		"\tnRequestID [%d]\n"
		"</ReqForQuoteInsert>\n",
		pInputForQuote->BrokerID, 
		pInputForQuote->InvestorID,
		pInputForQuote->InstrumentID,
		pInputForQuote->InstrumentID,
		pInputForQuote->UserID,
		pInputForQuote->ExchangeID,
		pInputForQuote->InvestUnitID,
		pInputForQuote->IPAddress,
		pInputForQuote->MacAddress,
		nRequestID);

	return m_pApi->ReqForQuoteInsert(pInputForQuote, nRequestID);
};


int CTraderApi::ReqQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, int nRequestID)
{
	INFO_LOG("<ReqQuoteInsert>\n");
	if (pInputQuote)
	{
		INFO_LOG("\tBrokerID [%s]\n", pInputQuote->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pInputQuote->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pInputQuote->InstrumentID);
		INFO_LOG("\tQuoteRef [%s]\n", pInputQuote->QuoteRef);
		INFO_LOG("\tUserID [%s]\n", pInputQuote->UserID);
		INFO_LOG("\tBusinessUnit [%s]\n", pInputQuote->BusinessUnit);
		INFO_LOG("\tAskOrderRef [%s]\n", pInputQuote->AskOrderRef);
		INFO_LOG("\tBidOrderRef [%s]\n", pInputQuote->BidOrderRef);
		INFO_LOG("\tForQuoteSysID [%s]\n", pInputQuote->ForQuoteSysID);
		INFO_LOG("\tExchangeID [%s]\n", pInputQuote->ExchangeID);
		INFO_LOG("\tInvestUnitID [%s]\n", pInputQuote->InvestUnitID);
		INFO_LOG("\tClientID [%s]\n", pInputQuote->ClientID);
		INFO_LOG("\tIPAddress [%s]\n", pInputQuote->IPAddress);
		INFO_LOG("\tMacAddress [%s]\n", pInputQuote->MacAddress);
		INFO_LOG("\tAskVolume [%d]\n", pInputQuote->AskVolume);
		INFO_LOG("\tBidVolume [%d]\n", pInputQuote->BidVolume);
		INFO_LOG("\tRequestID [%d]\n", pInputQuote->RequestID);
		INFO_LOG("\tAskOffsetFlag [%c]\n", pInputQuote->AskOffsetFlag);
		INFO_LOG("\tBidOffsetFlag [%c]\n", pInputQuote->BidOffsetFlag);
		INFO_LOG("\tAskHedgeFlag [%c]\n", pInputQuote->AskHedgeFlag);
		INFO_LOG("\tBidHedgeFlag [%c]\n", pInputQuote->BidHedgeFlag);
		INFO_LOG("\tAskPrice [%.8lf]\n", pInputQuote->AskPrice);
		INFO_LOG("\tBidPrice [%.8lf]\n", pInputQuote->BidPrice);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQuoteInsert>\n");

	return m_pApi->ReqQuoteInsert(pInputQuote, nRequestID);
};


int CTraderApi::ReqQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, int nRequestID)
{
	INFO_LOG("<ReqQuoteAction>\n");
	if (pInputQuoteAction)
	{
		INFO_LOG("\tBrokerID [%s]\n", pInputQuoteAction->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pInputQuoteAction->InvestorID);
		INFO_LOG("\tQuoteRef [%s]\n", pInputQuoteAction->QuoteRef);
		INFO_LOG("\tExchangeID [%s]\n", pInputQuoteAction->ExchangeID);
		INFO_LOG("\tQuoteSysID [%s]\n", pInputQuoteAction->QuoteSysID);
		INFO_LOG("\tUserID [%s]\n", pInputQuoteAction->UserID);
		INFO_LOG("\tInstrumentID [%s]\n", pInputQuoteAction->InstrumentID);
		INFO_LOG("\tInvestUnitID [%s]\n", pInputQuoteAction->InvestUnitID);
		INFO_LOG("\tClientID [%s]\n", pInputQuoteAction->ClientID);
		INFO_LOG("\tIPAddress [%s]\n", pInputQuoteAction->IPAddress);
		INFO_LOG("\tMacAddress [%s]\n", pInputQuoteAction->MacAddress);
		INFO_LOG("\tQuoteActionRef [%d]\n", pInputQuoteAction->QuoteActionRef);
		INFO_LOG("\tRequestID [%d]\n", pInputQuoteAction->RequestID);
		INFO_LOG("\tFrontID [%d]\n", pInputQuoteAction->FrontID);
		INFO_LOG("\tSessionID [%d]\n", pInputQuoteAction->SessionID);
		INFO_LOG("\tActionFlag [%c]\n", pInputQuoteAction->ActionFlag);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQuoteAction>\n");

	return m_pApi->ReqQuoteAction(pInputQuoteAction, nRequestID);
};


int CTraderApi::ReqBatchOrderAction(CThostFtdcInputBatchOrderActionField *pInputBatchOrderAction, int nRequestID)
{
	INFO_LOG("<ReqBatchOrderAction>\n");
	if (pInputBatchOrderAction)
	{
		INFO_LOG("\tBrokerID [%s]\n", pInputBatchOrderAction->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pInputBatchOrderAction->InvestorID);
		INFO_LOG("\tExchangeID [%s]\n", pInputBatchOrderAction->ExchangeID);
		INFO_LOG("\tUserID [%s]\n", pInputBatchOrderAction->UserID);
		INFO_LOG("\tInvestUnitID [%s]\n", pInputBatchOrderAction->InvestUnitID);
		INFO_LOG("\tIPAddress [%s]\n", pInputBatchOrderAction->IPAddress);
		INFO_LOG("\tMacAddress [%s]\n", pInputBatchOrderAction->MacAddress);
		INFO_LOG("\tOrderActionRef [%d]\n", pInputBatchOrderAction->OrderActionRef);
		INFO_LOG("\tRequestID [%d]\n", pInputBatchOrderAction->RequestID);
		INFO_LOG("\tFrontID [%d]\n", pInputBatchOrderAction->FrontID);
		INFO_LOG("\tSessionID [%d]\n", pInputBatchOrderAction->SessionID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqBatchOrderAction>\n");

	return m_pApi->ReqBatchOrderAction(pInputBatchOrderAction, nRequestID);
};


int CTraderApi::ReqOptionSelfCloseInsert(CThostFtdcInputOptionSelfCloseField *pInputOptionSelfClose, int nRequestID)
{
	INFO_LOG("<ReqOptionSelfCloseInsert>\n");
	if (pInputOptionSelfClose)
	{
		INFO_LOG("\tBrokerID [%s]\n", pInputOptionSelfClose->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pInputOptionSelfClose->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pInputOptionSelfClose->InstrumentID);
		INFO_LOG("\tOptionSelfCloseRef [%s]\n", pInputOptionSelfClose->OptionSelfCloseRef);
		INFO_LOG("\tUserID [%s]\n", pInputOptionSelfClose->UserID);
		INFO_LOG("\tBusinessUnit [%s]\n", pInputOptionSelfClose->BusinessUnit);
		INFO_LOG("\tExchangeID [%s]\n", pInputOptionSelfClose->ExchangeID);
		INFO_LOG("\tInvestUnitID [%s]\n", pInputOptionSelfClose->InvestUnitID);
		INFO_LOG("\tAccountID [%s]\n", pInputOptionSelfClose->AccountID);
		INFO_LOG("\tCurrencyID [%s]\n", pInputOptionSelfClose->CurrencyID);
		INFO_LOG("\tClientID [%s]\n", pInputOptionSelfClose->ClientID);
		INFO_LOG("\tIPAddress [%s]\n", pInputOptionSelfClose->IPAddress);
		INFO_LOG("\tMacAddress [%s]\n", pInputOptionSelfClose->MacAddress);
		INFO_LOG("\tVolume [%d]\n", pInputOptionSelfClose->Volume);
		INFO_LOG("\tRequestID [%d]\n", pInputOptionSelfClose->RequestID);
		INFO_LOG("\tHedgeFlag [%c]\n", pInputOptionSelfClose->HedgeFlag);
		INFO_LOG("\tOptSelfCloseFlag [%c]\n", pInputOptionSelfClose->OptSelfCloseFlag);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqOptionSelfCloseInsert>\n");

	return m_pApi->ReqOptionSelfCloseInsert(pInputOptionSelfClose, nRequestID);
};


int CTraderApi::ReqOptionSelfCloseAction(CThostFtdcInputOptionSelfCloseActionField *pInputOptionSelfCloseAction, int nRequestID)
{
	INFO_LOG("<ReqOptionSelfCloseAction>\n");
	if (pInputOptionSelfCloseAction)
	{
		INFO_LOG("\tBrokerID [%s]\n", pInputOptionSelfCloseAction->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pInputOptionSelfCloseAction->InvestorID);
		INFO_LOG("\tOptionSelfCloseRef [%s]\n", pInputOptionSelfCloseAction->OptionSelfCloseRef);
		INFO_LOG("\tExchangeID [%s]\n", pInputOptionSelfCloseAction->ExchangeID);
		INFO_LOG("\tOptionSelfCloseSysID [%s]\n", pInputOptionSelfCloseAction->OptionSelfCloseSysID);
		INFO_LOG("\tUserID [%s]\n", pInputOptionSelfCloseAction->UserID);
		INFO_LOG("\tInstrumentID [%s]\n", pInputOptionSelfCloseAction->InstrumentID);
		INFO_LOG("\tInvestUnitID [%s]\n", pInputOptionSelfCloseAction->InvestUnitID);
		INFO_LOG("\tIPAddress [%s]\n", pInputOptionSelfCloseAction->IPAddress);
		INFO_LOG("\tMacAddress [%s]\n", pInputOptionSelfCloseAction->MacAddress);
		INFO_LOG("\tOptionSelfCloseActionRef [%d]\n", pInputOptionSelfCloseAction->OptionSelfCloseActionRef);
		INFO_LOG("\tRequestID [%d]\n", pInputOptionSelfCloseAction->RequestID);
		INFO_LOG("\tFrontID [%d]\n", pInputOptionSelfCloseAction->FrontID);
		INFO_LOG("\tSessionID [%d]\n", pInputOptionSelfCloseAction->SessionID);
		INFO_LOG("\tActionFlag [%c]\n", pInputOptionSelfCloseAction->ActionFlag);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqOptionSelfCloseAction>\n");

	return m_pApi->ReqOptionSelfCloseAction(pInputOptionSelfCloseAction, nRequestID);
};


int CTraderApi::ReqCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, int nRequestID)
{
	INFO_LOG("<ReqCombActionInsert>\n");
	if (pInputCombAction)
	{
		INFO_LOG("\tBrokerID [%s]\n", pInputCombAction->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pInputCombAction->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pInputCombAction->InstrumentID);
		INFO_LOG("\tCombActionRef [%s]\n", pInputCombAction->CombActionRef);
		INFO_LOG("\tUserID [%s]\n", pInputCombAction->UserID);
		INFO_LOG("\tExchangeID [%s]\n", pInputCombAction->ExchangeID);
		INFO_LOG("\tIPAddress [%s]\n", pInputCombAction->IPAddress);
		INFO_LOG("\tMacAddress [%s]\n", pInputCombAction->MacAddress);
		INFO_LOG("\tInvestUnitID [%s]\n", pInputCombAction->InvestUnitID);
		INFO_LOG("\tVolume [%d]\n", pInputCombAction->Volume);
		INFO_LOG("\tDirection [%c]\n", pInputCombAction->Direction);
		INFO_LOG("\tCombDirection [%c]\n", pInputCombAction->CombDirection);
		INFO_LOG("\tHedgeFlag [%c]\n", pInputCombAction->HedgeFlag);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqCombActionInsert>\n");

	return m_pApi->ReqCombActionInsert(pInputCombAction, nRequestID);
};


int CTraderApi::ReqQryOrder(CThostFtdcQryOrderField *pQryOrder, int nRequestID)
{
	INFO_LOG("<ReqQryOrder>\n");
	if (pQryOrder)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryOrder->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryOrder->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryOrder->InstrumentID);
		INFO_LOG("\tExchangeID [%s]\n", pQryOrder->ExchangeID);
		INFO_LOG("\tOrderSysID [%s]\n", pQryOrder->OrderSysID);
		INFO_LOG("\tInsertTimeStart [%s]\n", pQryOrder->InsertTimeStart);
		INFO_LOG("\tInsertTimeEnd [%s]\n", pQryOrder->InsertTimeEnd);
		INFO_LOG("\tInvestUnitID [%s]\n", pQryOrder->InvestUnitID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryOrder>\n");

	return m_pApi->ReqQryOrder(pQryOrder, nRequestID);
};


int CTraderApi::ReqQryTrade(CThostFtdcQryTradeField *pQryTrade, int nRequestID)
{
	INFO_LOG("<ReqQryTrade>\n");
	if (pQryTrade)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryTrade->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryTrade->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryTrade->InstrumentID);
		INFO_LOG("\tExchangeID [%s]\n", pQryTrade->ExchangeID);
		INFO_LOG("\tTradeID [%s]\n", pQryTrade->TradeID);
		INFO_LOG("\tTradeTimeStart [%s]\n", pQryTrade->TradeTimeStart);
		INFO_LOG("\tTradeTimeEnd [%s]\n", pQryTrade->TradeTimeEnd);
		INFO_LOG("\tInvestUnitID [%s]\n", pQryTrade->InvestUnitID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryTrade>\n");

	return m_pApi->ReqQryTrade(pQryTrade, nRequestID);
};


int CTraderApi::ReqQryInvestorPosition(CThostFtdcQryInvestorPositionField *pQryInvestorPosition, int nRequestID)
{
	INFO_LOG("<ReqQryInvestorPosition>\n");
	if (pQryInvestorPosition)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryInvestorPosition->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryInvestorPosition->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryInvestorPosition->InstrumentID);
		INFO_LOG("\tExchangeID [%s]\n", pQryInvestorPosition->ExchangeID);
		INFO_LOG("\tInvestUnitID [%s]\n", pQryInvestorPosition->InvestUnitID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryInvestorPosition>\n");

	return m_pApi->ReqQryInvestorPosition(pQryInvestorPosition, nRequestID);
};


int CTraderApi::ReqQryTradingAccount(CThostFtdcQryTradingAccountField *pQryTradingAccount, int nRequestID)
{
	INFO_LOG("<ReqQryTradingAccount>\n");
	if (pQryTradingAccount)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryTradingAccount->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryTradingAccount->InvestorID);
		INFO_LOG("\tCurrencyID [%s]\n", pQryTradingAccount->CurrencyID);
		INFO_LOG("\tAccountID [%s]\n", pQryTradingAccount->AccountID);
		INFO_LOG("\tBizType [%c]\n", pQryTradingAccount->BizType);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryTradingAccount>\n");

	return m_pApi->ReqQryTradingAccount(pQryTradingAccount, nRequestID);
};


int CTraderApi::ReqQryInvestor(CThostFtdcQryInvestorField *pQryInvestor, int nRequestID)
{
	INFO_LOG("<ReqQryInvestor>\n");
	if (pQryInvestor)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryInvestor->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryInvestor->InvestorID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryInvestor>\n");

	return m_pApi->ReqQryInvestor(pQryInvestor, nRequestID);
};


int CTraderApi::ReqQryTradingCode(CThostFtdcQryTradingCodeField *pQryTradingCode, int nRequestID)
{
	INFO_LOG("<ReqQryTradingCode>\n");
	if (pQryTradingCode)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryTradingCode->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryTradingCode->InvestorID);
		INFO_LOG("\tExchangeID [%s]\n", pQryTradingCode->ExchangeID);
		INFO_LOG("\tClientID [%s]\n", pQryTradingCode->ClientID);
		INFO_LOG("\tInvestUnitID [%s]\n", pQryTradingCode->InvestUnitID);
		INFO_LOG("\tClientIDType [%c]\n", pQryTradingCode->ClientIDType);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryTradingCode>\n");

	return m_pApi->ReqQryTradingCode(pQryTradingCode, nRequestID);
};


int CTraderApi::ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate, int nRequestID)
{
	INFO_LOG("<ReqQryInstrumentMarginRate>\n");
	if (pQryInstrumentMarginRate)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryInstrumentMarginRate->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryInstrumentMarginRate->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryInstrumentMarginRate->InstrumentID);
		INFO_LOG("\tExchangeID [%s]\n", pQryInstrumentMarginRate->ExchangeID);
		INFO_LOG("\tInvestUnitID [%s]\n", pQryInstrumentMarginRate->InvestUnitID);
		INFO_LOG("\tHedgeFlag [%c]\n", pQryInstrumentMarginRate->HedgeFlag);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryInstrumentMarginRate>\n");

	return m_pApi->ReqQryInstrumentMarginRate(pQryInstrumentMarginRate, nRequestID);
};


int CTraderApi::ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID)
{
	INFO_LOG("<ReqQryInstrumentCommissionRate>\n");
	if (pQryInstrumentCommissionRate)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryInstrumentCommissionRate->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryInstrumentCommissionRate->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryInstrumentCommissionRate->InstrumentID);
		INFO_LOG("\tExchangeID [%s]\n", pQryInstrumentCommissionRate->ExchangeID);
		INFO_LOG("\tInvestUnitID [%s]\n", pQryInstrumentCommissionRate->InvestUnitID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryInstrumentCommissionRate>\n");

	return m_pApi->ReqQryInstrumentCommissionRate(pQryInstrumentCommissionRate, nRequestID);
};


int CTraderApi::ReqQryExchange(CThostFtdcQryExchangeField *pQryExchange, int nRequestID)
{
	INFO_LOG("<ReqQryExchange>\n");
	if (pQryExchange)
	{
		INFO_LOG("\tExchangeID [%s]\n", pQryExchange->ExchangeID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryExchange>\n");

	return m_pApi->ReqQryExchange(pQryExchange, nRequestID);
};


int CTraderApi::ReqQryProduct(CThostFtdcQryProductField *pQryProduct, int nRequestID)
{
	INFO_LOG("<ReqQryProduct>\n");
	if (pQryProduct)
	{
		INFO_LOG("\tProductID [%s]\n", pQryProduct->ProductID);
		INFO_LOG("\tExchangeID [%s]\n", pQryProduct->ExchangeID);
		INFO_LOG("\tProductClass [%c]\n", pQryProduct->ProductClass);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryProduct>\n");

	return m_pApi->ReqQryProduct(pQryProduct, nRequestID);
};


int CTraderApi::ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument, int nRequestID)
{
	INFO_LOG("%s","<ReqQryInstrument>");
	if (pQryInstrument)
	{
		INFO_LOG("\tInstrumentID [%s]", pQryInstrument->InstrumentID);
		INFO_LOG("\tExchangeID [%s]", pQryInstrument->ExchangeID);
		INFO_LOG("\tExchangeInstID [%s]", pQryInstrument->ExchangeInstID);
		INFO_LOG("\tProductID [%s]", pQryInstrument->ProductID);
	}
	INFO_LOG("\tnRequestID [%d]", nRequestID);
	INFO_LOG("%s","</ReqQryInstrument>\n");

	return m_pApi->ReqQryInstrument(pQryInstrument, nRequestID);
};


int CTraderApi::ReqQryDepthMarketData(CThostFtdcQryDepthMarketDataField *pQryDepthMarketData, int nRequestID)
{
	INFO_LOG("<ReqQryDepthMarketData>\n");
	if (pQryDepthMarketData)
	{
		INFO_LOG("\tInstrumentID [%s]\n", pQryDepthMarketData->InstrumentID);
		INFO_LOG("\tExchangeID [%s]\n", pQryDepthMarketData->ExchangeID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryDepthMarketData>\n");

	return m_pApi->ReqQryDepthMarketData(pQryDepthMarketData, nRequestID);
};


int CTraderApi::ReqQrySettlementInfo(CThostFtdcQrySettlementInfoField *pQrySettlementInfo, int nRequestID)
{
	INFO_LOG("<ReqQrySettlementInfo>\n");
	if (pQrySettlementInfo)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQrySettlementInfo->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQrySettlementInfo->InvestorID);
		INFO_LOG("\tTradingDay [%s]\n", pQrySettlementInfo->TradingDay);
		INFO_LOG("\tAccountID [%s]\n", pQrySettlementInfo->AccountID);
		INFO_LOG("\tCurrencyID [%s]\n", pQrySettlementInfo->CurrencyID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQrySettlementInfo>\n");

	return m_pApi->ReqQrySettlementInfo(pQrySettlementInfo, nRequestID);
};


int CTraderApi::ReqQryTransferBank(CThostFtdcQryTransferBankField *pQryTransferBank, int nRequestID)
{
	INFO_LOG("<ReqQryTransferBank>\n");
	if (pQryTransferBank)
	{
		INFO_LOG("\tBankID [%s]\n", pQryTransferBank->BankID);
		INFO_LOG("\tBankBrchID [%s]\n", pQryTransferBank->BankBrchID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryTransferBank>\n");

	return m_pApi->ReqQryTransferBank(pQryTransferBank, nRequestID);
};


int CTraderApi::ReqQryInvestorPositionDetail(CThostFtdcQryInvestorPositionDetailField *pQryInvestorPositionDetail, int nRequestID)
{
	INFO_LOG("<ReqQryInvestorPositionDetail>\n");
	if (pQryInvestorPositionDetail)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryInvestorPositionDetail->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryInvestorPositionDetail->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryInvestorPositionDetail->InstrumentID);
		INFO_LOG("\tExchangeID [%s]\n", pQryInvestorPositionDetail->ExchangeID);
		INFO_LOG("\tInvestUnitID [%s]\n", pQryInvestorPositionDetail->InvestUnitID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryInvestorPositionDetail>\n");

	return m_pApi->ReqQryInvestorPositionDetail(pQryInvestorPositionDetail, nRequestID);
};


int CTraderApi::ReqQryNotice(CThostFtdcQryNoticeField *pQryNotice, int nRequestID)
{
	INFO_LOG("<ReqQryNotice>\n");
	if (pQryNotice)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryNotice->BrokerID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryNotice>\n");

	return m_pApi->ReqQryNotice(pQryNotice, nRequestID);
};


int CTraderApi::ReqQrySettlementInfoConfirm(CThostFtdcQrySettlementInfoConfirmField *pQrySettlementInfoConfirm, int nRequestID)
{
	INFO_LOG("<ReqQrySettlementInfoConfirm>\n");
	if (pQrySettlementInfoConfirm)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQrySettlementInfoConfirm->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQrySettlementInfoConfirm->InvestorID);
		INFO_LOG("\tAccountID [%s]\n", pQrySettlementInfoConfirm->AccountID);
		INFO_LOG("\tCurrencyID [%s]\n", pQrySettlementInfoConfirm->CurrencyID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQrySettlementInfoConfirm>\n");

	return m_pApi->ReqQrySettlementInfoConfirm(pQrySettlementInfoConfirm, nRequestID);
};


int CTraderApi::ReqQryInvestorPositionCombineDetail(CThostFtdcQryInvestorPositionCombineDetailField *pQryInvestorPositionCombineDetail, int nRequestID)
{
	INFO_LOG("<ReqQryInvestorPositionCombineDetail>\n");
	if (pQryInvestorPositionCombineDetail)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryInvestorPositionCombineDetail->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryInvestorPositionCombineDetail->InvestorID);
		INFO_LOG("\tCombInstrumentID [%s]\n", pQryInvestorPositionCombineDetail->CombInstrumentID);
		INFO_LOG("\tExchangeID [%s]\n", pQryInvestorPositionCombineDetail->ExchangeID);
		INFO_LOG("\tInvestUnitID [%s]\n", pQryInvestorPositionCombineDetail->InvestUnitID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryInvestorPositionCombineDetail>\n");

	return m_pApi->ReqQryInvestorPositionCombineDetail(pQryInvestorPositionCombineDetail, nRequestID);
};


int CTraderApi::ReqQryCFMMCTradingAccountKey(CThostFtdcQryCFMMCTradingAccountKeyField *pQryCFMMCTradingAccountKey, int nRequestID)
{
	INFO_LOG("<ReqQryCFMMCTradingAccountKey>\n");
	if (pQryCFMMCTradingAccountKey)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryCFMMCTradingAccountKey->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryCFMMCTradingAccountKey->InvestorID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryCFMMCTradingAccountKey>\n");

	return m_pApi->ReqQryCFMMCTradingAccountKey(pQryCFMMCTradingAccountKey, nRequestID);
};


int CTraderApi::ReqQryEWarrantOffset(CThostFtdcQryEWarrantOffsetField *pQryEWarrantOffset, int nRequestID)
{
	INFO_LOG("<ReqQryEWarrantOffset>\n");
	if (pQryEWarrantOffset)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryEWarrantOffset->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryEWarrantOffset->InvestorID);
		INFO_LOG("\tExchangeID [%s]\n", pQryEWarrantOffset->ExchangeID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryEWarrantOffset->InstrumentID);
		INFO_LOG("\tInvestUnitID [%s]\n", pQryEWarrantOffset->InvestUnitID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryEWarrantOffset>\n");

	return m_pApi->ReqQryEWarrantOffset(pQryEWarrantOffset, nRequestID);
};


int CTraderApi::ReqQryInvestorProductGroupMargin(CThostFtdcQryInvestorProductGroupMarginField *pQryInvestorProductGroupMargin, int nRequestID)
{
	INFO_LOG("<ReqQryInvestorProductGroupMargin>\n");
	if (pQryInvestorProductGroupMargin)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryInvestorProductGroupMargin->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryInvestorProductGroupMargin->InvestorID);
		INFO_LOG("\tProductGroupID [%s]\n", pQryInvestorProductGroupMargin->ProductGroupID);
		INFO_LOG("\tExchangeID [%s]\n", pQryInvestorProductGroupMargin->ExchangeID);
		INFO_LOG("\tInvestUnitID [%s]\n", pQryInvestorProductGroupMargin->InvestUnitID);
		INFO_LOG("\tHedgeFlag [%c]\n", pQryInvestorProductGroupMargin->HedgeFlag);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryInvestorProductGroupMargin>\n");

	return m_pApi->ReqQryInvestorProductGroupMargin(pQryInvestorProductGroupMargin, nRequestID);
};


int CTraderApi::ReqQryExchangeMarginRate(CThostFtdcQryExchangeMarginRateField *pQryExchangeMarginRate, int nRequestID)
{
	INFO_LOG("<ReqQryExchangeMarginRate>\n");
	if (pQryExchangeMarginRate)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryExchangeMarginRate->BrokerID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryExchangeMarginRate->InstrumentID);
		INFO_LOG("\tExchangeID [%s]\n", pQryExchangeMarginRate->ExchangeID);
		INFO_LOG("\tHedgeFlag [%c]\n", pQryExchangeMarginRate->HedgeFlag);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryExchangeMarginRate>\n");

	return m_pApi->ReqQryExchangeMarginRate(pQryExchangeMarginRate, nRequestID);
};


int CTraderApi::ReqQryExchangeMarginRateAdjust(CThostFtdcQryExchangeMarginRateAdjustField *pQryExchangeMarginRateAdjust, int nRequestID)
{
	INFO_LOG("<ReqQryExchangeMarginRateAdjust>\n");
	if (pQryExchangeMarginRateAdjust)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryExchangeMarginRateAdjust->BrokerID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryExchangeMarginRateAdjust->InstrumentID);
		INFO_LOG("\tHedgeFlag [%c]\n", pQryExchangeMarginRateAdjust->HedgeFlag);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryExchangeMarginRateAdjust>\n");

	return m_pApi->ReqQryExchangeMarginRateAdjust(pQryExchangeMarginRateAdjust, nRequestID);
};


int CTraderApi::ReqQryExchangeRate(CThostFtdcQryExchangeRateField *pQryExchangeRate, int nRequestID)
{
	INFO_LOG("<ReqQryExchangeRate>\n");
	if (pQryExchangeRate)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryExchangeRate->BrokerID);
		INFO_LOG("\tFromCurrencyID [%s]\n", pQryExchangeRate->FromCurrencyID);
		INFO_LOG("\tToCurrencyID [%s]\n", pQryExchangeRate->ToCurrencyID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryExchangeRate>\n");

	return m_pApi->ReqQryExchangeRate(pQryExchangeRate, nRequestID);
};


int CTraderApi::ReqQrySecAgentACIDMap(CThostFtdcQrySecAgentACIDMapField *pQrySecAgentACIDMap, int nRequestID)
{
	INFO_LOG("<ReqQrySecAgentACIDMap>\n");
	if (pQrySecAgentACIDMap)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQrySecAgentACIDMap->BrokerID);
		INFO_LOG("\tUserID [%s]\n", pQrySecAgentACIDMap->UserID);
		INFO_LOG("\tAccountID [%s]\n", pQrySecAgentACIDMap->AccountID);
		INFO_LOG("\tCurrencyID [%s]\n", pQrySecAgentACIDMap->CurrencyID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQrySecAgentACIDMap>\n");

	return m_pApi->ReqQrySecAgentACIDMap(pQrySecAgentACIDMap, nRequestID);
};


int CTraderApi::ReqQryProductExchRate(CThostFtdcQryProductExchRateField *pQryProductExchRate, int nRequestID)
{
	INFO_LOG("<ReqQryProductExchRate>\n");
	if (pQryProductExchRate)
	{
		INFO_LOG("\tProductID [%s]\n", pQryProductExchRate->ProductID);
		INFO_LOG("\tExchangeID [%s]\n", pQryProductExchRate->ExchangeID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryProductExchRate>\n");

	return m_pApi->ReqQryProductExchRate(pQryProductExchRate, nRequestID);
};


int CTraderApi::ReqQryProductGroup(CThostFtdcQryProductGroupField *pQryProductGroup, int nRequestID)
{
	INFO_LOG("<ReqQryProductGroup>\n");
	if (pQryProductGroup)
	{
		INFO_LOG("\tProductID [%s]\n", pQryProductGroup->ProductID);
		INFO_LOG("\tExchangeID [%s]\n", pQryProductGroup->ExchangeID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryProductGroup>\n");

	return m_pApi->ReqQryProductGroup(pQryProductGroup, nRequestID);
};


int CTraderApi::ReqQryMMInstrumentCommissionRate(CThostFtdcQryMMInstrumentCommissionRateField *pQryMMInstrumentCommissionRate, int nRequestID)
{
	INFO_LOG("<ReqQryMMInstrumentCommissionRate>\n");
	if (pQryMMInstrumentCommissionRate)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryMMInstrumentCommissionRate->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryMMInstrumentCommissionRate->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryMMInstrumentCommissionRate->InstrumentID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryMMInstrumentCommissionRate>\n");

	return m_pApi->ReqQryMMInstrumentCommissionRate(pQryMMInstrumentCommissionRate, nRequestID);
};


int CTraderApi::ReqQryMMOptionInstrCommRate(CThostFtdcQryMMOptionInstrCommRateField *pQryMMOptionInstrCommRate, int nRequestID)
{
	INFO_LOG("<ReqQryMMOptionInstrCommRate>\n");
	if (pQryMMOptionInstrCommRate)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryMMOptionInstrCommRate->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryMMOptionInstrCommRate->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryMMOptionInstrCommRate->InstrumentID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryMMOptionInstrCommRate>\n");

	return m_pApi->ReqQryMMOptionInstrCommRate(pQryMMOptionInstrCommRate, nRequestID);
};


int CTraderApi::ReqQryInstrumentOrderCommRate(CThostFtdcQryInstrumentOrderCommRateField *pQryInstrumentOrderCommRate, int nRequestID)
{
	INFO_LOG("<ReqQryInstrumentOrderCommRate>\n");
	if (pQryInstrumentOrderCommRate)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryInstrumentOrderCommRate->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryInstrumentOrderCommRate->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryInstrumentOrderCommRate->InstrumentID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryInstrumentOrderCommRate>\n");

	return m_pApi->ReqQryInstrumentOrderCommRate(pQryInstrumentOrderCommRate, nRequestID);
};


int CTraderApi::ReqQryOptionInstrTradeCost(CThostFtdcQryOptionInstrTradeCostField *pQryOptionInstrTradeCost, int nRequestID)
{
	INFO_LOG("<ReqQryOptionInstrTradeCost>\n");
	if (pQryOptionInstrTradeCost)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryOptionInstrTradeCost->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryOptionInstrTradeCost->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryOptionInstrTradeCost->InstrumentID);
		INFO_LOG("\tExchangeID [%s]\n", pQryOptionInstrTradeCost->ExchangeID);
		INFO_LOG("\tInvestUnitID [%s]\n", pQryOptionInstrTradeCost->InvestUnitID);
		INFO_LOG("\tHedgeFlag [%c]\n", pQryOptionInstrTradeCost->HedgeFlag);
		INFO_LOG("\tInputPrice [%.8lf]\n", pQryOptionInstrTradeCost->InputPrice);
		INFO_LOG("\tUnderlyingPrice [%.8lf]\n", pQryOptionInstrTradeCost->UnderlyingPrice);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryOptionInstrTradeCost>\n");

	return m_pApi->ReqQryOptionInstrTradeCost(pQryOptionInstrTradeCost, nRequestID);
};


int CTraderApi::ReqQryOptionInstrCommRate(CThostFtdcQryOptionInstrCommRateField *pQryOptionInstrCommRate, int nRequestID)
{
	INFO_LOG("<ReqQryOptionInstrCommRate>\n");
	if (pQryOptionInstrCommRate)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryOptionInstrCommRate->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryOptionInstrCommRate->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryOptionInstrCommRate->InstrumentID);
		INFO_LOG("\tExchangeID [%s]\n", pQryOptionInstrCommRate->ExchangeID);
		INFO_LOG("\tInvestUnitID [%s]\n", pQryOptionInstrCommRate->InvestUnitID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryOptionInstrCommRate>\n");

	return m_pApi->ReqQryOptionInstrCommRate(pQryOptionInstrCommRate, nRequestID);
};


int CTraderApi::ReqQryExecOrder(CThostFtdcQryExecOrderField *pQryExecOrder, int nRequestID)
{
	INFO_LOG("<ReqQryExecOrder>\n");
	if (pQryExecOrder)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryExecOrder->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryExecOrder->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryExecOrder->InstrumentID);
		INFO_LOG("\tExchangeID [%s]\n", pQryExecOrder->ExchangeID);
		INFO_LOG("\tExecOrderSysID [%s]\n", pQryExecOrder->ExecOrderSysID);
		INFO_LOG("\tInsertTimeStart [%s]\n", pQryExecOrder->InsertTimeStart);
		INFO_LOG("\tInsertTimeEnd [%s]\n", pQryExecOrder->InsertTimeEnd);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryExecOrder>\n");

	return m_pApi->ReqQryExecOrder(pQryExecOrder, nRequestID);
};


int CTraderApi::ReqQryForQuote(CThostFtdcQryForQuoteField *pQryForQuote, int nRequestID)
{
	INFO_LOG("<ReqQryForQuote>\n");
	if (pQryForQuote)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryForQuote->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryForQuote->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryForQuote->InstrumentID);
		INFO_LOG("\tExchangeID [%s]\n", pQryForQuote->ExchangeID);
		INFO_LOG("\tInsertTimeStart [%s]\n", pQryForQuote->InsertTimeStart);
		INFO_LOG("\tInsertTimeEnd [%s]\n", pQryForQuote->InsertTimeEnd);
		INFO_LOG("\tInvestUnitID [%s]\n", pQryForQuote->InvestUnitID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryForQuote>\n");

	return m_pApi->ReqQryForQuote(pQryForQuote, nRequestID);
};


int CTraderApi::ReqQryQuote(CThostFtdcQryQuoteField *pQryQuote, int nRequestID)
{
	INFO_LOG("<ReqQryQuote>\n");
	if (pQryQuote)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryQuote->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryQuote->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryQuote->InstrumentID);
		INFO_LOG("\tExchangeID [%s]\n", pQryQuote->ExchangeID);
		INFO_LOG("\tQuoteSysID [%s]\n", pQryQuote->QuoteSysID);
		INFO_LOG("\tInsertTimeStart [%s]\n", pQryQuote->InsertTimeStart);
		INFO_LOG("\tInsertTimeEnd [%s]\n", pQryQuote->InsertTimeEnd);
		INFO_LOG("\tInvestUnitID [%s]\n", pQryQuote->InvestUnitID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryQuote>\n");

	return m_pApi->ReqQryQuote(pQryQuote, nRequestID);
};


int CTraderApi::ReqQryOptionSelfClose(CThostFtdcQryOptionSelfCloseField *pQryOptionSelfClose, int nRequestID)
{
	INFO_LOG("<ReqQryOptionSelfClose>\n");
	if (pQryOptionSelfClose)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryOptionSelfClose->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryOptionSelfClose->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryOptionSelfClose->InstrumentID);
		INFO_LOG("\tExchangeID [%s]\n", pQryOptionSelfClose->ExchangeID);
		INFO_LOG("\tOptionSelfCloseSysID [%s]\n", pQryOptionSelfClose->OptionSelfCloseSysID);
		INFO_LOG("\tInsertTimeStart [%s]\n", pQryOptionSelfClose->InsertTimeStart);
		INFO_LOG("\tInsertTimeEnd [%s]\n", pQryOptionSelfClose->InsertTimeEnd);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryOptionSelfClose>\n");

	return m_pApi->ReqQryOptionSelfClose(pQryOptionSelfClose, nRequestID);
};


int CTraderApi::ReqQryInvestUnit(CThostFtdcQryInvestUnitField *pQryInvestUnit, int nRequestID)
{
	INFO_LOG("<ReqQryInvestUnit>\n");
	if (pQryInvestUnit)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryInvestUnit->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryInvestUnit->InvestorID);
		INFO_LOG("\tInvestUnitID [%s]\n", pQryInvestUnit->InvestUnitID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryInvestUnit>\n");

	return m_pApi->ReqQryInvestUnit(pQryInvestUnit, nRequestID);
};


int CTraderApi::ReqQryCombInstrumentGuard(CThostFtdcQryCombInstrumentGuardField *pQryCombInstrumentGuard, int nRequestID)
{
	INFO_LOG("<ReqQryCombInstrumentGuard>\n");
	if (pQryCombInstrumentGuard)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryCombInstrumentGuard->BrokerID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryCombInstrumentGuard->InstrumentID);
		INFO_LOG("\tExchangeID [%s]\n", pQryCombInstrumentGuard->ExchangeID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryCombInstrumentGuard>\n");

	return m_pApi->ReqQryCombInstrumentGuard(pQryCombInstrumentGuard, nRequestID);
};


int CTraderApi::ReqQryCombAction(CThostFtdcQryCombActionField *pQryCombAction, int nRequestID)
{
	INFO_LOG("<ReqQryCombAction>\n");
	if (pQryCombAction)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryCombAction->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryCombAction->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryCombAction->InstrumentID);
		INFO_LOG("\tExchangeID [%s]\n", pQryCombAction->ExchangeID);
		INFO_LOG("\tInvestUnitID [%s]\n", pQryCombAction->InvestUnitID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryCombAction>\n");

	return m_pApi->ReqQryCombAction(pQryCombAction, nRequestID);
};


int CTraderApi::ReqQryTransferSerial(CThostFtdcQryTransferSerialField *pQryTransferSerial, int nRequestID)
{
	INFO_LOG("<ReqQryTransferSerial>\n");
	if (pQryTransferSerial)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryTransferSerial->BrokerID);
		INFO_LOG("\tAccountID [%s]\n", pQryTransferSerial->AccountID);
		INFO_LOG("\tBankID [%s]\n", pQryTransferSerial->BankID);
		INFO_LOG("\tCurrencyID [%s]\n", pQryTransferSerial->CurrencyID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryTransferSerial>\n");

	return m_pApi->ReqQryTransferSerial(pQryTransferSerial, nRequestID);
};


int CTraderApi::ReqQryAccountregister(CThostFtdcQryAccountregisterField *pQryAccountregister, int nRequestID)
{
	INFO_LOG("<ReqQryAccountregister>\n");
	if (pQryAccountregister)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryAccountregister->BrokerID);
		INFO_LOG("\tAccountID [%s]\n", pQryAccountregister->AccountID);
		INFO_LOG("\tBankID [%s]\n", pQryAccountregister->BankID);
		INFO_LOG("\tBankBranchID [%s]\n", pQryAccountregister->BankBranchID);
		INFO_LOG("\tCurrencyID [%s]\n", pQryAccountregister->CurrencyID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryAccountregister>\n");

	return m_pApi->ReqQryAccountregister(pQryAccountregister, nRequestID);
};


int CTraderApi::ReqQryContractBank(CThostFtdcQryContractBankField *pQryContractBank, int nRequestID)
{
	INFO_LOG("<ReqQryContractBank>\n");
	if (pQryContractBank)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryContractBank->BrokerID);
		INFO_LOG("\tBankID [%s]\n", pQryContractBank->BankID);
		INFO_LOG("\tBankBrchID [%s]\n", pQryContractBank->BankBrchID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryContractBank>\n");

	return m_pApi->ReqQryContractBank(pQryContractBank, nRequestID);
};


int CTraderApi::ReqQryParkedOrder(CThostFtdcQryParkedOrderField *pQryParkedOrder, int nRequestID)
{
	INFO_LOG("<ReqQryParkedOrder>\n");
	if (pQryParkedOrder)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryParkedOrder->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryParkedOrder->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryParkedOrder->InstrumentID);
		INFO_LOG("\tExchangeID [%s]\n", pQryParkedOrder->ExchangeID);
		INFO_LOG("\tInvestUnitID [%s]\n", pQryParkedOrder->InvestUnitID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryParkedOrder>\n");

	return m_pApi->ReqQryParkedOrder(pQryParkedOrder, nRequestID);
};


int CTraderApi::ReqQryParkedOrderAction(CThostFtdcQryParkedOrderActionField *pQryParkedOrderAction, int nRequestID)
{
	INFO_LOG("<ReqQryParkedOrderAction>\n");
	if (pQryParkedOrderAction)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryParkedOrderAction->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryParkedOrderAction->InvestorID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryParkedOrderAction->InstrumentID);
		INFO_LOG("\tExchangeID [%s]\n", pQryParkedOrderAction->ExchangeID);
		INFO_LOG("\tInvestUnitID [%s]\n", pQryParkedOrderAction->InvestUnitID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryParkedOrderAction>\n");

	return m_pApi->ReqQryParkedOrderAction(pQryParkedOrderAction, nRequestID);
};


int CTraderApi::ReqQryTradingNotice(CThostFtdcQryTradingNoticeField *pQryTradingNotice, int nRequestID)
{
	INFO_LOG("<ReqQryTradingNotice>\n");
	if (pQryTradingNotice)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryTradingNotice->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryTradingNotice->InvestorID);
		INFO_LOG("\tInvestUnitID [%s]\n", pQryTradingNotice->InvestUnitID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryTradingNotice>\n");

	return m_pApi->ReqQryTradingNotice(pQryTradingNotice, nRequestID);
};


int CTraderApi::ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField *pQryBrokerTradingParams, int nRequestID)
{
	INFO_LOG("<ReqQryBrokerTradingParams>\n");
	if (pQryBrokerTradingParams)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryBrokerTradingParams->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryBrokerTradingParams->InvestorID);
		INFO_LOG("\tCurrencyID [%s]\n", pQryBrokerTradingParams->CurrencyID);
		INFO_LOG("\tAccountID [%s]\n", pQryBrokerTradingParams->AccountID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryBrokerTradingParams>\n");

	return m_pApi->ReqQryBrokerTradingParams(pQryBrokerTradingParams, nRequestID);
};


int CTraderApi::ReqQryBrokerTradingAlgos(CThostFtdcQryBrokerTradingAlgosField *pQryBrokerTradingAlgos, int nRequestID)
{
	INFO_LOG("<ReqQryBrokerTradingAlgos>\n");
	if (pQryBrokerTradingAlgos)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryBrokerTradingAlgos->BrokerID);
		INFO_LOG("\tExchangeID [%s]\n", pQryBrokerTradingAlgos->ExchangeID);
		INFO_LOG("\tInstrumentID [%s]\n", pQryBrokerTradingAlgos->InstrumentID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQryBrokerTradingAlgos>\n");

	return m_pApi->ReqQryBrokerTradingAlgos(pQryBrokerTradingAlgos, nRequestID);
};


int CTraderApi::ReqQueryCFMMCTradingAccountToken(CThostFtdcQueryCFMMCTradingAccountTokenField *pQueryCFMMCTradingAccountToken, int nRequestID)
{
	INFO_LOG("<ReqQueryCFMMCTradingAccountToken>\n");
	if (pQueryCFMMCTradingAccountToken)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQueryCFMMCTradingAccountToken->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQueryCFMMCTradingAccountToken->InvestorID);
		INFO_LOG("\tInvestUnitID [%s]\n", pQueryCFMMCTradingAccountToken->InvestUnitID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQueryCFMMCTradingAccountToken>\n");

	return m_pApi->ReqQueryCFMMCTradingAccountToken(pQueryCFMMCTradingAccountToken, nRequestID);
};


int CTraderApi::ReqFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID)
{
	INFO_LOG("<ReqFromBankToFutureByFuture>\n");
	if (pReqTransfer)
	{
		INFO_LOG("\tTradeCode [%s]\n", pReqTransfer->TradeCode);
		INFO_LOG("\tBankID [%s]\n", pReqTransfer->BankID);
		INFO_LOG("\tBankBranchID [%s]\n", pReqTransfer->BankBranchID);
		INFO_LOG("\tBrokerID [%s]\n", pReqTransfer->BrokerID);
		INFO_LOG("\tBrokerBranchID [%s]\n", pReqTransfer->BrokerBranchID);
		INFO_LOG("\tTradeDate [%s]\n", pReqTransfer->TradeDate);
		INFO_LOG("\tTradeTime [%s]\n", pReqTransfer->TradeTime);
		INFO_LOG("\tBankSerial [%s]\n", pReqTransfer->BankSerial);
		INFO_LOG("\tTradingDay [%s]\n", pReqTransfer->TradingDay);
		INFO_LOG("\tCustomerName [%s]\n", pReqTransfer->CustomerName);
		INFO_LOG("\tIdentifiedCardNo [%s]\n", pReqTransfer->IdentifiedCardNo);
		INFO_LOG("\tBankAccount [%s]\n", pReqTransfer->BankAccount);
		INFO_LOG("\tBankPassWord [%s]\n", pReqTransfer->BankPassWord);
		INFO_LOG("\tAccountID [%s]\n", pReqTransfer->AccountID);
		INFO_LOG("\tPassword [%s]\n", pReqTransfer->Password);
		INFO_LOG("\tUserID [%s]\n", pReqTransfer->UserID);
		INFO_LOG("\tCurrencyID [%s]\n", pReqTransfer->CurrencyID);
		INFO_LOG("\tMessage [%s]\n", pReqTransfer->Message);
		INFO_LOG("\tDigest [%s]\n", pReqTransfer->Digest);
		INFO_LOG("\tDeviceID [%s]\n", pReqTransfer->DeviceID);
		INFO_LOG("\tBrokerIDByBank [%s]\n", pReqTransfer->BrokerIDByBank);
		INFO_LOG("\tBankSecuAcc [%s]\n", pReqTransfer->BankSecuAcc);
		INFO_LOG("\tOperNo [%s]\n", pReqTransfer->OperNo);
		INFO_LOG("\tLongCustomerName [%s]\n", pReqTransfer->LongCustomerName);
		INFO_LOG("\tPlateSerial [%d]\n", pReqTransfer->PlateSerial);
		INFO_LOG("\tSessionID [%d]\n", pReqTransfer->SessionID);
		INFO_LOG("\tInstallID [%d]\n", pReqTransfer->InstallID);
		INFO_LOG("\tFutureSerial [%d]\n", pReqTransfer->FutureSerial);
		INFO_LOG("\tRequestID [%d]\n", pReqTransfer->RequestID);
		INFO_LOG("\tTID [%d]\n", pReqTransfer->TID);
		INFO_LOG("\tLastFragment [%c]\n", pReqTransfer->LastFragment);
		INFO_LOG("\tIdCardType [%c]\n", pReqTransfer->IdCardType);
		INFO_LOG("\tCustType [%c]\n", pReqTransfer->CustType);
		INFO_LOG("\tVerifyCertNoFlag [%c]\n", pReqTransfer->VerifyCertNoFlag);
		INFO_LOG("\tFeePayFlag [%c]\n", pReqTransfer->FeePayFlag);
		INFO_LOG("\tBankAccType [%c]\n", pReqTransfer->BankAccType);
		INFO_LOG("\tBankSecuAccType [%c]\n", pReqTransfer->BankSecuAccType);
		INFO_LOG("\tBankPwdFlag [%c]\n", pReqTransfer->BankPwdFlag);
		INFO_LOG("\tSecuPwdFlag [%c]\n", pReqTransfer->SecuPwdFlag);
		INFO_LOG("\tTransferStatus [%c]\n", pReqTransfer->TransferStatus);
		INFO_LOG("\tTradeAmount [%.8lf]\n", pReqTransfer->TradeAmount);
		INFO_LOG("\tFutureFetchAmount [%.8lf]\n", pReqTransfer->FutureFetchAmount);
		INFO_LOG("\tCustFee [%.8lf]\n", pReqTransfer->CustFee);
		INFO_LOG("\tBrokerFee [%.8lf]\n", pReqTransfer->BrokerFee);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqFromBankToFutureByFuture>\n");

	return m_pApi->ReqFromBankToFutureByFuture(pReqTransfer, nRequestID);
};


int CTraderApi::ReqFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID)
{
	INFO_LOG("<ReqFromFutureToBankByFuture>\n");
	if (pReqTransfer)
	{
		INFO_LOG("\tTradeCode [%s]\n", pReqTransfer->TradeCode);
		INFO_LOG("\tBankID [%s]\n", pReqTransfer->BankID);
		INFO_LOG("\tBankBranchID [%s]\n", pReqTransfer->BankBranchID);
		INFO_LOG("\tBrokerID [%s]\n", pReqTransfer->BrokerID);
		INFO_LOG("\tBrokerBranchID [%s]\n", pReqTransfer->BrokerBranchID);
		INFO_LOG("\tTradeDate [%s]\n", pReqTransfer->TradeDate);
		INFO_LOG("\tTradeTime [%s]\n", pReqTransfer->TradeTime);
		INFO_LOG("\tBankSerial [%s]\n", pReqTransfer->BankSerial);
		INFO_LOG("\tTradingDay [%s]\n", pReqTransfer->TradingDay);
		INFO_LOG("\tCustomerName [%s]\n", pReqTransfer->CustomerName);
		INFO_LOG("\tIdentifiedCardNo [%s]\n", pReqTransfer->IdentifiedCardNo);
		INFO_LOG("\tBankAccount [%s]\n", pReqTransfer->BankAccount);
		INFO_LOG("\tBankPassWord [%s]\n", pReqTransfer->BankPassWord);
		INFO_LOG("\tAccountID [%s]\n", pReqTransfer->AccountID);
		INFO_LOG("\tPassword [%s]\n", pReqTransfer->Password);
		INFO_LOG("\tUserID [%s]\n", pReqTransfer->UserID);
		INFO_LOG("\tCurrencyID [%s]\n", pReqTransfer->CurrencyID);
		INFO_LOG("\tMessage [%s]\n", pReqTransfer->Message);
		INFO_LOG("\tDigest [%s]\n", pReqTransfer->Digest);
		INFO_LOG("\tDeviceID [%s]\n", pReqTransfer->DeviceID);
		INFO_LOG("\tBrokerIDByBank [%s]\n", pReqTransfer->BrokerIDByBank);
		INFO_LOG("\tBankSecuAcc [%s]\n", pReqTransfer->BankSecuAcc);
		INFO_LOG("\tOperNo [%s]\n", pReqTransfer->OperNo);
		INFO_LOG("\tLongCustomerName [%s]\n", pReqTransfer->LongCustomerName);
		INFO_LOG("\tPlateSerial [%d]\n", pReqTransfer->PlateSerial);
		INFO_LOG("\tSessionID [%d]\n", pReqTransfer->SessionID);
		INFO_LOG("\tInstallID [%d]\n", pReqTransfer->InstallID);
		INFO_LOG("\tFutureSerial [%d]\n", pReqTransfer->FutureSerial);
		INFO_LOG("\tRequestID [%d]\n", pReqTransfer->RequestID);
		INFO_LOG("\tTID [%d]\n", pReqTransfer->TID);
		INFO_LOG("\tLastFragment [%c]\n", pReqTransfer->LastFragment);
		INFO_LOG("\tIdCardType [%c]\n", pReqTransfer->IdCardType);
		INFO_LOG("\tCustType [%c]\n", pReqTransfer->CustType);
		INFO_LOG("\tVerifyCertNoFlag [%c]\n", pReqTransfer->VerifyCertNoFlag);
		INFO_LOG("\tFeePayFlag [%c]\n", pReqTransfer->FeePayFlag);
		INFO_LOG("\tBankAccType [%c]\n", pReqTransfer->BankAccType);
		INFO_LOG("\tBankSecuAccType [%c]\n", pReqTransfer->BankSecuAccType);
		INFO_LOG("\tBankPwdFlag [%c]\n", pReqTransfer->BankPwdFlag);
		INFO_LOG("\tSecuPwdFlag [%c]\n", pReqTransfer->SecuPwdFlag);
		INFO_LOG("\tTransferStatus [%c]\n", pReqTransfer->TransferStatus);
		INFO_LOG("\tTradeAmount [%.8lf]\n", pReqTransfer->TradeAmount);
		INFO_LOG("\tFutureFetchAmount [%.8lf]\n", pReqTransfer->FutureFetchAmount);
		INFO_LOG("\tCustFee [%.8lf]\n", pReqTransfer->CustFee);
		INFO_LOG("\tBrokerFee [%.8lf]\n", pReqTransfer->BrokerFee);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqFromFutureToBankByFuture>\n");

	return m_pApi->ReqFromFutureToBankByFuture(pReqTransfer, nRequestID);
};


int CTraderApi::ReqQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, int nRequestID)
{
	INFO_LOG("<ReqQueryBankAccountMoneyByFuture>\n");
	if (pReqQueryAccount)
	{
		INFO_LOG("\tTradeCode [%s]\n", pReqQueryAccount->TradeCode);
		INFO_LOG("\tBankID [%s]\n", pReqQueryAccount->BankID);
		INFO_LOG("\tBankBranchID [%s]\n", pReqQueryAccount->BankBranchID);
		INFO_LOG("\tBrokerID [%s]\n", pReqQueryAccount->BrokerID);
		INFO_LOG("\tBrokerBranchID [%s]\n", pReqQueryAccount->BrokerBranchID);
		INFO_LOG("\tTradeDate [%s]\n", pReqQueryAccount->TradeDate);
		INFO_LOG("\tTradeTime [%s]\n", pReqQueryAccount->TradeTime);
		INFO_LOG("\tBankSerial [%s]\n", pReqQueryAccount->BankSerial);
		INFO_LOG("\tTradingDay [%s]\n", pReqQueryAccount->TradingDay);
		INFO_LOG("\tCustomerName [%s]\n", pReqQueryAccount->CustomerName);
		INFO_LOG("\tIdentifiedCardNo [%s]\n", pReqQueryAccount->IdentifiedCardNo);
		INFO_LOG("\tBankAccount [%s]\n", pReqQueryAccount->BankAccount);
		INFO_LOG("\tBankPassWord [%s]\n", pReqQueryAccount->BankPassWord);
		INFO_LOG("\tAccountID [%s]\n", pReqQueryAccount->AccountID);
		INFO_LOG("\tPassword [%s]\n", pReqQueryAccount->Password);
		INFO_LOG("\tUserID [%s]\n", pReqQueryAccount->UserID);
		INFO_LOG("\tCurrencyID [%s]\n", pReqQueryAccount->CurrencyID);
		INFO_LOG("\tDigest [%s]\n", pReqQueryAccount->Digest);
		INFO_LOG("\tDeviceID [%s]\n", pReqQueryAccount->DeviceID);
		INFO_LOG("\tBrokerIDByBank [%s]\n", pReqQueryAccount->BrokerIDByBank);
		INFO_LOG("\tBankSecuAcc [%s]\n", pReqQueryAccount->BankSecuAcc);
		INFO_LOG("\tOperNo [%s]\n", pReqQueryAccount->OperNo);
		INFO_LOG("\tLongCustomerName [%s]\n", pReqQueryAccount->LongCustomerName);
		INFO_LOG("\tPlateSerial [%d]\n", pReqQueryAccount->PlateSerial);
		INFO_LOG("\tSessionID [%d]\n", pReqQueryAccount->SessionID);
		INFO_LOG("\tFutureSerial [%d]\n", pReqQueryAccount->FutureSerial);
		INFO_LOG("\tInstallID [%d]\n", pReqQueryAccount->InstallID);
		INFO_LOG("\tRequestID [%d]\n", pReqQueryAccount->RequestID);
		INFO_LOG("\tTID [%d]\n", pReqQueryAccount->TID);
		INFO_LOG("\tLastFragment [%c]\n", pReqQueryAccount->LastFragment);
		INFO_LOG("\tIdCardType [%c]\n", pReqQueryAccount->IdCardType);
		INFO_LOG("\tCustType [%c]\n", pReqQueryAccount->CustType);
		INFO_LOG("\tVerifyCertNoFlag [%c]\n", pReqQueryAccount->VerifyCertNoFlag);
		INFO_LOG("\tBankAccType [%c]\n", pReqQueryAccount->BankAccType);
		INFO_LOG("\tBankSecuAccType [%c]\n", pReqQueryAccount->BankSecuAccType);
		INFO_LOG("\tBankPwdFlag [%c]\n", pReqQueryAccount->BankPwdFlag);
		INFO_LOG("\tSecuPwdFlag [%c]\n", pReqQueryAccount->SecuPwdFlag);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQueryBankAccountMoneyByFuture>\n");

	return m_pApi->ReqQueryBankAccountMoneyByFuture(pReqQueryAccount, nRequestID);
}

///请求查询资金账户
int CTraderApi::ReqQrySecAgentTradingAccount(CThostFtdcQryTradingAccountField *pQryTradingAccount, int nRequestID)
{
	INFO_LOG("<ReqQrySecAgentTradingAccount>\n");
	if (pQryTradingAccount)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQryTradingAccount->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQryTradingAccount->InvestorID);
		INFO_LOG("\tCurrencyID [%s]\n", pQryTradingAccount->CurrencyID);
		INFO_LOG("\tBizType [%c]\n", pQryTradingAccount->BizType);
		INFO_LOG("\tAccountID [%s]\n", pQryTradingAccount->AccountID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQrySecAgentTradingAccount>\n");

	return m_pApi->ReqQrySecAgentTradingAccount(pQryTradingAccount, nRequestID);
};

///请求查询二级代理商资金校验模式
int CTraderApi::ReqQrySecAgentCheckMode(CThostFtdcQrySecAgentCheckModeField *pQrySecAgentCheckMode, int nRequestID)
{
	INFO_LOG("<ReqQrySecAgentCheckMode>\n");
	if (pQrySecAgentCheckMode)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQrySecAgentCheckMode->BrokerID);
		INFO_LOG("\tInvestorID [%s]\n", pQrySecAgentCheckMode->InvestorID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQrySecAgentCheckMode>\n");

	return m_pApi->ReqQrySecAgentCheckMode(pQrySecAgentCheckMode, nRequestID);
};

///注册用户终端信息，用于中继服务器多连接模式
///需要在终端认证成功后，用户登录前调用该接口
int CTraderApi::RegisterUserSystemInfo(CThostFtdcUserSystemInfoField *pUserSystemInfo)
{
	INFO_LOG("<RegisterUserSystemInfo>\n");
	if (pUserSystemInfo)
	{
		INFO_LOG("\tBrokerID [%s]\n", pUserSystemInfo->BrokerID);
		INFO_LOG("\tUserID [%s]\n", pUserSystemInfo->UserID);
		INFO_LOG("\tClientSystemInfoLen [%d]\n", pUserSystemInfo->ClientSystemInfoLen);
		INFO_LOG("\tClientSystemInfo [%s]\n", pUserSystemInfo->ClientSystemInfo);
        TThostFtdcIPAddressType ip;
        gbk2utf8(pUserSystemInfo->ClientPublicIP,ip,sizeof(ip));
		INFO_LOG("\tClientPublicIP [%s]\n", ip);
		INFO_LOG("\tClientIPPort [%d]\n", pUserSystemInfo->ClientIPPort);
        TThostFtdcTimeType loginTime;
        gbk2utf8(pUserSystemInfo->ClientPublicIP, loginTime, sizeof(loginTime));
		INFO_LOG("\tClientLoginTime [%s]\n", loginTime);
		INFO_LOG("\tClientAppID [%s]\n", pUserSystemInfo->ClientAppID);
	}
	INFO_LOG("</RegisterUserSystemInfo>\n");

	return m_pApi->RegisterUserSystemInfo(pUserSystemInfo);
}

///上报用户终端信息，用于中继服务器操作员登录模式
///操作员登录后，可以多次调用该接口上报客户信息
int CTraderApi::SubmitUserSystemInfo(CThostFtdcUserSystemInfoField *pUserSystemInfo)
{
	INFO_LOG("<SubmitUserSystemInfo>\n");
	if (pUserSystemInfo)
	{
		INFO_LOG("\tBrokerID [%s]\n", pUserSystemInfo->BrokerID);
		INFO_LOG("\tUserID [%s]\n", pUserSystemInfo->UserID);
		INFO_LOG("\tClientSystemInfoLen [%d]\n", pUserSystemInfo->ClientSystemInfoLen);
		INFO_LOG("\tClientSystemInfo [%s]\n", pUserSystemInfo->ClientSystemInfo);
		INFO_LOG("\tClientPublicIP [%s]\n", pUserSystemInfo->ClientPublicIP);
		INFO_LOG("\tClientIPPort [%d]\n", pUserSystemInfo->ClientIPPort);
		INFO_LOG("\tClientLoginTime [%s]\n", pUserSystemInfo->ClientLoginTime);
		INFO_LOG("\tClientAppID [%s]\n", pUserSystemInfo->ClientAppID);
	}
	INFO_LOG("</SubmitUserSystemInfo>\n");

	return m_pApi->RegisterUserSystemInfo(pUserSystemInfo);
}

///查询用户当前支持的认证模式
int CTraderApi::ReqUserAuthMethod(CThostFtdcReqUserAuthMethodField *pReqUserAuthMethod, int nRequestID)
{
	INFO_LOG("<ReqUserAuthMethod>\n");
	if (pReqUserAuthMethod)
	{
		INFO_LOG("\tTradingDay [%s]\n", pReqUserAuthMethod->TradingDay);
		INFO_LOG("\tBrokerID [%s]\n", pReqUserAuthMethod->BrokerID);
		INFO_LOG("\tUserID [%s]\n", pReqUserAuthMethod->UserID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqUserAuthMethod>\n");

	return m_pApi->ReqUserAuthMethod(pReqUserAuthMethod, nRequestID);
}

///用户发出获取图形验证码请求
int CTraderApi::ReqGenUserCaptcha(CThostFtdcReqGenUserCaptchaField *pReqGenUserCaptcha, int nRequestID)
{
	INFO_LOG("<ReqGenUserCaptcha>\n");
	if (pReqGenUserCaptcha)
	{
		INFO_LOG("\tTradingDay [%s]\n", pReqGenUserCaptcha->TradingDay);
		INFO_LOG("\tBrokerID [%s]\n", pReqGenUserCaptcha->BrokerID);
		INFO_LOG("\tUserID [%s]\n", pReqGenUserCaptcha->UserID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqGenUserCaptcha>\n");

	return m_pApi->ReqGenUserCaptcha(pReqGenUserCaptcha, nRequestID);
}

///用户发出获取短信验证码请求
int CTraderApi::ReqGenUserText(CThostFtdcReqGenUserTextField *pReqGenUserText, int nRequestID)
{
	INFO_LOG("<ReqGenUserText>\n");
	if (pReqGenUserText)
	{
		INFO_LOG("\tTradingDay [%s]\n", pReqGenUserText->TradingDay);
		INFO_LOG("\tBrokerID [%s]\n", pReqGenUserText->BrokerID);
		INFO_LOG("\tUserID [%s]\n", pReqGenUserText->UserID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqGenUserText>\n");

	return m_pApi->ReqGenUserText(pReqGenUserText, nRequestID);
}

///用户发出带有图片验证码的登陆请求
int CTraderApi::ReqUserLoginWithCaptcha(CThostFtdcReqUserLoginWithCaptchaField *pReqUserLoginWithCaptcha, int nRequestID)
{
	INFO_LOG("<ReqUserLoginWithCaptcha>\n");
	if (pReqUserLoginWithCaptcha)
	{
		INFO_LOG("\tTradingDay [%s]\n", pReqUserLoginWithCaptcha->TradingDay);
		INFO_LOG("\tBrokerID [%s]\n", pReqUserLoginWithCaptcha->BrokerID);
		INFO_LOG("\tUserID [%s]\n", pReqUserLoginWithCaptcha->UserID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqUserLoginWithCaptcha>\n");

	return m_pApi->ReqUserLoginWithCaptcha(pReqUserLoginWithCaptcha, nRequestID);
}

///用户发出带有短信验证码的登陆请求
int CTraderApi::ReqUserLoginWithText(CThostFtdcReqUserLoginWithTextField *pReqUserLoginWithText, int nRequestID)
{
	INFO_LOG("<ReqUserLoginWithText>\n");
	if (pReqUserLoginWithText)
	{
		INFO_LOG("\tTradingDay [%s]\n", pReqUserLoginWithText->TradingDay);
		INFO_LOG("\tBrokerID [%s]\n", pReqUserLoginWithText->BrokerID);
		INFO_LOG("\tUserID [%s]\n", pReqUserLoginWithText->UserID);
		INFO_LOG("\tPassword [%s]\n", pReqUserLoginWithText->Password);
		INFO_LOG("\tUserProductInfo [%s]\n", pReqUserLoginWithText->UserProductInfo);
		INFO_LOG("\tInterfaceProductInfo [%s]\n", pReqUserLoginWithText->InterfaceProductInfo);
		INFO_LOG("\tProtocolInfo [%s]\n", pReqUserLoginWithText->ProtocolInfo);
		INFO_LOG("\tMacAddress [%s]\n", pReqUserLoginWithText->MacAddress);
		INFO_LOG("\tClientIPAddress [%s]\n", pReqUserLoginWithText->ClientIPAddress);
		INFO_LOG("\tLoginRemark [%s]\n", pReqUserLoginWithText->LoginRemark);
		INFO_LOG("\tText [%s]\n", pReqUserLoginWithText->Text);
		INFO_LOG("\tClientIPPort [%d]\n", pReqUserLoginWithText->ClientIPPort);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqUserLoginWithText>\n");

	return m_pApi->ReqUserLoginWithText(pReqUserLoginWithText, nRequestID);
}

///用户发出带有动态口令的登陆请求
int CTraderApi::ReqUserLoginWithOTP(CThostFtdcReqUserLoginWithOTPField *pReqUserLoginWithOTP, int nRequestID)
{
	INFO_LOG("<ReqUserLoginWithOTP>\n");
	if (pReqUserLoginWithOTP)
	{
		INFO_LOG("\tTradingDay [%s]\n", pReqUserLoginWithOTP->TradingDay);
		INFO_LOG("\tBrokerID [%s]\n", pReqUserLoginWithOTP->BrokerID);
		INFO_LOG("\tUserID [%s]\n", pReqUserLoginWithOTP->UserID);
		INFO_LOG("\tPassword [%s]\n", pReqUserLoginWithOTP->Password);
		INFO_LOG("\tUserProductInfo [%s]\n", pReqUserLoginWithOTP->UserProductInfo);
		INFO_LOG("\tInterfaceProductInfo [%s]\n", pReqUserLoginWithOTP->InterfaceProductInfo);
		INFO_LOG("\tProtocolInfo [%s]\n", pReqUserLoginWithOTP->ProtocolInfo);
		INFO_LOG("\tMacAddress [%s]\n", pReqUserLoginWithOTP->MacAddress);
		INFO_LOG("\tClientIPAddress [%s]\n", pReqUserLoginWithOTP->ClientIPAddress);
		INFO_LOG("\tLoginRemark [%s]\n", pReqUserLoginWithOTP->LoginRemark);
		INFO_LOG("\tOTPPassword [%s]\n", pReqUserLoginWithOTP->OTPPassword);
		INFO_LOG("\tClientIPPort [%d]\n", pReqUserLoginWithOTP->ClientIPPort);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqUserLoginWithOTP>\n");

	return m_pApi->ReqUserLoginWithOTP(pReqUserLoginWithOTP, nRequestID);
}

///请求查询二级代理商信息
int CTraderApi::ReqQrySecAgentTradeInfo(CThostFtdcQrySecAgentTradeInfoField *pQrySecAgentTradeInfo, int nRequestID)
{
	INFO_LOG("<ReqQrySecAgentTradeInfo>\n");
	if (pQrySecAgentTradeInfo)
	{
		INFO_LOG("\tBrokerID [%s]\n", pQrySecAgentTradeInfo->BrokerID);
		INFO_LOG("\tBrokerSecAgentID [%s]\n", pQrySecAgentTradeInfo->BrokerSecAgentID);
	}
	INFO_LOG("\tnRequestID [%d]\n", nRequestID);
	INFO_LOG("</ReqQrySecAgentTradeInfo>\n");

	return m_pApi->ReqQrySecAgentTradeInfo(pQrySecAgentTradeInfo, nRequestID);
}
