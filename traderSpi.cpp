//#include "stdafx.h"
#include "traderSpi.h"
#include <stdio.h>
#include <semaphore.h>
#include "define.h"
#include "trader.h"
#include "log.h"
#include "semaphorePart.h"
extern GlobalSem globalSem;
//extern sem_t sem;
extern InsertResult repResult;
void CTraderSpi::OnFrontConnected()
{
    PURE_LOG("<OnFrontConnected>");
    PURE_LOG("</OnFrontConnected>");
};

void CTraderSpi::OnFrontDisconnected(int nReason)
{
    PURE_LOG("<OnFrontDisconnected>");
    DEBUG_LOG("for check time");
    PURE_LOG("\tnReason [%d]", nReason);
    PURE_LOG("</OnFrontDisconnected>");
}

void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
    PURE_LOG("<OnHeartBeatWarning>");
    PURE_LOG("\tnTimeLapse [%d]", nTimeLapse);
    PURE_LOG("</OnHeartBeatWarning>");
}

void CTraderSpi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    MSG_LOG("%s","<OnRspAuthenticate>");
	if (pRspAuthenticateField)
	{
        PURE_LOG("\tBrokerID [%s]", pRspAuthenticateField->BrokerID);
        PURE_LOG("\tUserID [%s]", pRspAuthenticateField->UserID);
        PURE_LOG("\tUserProductInfo [%s]", pRspAuthenticateField->UserProductInfo);
        PURE_LOG("\tAppID [%s]", pRspAuthenticateField->AppID);
        PURE_LOG("\tAppType [%c]", pRspAuthenticateField->AppType);
	}
	if (pRspInfo)
	{   
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    MSG_LOG("%s","</OnRspAuthenticate>");
    INFO_LOG("%s","ReqAuthenticate OK");
};

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    MSG_LOG("%s","<OnRspUserLogin>");
	if (pRspUserLogin)
	{
        PURE_LOG("\tTradingDay [%s]", pRspUserLogin->TradingDay);
        PURE_LOG("\tLoginTime [%s]", pRspUserLogin->LoginTime);
        PURE_LOG("\tBrokerID [%s]", pRspUserLogin->BrokerID);
        PURE_LOG("\tUserID [%s]", pRspUserLogin->UserID);
        PURE_LOG("\tSystemName [%s]", pRspUserLogin->SystemName);
        PURE_LOG("\tMaxOrderRef [%s]", pRspUserLogin->MaxOrderRef);
        PURE_LOG("\tSHFETime [%s]", pRspUserLogin->SHFETime);
        PURE_LOG("\tDCETime [%s]", pRspUserLogin->DCETime);
        PURE_LOG("\tCZCETime [%s]", pRspUserLogin->CZCETime);
        PURE_LOG("\tFFEXTime [%s]", pRspUserLogin->FFEXTime);
        PURE_LOG("\tINETime [%s]", pRspUserLogin->INETime);
        PURE_LOG("\tFrontID [%d]", pRspUserLogin->FrontID);
        PURE_LOG("\tSessionID [%d]", pRspUserLogin->SessionID);
	}
    TThostFtdcErrorMsgType msg;
    gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
	if (pRspInfo)
	{   
        
        PURE_LOG("\tErrorMsg [%s]", msg);
        //TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tErrorMsg [%s]", msg);
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    MSG_LOG("%s","</OnRspUserLogin>");
};

void CTraderSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspUserLogout>");
	if (pUserLogout)
	{
        PURE_LOG("\tBrokerID [%s]", pUserLogout->BrokerID);
        PURE_LOG("\tUserID [%s]", pUserLogout->UserID);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspUserLogout>");
};

void CTraderSpi::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspUserPasswordUpdate>");
	if (pUserPasswordUpdate)
	{
        PURE_LOG("\tBrokerID [%s]", pUserPasswordUpdate->BrokerID);
        PURE_LOG("\tUserID [%s]", pUserPasswordUpdate->UserID);
        PURE_LOG("\tOldPassword [%s]", pUserPasswordUpdate->OldPassword);
        PURE_LOG("\tNewPassword [%s]", pUserPasswordUpdate->NewPassword);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspUserPasswordUpdate>");
};

void CTraderSpi::OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspTradingAccountPasswordUpdate>");
	if (pTradingAccountPasswordUpdate)
	{
        PURE_LOG("\tBrokerID [%s]", pTradingAccountPasswordUpdate->BrokerID);
        PURE_LOG("\tAccountID [%s]", pTradingAccountPasswordUpdate->AccountID);
        PURE_LOG("\tOldPassword [%s]", pTradingAccountPasswordUpdate->OldPassword);
        PURE_LOG("\tNewPassword [%s]", pTradingAccountPasswordUpdate->NewPassword);
        PURE_LOG("\tCurrencyID [%s]", pTradingAccountPasswordUpdate->CurrencyID);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
		
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspTradingAccountPasswordUpdate>");
};

void CTraderSpi::OnRspUserAuthMethod(CThostFtdcRspUserAuthMethodField *pRspUserAuthMethod, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspUserAuthMethod>");
	if (pRspUserAuthMethod)
	{
        PURE_LOG("\tUsableAuthMethod [%d]", pRspUserAuthMethod->UsableAuthMethod);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
		
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspUserAuthMethod>");
};

void CTraderSpi::OnRspGenUserCaptcha(CThostFtdcRspGenUserCaptchaField *pRspGenUserCaptcha, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspGenUserCaptcha>");
	if (pRspGenUserCaptcha)
	{
        PURE_LOG("\tBrokerID [%s]", pRspGenUserCaptcha->BrokerID);
        PURE_LOG("\tUserID [%s]", pRspGenUserCaptcha->UserID);
        PURE_LOG("\tCaptchaInfo [%s]", pRspGenUserCaptcha->CaptchaInfo);
        PURE_LOG("\tCaptchaInfoLen [%d]", pRspGenUserCaptcha->CaptchaInfoLen);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
		
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspGenUserCaptcha>");
};

void CTraderSpi::OnRspGenUserText(CThostFtdcRspGenUserTextField *pRspGenUserText, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspGenUserText>");
	if (pRspGenUserText)
	{
        PURE_LOG("\tUserTextSeq [%d]", pRspGenUserText->UserTextSeq);
	}
	if (pRspInfo)
	{   
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
		
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspGenUserText>");
};

void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspOrderInsert>");
	if (pInputOrder)
	{
        PURE_LOG("\tBrokerID [%s]", pInputOrder->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInputOrder->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pInputOrder->InstrumentID);
        PURE_LOG("\tOrderRef [%s]", pInputOrder->OrderRef);
        PURE_LOG("\tUserID [%s]", pInputOrder->UserID);
        PURE_LOG("\tCombOffsetFlag [%s]", pInputOrder->CombOffsetFlag);
        PURE_LOG("\tCombHedgeFlag [%s]", pInputOrder->CombHedgeFlag);
        PURE_LOG("\tGTDDate [%s]", pInputOrder->GTDDate);
        PURE_LOG("\tBusinessUnit [%s]", pInputOrder->BusinessUnit);
        PURE_LOG("\tExchangeID [%s]", pInputOrder->ExchangeID);
        PURE_LOG("\tInvestUnitID [%s]", pInputOrder->InvestUnitID);
        PURE_LOG("\tAccountID [%s]", pInputOrder->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pInputOrder->CurrencyID);
        PURE_LOG("\tClientID [%s]", pInputOrder->ClientID);
        PURE_LOG("\tIPAddress [%s]", pInputOrder->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pInputOrder->MacAddress);
        PURE_LOG("\tVolumeTotalOriginal [%d]", pInputOrder->VolumeTotalOriginal);
        PURE_LOG("\tMinVolume [%d]", pInputOrder->MinVolume);
        PURE_LOG("\tIsAutoSuspend [%d]", pInputOrder->IsAutoSuspend);
        PURE_LOG("\tRequestID [%d]", pInputOrder->RequestID);
        PURE_LOG("\tUserForceClose [%d]", pInputOrder->UserForceClose);
        PURE_LOG("\tIsSwapOrder [%d]", pInputOrder->IsSwapOrder);
        PURE_LOG("\tOrderPriceType [%c]", pInputOrder->OrderPriceType);
        PURE_LOG("\tDirection [%c]", pInputOrder->Direction);
        PURE_LOG("\tTimeCondition [%c]", pInputOrder->TimeCondition);
        PURE_LOG("\tVolumeCondition [%c]", pInputOrder->VolumeCondition);
        PURE_LOG("\tContingentCondition [%c]", pInputOrder->ContingentCondition);
        PURE_LOG("\tForceCloseReason [%c]", pInputOrder->ForceCloseReason);
        PURE_LOG("\tLimitPrice [%.8lf]", pInputOrder->LimitPrice);
        PURE_LOG("\tStopPrice [%.8lf]", pInputOrder->StopPrice);
	}
	if (pRspInfo)
	{   
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
		
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);

	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspOrderInsert>");
};

void CTraderSpi::OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspParkedOrderInsert>");
	if (pParkedOrder)
	{
        PURE_LOG("\tBrokerID [%s]", pParkedOrder->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pParkedOrder->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pParkedOrder->InstrumentID);
        PURE_LOG("\tOrderRef [%s]", pParkedOrder->OrderRef);
        PURE_LOG("\tUserID [%s]", pParkedOrder->UserID);
        PURE_LOG("\tCombOffsetFlag [%s]", pParkedOrder->CombOffsetFlag);
        PURE_LOG("\tCombHedgeFlag [%s]", pParkedOrder->CombHedgeFlag);
        PURE_LOG("\tGTDDate [%s]", pParkedOrder->GTDDate);
        PURE_LOG("\tBusinessUnit [%s]", pParkedOrder->BusinessUnit);
        PURE_LOG("\tExchangeID [%s]", pParkedOrder->ExchangeID);
        PURE_LOG("\tParkedOrderID [%s]", pParkedOrder->ParkedOrderID);

        TThostFtdcErrorMsgType msg;
        gbk2utf8(pParkedOrder->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
		
        PURE_LOG("\tAccountID [%s]", pParkedOrder->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pParkedOrder->CurrencyID);
        PURE_LOG("\tClientID [%s]", pParkedOrder->ClientID);
        PURE_LOG("\tInvestUnitID [%s]", pParkedOrder->InvestUnitID);
        PURE_LOG("\tIPAddress [%s]", pParkedOrder->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pParkedOrder->MacAddress);
        PURE_LOG("\tVolumeTotalOriginal [%d]", pParkedOrder->VolumeTotalOriginal);
        PURE_LOG("\tMinVolume [%d]", pParkedOrder->MinVolume);
        PURE_LOG("\tIsAutoSuspend [%d]", pParkedOrder->IsAutoSuspend);
        PURE_LOG("\tRequestID [%d]", pParkedOrder->RequestID);
        PURE_LOG("\tUserForceClose [%d]", pParkedOrder->UserForceClose);
        PURE_LOG("\tErrorID [%d]", pParkedOrder->ErrorID);
        PURE_LOG("\tIsSwapOrder [%d]", pParkedOrder->IsSwapOrder);
        PURE_LOG("\tOrderPriceType [%c]", pParkedOrder->OrderPriceType);
        PURE_LOG("\tDirection [%c]", pParkedOrder->Direction);
        PURE_LOG("\tTimeCondition [%c]", pParkedOrder->TimeCondition);
        PURE_LOG("\tVolumeCondition [%c]", pParkedOrder->VolumeCondition);
        PURE_LOG("\tContingentCondition [%c]", pParkedOrder->ContingentCondition);
        PURE_LOG("\tForceCloseReason [%c]", pParkedOrder->ForceCloseReason);
        PURE_LOG("\tUserType [%c]", pParkedOrder->UserType);
        PURE_LOG("\tStatus [%c]", pParkedOrder->Status);
        PURE_LOG("\tLimitPrice [%.8lf]", pParkedOrder->LimitPrice);
        PURE_LOG("\tStopPrice [%.8lf]", pParkedOrder->StopPrice);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspParkedOrderInsert>");
};

void CTraderSpi::OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspParkedOrderAction>");
	if (pParkedOrderAction)
	{
        PURE_LOG("\tBrokerID [%s]", pParkedOrderAction->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pParkedOrderAction->InvestorID);
        PURE_LOG("\tOrderRef [%s]", pParkedOrderAction->OrderRef);
        PURE_LOG("\tExchangeID [%s]", pParkedOrderAction->ExchangeID);
        PURE_LOG("\tOrderSysID [%s]", pParkedOrderAction->OrderSysID);
        PURE_LOG("\tUserID [%s]", pParkedOrderAction->UserID);
        PURE_LOG("\tInstrumentID [%s]", pParkedOrderAction->InstrumentID);
        PURE_LOG("\tParkedOrderActionID [%s]", pParkedOrderAction->ParkedOrderActionID);

        TThostFtdcErrorMsgType msg;
        gbk2utf8(pParkedOrderAction->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
		
        PURE_LOG("\tInvestUnitID [%s]", pParkedOrderAction->InvestUnitID);
        PURE_LOG("\tIPAddress [%s]", pParkedOrderAction->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pParkedOrderAction->MacAddress);
        PURE_LOG("\tOrderActionRef [%d]", pParkedOrderAction->OrderActionRef);
        PURE_LOG("\tRequestID [%d]", pParkedOrderAction->RequestID);
        PURE_LOG("\tFrontID [%d]", pParkedOrderAction->FrontID);
        PURE_LOG("\tSessionID [%d]", pParkedOrderAction->SessionID);
        PURE_LOG("\tVolumeChange [%d]", pParkedOrderAction->VolumeChange);
        PURE_LOG("\tErrorID [%d]", pParkedOrderAction->ErrorID);
        PURE_LOG("\tActionFlag [%c]", pParkedOrderAction->ActionFlag);
        PURE_LOG("\tUserType [%c]", pParkedOrderAction->UserType);
        PURE_LOG("\tStatus [%c]", pParkedOrderAction->Status);
        PURE_LOG("\tLimitPrice [%.8lf]", pParkedOrderAction->LimitPrice);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspParkedOrderAction>");
};

void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspOrderAction>");
	if (pInputOrderAction)
	{
        PURE_LOG("\tBrokerID [%s]", pInputOrderAction->BrokerID);
        //fPURE_LOG(outfile_demo, "BrokerID:[%s]\t", pInputOrderAction->BrokerID);

        PURE_LOG("\tInvestorID [%s]", pInputOrderAction->InvestorID);
        //fPURE_LOG(outfile_demo, "InvestorID:[%s]\t", pInputOrderAction->InvestorID);

        PURE_LOG("\tOrderRef [%s]", pInputOrderAction->OrderRef);
        PURE_LOG("\tExchangeID [%s]", pInputOrderAction->ExchangeID);
        //fPURE_LOG(outfile_demo, "ExchangeID:[%s]\t", pInputOrderAction->ExchangeID);

        PURE_LOG("\tOrderSysID [%s]", pInputOrderAction->OrderSysID);
        //fPURE_LOG(outfile_demo, "OrderSysID:[%s]\t", pInputOrderAction->OrderSysID);

        PURE_LOG("\tUserID [%s]", pInputOrderAction->UserID);
        //fPURE_LOG(outfile_demo, "UserID:[%s]\t", pInputOrderAction->UserID);

        PURE_LOG("\tInstrumentID [%s]", pInputOrderAction->InstrumentID);
        //fPURE_LOG(outfile_demo, "InstrumentID:[%s]\t", pInputOrderAction->InstrumentID);

        PURE_LOG("\tInvestUnitID [%s]", pInputOrderAction->InvestUnitID);
        PURE_LOG("\tIPAddress [%s]", pInputOrderAction->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pInputOrderAction->MacAddress);
        PURE_LOG("\tOrderActionRef [%d]", pInputOrderAction->OrderActionRef);
        PURE_LOG("\tRequestID [%d]", pInputOrderAction->RequestID);
        PURE_LOG("\tFrontID [%d]", pInputOrderAction->FrontID);
        PURE_LOG("\tSessionID [%d]", pInputOrderAction->SessionID);
        //fPURE_LOG(outfile_demo, "SessionID:[%c]\t", pInputOrderAction->SessionID);

        PURE_LOG("\tVolumeChange [%d]", pInputOrderAction->VolumeChange);
        PURE_LOG("\tActionFlag [%c]", pInputOrderAction->ActionFlag);
        //fPURE_LOG(outfile_demo, "ActionFlag:[%c]", pInputOrderAction->ActionFlag);

        PURE_LOG("\tLimitPrice [%.8lf]", pInputOrderAction->LimitPrice);
	}
	if (pRspInfo)
	{   
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg,msg,sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspOrderAction>");
//	fflush(outfile_demo);
};

void CTraderSpi::OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQueryMaxOrderVolume>");
	if (pQueryMaxOrderVolume)
	{
        PURE_LOG("\tBrokerID [%s]", pQueryMaxOrderVolume->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pQueryMaxOrderVolume->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pQueryMaxOrderVolume->InstrumentID);
        PURE_LOG("\tExchangeID [%s]", pQueryMaxOrderVolume->ExchangeID);
        PURE_LOG("\tInvestUnitID [%s]", pQueryMaxOrderVolume->InvestUnitID);
        PURE_LOG("\tMaxVolume [%d]", pQueryMaxOrderVolume->MaxVolume);
        PURE_LOG("\tDirection [%c]", pQueryMaxOrderVolume->Direction);
        PURE_LOG("\tOffsetFlag [%c]", pQueryMaxOrderVolume->OffsetFlag);
        PURE_LOG("\tHedgeFlag [%c]", pQueryMaxOrderVolume->HedgeFlag);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQueryMaxOrderVolume>");
};

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspSettlementInfoConfirm>");
	if (pSettlementInfoConfirm)
	{
        PURE_LOG("\tBrokerID [%s]", pSettlementInfoConfirm->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pSettlementInfoConfirm->InvestorID);
        PURE_LOG("\tConfirmDate [%s]", pSettlementInfoConfirm->ConfirmDate);
        PURE_LOG("\tConfirmTime [%s]", pSettlementInfoConfirm->ConfirmTime);
        PURE_LOG("\tAccountID [%s]", pSettlementInfoConfirm->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pSettlementInfoConfirm->CurrencyID);
        PURE_LOG("\tSettlementID [%d]", pSettlementInfoConfirm->SettlementID);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspSettlementInfoConfirm>");
};

void CTraderSpi::OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspRemoveParkedOrder>");
	if (pRemoveParkedOrder)
	{
        PURE_LOG("\tBrokerID [%s]", pRemoveParkedOrder->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pRemoveParkedOrder->InvestorID);
        PURE_LOG("\tParkedOrderID [%s]", pRemoveParkedOrder->ParkedOrderID);
        PURE_LOG("\tInvestUnitID [%s]", pRemoveParkedOrder->InvestUnitID);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspRemoveParkedOrder>");
};

void CTraderSpi::OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspRemoveParkedOrderAction>");
	if (pRemoveParkedOrderAction)
	{
        PURE_LOG("\tBrokerID [%s]", pRemoveParkedOrderAction->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pRemoveParkedOrderAction->InvestorID);
        PURE_LOG("\tParkedOrderActionID [%s]", pRemoveParkedOrderAction->ParkedOrderActionID);
        PURE_LOG("\tInvestUnitID [%s]", pRemoveParkedOrderAction->InvestUnitID);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspRemoveParkedOrderAction>");
};

void CTraderSpi::OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspExecOrderInsert>");
	if (pInputExecOrder)
	{
        PURE_LOG("\tBrokerID [%s]", pInputExecOrder->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInputExecOrder->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pInputExecOrder->InstrumentID);
        PURE_LOG("\tExecOrderRef [%s]", pInputExecOrder->ExecOrderRef);
        PURE_LOG("\tUserID [%s]", pInputExecOrder->UserID);
        PURE_LOG("\tBusinessUnit [%s]", pInputExecOrder->BusinessUnit);
        PURE_LOG("\tExchangeID [%s]", pInputExecOrder->ExchangeID);
        PURE_LOG("\tInvestUnitID [%s]", pInputExecOrder->InvestUnitID);
        PURE_LOG("\tAccountID [%s]", pInputExecOrder->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pInputExecOrder->CurrencyID);
        PURE_LOG("\tClientID [%s]", pInputExecOrder->ClientID);
        PURE_LOG("\tIPAddress [%s]", pInputExecOrder->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pInputExecOrder->MacAddress);
        PURE_LOG("\tVolume [%d]", pInputExecOrder->Volume);
        PURE_LOG("\tRequestID [%d]", pInputExecOrder->RequestID);
        PURE_LOG("\tOffsetFlag [%c]", pInputExecOrder->OffsetFlag);
        PURE_LOG("\tHedgeFlag [%c]", pInputExecOrder->HedgeFlag);
        PURE_LOG("\tActionType [%c]", pInputExecOrder->ActionType);
        PURE_LOG("\tPosiDirection [%c]", pInputExecOrder->PosiDirection);
        PURE_LOG("\tReservePositionFlag [%c]", pInputExecOrder->ReservePositionFlag);
        PURE_LOG("\tCloseFlag [%c]", pInputExecOrder->CloseFlag);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspExecOrderInsert>");
};

void CTraderSpi::OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspExecOrderAction>");
	if (pInputExecOrderAction)
	{
        PURE_LOG("\tBrokerID [%s]", pInputExecOrderAction->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInputExecOrderAction->InvestorID);
        PURE_LOG("\tExecOrderRef [%s]", pInputExecOrderAction->ExecOrderRef);
        PURE_LOG("\tExchangeID [%s]", pInputExecOrderAction->ExchangeID);
        PURE_LOG("\tExecOrderSysID [%s]", pInputExecOrderAction->ExecOrderSysID);
        PURE_LOG("\tUserID [%s]", pInputExecOrderAction->UserID);
        PURE_LOG("\tInstrumentID [%s]", pInputExecOrderAction->InstrumentID);
        PURE_LOG("\tInvestUnitID [%s]", pInputExecOrderAction->InvestUnitID);
        PURE_LOG("\tIPAddress [%s]", pInputExecOrderAction->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pInputExecOrderAction->MacAddress);
        PURE_LOG("\tExecOrderActionRef [%d]", pInputExecOrderAction->ExecOrderActionRef);
        PURE_LOG("\tRequestID [%d]", pInputExecOrderAction->RequestID);
        PURE_LOG("\tFrontID [%d]", pInputExecOrderAction->FrontID);
        PURE_LOG("\tSessionID [%d]", pInputExecOrderAction->SessionID);
        PURE_LOG("\tActionFlag [%c]", pInputExecOrderAction->ActionFlag);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspExecOrderAction>");
};

void CTraderSpi::OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspForQuoteInsert>");
	if (pInputForQuote)
	{
        PURE_LOG("\tBrokerID [%s]", pInputForQuote->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInputForQuote->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pInputForQuote->InstrumentID);
        PURE_LOG("\tForQuoteRef [%s]", pInputForQuote->ForQuoteRef);
        PURE_LOG("\tUserID [%s]", pInputForQuote->UserID);
        PURE_LOG("\tExchangeID [%s]", pInputForQuote->ExchangeID);
        PURE_LOG("\tInvestUnitID [%s]", pInputForQuote->InvestUnitID);
        PURE_LOG("\tIPAddress [%s]", pInputForQuote->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pInputForQuote->MacAddress);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);;
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspForQuoteInsert>");
};

void CTraderSpi::OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQuoteInsert>");
	if (pInputQuote)
	{
        PURE_LOG("\tBrokerID [%s]", pInputQuote->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInputQuote->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pInputQuote->InstrumentID);
        PURE_LOG("\tQuoteRef [%s]", pInputQuote->QuoteRef);
        PURE_LOG("\tUserID [%s]", pInputQuote->UserID);
        PURE_LOG("\tBusinessUnit [%s]", pInputQuote->BusinessUnit);
        PURE_LOG("\tAskOrderRef [%s]", pInputQuote->AskOrderRef);
        PURE_LOG("\tBidOrderRef [%s]", pInputQuote->BidOrderRef);
        PURE_LOG("\tForQuoteSysID [%s]", pInputQuote->ForQuoteSysID);
        PURE_LOG("\tExchangeID [%s]", pInputQuote->ExchangeID);
        PURE_LOG("\tInvestUnitID [%s]", pInputQuote->InvestUnitID);
        PURE_LOG("\tClientID [%s]", pInputQuote->ClientID);
        PURE_LOG("\tIPAddress [%s]", pInputQuote->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pInputQuote->MacAddress);
        PURE_LOG("\tAskVolume [%d]", pInputQuote->AskVolume);
        PURE_LOG("\tBidVolume [%d]", pInputQuote->BidVolume);
        PURE_LOG("\tRequestID [%d]", pInputQuote->RequestID);
        PURE_LOG("\tAskOffsetFlag [%c]", pInputQuote->AskOffsetFlag);
        PURE_LOG("\tBidOffsetFlag [%c]", pInputQuote->BidOffsetFlag);
        PURE_LOG("\tAskHedgeFlag [%c]", pInputQuote->AskHedgeFlag);
        PURE_LOG("\tBidHedgeFlag [%c]", pInputQuote->BidHedgeFlag);
        PURE_LOG("\tAskPrice [%.8lf]", pInputQuote->AskPrice);
        PURE_LOG("\tBidPrice [%.8lf]", pInputQuote->BidPrice);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQuoteInsert>");
};

void CTraderSpi::OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQuoteAction>");
	if (pInputQuoteAction)
	{
        PURE_LOG("\tBrokerID [%s]", pInputQuoteAction->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInputQuoteAction->InvestorID);
        PURE_LOG("\tQuoteRef [%s]", pInputQuoteAction->QuoteRef);
        PURE_LOG("\tExchangeID [%s]", pInputQuoteAction->ExchangeID);
        PURE_LOG("\tQuoteSysID [%s]", pInputQuoteAction->QuoteSysID);
        PURE_LOG("\tUserID [%s]", pInputQuoteAction->UserID);
        PURE_LOG("\tInstrumentID [%s]", pInputQuoteAction->InstrumentID);
        PURE_LOG("\tInvestUnitID [%s]", pInputQuoteAction->InvestUnitID);
        PURE_LOG("\tClientID [%s]", pInputQuoteAction->ClientID);
        PURE_LOG("\tIPAddress [%s]", pInputQuoteAction->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pInputQuoteAction->MacAddress);
        PURE_LOG("\tQuoteActionRef [%d]", pInputQuoteAction->QuoteActionRef);
        PURE_LOG("\tRequestID [%d]", pInputQuoteAction->RequestID);
        PURE_LOG("\tFrontID [%d]", pInputQuoteAction->FrontID);
        PURE_LOG("\tSessionID [%d]", pInputQuoteAction->SessionID);
        PURE_LOG("\tActionFlag [%c]", pInputQuoteAction->ActionFlag);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQuoteAction>");
};

void CTraderSpi::OnRspBatchOrderAction(CThostFtdcInputBatchOrderActionField *pInputBatchOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspBatchOrderAction>");
	if (pInputBatchOrderAction)
	{
        PURE_LOG("\tBrokerID [%s]", pInputBatchOrderAction->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInputBatchOrderAction->InvestorID);
        PURE_LOG("\tExchangeID [%s]", pInputBatchOrderAction->ExchangeID);
        PURE_LOG("\tUserID [%s]", pInputBatchOrderAction->UserID);
        PURE_LOG("\tInvestUnitID [%s]", pInputBatchOrderAction->InvestUnitID);
        PURE_LOG("\tIPAddress [%s]", pInputBatchOrderAction->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pInputBatchOrderAction->MacAddress);
        PURE_LOG("\tOrderActionRef [%d]", pInputBatchOrderAction->OrderActionRef);
        PURE_LOG("\tRequestID [%d]", pInputBatchOrderAction->RequestID);
        PURE_LOG("\tFrontID [%d]", pInputBatchOrderAction->FrontID);
        PURE_LOG("\tSessionID [%d]", pInputBatchOrderAction->SessionID);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspBatchOrderAction>");
};

void CTraderSpi::OnRspOptionSelfCloseInsert(CThostFtdcInputOptionSelfCloseField *pInputOptionSelfClose, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspOptionSelfCloseInsert>");
	if (pInputOptionSelfClose)
	{
        PURE_LOG("\tBrokerID [%s]", pInputOptionSelfClose->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInputOptionSelfClose->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pInputOptionSelfClose->InstrumentID);
        PURE_LOG("\tOptionSelfCloseRef [%s]", pInputOptionSelfClose->OptionSelfCloseRef);
        PURE_LOG("\tUserID [%s]", pInputOptionSelfClose->UserID);
        PURE_LOG("\tBusinessUnit [%s]", pInputOptionSelfClose->BusinessUnit);
        PURE_LOG("\tExchangeID [%s]", pInputOptionSelfClose->ExchangeID);
        PURE_LOG("\tInvestUnitID [%s]", pInputOptionSelfClose->InvestUnitID);
        PURE_LOG("\tAccountID [%s]", pInputOptionSelfClose->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pInputOptionSelfClose->CurrencyID);
        PURE_LOG("\tClientID [%s]", pInputOptionSelfClose->ClientID);
        PURE_LOG("\tIPAddress [%s]", pInputOptionSelfClose->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pInputOptionSelfClose->MacAddress);
        PURE_LOG("\tVolume [%d]", pInputOptionSelfClose->Volume);
        PURE_LOG("\tRequestID [%d]", pInputOptionSelfClose->RequestID);
        PURE_LOG("\tHedgeFlag [%c]", pInputOptionSelfClose->HedgeFlag);
        PURE_LOG("\tOptSelfCloseFlag [%c]", pInputOptionSelfClose->OptSelfCloseFlag);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);

        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspOptionSelfCloseInsert>");
};

void CTraderSpi::OnRspOptionSelfCloseAction(CThostFtdcInputOptionSelfCloseActionField *pInputOptionSelfCloseAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspOptionSelfCloseAction>");
	if (pInputOptionSelfCloseAction)
	{
        PURE_LOG("\tBrokerID [%s]", pInputOptionSelfCloseAction->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInputOptionSelfCloseAction->InvestorID);
        PURE_LOG("\tOptionSelfCloseRef [%s]", pInputOptionSelfCloseAction->OptionSelfCloseRef);
        PURE_LOG("\tExchangeID [%s]", pInputOptionSelfCloseAction->ExchangeID);
        PURE_LOG("\tOptionSelfCloseSysID [%s]", pInputOptionSelfCloseAction->OptionSelfCloseSysID);
        PURE_LOG("\tUserID [%s]", pInputOptionSelfCloseAction->UserID);
        PURE_LOG("\tInstrumentID [%s]", pInputOptionSelfCloseAction->InstrumentID);
        PURE_LOG("\tInvestUnitID [%s]", pInputOptionSelfCloseAction->InvestUnitID);
        PURE_LOG("\tIPAddress [%s]", pInputOptionSelfCloseAction->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pInputOptionSelfCloseAction->MacAddress);
        PURE_LOG("\tOptionSelfCloseActionRef [%d]", pInputOptionSelfCloseAction->OptionSelfCloseActionRef);
        PURE_LOG("\tRequestID [%d]", pInputOptionSelfCloseAction->RequestID);
        PURE_LOG("\tFrontID [%d]", pInputOptionSelfCloseAction->FrontID);
        PURE_LOG("\tSessionID [%d]", pInputOptionSelfCloseAction->SessionID);
        PURE_LOG("\tActionFlag [%c]", pInputOptionSelfCloseAction->ActionFlag);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspOptionSelfCloseAction>");
};

void CTraderSpi::OnRspCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspCombActionInsert>");
	if (pInputCombAction)
	{
        PURE_LOG("\tBrokerID [%s]", pInputCombAction->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInputCombAction->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pInputCombAction->InstrumentID);
        PURE_LOG("\tCombActionRef [%s]", pInputCombAction->CombActionRef);
        PURE_LOG("\tUserID [%s]", pInputCombAction->UserID);
        PURE_LOG("\tExchangeID [%s]", pInputCombAction->ExchangeID);
        PURE_LOG("\tIPAddress [%s]", pInputCombAction->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pInputCombAction->MacAddress);
        PURE_LOG("\tInvestUnitID [%s]", pInputCombAction->InvestUnitID);
        PURE_LOG("\tVolume [%d]", pInputCombAction->Volume);
        PURE_LOG("\tDirection [%c]", pInputCombAction->Direction);
        PURE_LOG("\tCombDirection [%c]", pInputCombAction->CombDirection);
        PURE_LOG("\tHedgeFlag [%c]", pInputCombAction->HedgeFlag);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspCombActionInsert>");
};

void CTraderSpi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryOrder>");
	if (pOrder)
	{
        PURE_LOG("\tBrokerID [%s]", pOrder->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pOrder->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pOrder->InstrumentID);
        PURE_LOG("\tOrderRef [%s]", pOrder->OrderRef);
        PURE_LOG("\tUserID [%s]", pOrder->UserID);
        PURE_LOG("\tCombOffsetFlag [%s]", pOrder->CombOffsetFlag);
        PURE_LOG("\tCombHedgeFlag [%s]", pOrder->CombHedgeFlag);
        PURE_LOG("\tGTDDate [%s]", pOrder->GTDDate);
        PURE_LOG("\tBusinessUnit [%s]", pOrder->BusinessUnit);
        PURE_LOG("\tOrderLocalID [%s]", pOrder->OrderLocalID);
        PURE_LOG("\tExchangeID [%s]", pOrder->ExchangeID);
        PURE_LOG("\tParticipantID [%s]", pOrder->ParticipantID);
        PURE_LOG("\tClientID [%s]", pOrder->ClientID);
        PURE_LOG("\tExchangeInstID [%s]", pOrder->ExchangeInstID);
        PURE_LOG("\tTraderID [%s]", pOrder->TraderID);
        PURE_LOG("\tTradingDay [%s]", pOrder->TradingDay);
        PURE_LOG("\tOrderSysID [%s]", pOrder->OrderSysID);
        PURE_LOG("\tInsertDate [%s]", pOrder->InsertDate);
        PURE_LOG("\tInsertTime [%s]", pOrder->InsertTime);
        PURE_LOG("\tActiveTime [%s]", pOrder->ActiveTime);
        PURE_LOG("\tSuspendTime [%s]", pOrder->SuspendTime);
        PURE_LOG("\tUpdateTime [%s]", pOrder->UpdateTime);
        PURE_LOG("\tCancelTime [%s]", pOrder->CancelTime);
        PURE_LOG("\tActiveTraderID [%s]", pOrder->ActiveTraderID);
        PURE_LOG("\tClearingPartID [%s]", pOrder->ClearingPartID);
        PURE_LOG("\tUserProductInfo [%s]", pOrder->UserProductInfo);
        PURE_LOG("\tStatusMsg [%s]", pOrder->StatusMsg);
        PURE_LOG("\tActiveUserID [%s]", pOrder->ActiveUserID);
        PURE_LOG("\tRelativeOrderSysID [%s]", pOrder->RelativeOrderSysID);
        PURE_LOG("\tBranchID [%s]", pOrder->BranchID);
        PURE_LOG("\tInvestUnitID [%s]", pOrder->InvestUnitID);
        PURE_LOG("\tAccountID [%s]", pOrder->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pOrder->CurrencyID);
        PURE_LOG("\tIPAddress [%s]", pOrder->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pOrder->MacAddress);
        PURE_LOG("\tVolumeTotalOriginal [%d]", pOrder->VolumeTotalOriginal);
        PURE_LOG("\tMinVolume [%d]", pOrder->MinVolume);
        PURE_LOG("\tIsAutoSuspend [%d]", pOrder->IsAutoSuspend);
        PURE_LOG("\tRequestID [%d]", pOrder->RequestID);
        PURE_LOG("\tInstallID [%d]", pOrder->InstallID);
        PURE_LOG("\tNotifySequence [%d]", pOrder->NotifySequence);
        PURE_LOG("\tSettlementID [%d]", pOrder->SettlementID);
        PURE_LOG("\tVolumeTraded [%d]", pOrder->VolumeTraded);
        PURE_LOG("\tVolumeTotal [%d]", pOrder->VolumeTotal);
        PURE_LOG("\tSequenceNo [%d]", pOrder->SequenceNo);
        PURE_LOG("\tFrontID [%d]", pOrder->FrontID);
        PURE_LOG("\tSessionID [%d]", pOrder->SessionID);
        PURE_LOG("\tUserForceClose [%d]", pOrder->UserForceClose);
        PURE_LOG("\tBrokerOrderSeq [%d]", pOrder->BrokerOrderSeq);
        PURE_LOG("\tZCETotalTradedVolume [%d]", pOrder->ZCETotalTradedVolume);
        PURE_LOG("\tIsSwapOrder [%d]", pOrder->IsSwapOrder);
        PURE_LOG("\tOrderPriceType [%c]", pOrder->OrderPriceType);
        PURE_LOG("\tDirection [%c]", pOrder->Direction);
        PURE_LOG("\tTimeCondition [%c]", pOrder->TimeCondition);
        PURE_LOG("\tVolumeCondition [%c]", pOrder->VolumeCondition);
        PURE_LOG("\tContingentCondition [%c]", pOrder->ContingentCondition);
        PURE_LOG("\tForceCloseReason [%c]", pOrder->ForceCloseReason);
        PURE_LOG("\tOrderSubmitStatus [%c]", pOrder->OrderSubmitStatus);
        PURE_LOG("\tOrderSource [%c]", pOrder->OrderSource);
        PURE_LOG("\tOrderStatus [%c]", pOrder->OrderStatus);
        PURE_LOG("\tOrderType [%c]", pOrder->OrderType);
        PURE_LOG("\tLimitPrice [%.8lf]", pOrder->LimitPrice);
        PURE_LOG("\tStopPrice [%.8lf]", pOrder->StopPrice);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryOrder>");
};

void CTraderSpi::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryTrade>");
	if (pTrade)
	{
        PURE_LOG("\tBrokerID [%s]", pTrade->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pTrade->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pTrade->InstrumentID);
        PURE_LOG("\tOrderRef [%s]", pTrade->OrderRef);
        PURE_LOG("\tUserID [%s]", pTrade->UserID);
        PURE_LOG("\tExchangeID [%s]", pTrade->ExchangeID);
        PURE_LOG("\tTradeID [%s]", pTrade->TradeID);
        PURE_LOG("\tOrderSysID [%s]", pTrade->OrderSysID);
        PURE_LOG("\tParticipantID [%s]", pTrade->ParticipantID);
        PURE_LOG("\tClientID [%s]", pTrade->ClientID);
        PURE_LOG("\tExchangeInstID [%s]", pTrade->ExchangeInstID);
        PURE_LOG("\tTradeDate [%s]", pTrade->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pTrade->TradeTime);
        PURE_LOG("\tTraderID [%s]", pTrade->TraderID);
        PURE_LOG("\tOrderLocalID [%s]", pTrade->OrderLocalID);
        PURE_LOG("\tClearingPartID [%s]", pTrade->ClearingPartID);
        PURE_LOG("\tBusinessUnit [%s]", pTrade->BusinessUnit);
        PURE_LOG("\tTradingDay [%s]", pTrade->TradingDay);
        PURE_LOG("\tInvestUnitID [%s]", pTrade->InvestUnitID);
        PURE_LOG("\tVolume [%d]", pTrade->Volume);
        PURE_LOG("\tSequenceNo [%d]", pTrade->SequenceNo);
        PURE_LOG("\tSettlementID [%d]", pTrade->SettlementID);
        PURE_LOG("\tBrokerOrderSeq [%d]", pTrade->BrokerOrderSeq);
        PURE_LOG("\tDirection [%c]", pTrade->Direction);
        PURE_LOG("\tTradingRole [%c]", pTrade->TradingRole);
        PURE_LOG("\tOffsetFlag [%c]", pTrade->OffsetFlag);
        PURE_LOG("\tHedgeFlag [%c]", pTrade->HedgeFlag);
        PURE_LOG("\tTradeType [%c]", pTrade->TradeType);
        PURE_LOG("\tPriceSource [%c]", pTrade->PriceSource);
        PURE_LOG("\tTradeSource [%c]", pTrade->TradeSource);
        PURE_LOG("\tPrice [%.8lf]", pTrade->Price);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryTrade>");
};

void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryInvestorPosition>");
	if (pInvestorPosition)
	{
        PURE_LOG("\tInstrumentID [%s]", pInvestorPosition->InstrumentID);
        PURE_LOG("\tBrokerID [%s]", pInvestorPosition->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInvestorPosition->InvestorID);
        PURE_LOG("\tTradingDay [%s]", pInvestorPosition->TradingDay);
        PURE_LOG("\tExchangeID [%s]", pInvestorPosition->ExchangeID);
        PURE_LOG("\tInvestUnitID [%s]", pInvestorPosition->InvestUnitID);
        PURE_LOG("\tYdPosition [%d]", pInvestorPosition->YdPosition);
        PURE_LOG("\tPosition [%d]", pInvestorPosition->Position);
        PURE_LOG("\tLongFrozen [%d]", pInvestorPosition->LongFrozen);
        PURE_LOG("\tShortFrozen [%d]", pInvestorPosition->ShortFrozen);
        PURE_LOG("\tOpenVolume [%d]", pInvestorPosition->OpenVolume);
        PURE_LOG("\tCloseVolume [%d]", pInvestorPosition->CloseVolume);
        PURE_LOG("\tSettlementID [%d]", pInvestorPosition->SettlementID);
        PURE_LOG("\tCombPosition [%d]", pInvestorPosition->CombPosition);
        PURE_LOG("\tCombLongFrozen [%d]", pInvestorPosition->CombLongFrozen);
        PURE_LOG("\tCombShortFrozen [%d]", pInvestorPosition->CombShortFrozen);
        PURE_LOG("\tTodayPosition [%d]", pInvestorPosition->TodayPosition);
        PURE_LOG("\tStrikeFrozen [%d]", pInvestorPosition->StrikeFrozen);
        PURE_LOG("\tAbandonFrozen [%d]", pInvestorPosition->AbandonFrozen);
        PURE_LOG("\tYdStrikeFrozen [%d]", pInvestorPosition->YdStrikeFrozen);
        PURE_LOG("\tPosiDirection [%c]", pInvestorPosition->PosiDirection);
        PURE_LOG("\tHedgeFlag [%c]", pInvestorPosition->HedgeFlag);
        PURE_LOG("\tPositionDate [%c]", pInvestorPosition->PositionDate);
        PURE_LOG("\tLongFrozenAmount [%.8lf]", pInvestorPosition->LongFrozenAmount);
        PURE_LOG("\tShortFrozenAmount [%.8lf]", pInvestorPosition->ShortFrozenAmount);
        PURE_LOG("\tOpenAmount [%.8lf]", pInvestorPosition->OpenAmount);
        PURE_LOG("\tCloseAmount [%.8lf]", pInvestorPosition->CloseAmount);
        PURE_LOG("\tPositionCost [%.8lf]", pInvestorPosition->PositionCost);
        PURE_LOG("\tPreMargin [%.8lf]", pInvestorPosition->PreMargin);
        PURE_LOG("\tUseMargin [%.8lf]", pInvestorPosition->UseMargin);
        PURE_LOG("\tFrozenMargin [%.8lf]", pInvestorPosition->FrozenMargin);
        PURE_LOG("\tFrozenCash [%.8lf]", pInvestorPosition->FrozenCash);
        PURE_LOG("\tFrozenCommission [%.8lf]", pInvestorPosition->FrozenCommission);
        PURE_LOG("\tCashIn [%.8lf]", pInvestorPosition->CashIn);
        PURE_LOG("\tCommission [%.8lf]", pInvestorPosition->Commission);
        PURE_LOG("\tCloseProfit [%.8lf]", pInvestorPosition->CloseProfit);
        PURE_LOG("\tPositionProfit [%.8lf]", pInvestorPosition->PositionProfit);
        PURE_LOG("\tPreSettlementPrice [%.8lf]", pInvestorPosition->PreSettlementPrice);
        PURE_LOG("\tSettlementPrice [%.8lf]", pInvestorPosition->SettlementPrice);
        PURE_LOG("\tOpenCost [%.8lf]", pInvestorPosition->OpenCost);
        PURE_LOG("\tExchangeMargin [%.8lf]", pInvestorPosition->ExchangeMargin);
        PURE_LOG("\tCloseProfitByDate [%.8lf]", pInvestorPosition->CloseProfitByDate);
        PURE_LOG("\tCloseProfitByTrade [%.8lf]", pInvestorPosition->CloseProfitByTrade);
        PURE_LOG("\tMarginRateByMoney [%.8lf]", pInvestorPosition->MarginRateByMoney);
        PURE_LOG("\tMarginRateByVolume [%.8lf]", pInvestorPosition->MarginRateByVolume);
        PURE_LOG("\tStrikeFrozenAmount [%.8lf]", pInvestorPosition->StrikeFrozenAmount);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryInvestorPosition>");
};

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryTradingAccount>");
	if (pTradingAccount)
	{
        PURE_LOG("\tBrokerID [%s]", pTradingAccount->BrokerID);
        PURE_LOG("\tAccountID [%s]", pTradingAccount->AccountID);
        PURE_LOG("\tTradingDay [%s]", pTradingAccount->TradingDay);
        PURE_LOG("\tCurrencyID [%s]", pTradingAccount->CurrencyID);
        PURE_LOG("\tSettlementID [%d]", pTradingAccount->SettlementID);
        PURE_LOG("\tBizType [%c]", pTradingAccount->BizType);
        PURE_LOG("\tPreMortgage [%.8lf]", pTradingAccount->PreMortgage);
        PURE_LOG("\tPreCredit [%.8lf]", pTradingAccount->PreCredit);
        PURE_LOG("\tPreDeposit [%.8lf]", pTradingAccount->PreDeposit);
        PURE_LOG("\tPreBalance [%.8lf]", pTradingAccount->PreBalance);
        PURE_LOG("\tPreMargin [%.8lf]", pTradingAccount->PreMargin);
        PURE_LOG("\tInterestBase [%.8lf]", pTradingAccount->InterestBase);
        PURE_LOG("\tInterest [%.8lf]", pTradingAccount->Interest);
        PURE_LOG("\tDeposit [%.8lf]", pTradingAccount->Deposit);
        PURE_LOG("\tWithdraw [%.8lf]", pTradingAccount->Withdraw);
        PURE_LOG("\tFrozenMargin [%.8lf]", pTradingAccount->FrozenMargin);
        PURE_LOG("\tFrozenCash [%.8lf]", pTradingAccount->FrozenCash);
        PURE_LOG("\tFrozenCommission [%.8lf]", pTradingAccount->FrozenCommission);
        PURE_LOG("\tCurrMargin [%.8lf]", pTradingAccount->CurrMargin);
        PURE_LOG("\tCashIn [%.8lf]", pTradingAccount->CashIn);
        PURE_LOG("\tCommission [%.8lf]", pTradingAccount->Commission);
        PURE_LOG("\tCloseProfit [%.8lf]", pTradingAccount->CloseProfit);
        PURE_LOG("\tPositionProfit [%.8lf]", pTradingAccount->PositionProfit);
        PURE_LOG("\tBalance [%.8lf]", pTradingAccount->Balance);
        PURE_LOG("\tAvailable [%.8lf]", pTradingAccount->Available);
        PURE_LOG("\tWithdrawQuota [%.8lf]", pTradingAccount->WithdrawQuota);
        PURE_LOG("\tReserve [%.8lf]", pTradingAccount->Reserve);
        PURE_LOG("\tCredit [%.8lf]", pTradingAccount->Credit);
        PURE_LOG("\tMortgage [%.8lf]", pTradingAccount->Mortgage);
        PURE_LOG("\tExchangeMargin [%.8lf]", pTradingAccount->ExchangeMargin);
        PURE_LOG("\tDeliveryMargin [%.8lf]", pTradingAccount->DeliveryMargin);
        PURE_LOG("\tExchangeDeliveryMargin [%.8lf]", pTradingAccount->ExchangeDeliveryMargin);
        PURE_LOG("\tReserveBalance [%.8lf]", pTradingAccount->ReserveBalance);
        PURE_LOG("\tPreFundMortgageIn [%.8lf]", pTradingAccount->PreFundMortgageIn);
        PURE_LOG("\tPreFundMortgageOut [%.8lf]", pTradingAccount->PreFundMortgageOut);
        PURE_LOG("\tFundMortgageIn [%.8lf]", pTradingAccount->FundMortgageIn);
        PURE_LOG("\tFundMortgageOut [%.8lf]", pTradingAccount->FundMortgageOut);
        PURE_LOG("\tFundMortgageAvailable [%.8lf]", pTradingAccount->FundMortgageAvailable);
        PURE_LOG("\tMortgageableFund [%.8lf]", pTradingAccount->MortgageableFund);
        PURE_LOG("\tSpecProductMargin [%.8lf]", pTradingAccount->SpecProductMargin);
        PURE_LOG("\tSpecProductFrozenMargin [%.8lf]", pTradingAccount->SpecProductFrozenMargin);
        PURE_LOG("\tSpecProductCommission [%.8lf]", pTradingAccount->SpecProductCommission);
        PURE_LOG("\tSpecProductFrozenCommission [%.8lf]", pTradingAccount->SpecProductFrozenCommission);
        PURE_LOG("\tSpecProductPositionProfit [%.8lf]", pTradingAccount->SpecProductPositionProfit);
        PURE_LOG("\tSpecProductCloseProfit [%.8lf]", pTradingAccount->SpecProductCloseProfit);
        PURE_LOG("\tSpecProductPositionProfitByAlg [%.8lf]", pTradingAccount->SpecProductPositionProfitByAlg);
        PURE_LOG("\tSpecProductExchangeMargin [%.8lf]", pTradingAccount->SpecProductExchangeMargin);
        PURE_LOG("\tFrozenSwap [%.8lf]", pTradingAccount->FrozenSwap);
        PURE_LOG("\tRemainSwap [%.8lf]", pTradingAccount->RemainSwap);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryTradingAccount>");
};

void CTraderSpi::OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryInvestor>");
	if (pInvestor)
	{
        PURE_LOG("\tInvestorID [%s]", pInvestor->InvestorID);
        PURE_LOG("\tBrokerID [%s]", pInvestor->BrokerID);
        PURE_LOG("\tInvestorGroupID [%s]", pInvestor->InvestorGroupID);
        PURE_LOG("\tInvestorName [%s]", pInvestor->InvestorName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pInvestor->IdentifiedCardNo);
        PURE_LOG("\tTelephone [%s]", pInvestor->Telephone);
        PURE_LOG("\tAddress [%s]", pInvestor->Address);
        PURE_LOG("\tOpenDate [%s]", pInvestor->OpenDate);
        PURE_LOG("\tMobile [%s]", pInvestor->Mobile);
        PURE_LOG("\tCommModelID [%s]", pInvestor->CommModelID);
        PURE_LOG("\tMarginModelID [%s]", pInvestor->MarginModelID);
        PURE_LOG("\tIsActive [%d]", pInvestor->IsActive);
        PURE_LOG("\tIdentifiedCardType [%c]", pInvestor->IdentifiedCardType);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryInvestor>");
};

void CTraderSpi::OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryTradingCode>");
	if (pTradingCode)
	{
        PURE_LOG("\tInvestorID [%s]", pTradingCode->InvestorID);
        PURE_LOG("\tBrokerID [%s]", pTradingCode->BrokerID);
        PURE_LOG("\tExchangeID [%s]", pTradingCode->ExchangeID);
        PURE_LOG("\tClientID [%s]", pTradingCode->ClientID);
        PURE_LOG("\tBranchID [%s]", pTradingCode->BranchID);
        PURE_LOG("\tInvestUnitID [%s]", pTradingCode->InvestUnitID);
        PURE_LOG("\tIsActive [%d]", pTradingCode->IsActive);
        PURE_LOG("\tClientIDType [%c]", pTradingCode->ClientIDType);
        PURE_LOG("\tBizType [%c]", pTradingCode->BizType);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryTradingCode>");
};

void CTraderSpi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryInstrumentMarginRate>");
	if (pInstrumentMarginRate)
	{
        PURE_LOG("\tInstrumentID [%s]", pInstrumentMarginRate->InstrumentID);
        PURE_LOG("\tBrokerID [%s]", pInstrumentMarginRate->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInstrumentMarginRate->InvestorID);
        PURE_LOG("\tExchangeID [%s]", pInstrumentMarginRate->ExchangeID);
        PURE_LOG("\tInvestUnitID [%s]", pInstrumentMarginRate->InvestUnitID);
        PURE_LOG("\tIsRelative [%d]", pInstrumentMarginRate->IsRelative);
        PURE_LOG("\tInvestorRange [%c]", pInstrumentMarginRate->InvestorRange);
        PURE_LOG("\tHedgeFlag [%c]", pInstrumentMarginRate->HedgeFlag);
        PURE_LOG("\tLongMarginRatioByMoney [%.8lf]", pInstrumentMarginRate->LongMarginRatioByMoney);
        PURE_LOG("\tLongMarginRatioByVolume [%.8lf]", pInstrumentMarginRate->LongMarginRatioByVolume);
        PURE_LOG("\tShortMarginRatioByMoney [%.8lf]", pInstrumentMarginRate->ShortMarginRatioByMoney);
        PURE_LOG("\tShortMarginRatioByVolume [%.8lf]", pInstrumentMarginRate->ShortMarginRatioByVolume);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryInstrumentMarginRate>");
};

void CTraderSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryInstrumentCommissionRate>");
	if (pInstrumentCommissionRate)
	{
        PURE_LOG("\tInstrumentID [%s]", pInstrumentCommissionRate->InstrumentID);
        PURE_LOG("\tBrokerID [%s]", pInstrumentCommissionRate->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInstrumentCommissionRate->InvestorID);
        PURE_LOG("\tExchangeID [%s]", pInstrumentCommissionRate->ExchangeID);
        PURE_LOG("\tInvestUnitID [%s]", pInstrumentCommissionRate->InvestUnitID);
        PURE_LOG("\tInvestorRange [%c]", pInstrumentCommissionRate->InvestorRange);
        PURE_LOG("\tBizType [%c]", pInstrumentCommissionRate->BizType);
        PURE_LOG("\tOpenRatioByMoney [%.8lf]", pInstrumentCommissionRate->OpenRatioByMoney);
        PURE_LOG("\tOpenRatioByVolume [%.8lf]", pInstrumentCommissionRate->OpenRatioByVolume);
        PURE_LOG("\tCloseRatioByMoney [%.8lf]", pInstrumentCommissionRate->CloseRatioByMoney);
        PURE_LOG("\tCloseRatioByVolume [%.8lf]", pInstrumentCommissionRate->CloseRatioByVolume);
        PURE_LOG("\tCloseTodayRatioByMoney [%.8lf]", pInstrumentCommissionRate->CloseTodayRatioByMoney);
        PURE_LOG("\tCloseTodayRatioByVolume [%.8lf]", pInstrumentCommissionRate->CloseTodayRatioByVolume);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryInstrumentCommissionRate>");
};

void CTraderSpi::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryExchange>");
	if (pExchange)
	{
        PURE_LOG("\tExchangeID [%s]", pExchange->ExchangeID);
        PURE_LOG("\tExchangeName [%s]", pExchange->ExchangeName);
        PURE_LOG("\tExchangeProperty [%c]", pExchange->ExchangeProperty);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryExchange>");
};

void CTraderSpi::OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryProduct>");
	if (pProduct)
	{
        PURE_LOG("\tProductID [%s]", pProduct->ProductID);
        PURE_LOG("\tProductName [%s]", pProduct->ProductName);
        PURE_LOG("\tExchangeID [%s]", pProduct->ExchangeID);
        PURE_LOG("\tTradeCurrencyID [%s]", pProduct->TradeCurrencyID);
        PURE_LOG("\tExchangeProductID [%s]", pProduct->ExchangeProductID);
        PURE_LOG("\tVolumeMultiple [%d]", pProduct->VolumeMultiple);
        PURE_LOG("\tMaxMarketOrderVolume [%d]", pProduct->MaxMarketOrderVolume);
        PURE_LOG("\tMinMarketOrderVolume [%d]", pProduct->MinMarketOrderVolume);
        PURE_LOG("\tMaxLimitOrderVolume [%d]", pProduct->MaxLimitOrderVolume);
        PURE_LOG("\tMinLimitOrderVolume [%d]", pProduct->MinLimitOrderVolume);
        PURE_LOG("\tProductClass [%c]", pProduct->ProductClass);
        PURE_LOG("\tPositionType [%c]", pProduct->PositionType);
        PURE_LOG("\tPositionDateType [%c]", pProduct->PositionDateType);
        PURE_LOG("\tCloseDealType [%c]", pProduct->CloseDealType);
        PURE_LOG("\tMortgageFundUseRange [%c]", pProduct->MortgageFundUseRange);
        PURE_LOG("\tPriceTick [%.8lf]", pProduct->PriceTick);
        PURE_LOG("\tUnderlyingMultiple [%.8lf]", pProduct->UnderlyingMultiple);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryProduct>");
};

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryInstrument>");
	if (pInstrument)
	{
        PURE_LOG("\tInstrumentID [%s]", pInstrument->InstrumentID);
        PURE_LOG("\tExchangeID [%s]", pInstrument->ExchangeID);
        PURE_LOG("\tInstrumentName [%s]", pInstrument->InstrumentName);
        PURE_LOG("\tExchangeInstID [%s]", pInstrument->ExchangeInstID);
        PURE_LOG("\tProductID [%s]", pInstrument->ProductID);
        PURE_LOG("\tCreateDate [%s]", pInstrument->CreateDate);
        PURE_LOG("\tOpenDate [%s]", pInstrument->OpenDate);
        PURE_LOG("\tExpireDate [%s]", pInstrument->ExpireDate);
        PURE_LOG("\tStartDelivDate [%s]", pInstrument->StartDelivDate);
        PURE_LOG("\tEndDelivDate [%s]", pInstrument->EndDelivDate);
        PURE_LOG("\tUnderlyingInstrID [%s]", pInstrument->UnderlyingInstrID);
        PURE_LOG("\tDeliveryYear [%d]", pInstrument->DeliveryYear);
        PURE_LOG("\tDeliveryMonth [%d]", pInstrument->DeliveryMonth);
        PURE_LOG("\tMaxMarketOrderVolume [%d]", pInstrument->MaxMarketOrderVolume);
        PURE_LOG("\tMinMarketOrderVolume [%d]", pInstrument->MinMarketOrderVolume);
        PURE_LOG("\tMaxLimitOrderVolume [%d]", pInstrument->MaxLimitOrderVolume);
        PURE_LOG("\tMinLimitOrderVolume [%d]", pInstrument->MinLimitOrderVolume);
        PURE_LOG("\tVolumeMultiple [%d]", pInstrument->VolumeMultiple);
        PURE_LOG("\tIsTrading [%d]", pInstrument->IsTrading);
        PURE_LOG("\tProductClass [%c]", pInstrument->ProductClass);
        PURE_LOG("\tInstLifePhase [%c]", pInstrument->InstLifePhase);
        PURE_LOG("\tPositionType [%c]", pInstrument->PositionType);
        PURE_LOG("\tPositionDateType [%c]", pInstrument->PositionDateType);
        PURE_LOG("\tMaxMarginSideAlgorithm [%c]", pInstrument->MaxMarginSideAlgorithm);
        PURE_LOG("\tOptionsType [%c]", pInstrument->OptionsType);
        PURE_LOG("\tCombinationType [%c]", pInstrument->CombinationType);
        PURE_LOG("\tPriceTick [%.8lf]", pInstrument->PriceTick);
        PURE_LOG("\tLongMarginRatio [%.8lf]", pInstrument->LongMarginRatio);
        PURE_LOG("\tShortMarginRatio [%.8lf]", pInstrument->ShortMarginRatio);
        PURE_LOG("\tStrikePrice [%.8lf]", pInstrument->StrikePrice);
        PURE_LOG("\tUnderlyingMultiple [%.8lf]", pInstrument->UnderlyingMultiple);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("here!\n");
    PURE_LOG("</OnRspQryInstrument>");
};

void CTraderSpi::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryDepthMarketData>");
	if (pDepthMarketData)
	{
        PURE_LOG("\tTradingDay [%s]", pDepthMarketData->TradingDay);
        PURE_LOG("\tInstrumentID [%s]", pDepthMarketData->InstrumentID);
        PURE_LOG("\tExchangeID [%s]", pDepthMarketData->ExchangeID);
        PURE_LOG("\tExchangeInstID [%s]", pDepthMarketData->ExchangeInstID);
        PURE_LOG("\tUpdateTime [%s]", pDepthMarketData->UpdateTime);
        PURE_LOG("\tActionDay [%s]", pDepthMarketData->ActionDay);
        PURE_LOG("\tVolume [%d]", pDepthMarketData->Volume);
        PURE_LOG("\tUpdateMillisec [%d]", pDepthMarketData->UpdateMillisec);
        PURE_LOG("\tBidVolume1 [%d]", pDepthMarketData->BidVolume1);
        PURE_LOG("\tAskVolume1 [%d]", pDepthMarketData->AskVolume1);
        PURE_LOG("\tBidVolume2 [%d]", pDepthMarketData->BidVolume2);
        PURE_LOG("\tAskVolume2 [%d]", pDepthMarketData->AskVolume2);
        PURE_LOG("\tBidVolume3 [%d]", pDepthMarketData->BidVolume3);
        PURE_LOG("\tAskVolume3 [%d]", pDepthMarketData->AskVolume3);
        PURE_LOG("\tBidVolume4 [%d]", pDepthMarketData->BidVolume4);
        PURE_LOG("\tAskVolume4 [%d]", pDepthMarketData->AskVolume4);
        PURE_LOG("\tBidVolume5 [%d]", pDepthMarketData->BidVolume5);
        PURE_LOG("\tAskVolume5 [%d]", pDepthMarketData->AskVolume5);
        PURE_LOG("\tLastPrice [%.8lf]", pDepthMarketData->LastPrice);
        PURE_LOG("\tPreSettlementPrice [%.8lf]", pDepthMarketData->PreSettlementPrice);
        PURE_LOG("\tPreClosePrice [%.8lf]", pDepthMarketData->PreClosePrice);
        PURE_LOG("\tPreOpenInterest [%.8lf]", pDepthMarketData->PreOpenInterest);
        PURE_LOG("\tOpenPrice [%.8lf]", pDepthMarketData->OpenPrice);
        PURE_LOG("\tHighestPrice [%.8lf]", pDepthMarketData->HighestPrice);
        PURE_LOG("\tLowestPrice [%.8lf]", pDepthMarketData->LowestPrice);
        PURE_LOG("\tTurnover [%.8lf]", pDepthMarketData->Turnover);
        PURE_LOG("\tOpenInterest [%.8lf]", pDepthMarketData->OpenInterest);
        PURE_LOG("\tClosePrice [%.8lf]", pDepthMarketData->ClosePrice);
        PURE_LOG("\tSettlementPrice [%.8lf]", pDepthMarketData->SettlementPrice);
        PURE_LOG("\tUpperLimitPrice [%.8lf]", pDepthMarketData->UpperLimitPrice);
        PURE_LOG("\tLowerLimitPrice [%.8lf]", pDepthMarketData->LowerLimitPrice);
        PURE_LOG("\tPreDelta [%.8lf]", pDepthMarketData->PreDelta);
        PURE_LOG("\tCurrDelta [%.8lf]", pDepthMarketData->CurrDelta);
        PURE_LOG("\tBidPrice1 [%.8lf]", pDepthMarketData->BidPrice1);
        PURE_LOG("\tAskPrice1 [%.8lf]", pDepthMarketData->AskPrice1);
        PURE_LOG("\tBidPrice2 [%.8lf]", pDepthMarketData->BidPrice2);
        PURE_LOG("\tAskPrice2 [%.8lf]", pDepthMarketData->AskPrice2);
        PURE_LOG("\tBidPrice3 [%.8lf]", pDepthMarketData->BidPrice3);
        PURE_LOG("\tAskPrice3 [%.8lf]", pDepthMarketData->AskPrice3);
        PURE_LOG("\tBidPrice4 [%.8lf]", pDepthMarketData->BidPrice4);
        PURE_LOG("\tAskPrice4 [%.8lf]", pDepthMarketData->AskPrice4);
        PURE_LOG("\tBidPrice5 [%.8lf]", pDepthMarketData->BidPrice5);
        PURE_LOG("\tAskPrice5 [%.8lf]", pDepthMarketData->AskPrice5);
        PURE_LOG("\tAveragePrice [%.8lf]", pDepthMarketData->AveragePrice);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryDepthMarketData>");
};

void CTraderSpi::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQrySettlementInfo>");
	if (pSettlementInfo)
	{
        PURE_LOG("\tTradingDay [%s]", pSettlementInfo->TradingDay);
        PURE_LOG("\tBrokerID [%s]", pSettlementInfo->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pSettlementInfo->InvestorID);
        PURE_LOG("\tContent [%s]", pSettlementInfo->Content);
        PURE_LOG("\tAccountID [%s]", pSettlementInfo->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pSettlementInfo->CurrencyID);
        PURE_LOG("\tSettlementID [%d]", pSettlementInfo->SettlementID);
        PURE_LOG("\tSequenceNo [%d]", pSettlementInfo->SequenceNo);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQrySettlementInfo>");
};

void CTraderSpi::OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryTransferBank>");
	if (pTransferBank)
	{
        PURE_LOG("\tBankID [%s]", pTransferBank->BankID);
        PURE_LOG("\tBankBrchID [%s]", pTransferBank->BankBrchID);
        PURE_LOG("\tBankName [%s]", pTransferBank->BankName);
        PURE_LOG("\tIsActive [%d]", pTransferBank->IsActive);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryTransferBank>");
};

void CTraderSpi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryInvestorPositionDetail>");
	if (pInvestorPositionDetail)
	{
        PURE_LOG("\tInstrumentID [%s]", pInvestorPositionDetail->InstrumentID);
        PURE_LOG("\tBrokerID [%s]", pInvestorPositionDetail->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInvestorPositionDetail->InvestorID);
        PURE_LOG("\tOpenDate [%s]", pInvestorPositionDetail->OpenDate);
        PURE_LOG("\tTradeID [%s]", pInvestorPositionDetail->TradeID);
        PURE_LOG("\tTradingDay [%s]", pInvestorPositionDetail->TradingDay);
        PURE_LOG("\tCombInstrumentID [%s]", pInvestorPositionDetail->CombInstrumentID);
        PURE_LOG("\tExchangeID [%s]", pInvestorPositionDetail->ExchangeID);
        PURE_LOG("\tInvestUnitID [%s]", pInvestorPositionDetail->InvestUnitID);
        PURE_LOG("\tVolume [%d]", pInvestorPositionDetail->Volume);
        PURE_LOG("\tSettlementID [%d]", pInvestorPositionDetail->SettlementID);
        PURE_LOG("\tCloseVolume [%d]", pInvestorPositionDetail->CloseVolume);
        PURE_LOG("\tHedgeFlag [%c]", pInvestorPositionDetail->HedgeFlag);
        PURE_LOG("\tDirection [%c]", pInvestorPositionDetail->Direction);
        PURE_LOG("\tTradeType [%c]", pInvestorPositionDetail->TradeType);
        PURE_LOG("\tOpenPrice [%.8lf]", pInvestorPositionDetail->OpenPrice);
        PURE_LOG("\tCloseProfitByDate [%.8lf]", pInvestorPositionDetail->CloseProfitByDate);
        PURE_LOG("\tCloseProfitByTrade [%.8lf]", pInvestorPositionDetail->CloseProfitByTrade);
        PURE_LOG("\tPositionProfitByDate [%.8lf]", pInvestorPositionDetail->PositionProfitByDate);
        PURE_LOG("\tPositionProfitByTrade [%.8lf]", pInvestorPositionDetail->PositionProfitByTrade);
        PURE_LOG("\tMargin [%.8lf]", pInvestorPositionDetail->Margin);
        PURE_LOG("\tExchMargin [%.8lf]", pInvestorPositionDetail->ExchMargin);
        PURE_LOG("\tMarginRateByMoney [%.8lf]", pInvestorPositionDetail->MarginRateByMoney);
        PURE_LOG("\tMarginRateByVolume [%.8lf]", pInvestorPositionDetail->MarginRateByVolume);
        PURE_LOG("\tLastSettlementPrice [%.8lf]", pInvestorPositionDetail->LastSettlementPrice);
        PURE_LOG("\tSettlementPrice [%.8lf]", pInvestorPositionDetail->SettlementPrice);
        PURE_LOG("\tCloseAmount [%.8lf]", pInvestorPositionDetail->CloseAmount);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryInvestorPositionDetail>");
};

void CTraderSpi::OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryNotice>");
	if (pNotice)
	{
        PURE_LOG("\tBrokerID [%s]", pNotice->BrokerID);
        PURE_LOG("\tContent [%s]", pNotice->Content);
        PURE_LOG("\tSequenceLabel [%s]", pNotice->SequenceLabel);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryNotice>");
};

void CTraderSpi::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQrySettlementInfoConfirm>");
	if (pSettlementInfoConfirm)
	{
        PURE_LOG("\tBrokerID [%s]", pSettlementInfoConfirm->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pSettlementInfoConfirm->InvestorID);
        PURE_LOG("\tConfirmDate [%s]", pSettlementInfoConfirm->ConfirmDate);
        PURE_LOG("\tConfirmTime [%s]", pSettlementInfoConfirm->ConfirmTime);
        PURE_LOG("\tAccountID [%s]", pSettlementInfoConfirm->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pSettlementInfoConfirm->CurrencyID);
        PURE_LOG("\tSettlementID [%d]", pSettlementInfoConfirm->SettlementID);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQrySettlementInfoConfirm>");
};

void CTraderSpi::OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryInvestorPositionCombineDetail>");
	if (pInvestorPositionCombineDetail)
	{
        PURE_LOG("\tTradingDay [%s]", pInvestorPositionCombineDetail->TradingDay);
        PURE_LOG("\tOpenDate [%s]", pInvestorPositionCombineDetail->OpenDate);
        PURE_LOG("\tExchangeID [%s]", pInvestorPositionCombineDetail->ExchangeID);
        PURE_LOG("\tBrokerID [%s]", pInvestorPositionCombineDetail->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInvestorPositionCombineDetail->InvestorID);
        PURE_LOG("\tComTradeID [%s]", pInvestorPositionCombineDetail->ComTradeID);
        PURE_LOG("\tTradeID [%s]", pInvestorPositionCombineDetail->TradeID);
        PURE_LOG("\tInstrumentID [%s]", pInvestorPositionCombineDetail->InstrumentID);
        PURE_LOG("\tCombInstrumentID [%s]", pInvestorPositionCombineDetail->CombInstrumentID);
        PURE_LOG("\tInvestUnitID [%s]", pInvestorPositionCombineDetail->InvestUnitID);
        PURE_LOG("\tSettlementID [%d]", pInvestorPositionCombineDetail->SettlementID);
        PURE_LOG("\tTotalAmt [%d]", pInvestorPositionCombineDetail->TotalAmt);
        PURE_LOG("\tLegID [%d]", pInvestorPositionCombineDetail->LegID);
        PURE_LOG("\tLegMultiple [%d]", pInvestorPositionCombineDetail->LegMultiple);
        PURE_LOG("\tTradeGroupID [%d]", pInvestorPositionCombineDetail->TradeGroupID);
        PURE_LOG("\tHedgeFlag [%c]", pInvestorPositionCombineDetail->HedgeFlag);
        PURE_LOG("\tDirection [%c]", pInvestorPositionCombineDetail->Direction);
        PURE_LOG("\tMargin [%.8lf]", pInvestorPositionCombineDetail->Margin);
        PURE_LOG("\tExchMargin [%.8lf]", pInvestorPositionCombineDetail->ExchMargin);
        PURE_LOG("\tMarginRateByMoney [%.8lf]", pInvestorPositionCombineDetail->MarginRateByMoney);
        PURE_LOG("\tMarginRateByVolume [%.8lf]", pInvestorPositionCombineDetail->MarginRateByVolume);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryInvestorPositionCombineDetail>");
};

void CTraderSpi::OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryCFMMCTradingAccountKey>");
	if (pCFMMCTradingAccountKey)
	{
        PURE_LOG("\tBrokerID [%s]", pCFMMCTradingAccountKey->BrokerID);
        PURE_LOG("\tParticipantID [%s]", pCFMMCTradingAccountKey->ParticipantID);
        PURE_LOG("\tAccountID [%s]", pCFMMCTradingAccountKey->AccountID);
        PURE_LOG("\tCurrentKey [%s]", pCFMMCTradingAccountKey->CurrentKey);
        PURE_LOG("\tKeyID [%d]", pCFMMCTradingAccountKey->KeyID);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryCFMMCTradingAccountKey>");
};

void CTraderSpi::OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField *pEWarrantOffset, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryEWarrantOffset>");
	if (pEWarrantOffset)
	{
        PURE_LOG("\tTradingDay [%s]", pEWarrantOffset->TradingDay);
        PURE_LOG("\tBrokerID [%s]", pEWarrantOffset->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pEWarrantOffset->InvestorID);
        PURE_LOG("\tExchangeID [%s]", pEWarrantOffset->ExchangeID);
        PURE_LOG("\tInstrumentID [%s]", pEWarrantOffset->InstrumentID);
        PURE_LOG("\tInvestUnitID [%s]", pEWarrantOffset->InvestUnitID);
        PURE_LOG("\tVolume [%d]", pEWarrantOffset->Volume);
        PURE_LOG("\tDirection [%c]", pEWarrantOffset->Direction);
        PURE_LOG("\tHedgeFlag [%c]", pEWarrantOffset->HedgeFlag);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryEWarrantOffset>");
};

void CTraderSpi::OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField *pInvestorProductGroupMargin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryInvestorProductGroupMargin>");
	if (pInvestorProductGroupMargin)
	{
        PURE_LOG("\tProductGroupID [%s]", pInvestorProductGroupMargin->ProductGroupID);
        PURE_LOG("\tBrokerID [%s]", pInvestorProductGroupMargin->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInvestorProductGroupMargin->InvestorID);
        PURE_LOG("\tTradingDay [%s]", pInvestorProductGroupMargin->TradingDay);
        PURE_LOG("\tExchangeID [%s]", pInvestorProductGroupMargin->ExchangeID);
        PURE_LOG("\tInvestUnitID [%s]", pInvestorProductGroupMargin->InvestUnitID);
        PURE_LOG("\tSettlementID [%d]", pInvestorProductGroupMargin->SettlementID);
        PURE_LOG("\tHedgeFlag [%c]", pInvestorProductGroupMargin->HedgeFlag);
        PURE_LOG("\tFrozenMargin [%.8lf]", pInvestorProductGroupMargin->FrozenMargin);
        PURE_LOG("\tLongFrozenMargin [%.8lf]", pInvestorProductGroupMargin->LongFrozenMargin);
        PURE_LOG("\tShortFrozenMargin [%.8lf]", pInvestorProductGroupMargin->ShortFrozenMargin);
        PURE_LOG("\tUseMargin [%.8lf]", pInvestorProductGroupMargin->UseMargin);
        PURE_LOG("\tLongUseMargin [%.8lf]", pInvestorProductGroupMargin->LongUseMargin);
        PURE_LOG("\tShortUseMargin [%.8lf]", pInvestorProductGroupMargin->ShortUseMargin);
        PURE_LOG("\tExchMargin [%.8lf]", pInvestorProductGroupMargin->ExchMargin);
        PURE_LOG("\tLongExchMargin [%.8lf]", pInvestorProductGroupMargin->LongExchMargin);
        PURE_LOG("\tShortExchMargin [%.8lf]", pInvestorProductGroupMargin->ShortExchMargin);
        PURE_LOG("\tCloseProfit [%.8lf]", pInvestorProductGroupMargin->CloseProfit);
        PURE_LOG("\tFrozenCommission [%.8lf]", pInvestorProductGroupMargin->FrozenCommission);
        PURE_LOG("\tCommission [%.8lf]", pInvestorProductGroupMargin->Commission);
        PURE_LOG("\tFrozenCash [%.8lf]", pInvestorProductGroupMargin->FrozenCash);
        PURE_LOG("\tCashIn [%.8lf]", pInvestorProductGroupMargin->CashIn);
        PURE_LOG("\tPositionProfit [%.8lf]", pInvestorProductGroupMargin->PositionProfit);
        PURE_LOG("\tOffsetAmount [%.8lf]", pInvestorProductGroupMargin->OffsetAmount);
        PURE_LOG("\tLongOffsetAmount [%.8lf]", pInvestorProductGroupMargin->LongOffsetAmount);
        PURE_LOG("\tShortOffsetAmount [%.8lf]", pInvestorProductGroupMargin->ShortOffsetAmount);
        PURE_LOG("\tExchOffsetAmount [%.8lf]", pInvestorProductGroupMargin->ExchOffsetAmount);
        PURE_LOG("\tLongExchOffsetAmount [%.8lf]", pInvestorProductGroupMargin->LongExchOffsetAmount);
        PURE_LOG("\tShortExchOffsetAmount [%.8lf]", pInvestorProductGroupMargin->ShortExchOffsetAmount);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryInvestorProductGroupMargin>");
};

void CTraderSpi::OnRspQryExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryExchangeMarginRate>");
	if (pExchangeMarginRate)
	{
        PURE_LOG("\tBrokerID [%s]", pExchangeMarginRate->BrokerID);
        PURE_LOG("\tInstrumentID [%s]", pExchangeMarginRate->InstrumentID);
        PURE_LOG("\tExchangeID [%s]", pExchangeMarginRate->ExchangeID);
        PURE_LOG("\tHedgeFlag [%c]", pExchangeMarginRate->HedgeFlag);
        PURE_LOG("\tLongMarginRatioByMoney [%.8lf]", pExchangeMarginRate->LongMarginRatioByMoney);
        PURE_LOG("\tLongMarginRatioByVolume [%.8lf]", pExchangeMarginRate->LongMarginRatioByVolume);
        PURE_LOG("\tShortMarginRatioByMoney [%.8lf]", pExchangeMarginRate->ShortMarginRatioByMoney);
        PURE_LOG("\tShortMarginRatioByVolume [%.8lf]", pExchangeMarginRate->ShortMarginRatioByVolume);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryExchangeMarginRate>");
};

void CTraderSpi::OnRspQryExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryExchangeMarginRateAdjust>");
	if (pExchangeMarginRateAdjust)
	{
        PURE_LOG("\tBrokerID [%s]", pExchangeMarginRateAdjust->BrokerID);
        PURE_LOG("\tInstrumentID [%s]", pExchangeMarginRateAdjust->InstrumentID);
        PURE_LOG("\tHedgeFlag [%c]", pExchangeMarginRateAdjust->HedgeFlag);
        PURE_LOG("\tLongMarginRatioByMoney [%.8lf]", pExchangeMarginRateAdjust->LongMarginRatioByMoney);
        PURE_LOG("\tLongMarginRatioByVolume [%.8lf]", pExchangeMarginRateAdjust->LongMarginRatioByVolume);
        PURE_LOG("\tShortMarginRatioByMoney [%.8lf]", pExchangeMarginRateAdjust->ShortMarginRatioByMoney);
        PURE_LOG("\tShortMarginRatioByVolume [%.8lf]", pExchangeMarginRateAdjust->ShortMarginRatioByVolume);
        PURE_LOG("\tExchLongMarginRatioByMoney [%.8lf]", pExchangeMarginRateAdjust->ExchLongMarginRatioByMoney);
        PURE_LOG("\tExchLongMarginRatioByVolume [%.8lf]", pExchangeMarginRateAdjust->ExchLongMarginRatioByVolume);
        PURE_LOG("\tExchShortMarginRatioByMoney [%.8lf]", pExchangeMarginRateAdjust->ExchShortMarginRatioByMoney);
        PURE_LOG("\tExchShortMarginRatioByVolume [%.8lf]", pExchangeMarginRateAdjust->ExchShortMarginRatioByVolume);
        PURE_LOG("\tNoLongMarginRatioByMoney [%.8lf]", pExchangeMarginRateAdjust->NoLongMarginRatioByMoney);
        PURE_LOG("\tNoLongMarginRatioByVolume [%.8lf]", pExchangeMarginRateAdjust->NoLongMarginRatioByVolume);
        PURE_LOG("\tNoShortMarginRatioByMoney [%.8lf]", pExchangeMarginRateAdjust->NoShortMarginRatioByMoney);
        PURE_LOG("\tNoShortMarginRatioByVolume [%.8lf]", pExchangeMarginRateAdjust->NoShortMarginRatioByVolume);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryExchangeMarginRateAdjust>");
};

void CTraderSpi::OnRspQryExchangeRate(CThostFtdcExchangeRateField *pExchangeRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryExchangeRate>");
	if (pExchangeRate)
	{
        PURE_LOG("\tBrokerID [%s]", pExchangeRate->BrokerID);
        PURE_LOG("\tFromCurrencyID [%s]", pExchangeRate->FromCurrencyID);
        PURE_LOG("\tToCurrencyID [%s]", pExchangeRate->ToCurrencyID);
        PURE_LOG("\tFromCurrencyUnit [%.8lf]", pExchangeRate->FromCurrencyUnit);
        PURE_LOG("\tExchangeRate [%.8lf]", pExchangeRate->ExchangeRate);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryExchangeRate>");
};

void CTraderSpi::OnRspQrySecAgentACIDMap(CThostFtdcSecAgentACIDMapField *pSecAgentACIDMap, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQrySecAgentACIDMap>");
	if (pSecAgentACIDMap)
	{
        PURE_LOG("\tBrokerID [%s]", pSecAgentACIDMap->BrokerID);
        PURE_LOG("\tUserID [%s]", pSecAgentACIDMap->UserID);
        PURE_LOG("\tAccountID [%s]", pSecAgentACIDMap->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pSecAgentACIDMap->CurrencyID);
        PURE_LOG("\tBrokerSecAgentID [%s]", pSecAgentACIDMap->BrokerSecAgentID);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQrySecAgentACIDMap>");
};

void CTraderSpi::OnRspQryProductExchRate(CThostFtdcProductExchRateField *pProductExchRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryProductExchRate>");
	if (pProductExchRate)
	{
        PURE_LOG("\tProductID [%s]", pProductExchRate->ProductID);
        PURE_LOG("\tQuoteCurrencyID [%s]", pProductExchRate->QuoteCurrencyID);
        PURE_LOG("\tExchangeID [%s]", pProductExchRate->ExchangeID);
        PURE_LOG("\tExchangeRate [%.8lf]", pProductExchRate->ExchangeRate);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryProductExchRate>");
};

void CTraderSpi::OnRspQryProductGroup(CThostFtdcProductGroupField *pProductGroup, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryProductGroup>");
	if (pProductGroup)
	{
        PURE_LOG("\tProductID [%s]", pProductGroup->ProductID);
        PURE_LOG("\tExchangeID [%s]", pProductGroup->ExchangeID);
        PURE_LOG("\tProductGroupID [%s]", pProductGroup->ProductGroupID);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryProductGroup>");
};

void CTraderSpi::OnRspQryMMInstrumentCommissionRate(CThostFtdcMMInstrumentCommissionRateField *pMMInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryMMInstrumentCommissionRate>");
	if (pMMInstrumentCommissionRate)
	{
        PURE_LOG("\tInstrumentID [%s]", pMMInstrumentCommissionRate->InstrumentID);
        PURE_LOG("\tBrokerID [%s]", pMMInstrumentCommissionRate->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pMMInstrumentCommissionRate->InvestorID);
        PURE_LOG("\tInvestorRange [%c]", pMMInstrumentCommissionRate->InvestorRange);
        PURE_LOG("\tOpenRatioByMoney [%.8lf]", pMMInstrumentCommissionRate->OpenRatioByMoney);
        PURE_LOG("\tOpenRatioByVolume [%.8lf]", pMMInstrumentCommissionRate->OpenRatioByVolume);
        PURE_LOG("\tCloseRatioByMoney [%.8lf]", pMMInstrumentCommissionRate->CloseRatioByMoney);
        PURE_LOG("\tCloseRatioByVolume [%.8lf]", pMMInstrumentCommissionRate->CloseRatioByVolume);
        PURE_LOG("\tCloseTodayRatioByMoney [%.8lf]", pMMInstrumentCommissionRate->CloseTodayRatioByMoney);
        PURE_LOG("\tCloseTodayRatioByVolume [%.8lf]", pMMInstrumentCommissionRate->CloseTodayRatioByVolume);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryMMInstrumentCommissionRate>");
};

void CTraderSpi::OnRspQryMMOptionInstrCommRate(CThostFtdcMMOptionInstrCommRateField *pMMOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryMMOptionInstrCommRate>");
	if (pMMOptionInstrCommRate)
	{
        PURE_LOG("\tInstrumentID [%s]", pMMOptionInstrCommRate->InstrumentID);
        PURE_LOG("\tBrokerID [%s]", pMMOptionInstrCommRate->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pMMOptionInstrCommRate->InvestorID);
        PURE_LOG("\tInvestorRange [%c]", pMMOptionInstrCommRate->InvestorRange);
        PURE_LOG("\tOpenRatioByMoney [%.8lf]", pMMOptionInstrCommRate->OpenRatioByMoney);
        PURE_LOG("\tOpenRatioByVolume [%.8lf]", pMMOptionInstrCommRate->OpenRatioByVolume);
        PURE_LOG("\tCloseRatioByMoney [%.8lf]", pMMOptionInstrCommRate->CloseRatioByMoney);
        PURE_LOG("\tCloseRatioByVolume [%.8lf]", pMMOptionInstrCommRate->CloseRatioByVolume);
        PURE_LOG("\tCloseTodayRatioByMoney [%.8lf]", pMMOptionInstrCommRate->CloseTodayRatioByMoney);
        PURE_LOG("\tCloseTodayRatioByVolume [%.8lf]", pMMOptionInstrCommRate->CloseTodayRatioByVolume);
        PURE_LOG("\tStrikeRatioByMoney [%.8lf]", pMMOptionInstrCommRate->StrikeRatioByMoney);
        PURE_LOG("\tStrikeRatioByVolume [%.8lf]", pMMOptionInstrCommRate->StrikeRatioByVolume);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryMMOptionInstrCommRate>");
};

void CTraderSpi::OnRspQryInstrumentOrderCommRate(CThostFtdcInstrumentOrderCommRateField *pInstrumentOrderCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryInstrumentOrderCommRate>");
	if (pInstrumentOrderCommRate)
	{
        PURE_LOG("\tInstrumentID [%s]", pInstrumentOrderCommRate->InstrumentID);
        PURE_LOG("\tBrokerID [%s]", pInstrumentOrderCommRate->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInstrumentOrderCommRate->InvestorID);
        PURE_LOG("\tExchangeID [%s]", pInstrumentOrderCommRate->ExchangeID);
        PURE_LOG("\tInvestUnitID [%s]", pInstrumentOrderCommRate->InvestUnitID);
        PURE_LOG("\tInvestorRange [%c]", pInstrumentOrderCommRate->InvestorRange);
        PURE_LOG("\tHedgeFlag [%c]", pInstrumentOrderCommRate->HedgeFlag);
        PURE_LOG("\tOrderCommByVolume [%.8lf]", pInstrumentOrderCommRate->OrderCommByVolume);
        PURE_LOG("\tOrderActionCommByVolume [%.8lf]", pInstrumentOrderCommRate->OrderActionCommByVolume);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryInstrumentOrderCommRate>");
};

void CTraderSpi::OnRspQrySecAgentTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQrySecAgentTradingAccount>");
	if (pTradingAccount)
	{
        PURE_LOG("\tBrokerID [%s]", pTradingAccount->BrokerID);
        PURE_LOG("\tAccountID [%s]", pTradingAccount->AccountID);
        PURE_LOG("\tTradingDay [%s]", pTradingAccount->TradingDay);
        PURE_LOG("\tCurrencyID [%s]", pTradingAccount->CurrencyID);
        PURE_LOG("\tSettlementID [%d]", pTradingAccount->SettlementID);
        PURE_LOG("\tBizType [%c]", pTradingAccount->BizType);
        PURE_LOG("\tPreMortgage [%.8lf]", pTradingAccount->PreMortgage);
        PURE_LOG("\tPreCredit [%.8lf]", pTradingAccount->PreCredit);
        PURE_LOG("\tPreDeposit [%.8lf]", pTradingAccount->PreDeposit);
        PURE_LOG("\tPreBalance [%.8lf]", pTradingAccount->PreBalance);
        PURE_LOG("\tPreMargin [%.8lf]", pTradingAccount->PreMargin);
        PURE_LOG("\tInterestBase [%.8lf]", pTradingAccount->InterestBase);
        PURE_LOG("\tInterest [%.8lf]", pTradingAccount->Interest);
        PURE_LOG("\tDeposit [%.8lf]", pTradingAccount->Deposit);
        PURE_LOG("\tWithdraw [%.8lf]", pTradingAccount->Withdraw);
        PURE_LOG("\tFrozenMargin [%.8lf]", pTradingAccount->FrozenMargin);
        PURE_LOG("\tFrozenCash [%.8lf]", pTradingAccount->FrozenCash);
        PURE_LOG("\tFrozenCommission [%.8lf]", pTradingAccount->FrozenCommission);
        PURE_LOG("\tCurrMargin [%.8lf]", pTradingAccount->CurrMargin);
        PURE_LOG("\tCashIn [%.8lf]", pTradingAccount->CashIn);
        PURE_LOG("\tCommission [%.8lf]", pTradingAccount->Commission);
        PURE_LOG("\tCloseProfit [%.8lf]", pTradingAccount->CloseProfit);
        PURE_LOG("\tPositionProfit [%.8lf]", pTradingAccount->PositionProfit);
        PURE_LOG("\tBalance [%.8lf]", pTradingAccount->Balance);
        PURE_LOG("\tAvailable [%.8lf]", pTradingAccount->Available);
        PURE_LOG("\tWithdrawQuota [%.8lf]", pTradingAccount->WithdrawQuota);
        PURE_LOG("\tReserve [%.8lf]", pTradingAccount->Reserve);
        PURE_LOG("\tCredit [%.8lf]", pTradingAccount->Credit);
        PURE_LOG("\tMortgage [%.8lf]", pTradingAccount->Mortgage);
        PURE_LOG("\tExchangeMargin [%.8lf]", pTradingAccount->ExchangeMargin);
        PURE_LOG("\tDeliveryMargin [%.8lf]", pTradingAccount->DeliveryMargin);
        PURE_LOG("\tExchangeDeliveryMargin [%.8lf]", pTradingAccount->ExchangeDeliveryMargin);
        PURE_LOG("\tReserveBalance [%.8lf]", pTradingAccount->ReserveBalance);
        PURE_LOG("\tPreFundMortgageIn [%.8lf]", pTradingAccount->PreFundMortgageIn);
        PURE_LOG("\tPreFundMortgageOut [%.8lf]", pTradingAccount->PreFundMortgageOut);
        PURE_LOG("\tFundMortgageIn [%.8lf]", pTradingAccount->FundMortgageIn);
        PURE_LOG("\tFundMortgageOut [%.8lf]", pTradingAccount->FundMortgageOut);
        PURE_LOG("\tFundMortgageAvailable [%.8lf]", pTradingAccount->FundMortgageAvailable);
        PURE_LOG("\tMortgageableFund [%.8lf]", pTradingAccount->MortgageableFund);
        PURE_LOG("\tSpecProductMargin [%.8lf]", pTradingAccount->SpecProductMargin);
        PURE_LOG("\tSpecProductFrozenMargin [%.8lf]", pTradingAccount->SpecProductFrozenMargin);
        PURE_LOG("\tSpecProductCommission [%.8lf]", pTradingAccount->SpecProductCommission);
        PURE_LOG("\tSpecProductFrozenCommission [%.8lf]", pTradingAccount->SpecProductFrozenCommission);
        PURE_LOG("\tSpecProductPositionProfit [%.8lf]", pTradingAccount->SpecProductPositionProfit);
        PURE_LOG("\tSpecProductCloseProfit [%.8lf]", pTradingAccount->SpecProductCloseProfit);
        PURE_LOG("\tSpecProductPositionProfitByAlg [%.8lf]", pTradingAccount->SpecProductPositionProfitByAlg);
        PURE_LOG("\tSpecProductExchangeMargin [%.8lf]", pTradingAccount->SpecProductExchangeMargin);
        PURE_LOG("\tFrozenSwap [%.8lf]", pTradingAccount->FrozenSwap);
        PURE_LOG("\tRemainSwap [%.8lf]", pTradingAccount->RemainSwap);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQrySecAgentTradingAccount>");
};

void CTraderSpi::OnRspQrySecAgentCheckMode(CThostFtdcSecAgentCheckModeField *pSecAgentCheckMode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQrySecAgentCheckMode>");
	if (pSecAgentCheckMode)
	{
        PURE_LOG("\tInvestorID [%s]", pSecAgentCheckMode->InvestorID);
        PURE_LOG("\tBrokerID [%s]", pSecAgentCheckMode->BrokerID);
        PURE_LOG("\tCurrencyID [%s]", pSecAgentCheckMode->CurrencyID);
        PURE_LOG("\tBrokerSecAgentID [%s]", pSecAgentCheckMode->BrokerSecAgentID);
        PURE_LOG("\tCheckSelfAccount [%d]", pSecAgentCheckMode->CheckSelfAccount);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQrySecAgentCheckMode>");
};

void CTraderSpi::OnRspQrySecAgentTradeInfo(CThostFtdcSecAgentTradeInfoField *pSecAgentTradeInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQrySecAgentTradeInfo>");
	if (pSecAgentTradeInfo)
	{
        PURE_LOG("\tBrokerID [%s]", pSecAgentTradeInfo->BrokerID);
        PURE_LOG("\tBrokerSecAgentID [%s]", pSecAgentTradeInfo->BrokerSecAgentID);
        PURE_LOG("\tInvestorID [%s]", pSecAgentTradeInfo->InvestorID);
        PURE_LOG("\tLongCustomerName [%s]", pSecAgentTradeInfo->LongCustomerName);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQrySecAgentTradeInfo>");
};

void CTraderSpi::OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField *pOptionInstrTradeCost, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryOptionInstrTradeCost>");
	if (pOptionInstrTradeCost)
	{
        PURE_LOG("\tBrokerID [%s]", pOptionInstrTradeCost->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pOptionInstrTradeCost->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pOptionInstrTradeCost->InstrumentID);
        PURE_LOG("\tExchangeID [%s]", pOptionInstrTradeCost->ExchangeID);
        PURE_LOG("\tInvestUnitID [%s]", pOptionInstrTradeCost->InvestUnitID);
        PURE_LOG("\tHedgeFlag [%c]", pOptionInstrTradeCost->HedgeFlag);
        PURE_LOG("\tFixedMargin [%.8lf]", pOptionInstrTradeCost->FixedMargin);
        PURE_LOG("\tMiniMargin [%.8lf]", pOptionInstrTradeCost->MiniMargin);
        PURE_LOG("\tRoyalty [%.8lf]", pOptionInstrTradeCost->Royalty);
        PURE_LOG("\tExchFixedMargin [%.8lf]", pOptionInstrTradeCost->ExchFixedMargin);
        PURE_LOG("\tExchMiniMargin [%.8lf]", pOptionInstrTradeCost->ExchMiniMargin);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryOptionInstrTradeCost>");
};

void CTraderSpi::OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField *pOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryOptionInstrCommRate>");
	if (pOptionInstrCommRate)
	{
        PURE_LOG("\tInstrumentID [%s]", pOptionInstrCommRate->InstrumentID);
        PURE_LOG("\tBrokerID [%s]", pOptionInstrCommRate->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pOptionInstrCommRate->InvestorID);
        PURE_LOG("\tExchangeID [%s]", pOptionInstrCommRate->ExchangeID);
        PURE_LOG("\tInvestUnitID [%s]", pOptionInstrCommRate->InvestUnitID);
        PURE_LOG("\tInvestorRange [%c]", pOptionInstrCommRate->InvestorRange);
        PURE_LOG("\tOpenRatioByMoney [%.8lf]", pOptionInstrCommRate->OpenRatioByMoney);
        PURE_LOG("\tOpenRatioByVolume [%.8lf]", pOptionInstrCommRate->OpenRatioByVolume);
        PURE_LOG("\tCloseRatioByMoney [%.8lf]", pOptionInstrCommRate->CloseRatioByMoney);
        PURE_LOG("\tCloseRatioByVolume [%.8lf]", pOptionInstrCommRate->CloseRatioByVolume);
        PURE_LOG("\tCloseTodayRatioByMoney [%.8lf]", pOptionInstrCommRate->CloseTodayRatioByMoney);
        PURE_LOG("\tCloseTodayRatioByVolume [%.8lf]", pOptionInstrCommRate->CloseTodayRatioByVolume);
        PURE_LOG("\tStrikeRatioByMoney [%.8lf]", pOptionInstrCommRate->StrikeRatioByMoney);
        PURE_LOG("\tStrikeRatioByVolume [%.8lf]", pOptionInstrCommRate->StrikeRatioByVolume);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryOptionInstrCommRate>");
};

void CTraderSpi::OnRspQryExecOrder(CThostFtdcExecOrderField *pExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryExecOrder>");
	if (pExecOrder)
	{
        PURE_LOG("\tBrokerID [%s]", pExecOrder->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pExecOrder->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pExecOrder->InstrumentID);
        PURE_LOG("\tExecOrderRef [%s]", pExecOrder->ExecOrderRef);
        PURE_LOG("\tUserID [%s]", pExecOrder->UserID);
        PURE_LOG("\tBusinessUnit [%s]", pExecOrder->BusinessUnit);
        PURE_LOG("\tExecOrderLocalID [%s]", pExecOrder->ExecOrderLocalID);
        PURE_LOG("\tExchangeID [%s]", pExecOrder->ExchangeID);
        PURE_LOG("\tParticipantID [%s]", pExecOrder->ParticipantID);
        PURE_LOG("\tClientID [%s]", pExecOrder->ClientID);
        PURE_LOG("\tExchangeInstID [%s]", pExecOrder->ExchangeInstID);
        PURE_LOG("\tTraderID [%s]", pExecOrder->TraderID);
        PURE_LOG("\tTradingDay [%s]", pExecOrder->TradingDay);
        PURE_LOG("\tExecOrderSysID [%s]", pExecOrder->ExecOrderSysID);
        PURE_LOG("\tInsertDate [%s]", pExecOrder->InsertDate);
        PURE_LOG("\tInsertTime [%s]", pExecOrder->InsertTime);
        PURE_LOG("\tCancelTime [%s]", pExecOrder->CancelTime);
        PURE_LOG("\tClearingPartID [%s]", pExecOrder->ClearingPartID);
        PURE_LOG("\tUserProductInfo [%s]", pExecOrder->UserProductInfo);
        PURE_LOG("\tStatusMsg [%s]", pExecOrder->StatusMsg);
        PURE_LOG("\tActiveUserID [%s]", pExecOrder->ActiveUserID);
        PURE_LOG("\tBranchID [%s]", pExecOrder->BranchID);
        PURE_LOG("\tInvestUnitID [%s]", pExecOrder->InvestUnitID);
        PURE_LOG("\tAccountID [%s]", pExecOrder->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pExecOrder->CurrencyID);
        PURE_LOG("\tIPAddress [%s]", pExecOrder->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pExecOrder->MacAddress);
        PURE_LOG("\tVolume [%d]", pExecOrder->Volume);
        PURE_LOG("\tRequestID [%d]", pExecOrder->RequestID);
        PURE_LOG("\tInstallID [%d]", pExecOrder->InstallID);
        PURE_LOG("\tNotifySequence [%d]", pExecOrder->NotifySequence);
        PURE_LOG("\tSettlementID [%d]", pExecOrder->SettlementID);
        PURE_LOG("\tSequenceNo [%d]", pExecOrder->SequenceNo);
        PURE_LOG("\tFrontID [%d]", pExecOrder->FrontID);
        PURE_LOG("\tSessionID [%d]", pExecOrder->SessionID);
        PURE_LOG("\tBrokerExecOrderSeq [%d]", pExecOrder->BrokerExecOrderSeq);
        PURE_LOG("\tOffsetFlag [%c]", pExecOrder->OffsetFlag);
        PURE_LOG("\tHedgeFlag [%c]", pExecOrder->HedgeFlag);
        PURE_LOG("\tActionType [%c]", pExecOrder->ActionType);
        PURE_LOG("\tPosiDirection [%c]", pExecOrder->PosiDirection);
        PURE_LOG("\tReservePositionFlag [%c]", pExecOrder->ReservePositionFlag);
        PURE_LOG("\tCloseFlag [%c]", pExecOrder->CloseFlag);
        PURE_LOG("\tOrderSubmitStatus [%c]", pExecOrder->OrderSubmitStatus);
        PURE_LOG("\tExecResult [%c]", pExecOrder->ExecResult);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryExecOrder>");
};

void CTraderSpi::OnRspQryForQuote(CThostFtdcForQuoteField *pForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryForQuote>");
	if (pForQuote)
	{
        PURE_LOG("\tBrokerID [%s]", pForQuote->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pForQuote->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pForQuote->InstrumentID);
        PURE_LOG("\tForQuoteRef [%s]", pForQuote->ForQuoteRef);
        PURE_LOG("\tUserID [%s]", pForQuote->UserID);
        PURE_LOG("\tForQuoteLocalID [%s]", pForQuote->ForQuoteLocalID);
        PURE_LOG("\tExchangeID [%s]", pForQuote->ExchangeID);
        PURE_LOG("\tParticipantID [%s]", pForQuote->ParticipantID);
        PURE_LOG("\tClientID [%s]", pForQuote->ClientID);
        PURE_LOG("\tExchangeInstID [%s]", pForQuote->ExchangeInstID);
        PURE_LOG("\tTraderID [%s]", pForQuote->TraderID);
        PURE_LOG("\tInsertDate [%s]", pForQuote->InsertDate);
        PURE_LOG("\tInsertTime [%s]", pForQuote->InsertTime);
        PURE_LOG("\tStatusMsg [%s]", pForQuote->StatusMsg);
        PURE_LOG("\tActiveUserID [%s]", pForQuote->ActiveUserID);
        PURE_LOG("\tInvestUnitID [%s]", pForQuote->InvestUnitID);
        PURE_LOG("\tIPAddress [%s]", pForQuote->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pForQuote->MacAddress);
        PURE_LOG("\tInstallID [%d]", pForQuote->InstallID);
        PURE_LOG("\tFrontID [%d]", pForQuote->FrontID);
        PURE_LOG("\tSessionID [%d]", pForQuote->SessionID);
        PURE_LOG("\tBrokerForQutoSeq [%d]", pForQuote->BrokerForQutoSeq);
        PURE_LOG("\tForQuoteStatus [%c]", pForQuote->ForQuoteStatus);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryForQuote>");
};

void CTraderSpi::OnRspQryQuote(CThostFtdcQuoteField *pQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryQuote>");
	if (pQuote)
	{
        PURE_LOG("\tBrokerID [%s]", pQuote->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pQuote->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pQuote->InstrumentID);
        PURE_LOG("\tQuoteRef [%s]", pQuote->QuoteRef);
        PURE_LOG("\tUserID [%s]", pQuote->UserID);
        PURE_LOG("\tBusinessUnit [%s]", pQuote->BusinessUnit);
        PURE_LOG("\tQuoteLocalID [%s]", pQuote->QuoteLocalID);
        PURE_LOG("\tExchangeID [%s]", pQuote->ExchangeID);
        PURE_LOG("\tParticipantID [%s]", pQuote->ParticipantID);
        PURE_LOG("\tClientID [%s]", pQuote->ClientID);
        PURE_LOG("\tExchangeInstID [%s]", pQuote->ExchangeInstID);
        PURE_LOG("\tTraderID [%s]", pQuote->TraderID);
        PURE_LOG("\tTradingDay [%s]", pQuote->TradingDay);
        PURE_LOG("\tQuoteSysID [%s]", pQuote->QuoteSysID);
        PURE_LOG("\tInsertDate [%s]", pQuote->InsertDate);
        PURE_LOG("\tInsertTime [%s]", pQuote->InsertTime);
        PURE_LOG("\tCancelTime [%s]", pQuote->CancelTime);
        PURE_LOG("\tClearingPartID [%s]", pQuote->ClearingPartID);
        PURE_LOG("\tAskOrderSysID [%s]", pQuote->AskOrderSysID);
        PURE_LOG("\tBidOrderSysID [%s]", pQuote->BidOrderSysID);
        PURE_LOG("\tUserProductInfo [%s]", pQuote->UserProductInfo);
        PURE_LOG("\tStatusMsg [%s]", pQuote->StatusMsg);
        PURE_LOG("\tActiveUserID [%s]", pQuote->ActiveUserID);
        PURE_LOG("\tAskOrderRef [%s]", pQuote->AskOrderRef);
        PURE_LOG("\tBidOrderRef [%s]", pQuote->BidOrderRef);
        PURE_LOG("\tForQuoteSysID [%s]", pQuote->ForQuoteSysID);
        PURE_LOG("\tBranchID [%s]", pQuote->BranchID);
        PURE_LOG("\tInvestUnitID [%s]", pQuote->InvestUnitID);
        PURE_LOG("\tAccountID [%s]", pQuote->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pQuote->CurrencyID);
        PURE_LOG("\tIPAddress [%s]", pQuote->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pQuote->MacAddress);
        PURE_LOG("\tAskVolume [%d]", pQuote->AskVolume);
        PURE_LOG("\tBidVolume [%d]", pQuote->BidVolume);
        PURE_LOG("\tRequestID [%d]", pQuote->RequestID);
        PURE_LOG("\tInstallID [%d]", pQuote->InstallID);
        PURE_LOG("\tNotifySequence [%d]", pQuote->NotifySequence);
        PURE_LOG("\tSettlementID [%d]", pQuote->SettlementID);
        PURE_LOG("\tSequenceNo [%d]", pQuote->SequenceNo);
        PURE_LOG("\tFrontID [%d]", pQuote->FrontID);
        PURE_LOG("\tSessionID [%d]", pQuote->SessionID);
        PURE_LOG("\tBrokerQuoteSeq [%d]", pQuote->BrokerQuoteSeq);
        PURE_LOG("\tAskOffsetFlag [%c]", pQuote->AskOffsetFlag);
        PURE_LOG("\tBidOffsetFlag [%c]", pQuote->BidOffsetFlag);
        PURE_LOG("\tAskHedgeFlag [%c]", pQuote->AskHedgeFlag);
        PURE_LOG("\tBidHedgeFlag [%c]", pQuote->BidHedgeFlag);
        PURE_LOG("\tOrderSubmitStatus [%c]", pQuote->OrderSubmitStatus);
        PURE_LOG("\tQuoteStatus [%c]", pQuote->QuoteStatus);
        PURE_LOG("\tAskPrice [%.8lf]", pQuote->AskPrice);
        PURE_LOG("\tBidPrice [%.8lf]", pQuote->BidPrice);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryQuote>");
};

void CTraderSpi::OnRspQryOptionSelfClose(CThostFtdcOptionSelfCloseField *pOptionSelfClose, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryOptionSelfClose>");
	if (pOptionSelfClose)
	{
        PURE_LOG("\tBrokerID [%s]", pOptionSelfClose->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pOptionSelfClose->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pOptionSelfClose->InstrumentID);
        PURE_LOG("\tOptionSelfCloseRef [%s]", pOptionSelfClose->OptionSelfCloseRef);
        PURE_LOG("\tUserID [%s]", pOptionSelfClose->UserID);
        PURE_LOG("\tBusinessUnit [%s]", pOptionSelfClose->BusinessUnit);
        PURE_LOG("\tOptionSelfCloseLocalID [%s]", pOptionSelfClose->OptionSelfCloseLocalID);
        PURE_LOG("\tExchangeID [%s]", pOptionSelfClose->ExchangeID);
        PURE_LOG("\tParticipantID [%s]", pOptionSelfClose->ParticipantID);
        PURE_LOG("\tClientID [%s]", pOptionSelfClose->ClientID);
        PURE_LOG("\tExchangeInstID [%s]", pOptionSelfClose->ExchangeInstID);
        PURE_LOG("\tTraderID [%s]", pOptionSelfClose->TraderID);
        PURE_LOG("\tTradingDay [%s]", pOptionSelfClose->TradingDay);
        PURE_LOG("\tOptionSelfCloseSysID [%s]", pOptionSelfClose->OptionSelfCloseSysID);
        PURE_LOG("\tInsertDate [%s]", pOptionSelfClose->InsertDate);
        PURE_LOG("\tInsertTime [%s]", pOptionSelfClose->InsertTime);
        PURE_LOG("\tCancelTime [%s]", pOptionSelfClose->CancelTime);
        PURE_LOG("\tClearingPartID [%s]", pOptionSelfClose->ClearingPartID);
        PURE_LOG("\tUserProductInfo [%s]", pOptionSelfClose->UserProductInfo);
        PURE_LOG("\tStatusMsg [%s]", pOptionSelfClose->StatusMsg);
        PURE_LOG("\tActiveUserID [%s]", pOptionSelfClose->ActiveUserID);
        PURE_LOG("\tBranchID [%s]", pOptionSelfClose->BranchID);
        PURE_LOG("\tInvestUnitID [%s]", pOptionSelfClose->InvestUnitID);
        PURE_LOG("\tAccountID [%s]", pOptionSelfClose->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pOptionSelfClose->CurrencyID);
        PURE_LOG("\tIPAddress [%s]", pOptionSelfClose->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pOptionSelfClose->MacAddress);
        PURE_LOG("\tVolume [%d]", pOptionSelfClose->Volume);
        PURE_LOG("\tRequestID [%d]", pOptionSelfClose->RequestID);
        PURE_LOG("\tInstallID [%d]", pOptionSelfClose->InstallID);
        PURE_LOG("\tNotifySequence [%d]", pOptionSelfClose->NotifySequence);
        PURE_LOG("\tSettlementID [%d]", pOptionSelfClose->SettlementID);
        PURE_LOG("\tSequenceNo [%d]", pOptionSelfClose->SequenceNo);
        PURE_LOG("\tFrontID [%d]", pOptionSelfClose->FrontID);
        PURE_LOG("\tSessionID [%d]", pOptionSelfClose->SessionID);
        PURE_LOG("\tBrokerOptionSelfCloseSeq [%d]", pOptionSelfClose->BrokerOptionSelfCloseSeq);
        PURE_LOG("\tHedgeFlag [%c]", pOptionSelfClose->HedgeFlag);
        PURE_LOG("\tOptSelfCloseFlag [%c]", pOptionSelfClose->OptSelfCloseFlag);
        PURE_LOG("\tOrderSubmitStatus [%c]", pOptionSelfClose->OrderSubmitStatus);
        PURE_LOG("\tExecResult [%c]", pOptionSelfClose->ExecResult);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryOptionSelfClose>");
};

void CTraderSpi::OnRspQryInvestUnit(CThostFtdcInvestUnitField *pInvestUnit, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryInvestUnit>");
	if (pInvestUnit)
	{
        PURE_LOG("\tBrokerID [%s]", pInvestUnit->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInvestUnit->InvestorID);
        PURE_LOG("\tInvestUnitID [%s]", pInvestUnit->InvestUnitID);
        PURE_LOG("\tInvestorUnitName [%s]", pInvestUnit->InvestorUnitName);
        PURE_LOG("\tInvestorGroupID [%s]", pInvestUnit->InvestorGroupID);
        PURE_LOG("\tCommModelID [%s]", pInvestUnit->CommModelID);
        PURE_LOG("\tMarginModelID [%s]", pInvestUnit->MarginModelID);
        PURE_LOG("\tAccountID [%s]", pInvestUnit->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pInvestUnit->CurrencyID);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryInvestUnit>");
};

void CTraderSpi::OnRspQryCombInstrumentGuard(CThostFtdcCombInstrumentGuardField *pCombInstrumentGuard, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryCombInstrumentGuard>");
	if (pCombInstrumentGuard)
	{
        PURE_LOG("\tBrokerID [%s]", pCombInstrumentGuard->BrokerID);
        PURE_LOG("\tInstrumentID [%s]", pCombInstrumentGuard->InstrumentID);
        PURE_LOG("\tExchangeID [%s]", pCombInstrumentGuard->ExchangeID);
        PURE_LOG("\tGuarantRatio [%.8lf]", pCombInstrumentGuard->GuarantRatio);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryCombInstrumentGuard>");
};

void CTraderSpi::OnRspQryCombAction(CThostFtdcCombActionField *pCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryCombAction>");
	if (pCombAction)
	{
        PURE_LOG("\tBrokerID [%s]", pCombAction->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pCombAction->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pCombAction->InstrumentID);
        PURE_LOG("\tCombActionRef [%s]", pCombAction->CombActionRef);
        PURE_LOG("\tUserID [%s]", pCombAction->UserID);
        PURE_LOG("\tActionLocalID [%s]", pCombAction->ActionLocalID);
        PURE_LOG("\tExchangeID [%s]", pCombAction->ExchangeID);
        PURE_LOG("\tParticipantID [%s]", pCombAction->ParticipantID);
        PURE_LOG("\tClientID [%s]", pCombAction->ClientID);
        PURE_LOG("\tExchangeInstID [%s]", pCombAction->ExchangeInstID);
        PURE_LOG("\tTraderID [%s]", pCombAction->TraderID);
        PURE_LOG("\tTradingDay [%s]", pCombAction->TradingDay);
        PURE_LOG("\tUserProductInfo [%s]", pCombAction->UserProductInfo);
        PURE_LOG("\tStatusMsg [%s]", pCombAction->StatusMsg);
        PURE_LOG("\tIPAddress [%s]", pCombAction->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pCombAction->MacAddress);
        PURE_LOG("\tComTradeID [%s]", pCombAction->ComTradeID);
        PURE_LOG("\tBranchID [%s]", pCombAction->BranchID);
        PURE_LOG("\tInvestUnitID [%s]", pCombAction->InvestUnitID);
        PURE_LOG("\tVolume [%d]", pCombAction->Volume);
        PURE_LOG("\tInstallID [%d]", pCombAction->InstallID);
        PURE_LOG("\tNotifySequence [%d]", pCombAction->NotifySequence);
        PURE_LOG("\tSettlementID [%d]", pCombAction->SettlementID);
        PURE_LOG("\tSequenceNo [%d]", pCombAction->SequenceNo);
        PURE_LOG("\tFrontID [%d]", pCombAction->FrontID);
        PURE_LOG("\tSessionID [%d]", pCombAction->SessionID);
        PURE_LOG("\tDirection [%c]", pCombAction->Direction);
        PURE_LOG("\tCombDirection [%c]", pCombAction->CombDirection);
        PURE_LOG("\tHedgeFlag [%c]", pCombAction->HedgeFlag);
        PURE_LOG("\tActionStatus [%c]", pCombAction->ActionStatus);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryCombAction>");
};

void CTraderSpi::OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryTransferSerial>");
	if (pTransferSerial)
	{
        PURE_LOG("\tTradeDate [%s]", pTransferSerial->TradeDate);
        PURE_LOG("\tTradingDay [%s]", pTransferSerial->TradingDay);
        PURE_LOG("\tTradeTime [%s]", pTransferSerial->TradeTime);
        PURE_LOG("\tTradeCode [%s]", pTransferSerial->TradeCode);
        PURE_LOG("\tBankID [%s]", pTransferSerial->BankID);
        PURE_LOG("\tBankBranchID [%s]", pTransferSerial->BankBranchID);
        PURE_LOG("\tBankAccount [%s]", pTransferSerial->BankAccount);
        PURE_LOG("\tBankSerial [%s]", pTransferSerial->BankSerial);
        PURE_LOG("\tBrokerID [%s]", pTransferSerial->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pTransferSerial->BrokerBranchID);
        PURE_LOG("\tAccountID [%s]", pTransferSerial->AccountID);
        PURE_LOG("\tInvestorID [%s]", pTransferSerial->InvestorID);
        PURE_LOG("\tIdentifiedCardNo [%s]", pTransferSerial->IdentifiedCardNo);
        PURE_LOG("\tCurrencyID [%s]", pTransferSerial->CurrencyID);
        PURE_LOG("\tOperatorCode [%s]", pTransferSerial->OperatorCode);
        PURE_LOG("\tBankNewAccount [%s]", pTransferSerial->BankNewAccount);
        PURE_LOG("\tErrorMsg [%s]", pTransferSerial->ErrorMsg);
        PURE_LOG("\tPlateSerial [%d]", pTransferSerial->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pTransferSerial->SessionID);
        PURE_LOG("\tFutureSerial [%d]", pTransferSerial->FutureSerial);
        PURE_LOG("\tErrorID [%d]", pTransferSerial->ErrorID);
        PURE_LOG("\tBankAccType [%c]", pTransferSerial->BankAccType);
        PURE_LOG("\tFutureAccType [%c]", pTransferSerial->FutureAccType);
        PURE_LOG("\tIdCardType [%c]", pTransferSerial->IdCardType);
        PURE_LOG("\tAvailabilityFlag [%c]", pTransferSerial->AvailabilityFlag);
        PURE_LOG("\tTradeAmount [%.8lf]", pTransferSerial->TradeAmount);
        PURE_LOG("\tCustFee [%.8lf]", pTransferSerial->CustFee);
        PURE_LOG("\tBrokerFee [%.8lf]", pTransferSerial->BrokerFee);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryTransferSerial>");
};

void CTraderSpi::OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryAccountregister>");
	if (pAccountregister)
	{
        PURE_LOG("\tTradeDay [%s]", pAccountregister->TradeDay);
        PURE_LOG("\tBankID [%s]", pAccountregister->BankID);
        PURE_LOG("\tBankBranchID [%s]", pAccountregister->BankBranchID);
        PURE_LOG("\tBankAccount [%s]", pAccountregister->BankAccount);
        PURE_LOG("\tBrokerID [%s]", pAccountregister->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pAccountregister->BrokerBranchID);
        PURE_LOG("\tAccountID [%s]", pAccountregister->AccountID);
        PURE_LOG("\tIdentifiedCardNo [%s]", pAccountregister->IdentifiedCardNo);
        PURE_LOG("\tCustomerName [%s]", pAccountregister->CustomerName);
        PURE_LOG("\tCurrencyID [%s]", pAccountregister->CurrencyID);
        PURE_LOG("\tRegDate [%s]", pAccountregister->RegDate);
        PURE_LOG("\tOutDate [%s]", pAccountregister->OutDate);
        PURE_LOG("\tLongCustomerName [%s]", pAccountregister->LongCustomerName);
        PURE_LOG("\tTID [%d]", pAccountregister->TID);
        PURE_LOG("\tIdCardType [%c]", pAccountregister->IdCardType);
        PURE_LOG("\tOpenOrDestroy [%c]", pAccountregister->OpenOrDestroy);
        PURE_LOG("\tCustType [%c]", pAccountregister->CustType);
        PURE_LOG("\tBankAccType [%c]", pAccountregister->BankAccType);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryAccountregister>");
};

void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspError>");
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspError>");
};
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
    MSG_LOG("%s","******<OnRtnOrder>******");
	if (pOrder)
	{
        PURE_LOG("\tBrokerID [%s]", pOrder->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pOrder->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pOrder->InstrumentID);

        PURE_LOG("\tOrderRef [%s]", pOrder->OrderRef);
        PURE_LOG("\tUserID [%s]", pOrder->UserID);
        PURE_LOG("\tCombOffsetFlag [%s]", pOrder->CombOffsetFlag);

        PURE_LOG("\tCombHedgeFlag [%s]", pOrder->CombHedgeFlag);
        PURE_LOG("\tGTDDate [%s]", pOrder->GTDDate);
        PURE_LOG("\tBusinessUnit [%s]", pOrder->BusinessUnit);
        PURE_LOG("\tOrderLocalID [%s]", pOrder->OrderLocalID);
        PURE_LOG("\tExchangeID [%s]", pOrder->ExchangeID);

        PURE_LOG("\tParticipantID [%s]", pOrder->ParticipantID);
        PURE_LOG("\tClientID [%s]", pOrder->ClientID);
        PURE_LOG("\tExchangeInstID [%s]", pOrder->ExchangeInstID);
        PURE_LOG("\tTraderID [%s]", pOrder->TraderID);
        PURE_LOG("\tTradingDay [%s]", pOrder->TradingDay);

        PURE_LOG("\tOrderSysID [%s]", pOrder->OrderSysID);

        PURE_LOG("\tInsertDate [%s]", pOrder->InsertDate);
        PURE_LOG("\tInsertTime [%s]", pOrder->InsertTime);


        PURE_LOG("\tActiveTime [%s]", pOrder->ActiveTime);
        PURE_LOG("\tSuspendTime [%s]", pOrder->SuspendTime);
        PURE_LOG("\tUpdateTime [%s]", pOrder->UpdateTime);
        PURE_LOG("\tCancelTime [%s]", pOrder->CancelTime);
        PURE_LOG("\tActiveTraderID [%s]", pOrder->ActiveTraderID);
        PURE_LOG("\tClearingPartID [%s]", pOrder->ClearingPartID);
        PURE_LOG("\tUserProductInfo [%s]", pOrder->UserProductInfo);

        TThostFtdcErrorMsgType msg;
        gbk2utf8(pOrder->StatusMsg, msg, sizeof(msg));
        PURE_LOG("\tStatusMsg [%s]", msg);

        PURE_LOG("\tActiveUserID [%s]", pOrder->ActiveUserID);
        PURE_LOG("\tRelativeOrderSysID [%s]", pOrder->RelativeOrderSysID);
        PURE_LOG("\tBranchID [%s]", pOrder->BranchID);
        PURE_LOG("\tInvestUnitID [%s]", pOrder->InvestUnitID);
        PURE_LOG("\tAccountID [%s]", pOrder->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pOrder->CurrencyID);
        PURE_LOG("\tIPAddress [%s]", pOrder->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pOrder->MacAddress);
        PURE_LOG("\tVolumeTotalOriginal [%d]", pOrder->VolumeTotalOriginal);

        PURE_LOG("\tMinVolume [%d]", pOrder->MinVolume);
        PURE_LOG("\tIsAutoSuspend [%d]", pOrder->IsAutoSuspend);
        PURE_LOG("\tRequestID [%d]", pOrder->RequestID);
        PURE_LOG("\tInstallID [%d]", pOrder->InstallID);
        PURE_LOG("\tNotifySequence [%d]", pOrder->NotifySequence);
        PURE_LOG("\tSettlementID [%d]", pOrder->SettlementID);
        PURE_LOG("\tVolumeTraded [%d]", pOrder->VolumeTraded);
        PURE_LOG("\tVolumeTotal [%d]", pOrder->VolumeTotal);
        PURE_LOG("\tSequenceNo [%d]", pOrder->SequenceNo);
        PURE_LOG("\tFrontID [%d]", pOrder->FrontID);
        PURE_LOG("\tSessionID [%d]", pOrder->SessionID);

        PURE_LOG("\tUserForceClose [%d]", pOrder->UserForceClose);
        PURE_LOG("\tBrokerOrderSeq [%d]", pOrder->BrokerOrderSeq);
        PURE_LOG("\tZCETotalTradedVolume [%d]", pOrder->ZCETotalTradedVolume);
        PURE_LOG("\tIsSwapOrder [%d]", pOrder->IsSwapOrder);
        PURE_LOG("\tOrderPriceType [%c]", pOrder->OrderPriceType);
        PURE_LOG("\tDirection [%c]", pOrder->Direction);

        PURE_LOG("\tTimeCondition [%c]", pOrder->TimeCondition);
        PURE_LOG("\tVolumeCondition [%c]", pOrder->VolumeCondition);
        PURE_LOG("\tContingentCondition [%c]", pOrder->ContingentCondition);
        PURE_LOG("\tForceCloseReason [%c]", pOrder->ForceCloseReason);
        PURE_LOG("\tOrderSubmitStatus [%c]", pOrder->OrderSubmitStatus);
        PURE_LOG("\tOrderSource [%c]", pOrder->OrderSource);
        PURE_LOG("\tOrderStatus [%c]", pOrder->OrderStatus);
        PURE_LOG("\tOrderType [%c]", pOrder->OrderType);
        PURE_LOG("\tLimitPrice [%.8lf]", pOrder->LimitPrice);

        PURE_LOG("\tStopPrice [%.8lf]", pOrder->StopPrice);
	}
    MSG_LOG("%s","******</OnRtnOrder>******");

};
void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
    repResult = InsertResult::Success;
    INFO_LOG("******<OnRtnTrade>******");
	if (pTrade)
	{
        PURE_LOG("\tBrokerID [%s]", pTrade->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pTrade->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pTrade->InstrumentID);
        PURE_LOG("\tOrderRef [%s]", pTrade->OrderRef);
        PURE_LOG("\tUserID [%s]", pTrade->UserID);
        PURE_LOG("\tExchangeID [%s]", pTrade->ExchangeID);
        PURE_LOG("\tTradeID [%s]", pTrade->TradeID);
        PURE_LOG("\tOrderSysID [%s]", pTrade->OrderSysID);
        PURE_LOG("\tParticipantID [%s]", pTrade->ParticipantID);
        PURE_LOG("\tClientID [%s]", pTrade->ClientID);
        PURE_LOG("\tExchangeInstID [%s]", pTrade->ExchangeInstID);
        PURE_LOG("\tTradeDate [%s]", pTrade->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pTrade->TradeTime);
        PURE_LOG("\tTraderID [%s]", pTrade->TraderID);
        PURE_LOG("\tOrderLocalID [%s]", pTrade->OrderLocalID);
        PURE_LOG("\tClearingPartID [%s]", pTrade->ClearingPartID);
        PURE_LOG("\tBusinessUnit [%s]", pTrade->BusinessUnit);
        PURE_LOG("\tTradingDay [%s]", pTrade->TradingDay);
        PURE_LOG("\tInvestUnitID [%s]", pTrade->InvestUnitID);
        PURE_LOG("\tVolume [%d]", pTrade->Volume);
        PURE_LOG("\tSequenceNo [%d]", pTrade->SequenceNo);
        PURE_LOG("\tSettlementID [%d]", pTrade->SettlementID);
        PURE_LOG("\tBrokerOrderSeq [%d]", pTrade->BrokerOrderSeq);
        PURE_LOG("\tDirection [%c]", pTrade->Direction);
        PURE_LOG("\tTradingRole [%c]", pTrade->TradingRole);
        PURE_LOG("\tOffsetFlag [%c]", pTrade->OffsetFlag);
        PURE_LOG("\tHedgeFlag [%c]", pTrade->HedgeFlag);
        PURE_LOG("\tTradeType [%c]", pTrade->TradeType);
        PURE_LOG("\tPriceSource [%c]", pTrade->PriceSource);
        PURE_LOG("\tTradeSource [%c]", pTrade->TradeSource);
        PURE_LOG("\tPrice [%.8lf]", pTrade->Price);
	}
    INFO_LOG("******</OnRtnTrade>******");
    sem_post(&globalSem.sem);
};
void CTraderSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
    PURE_LOG("<OnErrRtnOrderInsert>");
	if (pInputOrder)
	{
        PURE_LOG("\tBrokerID [%s]", pInputOrder->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInputOrder->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pInputOrder->InstrumentID);
        PURE_LOG("\tOrderRef [%s]", pInputOrder->OrderRef);
        PURE_LOG("\tUserID [%s]", pInputOrder->UserID);
        PURE_LOG("\tCombOffsetFlag [%s]", pInputOrder->CombOffsetFlag);
        PURE_LOG("\tCombHedgeFlag [%s]", pInputOrder->CombHedgeFlag);
        PURE_LOG("\tGTDDate [%s]", pInputOrder->GTDDate);
        PURE_LOG("\tBusinessUnit [%s]", pInputOrder->BusinessUnit);
        PURE_LOG("\tExchangeID [%s]", pInputOrder->ExchangeID);
        PURE_LOG("\tInvestUnitID [%s]", pInputOrder->InvestUnitID);
        PURE_LOG("\tAccountID [%s]", pInputOrder->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pInputOrder->CurrencyID);
        PURE_LOG("\tClientID [%s]", pInputOrder->ClientID);
        PURE_LOG("\tIPAddress [%s]", pInputOrder->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pInputOrder->MacAddress);
        PURE_LOG("\tVolumeTotalOriginal [%d]", pInputOrder->VolumeTotalOriginal);
        PURE_LOG("\tMinVolume [%d]", pInputOrder->MinVolume);
        PURE_LOG("\tIsAutoSuspend [%d]", pInputOrder->IsAutoSuspend);
        PURE_LOG("\tRequestID [%d]", pInputOrder->RequestID);
        PURE_LOG("\tUserForceClose [%d]", pInputOrder->UserForceClose);
        PURE_LOG("\tIsSwapOrder [%d]", pInputOrder->IsSwapOrder);
        PURE_LOG("\tOrderPriceType [%c]", pInputOrder->OrderPriceType);
        PURE_LOG("\tDirection [%c]", pInputOrder->Direction);
        PURE_LOG("\tTimeCondition [%c]", pInputOrder->TimeCondition);
        PURE_LOG("\tVolumeCondition [%c]", pInputOrder->VolumeCondition);
        PURE_LOG("\tContingentCondition [%c]", pInputOrder->ContingentCondition);
        PURE_LOG("\tForceCloseReason [%c]", pInputOrder->ForceCloseReason);
        PURE_LOG("\tLimitPrice [%.8lf]", pInputOrder->LimitPrice);
        PURE_LOG("\tStopPrice [%.8lf]", pInputOrder->StopPrice);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("</OnErrRtnOrderInsert>");
};
void CTraderSpi::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
    PURE_LOG("<OnErrRtnOrderAction>");
	if (pOrderAction)
	{
        PURE_LOG("\tBrokerID [%s]", pOrderAction->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pOrderAction->InvestorID);
        PURE_LOG("\tOrderRef [%s]", pOrderAction->OrderRef);
        PURE_LOG("\tExchangeID [%s]", pOrderAction->ExchangeID);
        PURE_LOG("\tOrderSysID [%s]", pOrderAction->OrderSysID);
        PURE_LOG("\tActionDate [%s]", pOrderAction->ActionDate);
        PURE_LOG("\tActionTime [%s]", pOrderAction->ActionTime);
        PURE_LOG("\tTraderID [%s]", pOrderAction->TraderID);
        PURE_LOG("\tOrderLocalID [%s]", pOrderAction->OrderLocalID);
        PURE_LOG("\tActionLocalID [%s]", pOrderAction->ActionLocalID);
        PURE_LOG("\tParticipantID [%s]", pOrderAction->ParticipantID);
        PURE_LOG("\tClientID [%s]", pOrderAction->ClientID);
        PURE_LOG("\tBusinessUnit [%s]", pOrderAction->BusinessUnit);
        PURE_LOG("\tUserID [%s]", pOrderAction->UserID);
        PURE_LOG("\tStatusMsg [%s]", pOrderAction->StatusMsg);
        PURE_LOG("\tInstrumentID [%s]", pOrderAction->InstrumentID);
        PURE_LOG("\tBranchID [%s]", pOrderAction->BranchID);
        PURE_LOG("\tInvestUnitID [%s]", pOrderAction->InvestUnitID);
        PURE_LOG("\tIPAddress [%s]", pOrderAction->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pOrderAction->MacAddress);
        PURE_LOG("\tOrderActionRef [%d]", pOrderAction->OrderActionRef);
        PURE_LOG("\tRequestID [%d]", pOrderAction->RequestID);
        PURE_LOG("\tFrontID [%d]", pOrderAction->FrontID);
        PURE_LOG("\tSessionID [%d]", pOrderAction->SessionID);
        PURE_LOG("\tVolumeChange [%d]", pOrderAction->VolumeChange);
        PURE_LOG("\tInstallID [%d]", pOrderAction->InstallID);
        PURE_LOG("\tActionFlag [%c]", pOrderAction->ActionFlag);
        PURE_LOG("\tOrderActionStatus [%c]", pOrderAction->OrderActionStatus);
        PURE_LOG("\tLimitPrice [%.8lf]", pOrderAction->LimitPrice);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;
        gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
        PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("</OnErrRtnOrderAction>");
};
void CTraderSpi::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
    PURE_LOG("<OnRtnInstrumentStatus>");
	if (pInstrumentStatus)
	{
        PURE_LOG("\tExchangeID [%s]", pInstrumentStatus->ExchangeID);
        PURE_LOG("\tExchangeInstID [%s]", pInstrumentStatus->ExchangeInstID);
        PURE_LOG("\tSettlementGroupID [%s]", pInstrumentStatus->SettlementGroupID);
        PURE_LOG("\tInstrumentID [%s]", pInstrumentStatus->InstrumentID);
        PURE_LOG("\tEnterTime [%s]", pInstrumentStatus->EnterTime);
        PURE_LOG("\tTradingSegmentSN [%d]", pInstrumentStatus->TradingSegmentSN);
        PURE_LOG("\tInstrumentStatus [%c]", pInstrumentStatus->InstrumentStatus);
        PURE_LOG("\tEnterReason [%c]", pInstrumentStatus->EnterReason);
	}
    PURE_LOG("</OnRtnInstrumentStatus>");
};
void CTraderSpi::OnRtnBulletin(CThostFtdcBulletinField *pBulletin)
{
    PURE_LOG("<OnRtnBulletin>");
	if (pBulletin)
	{
        PURE_LOG("\tExchangeID [%s]", pBulletin->ExchangeID);
        PURE_LOG("\tTradingDay [%s]", pBulletin->TradingDay);
        PURE_LOG("\tNewsType [%s]", pBulletin->NewsType);
        PURE_LOG("\tSendTime [%s]", pBulletin->SendTime);
        PURE_LOG("\tAbstract [%s]", pBulletin->Abstract);
        PURE_LOG("\tComeFrom [%s]", pBulletin->ComeFrom);
        PURE_LOG("\tContent [%s]", pBulletin->Content);
        PURE_LOG("\tURLLink [%s]", pBulletin->URLLink);
        PURE_LOG("\tMarketID [%s]", pBulletin->MarketID);
        PURE_LOG("\tBulletinID [%d]", pBulletin->BulletinID);
        PURE_LOG("\tSequenceNo [%d]", pBulletin->SequenceNo);
        PURE_LOG("\tNewsUrgency [%c]", pBulletin->NewsUrgency);
	}
    PURE_LOG("</OnRtnBulletin>");
};
void CTraderSpi::OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo)
{
    PURE_LOG("<OnRtnTradingNotice>");
	if (pTradingNoticeInfo)
	{
        PURE_LOG("\tBrokerID [%s]", pTradingNoticeInfo->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pTradingNoticeInfo->InvestorID);
        PURE_LOG("\tSendTime [%s]", pTradingNoticeInfo->SendTime);
        PURE_LOG("\tFieldContent [%s]", pTradingNoticeInfo->FieldContent);
        PURE_LOG("\tInvestUnitID [%s]", pTradingNoticeInfo->InvestUnitID);
        PURE_LOG("\tSequenceNo [%d]", pTradingNoticeInfo->SequenceNo);
        PURE_LOG("\tSequenceSeries [%d]", pTradingNoticeInfo->SequenceSeries);
	}
    PURE_LOG("</OnRtnTradingNotice>");
};
void CTraderSpi::OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder)
{
    PURE_LOG("<OnRtnErrorConditionalOrder>");
	if (pErrorConditionalOrder)
	{
        PURE_LOG("\tBrokerID [%s]", pErrorConditionalOrder->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pErrorConditionalOrder->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pErrorConditionalOrder->InstrumentID);
        PURE_LOG("\tOrderRef [%s]", pErrorConditionalOrder->OrderRef);
        PURE_LOG("\tUserID [%s]", pErrorConditionalOrder->UserID);
        PURE_LOG("\tCombOffsetFlag [%s]", pErrorConditionalOrder->CombOffsetFlag);
        PURE_LOG("\tCombHedgeFlag [%s]", pErrorConditionalOrder->CombHedgeFlag);
        PURE_LOG("\tGTDDate [%s]", pErrorConditionalOrder->GTDDate);
        PURE_LOG("\tBusinessUnit [%s]", pErrorConditionalOrder->BusinessUnit);
        PURE_LOG("\tOrderLocalID [%s]", pErrorConditionalOrder->OrderLocalID);
        PURE_LOG("\tExchangeID [%s]", pErrorConditionalOrder->ExchangeID);
        PURE_LOG("\tParticipantID [%s]", pErrorConditionalOrder->ParticipantID);
        PURE_LOG("\tClientID [%s]", pErrorConditionalOrder->ClientID);
        PURE_LOG("\tExchangeInstID [%s]", pErrorConditionalOrder->ExchangeInstID);
        PURE_LOG("\tTraderID [%s]", pErrorConditionalOrder->TraderID);
        PURE_LOG("\tTradingDay [%s]", pErrorConditionalOrder->TradingDay);
        PURE_LOG("\tOrderSysID [%s]", pErrorConditionalOrder->OrderSysID);
        PURE_LOG("\tInsertDate [%s]", pErrorConditionalOrder->InsertDate);
        PURE_LOG("\tInsertTime [%s]", pErrorConditionalOrder->InsertTime);
        PURE_LOG("\tActiveTime [%s]", pErrorConditionalOrder->ActiveTime);
        PURE_LOG("\tSuspendTime [%s]", pErrorConditionalOrder->SuspendTime);
        PURE_LOG("\tUpdateTime [%s]", pErrorConditionalOrder->UpdateTime);
        PURE_LOG("\tCancelTime [%s]", pErrorConditionalOrder->CancelTime);
        PURE_LOG("\tActiveTraderID [%s]", pErrorConditionalOrder->ActiveTraderID);
        PURE_LOG("\tClearingPartID [%s]", pErrorConditionalOrder->ClearingPartID);
        PURE_LOG("\tUserProductInfo [%s]", pErrorConditionalOrder->UserProductInfo);
        PURE_LOG("\tStatusMsg [%s]", pErrorConditionalOrder->StatusMsg);
        PURE_LOG("\tActiveUserID [%s]", pErrorConditionalOrder->ActiveUserID);
        PURE_LOG("\tRelativeOrderSysID [%s]", pErrorConditionalOrder->RelativeOrderSysID);
        PURE_LOG("\tErrorMsg [%s]", pErrorConditionalOrder->ErrorMsg);
        PURE_LOG("\tBranchID [%s]", pErrorConditionalOrder->BranchID);
        PURE_LOG("\tInvestUnitID [%s]", pErrorConditionalOrder->InvestUnitID);
        PURE_LOG("\tAccountID [%s]", pErrorConditionalOrder->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pErrorConditionalOrder->CurrencyID);
        PURE_LOG("\tIPAddress [%s]", pErrorConditionalOrder->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pErrorConditionalOrder->MacAddress);
        PURE_LOG("\tVolumeTotalOriginal [%d]", pErrorConditionalOrder->VolumeTotalOriginal);
        PURE_LOG("\tMinVolume [%d]", pErrorConditionalOrder->MinVolume);
        PURE_LOG("\tIsAutoSuspend [%d]", pErrorConditionalOrder->IsAutoSuspend);
        PURE_LOG("\tRequestID [%d]", pErrorConditionalOrder->RequestID);
        PURE_LOG("\tInstallID [%d]", pErrorConditionalOrder->InstallID);
        PURE_LOG("\tNotifySequence [%d]", pErrorConditionalOrder->NotifySequence);
        PURE_LOG("\tSettlementID [%d]", pErrorConditionalOrder->SettlementID);
        PURE_LOG("\tVolumeTraded [%d]", pErrorConditionalOrder->VolumeTraded);
        PURE_LOG("\tVolumeTotal [%d]", pErrorConditionalOrder->VolumeTotal);
        PURE_LOG("\tSequenceNo [%d]", pErrorConditionalOrder->SequenceNo);
        PURE_LOG("\tFrontID [%d]", pErrorConditionalOrder->FrontID);
        PURE_LOG("\tSessionID [%d]", pErrorConditionalOrder->SessionID);
        PURE_LOG("\tUserForceClose [%d]", pErrorConditionalOrder->UserForceClose);
        PURE_LOG("\tBrokerOrderSeq [%d]", pErrorConditionalOrder->BrokerOrderSeq);
        PURE_LOG("\tZCETotalTradedVolume [%d]", pErrorConditionalOrder->ZCETotalTradedVolume);
        PURE_LOG("\tErrorID [%d]", pErrorConditionalOrder->ErrorID);
        PURE_LOG("\tIsSwapOrder [%d]", pErrorConditionalOrder->IsSwapOrder);
        PURE_LOG("\tOrderPriceType [%c]", pErrorConditionalOrder->OrderPriceType);
        PURE_LOG("\tDirection [%c]", pErrorConditionalOrder->Direction);
        PURE_LOG("\tTimeCondition [%c]", pErrorConditionalOrder->TimeCondition);
        PURE_LOG("\tVolumeCondition [%c]", pErrorConditionalOrder->VolumeCondition);
        PURE_LOG("\tContingentCondition [%c]", pErrorConditionalOrder->ContingentCondition);
        PURE_LOG("\tForceCloseReason [%c]", pErrorConditionalOrder->ForceCloseReason);
        PURE_LOG("\tOrderSubmitStatus [%c]", pErrorConditionalOrder->OrderSubmitStatus);
        PURE_LOG("\tOrderSource [%c]", pErrorConditionalOrder->OrderSource);
        PURE_LOG("\tOrderStatus [%c]", pErrorConditionalOrder->OrderStatus);
        PURE_LOG("\tOrderType [%c]", pErrorConditionalOrder->OrderType);
        PURE_LOG("\tLimitPrice [%.8lf]", pErrorConditionalOrder->LimitPrice);
        PURE_LOG("\tStopPrice [%.8lf]", pErrorConditionalOrder->StopPrice);
	}
    PURE_LOG("</OnRtnErrorConditionalOrder>");
};
void CTraderSpi::OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder)
{
    PURE_LOG("<OnRtnExecOrder>");
	if (pExecOrder)
	{
        PURE_LOG("\tBrokerID [%s]", pExecOrder->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pExecOrder->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pExecOrder->InstrumentID);
        PURE_LOG("\tExecOrderRef [%s]", pExecOrder->ExecOrderRef);
        PURE_LOG("\tUserID [%s]", pExecOrder->UserID);
        PURE_LOG("\tBusinessUnit [%s]", pExecOrder->BusinessUnit);
        PURE_LOG("\tExecOrderLocalID [%s]", pExecOrder->ExecOrderLocalID);
        PURE_LOG("\tExchangeID [%s]", pExecOrder->ExchangeID);
        PURE_LOG("\tParticipantID [%s]", pExecOrder->ParticipantID);
        PURE_LOG("\tClientID [%s]", pExecOrder->ClientID);
        PURE_LOG("\tExchangeInstID [%s]", pExecOrder->ExchangeInstID);
        PURE_LOG("\tTraderID [%s]", pExecOrder->TraderID);
        PURE_LOG("\tTradingDay [%s]", pExecOrder->TradingDay);
        PURE_LOG("\tExecOrderSysID [%s]", pExecOrder->ExecOrderSysID);
        PURE_LOG("\tInsertDate [%s]", pExecOrder->InsertDate);
        PURE_LOG("\tInsertTime [%s]", pExecOrder->InsertTime);
        PURE_LOG("\tCancelTime [%s]", pExecOrder->CancelTime);
        PURE_LOG("\tClearingPartID [%s]", pExecOrder->ClearingPartID);
        PURE_LOG("\tUserProductInfo [%s]", pExecOrder->UserProductInfo);
        PURE_LOG("\tStatusMsg [%s]", pExecOrder->StatusMsg);
        PURE_LOG("\tActiveUserID [%s]", pExecOrder->ActiveUserID);
        PURE_LOG("\tBranchID [%s]", pExecOrder->BranchID);
        PURE_LOG("\tInvestUnitID [%s]", pExecOrder->InvestUnitID);
        PURE_LOG("\tAccountID [%s]", pExecOrder->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pExecOrder->CurrencyID);
        PURE_LOG("\tIPAddress [%s]", pExecOrder->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pExecOrder->MacAddress);
        PURE_LOG("\tVolume [%d]", pExecOrder->Volume);
        PURE_LOG("\tRequestID [%d]", pExecOrder->RequestID);
        PURE_LOG("\tInstallID [%d]", pExecOrder->InstallID);
        PURE_LOG("\tNotifySequence [%d]", pExecOrder->NotifySequence);
        PURE_LOG("\tSettlementID [%d]", pExecOrder->SettlementID);
        PURE_LOG("\tSequenceNo [%d]", pExecOrder->SequenceNo);
        PURE_LOG("\tFrontID [%d]", pExecOrder->FrontID);
        PURE_LOG("\tSessionID [%d]", pExecOrder->SessionID);
        PURE_LOG("\tBrokerExecOrderSeq [%d]", pExecOrder->BrokerExecOrderSeq);
        PURE_LOG("\tOffsetFlag [%c]", pExecOrder->OffsetFlag);
        PURE_LOG("\tHedgeFlag [%c]", pExecOrder->HedgeFlag);
        PURE_LOG("\tActionType [%c]", pExecOrder->ActionType);
        PURE_LOG("\tPosiDirection [%c]", pExecOrder->PosiDirection);
        PURE_LOG("\tReservePositionFlag [%c]", pExecOrder->ReservePositionFlag);
        PURE_LOG("\tCloseFlag [%c]", pExecOrder->CloseFlag);
        PURE_LOG("\tOrderSubmitStatus [%c]", pExecOrder->OrderSubmitStatus);
        PURE_LOG("\tExecResult [%c]", pExecOrder->ExecResult);
	}
    PURE_LOG("</OnRtnExecOrder>");
};
void CTraderSpi::OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo)
{
    PURE_LOG("<OnErrRtnExecOrderInsert>");
	if (pInputExecOrder)
	{
        PURE_LOG("\tBrokerID [%s]", pInputExecOrder->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInputExecOrder->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pInputExecOrder->InstrumentID);
        PURE_LOG("\tExecOrderRef [%s]", pInputExecOrder->ExecOrderRef);
        PURE_LOG("\tUserID [%s]", pInputExecOrder->UserID);
        PURE_LOG("\tBusinessUnit [%s]", pInputExecOrder->BusinessUnit);
        PURE_LOG("\tExchangeID [%s]", pInputExecOrder->ExchangeID);
        PURE_LOG("\tInvestUnitID [%s]", pInputExecOrder->InvestUnitID);
        PURE_LOG("\tAccountID [%s]", pInputExecOrder->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pInputExecOrder->CurrencyID);
        PURE_LOG("\tClientID [%s]", pInputExecOrder->ClientID);
        PURE_LOG("\tIPAddress [%s]", pInputExecOrder->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pInputExecOrder->MacAddress);
        PURE_LOG("\tVolume [%d]", pInputExecOrder->Volume);
        PURE_LOG("\tRequestID [%d]", pInputExecOrder->RequestID);
        PURE_LOG("\tOffsetFlag [%c]", pInputExecOrder->OffsetFlag);
        PURE_LOG("\tHedgeFlag [%c]", pInputExecOrder->HedgeFlag);
        PURE_LOG("\tActionType [%c]", pInputExecOrder->ActionType);
        PURE_LOG("\tPosiDirection [%c]", pInputExecOrder->PosiDirection);
        PURE_LOG("\tReservePositionFlag [%c]", pInputExecOrder->ReservePositionFlag);
        PURE_LOG("\tCloseFlag [%c]", pInputExecOrder->CloseFlag);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("</OnErrRtnExecOrderInsert>");
};
void CTraderSpi::OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
    PURE_LOG("<OnErrRtnExecOrderAction>");
	if (pExecOrderAction)
	{
        PURE_LOG("\tBrokerID [%s]", pExecOrderAction->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pExecOrderAction->InvestorID);
        PURE_LOG("\tExecOrderRef [%s]", pExecOrderAction->ExecOrderRef);
        PURE_LOG("\tExchangeID [%s]", pExecOrderAction->ExchangeID);
        PURE_LOG("\tExecOrderSysID [%s]", pExecOrderAction->ExecOrderSysID);
        PURE_LOG("\tActionDate [%s]", pExecOrderAction->ActionDate);
        PURE_LOG("\tActionTime [%s]", pExecOrderAction->ActionTime);
        PURE_LOG("\tTraderID [%s]", pExecOrderAction->TraderID);
        PURE_LOG("\tExecOrderLocalID [%s]", pExecOrderAction->ExecOrderLocalID);
        PURE_LOG("\tActionLocalID [%s]", pExecOrderAction->ActionLocalID);
        PURE_LOG("\tParticipantID [%s]", pExecOrderAction->ParticipantID);
        PURE_LOG("\tClientID [%s]", pExecOrderAction->ClientID);
        PURE_LOG("\tBusinessUnit [%s]", pExecOrderAction->BusinessUnit);
        PURE_LOG("\tUserID [%s]", pExecOrderAction->UserID);
        PURE_LOG("\tStatusMsg [%s]", pExecOrderAction->StatusMsg);
        PURE_LOG("\tInstrumentID [%s]", pExecOrderAction->InstrumentID);
        PURE_LOG("\tBranchID [%s]", pExecOrderAction->BranchID);
        PURE_LOG("\tInvestUnitID [%s]", pExecOrderAction->InvestUnitID);
        PURE_LOG("\tIPAddress [%s]", pExecOrderAction->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pExecOrderAction->MacAddress);
        PURE_LOG("\tExecOrderActionRef [%d]", pExecOrderAction->ExecOrderActionRef);
        PURE_LOG("\tRequestID [%d]", pExecOrderAction->RequestID);
        PURE_LOG("\tFrontID [%d]", pExecOrderAction->FrontID);
        PURE_LOG("\tSessionID [%d]", pExecOrderAction->SessionID);
        PURE_LOG("\tInstallID [%d]", pExecOrderAction->InstallID);
        PURE_LOG("\tActionFlag [%c]", pExecOrderAction->ActionFlag);
        PURE_LOG("\tOrderActionStatus [%c]", pExecOrderAction->OrderActionStatus);
        PURE_LOG("\tActionType [%c]", pExecOrderAction->ActionType);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("</OnErrRtnExecOrderAction>");
};
void CTraderSpi::OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo)
{
    PURE_LOG("<OnErrRtnForQuoteInsert>");
	if (pInputForQuote)
	{
        PURE_LOG("\tBrokerID [%s]", pInputForQuote->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInputForQuote->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pInputForQuote->InstrumentID);
        PURE_LOG("\tForQuoteRef [%s]", pInputForQuote->ForQuoteRef);
        PURE_LOG("\tUserID [%s]", pInputForQuote->UserID);
        PURE_LOG("\tExchangeID [%s]", pInputForQuote->ExchangeID);
        PURE_LOG("\tInvestUnitID [%s]", pInputForQuote->InvestUnitID);
        PURE_LOG("\tIPAddress [%s]", pInputForQuote->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pInputForQuote->MacAddress);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("</OnErrRtnForQuoteInsert>");
};
void CTraderSpi::OnRtnQuote(CThostFtdcQuoteField *pQuote)
{
    PURE_LOG("<OnRtnQuote>");
	if (pQuote)
	{
        PURE_LOG("\tBrokerID [%s]", pQuote->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pQuote->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pQuote->InstrumentID);
        PURE_LOG("\tQuoteRef [%s]", pQuote->QuoteRef);
        PURE_LOG("\tUserID [%s]", pQuote->UserID);
        PURE_LOG("\tBusinessUnit [%s]", pQuote->BusinessUnit);
        PURE_LOG("\tQuoteLocalID [%s]", pQuote->QuoteLocalID);
        PURE_LOG("\tExchangeID [%s]", pQuote->ExchangeID);
        PURE_LOG("\tParticipantID [%s]", pQuote->ParticipantID);
        PURE_LOG("\tClientID [%s]", pQuote->ClientID);
        PURE_LOG("\tExchangeInstID [%s]", pQuote->ExchangeInstID);
        PURE_LOG("\tTraderID [%s]", pQuote->TraderID);
        PURE_LOG("\tTradingDay [%s]", pQuote->TradingDay);
        PURE_LOG("\tQuoteSysID [%s]", pQuote->QuoteSysID);
        PURE_LOG("\tInsertDate [%s]", pQuote->InsertDate);
        PURE_LOG("\tInsertTime [%s]", pQuote->InsertTime);
        PURE_LOG("\tCancelTime [%s]", pQuote->CancelTime);
        PURE_LOG("\tClearingPartID [%s]", pQuote->ClearingPartID);
        PURE_LOG("\tAskOrderSysID [%s]", pQuote->AskOrderSysID);
        PURE_LOG("\tBidOrderSysID [%s]", pQuote->BidOrderSysID);
        PURE_LOG("\tUserProductInfo [%s]", pQuote->UserProductInfo);
        PURE_LOG("\tStatusMsg [%s]", pQuote->StatusMsg);
        PURE_LOG("\tActiveUserID [%s]", pQuote->ActiveUserID);
        PURE_LOG("\tAskOrderRef [%s]", pQuote->AskOrderRef);
        PURE_LOG("\tBidOrderRef [%s]", pQuote->BidOrderRef);
        PURE_LOG("\tForQuoteSysID [%s]", pQuote->ForQuoteSysID);
        PURE_LOG("\tBranchID [%s]", pQuote->BranchID);
        PURE_LOG("\tInvestUnitID [%s]", pQuote->InvestUnitID);
        PURE_LOG("\tAccountID [%s]", pQuote->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pQuote->CurrencyID);
        PURE_LOG("\tIPAddress [%s]", pQuote->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pQuote->MacAddress);
        PURE_LOG("\tAskVolume [%d]", pQuote->AskVolume);
        PURE_LOG("\tBidVolume [%d]", pQuote->BidVolume);
        PURE_LOG("\tRequestID [%d]", pQuote->RequestID);
        PURE_LOG("\tInstallID [%d]", pQuote->InstallID);
        PURE_LOG("\tNotifySequence [%d]", pQuote->NotifySequence);
        PURE_LOG("\tSettlementID [%d]", pQuote->SettlementID);
        PURE_LOG("\tSequenceNo [%d]", pQuote->SequenceNo);
        PURE_LOG("\tFrontID [%d]", pQuote->FrontID);
        PURE_LOG("\tSessionID [%d]", pQuote->SessionID);
        PURE_LOG("\tBrokerQuoteSeq [%d]", pQuote->BrokerQuoteSeq);
        PURE_LOG("\tAskOffsetFlag [%c]", pQuote->AskOffsetFlag);
        PURE_LOG("\tBidOffsetFlag [%c]", pQuote->BidOffsetFlag);
        PURE_LOG("\tAskHedgeFlag [%c]", pQuote->AskHedgeFlag);
        PURE_LOG("\tBidHedgeFlag [%c]", pQuote->BidHedgeFlag);
        PURE_LOG("\tOrderSubmitStatus [%c]", pQuote->OrderSubmitStatus);
        PURE_LOG("\tQuoteStatus [%c]", pQuote->QuoteStatus);
        PURE_LOG("\tAskPrice [%.8lf]", pQuote->AskPrice);
        PURE_LOG("\tBidPrice [%.8lf]", pQuote->BidPrice);
	}
    PURE_LOG("</OnRtnQuote>");
};
void CTraderSpi::OnErrRtnQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo)
{
    PURE_LOG("<OnErrRtnQuoteInsert>");
	if (pInputQuote)
	{
        PURE_LOG("\tBrokerID [%s]", pInputQuote->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInputQuote->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pInputQuote->InstrumentID);
        PURE_LOG("\tQuoteRef [%s]", pInputQuote->QuoteRef);
        PURE_LOG("\tUserID [%s]", pInputQuote->UserID);
        PURE_LOG("\tBusinessUnit [%s]", pInputQuote->BusinessUnit);
        PURE_LOG("\tAskOrderRef [%s]", pInputQuote->AskOrderRef);
        PURE_LOG("\tBidOrderRef [%s]", pInputQuote->BidOrderRef);
        PURE_LOG("\tForQuoteSysID [%s]", pInputQuote->ForQuoteSysID);
        PURE_LOG("\tExchangeID [%s]", pInputQuote->ExchangeID);
        PURE_LOG("\tInvestUnitID [%s]", pInputQuote->InvestUnitID);
        PURE_LOG("\tClientID [%s]", pInputQuote->ClientID);
        PURE_LOG("\tIPAddress [%s]", pInputQuote->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pInputQuote->MacAddress);
        PURE_LOG("\tAskVolume [%d]", pInputQuote->AskVolume);
        PURE_LOG("\tBidVolume [%d]", pInputQuote->BidVolume);
        PURE_LOG("\tRequestID [%d]", pInputQuote->RequestID);
        PURE_LOG("\tAskOffsetFlag [%c]", pInputQuote->AskOffsetFlag);
        PURE_LOG("\tBidOffsetFlag [%c]", pInputQuote->BidOffsetFlag);
        PURE_LOG("\tAskHedgeFlag [%c]", pInputQuote->AskHedgeFlag);
        PURE_LOG("\tBidHedgeFlag [%c]", pInputQuote->BidHedgeFlag);
        PURE_LOG("\tAskPrice [%.8lf]", pInputQuote->AskPrice);
        PURE_LOG("\tBidPrice [%.8lf]", pInputQuote->BidPrice);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("</OnErrRtnQuoteInsert>");
};
void CTraderSpi::OnErrRtnQuoteAction(CThostFtdcQuoteActionField *pQuoteAction, CThostFtdcRspInfoField *pRspInfo)
{
    PURE_LOG("<OnErrRtnQuoteAction>");
	if (pQuoteAction)
	{
        PURE_LOG("\tBrokerID [%s]", pQuoteAction->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pQuoteAction->InvestorID);
        PURE_LOG("\tQuoteRef [%s]", pQuoteAction->QuoteRef);
        PURE_LOG("\tExchangeID [%s]", pQuoteAction->ExchangeID);
        PURE_LOG("\tQuoteSysID [%s]", pQuoteAction->QuoteSysID);
        PURE_LOG("\tActionDate [%s]", pQuoteAction->ActionDate);
        PURE_LOG("\tActionTime [%s]", pQuoteAction->ActionTime);
        PURE_LOG("\tTraderID [%s]", pQuoteAction->TraderID);
        PURE_LOG("\tQuoteLocalID [%s]", pQuoteAction->QuoteLocalID);
        PURE_LOG("\tActionLocalID [%s]", pQuoteAction->ActionLocalID);
        PURE_LOG("\tParticipantID [%s]", pQuoteAction->ParticipantID);
        PURE_LOG("\tClientID [%s]", pQuoteAction->ClientID);
        PURE_LOG("\tBusinessUnit [%s]", pQuoteAction->BusinessUnit);
        PURE_LOG("\tUserID [%s]", pQuoteAction->UserID);
        PURE_LOG("\tStatusMsg [%s]", pQuoteAction->StatusMsg);
        PURE_LOG("\tInstrumentID [%s]", pQuoteAction->InstrumentID);
        PURE_LOG("\tBranchID [%s]", pQuoteAction->BranchID);
        PURE_LOG("\tInvestUnitID [%s]", pQuoteAction->InvestUnitID);
        PURE_LOG("\tIPAddress [%s]", pQuoteAction->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pQuoteAction->MacAddress);
        PURE_LOG("\tQuoteActionRef [%d]", pQuoteAction->QuoteActionRef);
        PURE_LOG("\tRequestID [%d]", pQuoteAction->RequestID);
        PURE_LOG("\tFrontID [%d]", pQuoteAction->FrontID);
        PURE_LOG("\tSessionID [%d]", pQuoteAction->SessionID);
        PURE_LOG("\tInstallID [%d]", pQuoteAction->InstallID);
        PURE_LOG("\tActionFlag [%c]", pQuoteAction->ActionFlag);
        PURE_LOG("\tOrderActionStatus [%c]", pQuoteAction->OrderActionStatus);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("</OnErrRtnQuoteAction>");
};
void CTraderSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
    PURE_LOG("<OnRtnForQuoteRsp>");
	if (pForQuoteRsp)
	{
        PURE_LOG("\tTradingDay [%s]", pForQuoteRsp->TradingDay);
        PURE_LOG("\tInstrumentID [%s]", pForQuoteRsp->InstrumentID);
        PURE_LOG("\tForQuoteSysID [%s]", pForQuoteRsp->ForQuoteSysID);
        PURE_LOG("\tForQuoteTime [%s]", pForQuoteRsp->ForQuoteTime);
        PURE_LOG("\tActionDay [%s]", pForQuoteRsp->ActionDay);
        PURE_LOG("\tExchangeID [%s]", pForQuoteRsp->ExchangeID);
	}
    PURE_LOG("</OnRtnForQuoteRsp>");
};
void CTraderSpi::OnRtnCFMMCTradingAccountToken(CThostFtdcCFMMCTradingAccountTokenField *pCFMMCTradingAccountToken)
{
    PURE_LOG("<OnRtnCFMMCTradingAccountToken>");
	if (pCFMMCTradingAccountToken)
	{
        PURE_LOG("\tBrokerID [%s]", pCFMMCTradingAccountToken->BrokerID);
        PURE_LOG("\tParticipantID [%s]", pCFMMCTradingAccountToken->ParticipantID);
        PURE_LOG("\tAccountID [%s]", pCFMMCTradingAccountToken->AccountID);
        PURE_LOG("\tToken [%s]", pCFMMCTradingAccountToken->Token);
        PURE_LOG("\tKeyID [%d]", pCFMMCTradingAccountToken->KeyID);
	}
    PURE_LOG("</OnRtnCFMMCTradingAccountToken>");
};
void CTraderSpi::OnErrRtnBatchOrderAction(CThostFtdcBatchOrderActionField *pBatchOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
    PURE_LOG("<OnErrRtnBatchOrderAction>");
	if (pBatchOrderAction)
	{
        PURE_LOG("\tBrokerID [%s]", pBatchOrderAction->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pBatchOrderAction->InvestorID);
        PURE_LOG("\tExchangeID [%s]", pBatchOrderAction->ExchangeID);
        PURE_LOG("\tActionDate [%s]", pBatchOrderAction->ActionDate);
        PURE_LOG("\tActionTime [%s]", pBatchOrderAction->ActionTime);
        PURE_LOG("\tTraderID [%s]", pBatchOrderAction->TraderID);
        PURE_LOG("\tActionLocalID [%s]", pBatchOrderAction->ActionLocalID);
        PURE_LOG("\tParticipantID [%s]", pBatchOrderAction->ParticipantID);
        PURE_LOG("\tClientID [%s]", pBatchOrderAction->ClientID);
        PURE_LOG("\tBusinessUnit [%s]", pBatchOrderAction->BusinessUnit);
        PURE_LOG("\tUserID [%s]", pBatchOrderAction->UserID);
        PURE_LOG("\tStatusMsg [%s]", pBatchOrderAction->StatusMsg);
        PURE_LOG("\tInvestUnitID [%s]", pBatchOrderAction->InvestUnitID);
        PURE_LOG("\tIPAddress [%s]", pBatchOrderAction->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pBatchOrderAction->MacAddress);
        PURE_LOG("\tOrderActionRef [%d]", pBatchOrderAction->OrderActionRef);
        PURE_LOG("\tRequestID [%d]", pBatchOrderAction->RequestID);
        PURE_LOG("\tFrontID [%d]", pBatchOrderAction->FrontID);
        PURE_LOG("\tSessionID [%d]", pBatchOrderAction->SessionID);
        PURE_LOG("\tInstallID [%d]", pBatchOrderAction->InstallID);
        PURE_LOG("\tOrderActionStatus [%c]", pBatchOrderAction->OrderActionStatus);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("</OnErrRtnBatchOrderAction>");
};
void CTraderSpi::OnRtnOptionSelfClose(CThostFtdcOptionSelfCloseField *pOptionSelfClose)
{
    PURE_LOG("<OnRtnOptionSelfClose>");
	if (pOptionSelfClose)
	{
        PURE_LOG("\tBrokerID [%s]", pOptionSelfClose->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pOptionSelfClose->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pOptionSelfClose->InstrumentID);
        PURE_LOG("\tOptionSelfCloseRef [%s]", pOptionSelfClose->OptionSelfCloseRef);
        PURE_LOG("\tUserID [%s]", pOptionSelfClose->UserID);
        PURE_LOG("\tBusinessUnit [%s]", pOptionSelfClose->BusinessUnit);
        PURE_LOG("\tOptionSelfCloseLocalID [%s]", pOptionSelfClose->OptionSelfCloseLocalID);
        PURE_LOG("\tExchangeID [%s]", pOptionSelfClose->ExchangeID);
        PURE_LOG("\tParticipantID [%s]", pOptionSelfClose->ParticipantID);
        PURE_LOG("\tClientID [%s]", pOptionSelfClose->ClientID);
        PURE_LOG("\tExchangeInstID [%s]", pOptionSelfClose->ExchangeInstID);
        PURE_LOG("\tTraderID [%s]", pOptionSelfClose->TraderID);
        PURE_LOG("\tTradingDay [%s]", pOptionSelfClose->TradingDay);
        PURE_LOG("\tOptionSelfCloseSysID [%s]", pOptionSelfClose->OptionSelfCloseSysID);
        PURE_LOG("\tInsertDate [%s]", pOptionSelfClose->InsertDate);
        PURE_LOG("\tInsertTime [%s]", pOptionSelfClose->InsertTime);
        PURE_LOG("\tCancelTime [%s]", pOptionSelfClose->CancelTime);
        PURE_LOG("\tClearingPartID [%s]", pOptionSelfClose->ClearingPartID);
        PURE_LOG("\tUserProductInfo [%s]", pOptionSelfClose->UserProductInfo);
        PURE_LOG("\tStatusMsg [%s]", pOptionSelfClose->StatusMsg);
        PURE_LOG("\tActiveUserID [%s]", pOptionSelfClose->ActiveUserID);
        PURE_LOG("\tBranchID [%s]", pOptionSelfClose->BranchID);
        PURE_LOG("\tInvestUnitID [%s]", pOptionSelfClose->InvestUnitID);
        PURE_LOG("\tAccountID [%s]", pOptionSelfClose->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pOptionSelfClose->CurrencyID);
        PURE_LOG("\tIPAddress [%s]", pOptionSelfClose->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pOptionSelfClose->MacAddress);
        PURE_LOG("\tVolume [%d]", pOptionSelfClose->Volume);
        PURE_LOG("\tRequestID [%d]", pOptionSelfClose->RequestID);
        PURE_LOG("\tInstallID [%d]", pOptionSelfClose->InstallID);
        PURE_LOG("\tNotifySequence [%d]", pOptionSelfClose->NotifySequence);
        PURE_LOG("\tSettlementID [%d]", pOptionSelfClose->SettlementID);
        PURE_LOG("\tSequenceNo [%d]", pOptionSelfClose->SequenceNo);
        PURE_LOG("\tFrontID [%d]", pOptionSelfClose->FrontID);
        PURE_LOG("\tSessionID [%d]", pOptionSelfClose->SessionID);
        PURE_LOG("\tBrokerOptionSelfCloseSeq [%d]", pOptionSelfClose->BrokerOptionSelfCloseSeq);
        PURE_LOG("\tHedgeFlag [%c]", pOptionSelfClose->HedgeFlag);
        PURE_LOG("\tOptSelfCloseFlag [%c]", pOptionSelfClose->OptSelfCloseFlag);
        PURE_LOG("\tOrderSubmitStatus [%c]", pOptionSelfClose->OrderSubmitStatus);
        PURE_LOG("\tExecResult [%c]", pOptionSelfClose->ExecResult);
	}
    PURE_LOG("</OnRtnOptionSelfClose>");
};
void CTraderSpi::OnErrRtnOptionSelfCloseInsert(CThostFtdcInputOptionSelfCloseField *pInputOptionSelfClose, CThostFtdcRspInfoField *pRspInfo)
{
    PURE_LOG("<OnErrRtnOptionSelfCloseInsert>");
	if (pInputOptionSelfClose)
	{
        PURE_LOG("\tBrokerID [%s]", pInputOptionSelfClose->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInputOptionSelfClose->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pInputOptionSelfClose->InstrumentID);
        PURE_LOG("\tOptionSelfCloseRef [%s]", pInputOptionSelfClose->OptionSelfCloseRef);
        PURE_LOG("\tUserID [%s]", pInputOptionSelfClose->UserID);
        PURE_LOG("\tBusinessUnit [%s]", pInputOptionSelfClose->BusinessUnit);
        PURE_LOG("\tExchangeID [%s]", pInputOptionSelfClose->ExchangeID);
        PURE_LOG("\tInvestUnitID [%s]", pInputOptionSelfClose->InvestUnitID);
        PURE_LOG("\tAccountID [%s]", pInputOptionSelfClose->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pInputOptionSelfClose->CurrencyID);
        PURE_LOG("\tClientID [%s]", pInputOptionSelfClose->ClientID);
        PURE_LOG("\tIPAddress [%s]", pInputOptionSelfClose->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pInputOptionSelfClose->MacAddress);
        PURE_LOG("\tVolume [%d]", pInputOptionSelfClose->Volume);
        PURE_LOG("\tRequestID [%d]", pInputOptionSelfClose->RequestID);
        PURE_LOG("\tHedgeFlag [%c]", pInputOptionSelfClose->HedgeFlag);
        PURE_LOG("\tOptSelfCloseFlag [%c]", pInputOptionSelfClose->OptSelfCloseFlag);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("</OnErrRtnOptionSelfCloseInsert>");
};
void CTraderSpi::OnErrRtnOptionSelfCloseAction(CThostFtdcOptionSelfCloseActionField *pOptionSelfCloseAction, CThostFtdcRspInfoField *pRspInfo)
{
    PURE_LOG("<OnErrRtnOptionSelfCloseAction>");
	if (pOptionSelfCloseAction)
	{
        PURE_LOG("\tBrokerID [%s]", pOptionSelfCloseAction->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pOptionSelfCloseAction->InvestorID);
        PURE_LOG("\tOptionSelfCloseRef [%s]", pOptionSelfCloseAction->OptionSelfCloseRef);
        PURE_LOG("\tExchangeID [%s]", pOptionSelfCloseAction->ExchangeID);
        PURE_LOG("\tOptionSelfCloseSysID [%s]", pOptionSelfCloseAction->OptionSelfCloseSysID);
        PURE_LOG("\tActionDate [%s]", pOptionSelfCloseAction->ActionDate);
        PURE_LOG("\tActionTime [%s]", pOptionSelfCloseAction->ActionTime);
        PURE_LOG("\tTraderID [%s]", pOptionSelfCloseAction->TraderID);
        PURE_LOG("\tOptionSelfCloseLocalID [%s]", pOptionSelfCloseAction->OptionSelfCloseLocalID);
        PURE_LOG("\tActionLocalID [%s]", pOptionSelfCloseAction->ActionLocalID);
        PURE_LOG("\tParticipantID [%s]", pOptionSelfCloseAction->ParticipantID);
        PURE_LOG("\tClientID [%s]", pOptionSelfCloseAction->ClientID);
        PURE_LOG("\tBusinessUnit [%s]", pOptionSelfCloseAction->BusinessUnit);
        PURE_LOG("\tUserID [%s]", pOptionSelfCloseAction->UserID);
        PURE_LOG("\tStatusMsg [%s]", pOptionSelfCloseAction->StatusMsg);
        PURE_LOG("\tInstrumentID [%s]", pOptionSelfCloseAction->InstrumentID);
        PURE_LOG("\tBranchID [%s]", pOptionSelfCloseAction->BranchID);
        PURE_LOG("\tInvestUnitID [%s]", pOptionSelfCloseAction->InvestUnitID);
        PURE_LOG("\tIPAddress [%s]", pOptionSelfCloseAction->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pOptionSelfCloseAction->MacAddress);
        PURE_LOG("\tOptionSelfCloseActionRef [%d]", pOptionSelfCloseAction->OptionSelfCloseActionRef);
        PURE_LOG("\tRequestID [%d]", pOptionSelfCloseAction->RequestID);
        PURE_LOG("\tFrontID [%d]", pOptionSelfCloseAction->FrontID);
        PURE_LOG("\tSessionID [%d]", pOptionSelfCloseAction->SessionID);
        PURE_LOG("\tInstallID [%d]", pOptionSelfCloseAction->InstallID);
        PURE_LOG("\tActionFlag [%c]", pOptionSelfCloseAction->ActionFlag);
        PURE_LOG("\tOrderActionStatus [%c]", pOptionSelfCloseAction->OrderActionStatus);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("</OnErrRtnOptionSelfCloseAction>");
};
void CTraderSpi::OnRtnCombAction(CThostFtdcCombActionField *pCombAction)
{
    PURE_LOG("<OnRtnCombAction>");
	if (pCombAction)
	{
        PURE_LOG("\tBrokerID [%s]", pCombAction->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pCombAction->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pCombAction->InstrumentID);
        PURE_LOG("\tCombActionRef [%s]", pCombAction->CombActionRef);
        PURE_LOG("\tUserID [%s]", pCombAction->UserID);
        PURE_LOG("\tActionLocalID [%s]", pCombAction->ActionLocalID);
        PURE_LOG("\tExchangeID [%s]", pCombAction->ExchangeID);
        PURE_LOG("\tParticipantID [%s]", pCombAction->ParticipantID);
        PURE_LOG("\tClientID [%s]", pCombAction->ClientID);
        PURE_LOG("\tExchangeInstID [%s]", pCombAction->ExchangeInstID);
        PURE_LOG("\tTraderID [%s]", pCombAction->TraderID);
        PURE_LOG("\tTradingDay [%s]", pCombAction->TradingDay);
        PURE_LOG("\tUserProductInfo [%s]", pCombAction->UserProductInfo);
        PURE_LOG("\tStatusMsg [%s]", pCombAction->StatusMsg);
        PURE_LOG("\tIPAddress [%s]", pCombAction->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pCombAction->MacAddress);
        PURE_LOG("\tComTradeID [%s]", pCombAction->ComTradeID);
        PURE_LOG("\tBranchID [%s]", pCombAction->BranchID);
        PURE_LOG("\tInvestUnitID [%s]", pCombAction->InvestUnitID);
        PURE_LOG("\tVolume [%d]", pCombAction->Volume);
        PURE_LOG("\tInstallID [%d]", pCombAction->InstallID);
        PURE_LOG("\tNotifySequence [%d]", pCombAction->NotifySequence);
        PURE_LOG("\tSettlementID [%d]", pCombAction->SettlementID);
        PURE_LOG("\tSequenceNo [%d]", pCombAction->SequenceNo);
        PURE_LOG("\tFrontID [%d]", pCombAction->FrontID);
        PURE_LOG("\tSessionID [%d]", pCombAction->SessionID);
        PURE_LOG("\tDirection [%c]", pCombAction->Direction);
        PURE_LOG("\tCombDirection [%c]", pCombAction->CombDirection);
        PURE_LOG("\tHedgeFlag [%c]", pCombAction->HedgeFlag);
        PURE_LOG("\tActionStatus [%c]", pCombAction->ActionStatus);
	}
    PURE_LOG("</OnRtnCombAction>");
};
void CTraderSpi::OnErrRtnCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo)
{
    PURE_LOG("<OnErrRtnCombActionInsert>");
	if (pInputCombAction)
	{
        PURE_LOG("\tBrokerID [%s]", pInputCombAction->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pInputCombAction->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pInputCombAction->InstrumentID);
        PURE_LOG("\tCombActionRef [%s]", pInputCombAction->CombActionRef);
        PURE_LOG("\tUserID [%s]", pInputCombAction->UserID);
        PURE_LOG("\tExchangeID [%s]", pInputCombAction->ExchangeID);
        PURE_LOG("\tIPAddress [%s]", pInputCombAction->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pInputCombAction->MacAddress);
        PURE_LOG("\tInvestUnitID [%s]", pInputCombAction->InvestUnitID);
        PURE_LOG("\tVolume [%d]", pInputCombAction->Volume);
        PURE_LOG("\tDirection [%c]", pInputCombAction->Direction);
        PURE_LOG("\tCombDirection [%c]", pInputCombAction->CombDirection);
        PURE_LOG("\tHedgeFlag [%c]", pInputCombAction->HedgeFlag);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("</OnErrRtnCombActionInsert>");
};
void CTraderSpi::OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryContractBank>");
	if (pContractBank)
	{
        PURE_LOG("\tBrokerID [%s]", pContractBank->BrokerID);
        PURE_LOG("\tBankID [%s]", pContractBank->BankID);
        PURE_LOG("\tBankBrchID [%s]", pContractBank->BankBrchID);
        PURE_LOG("\tBankName [%s]", pContractBank->BankName);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryContractBank>");
};

void CTraderSpi::OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryParkedOrder>");
	if (pParkedOrder)
	{
        PURE_LOG("\tBrokerID [%s]", pParkedOrder->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pParkedOrder->InvestorID);
        PURE_LOG("\tInstrumentID [%s]", pParkedOrder->InstrumentID);
        PURE_LOG("\tOrderRef [%s]", pParkedOrder->OrderRef);
        PURE_LOG("\tUserID [%s]", pParkedOrder->UserID);
        PURE_LOG("\tCombOffsetFlag [%s]", pParkedOrder->CombOffsetFlag);
        PURE_LOG("\tCombHedgeFlag [%s]", pParkedOrder->CombHedgeFlag);
        PURE_LOG("\tGTDDate [%s]", pParkedOrder->GTDDate);
        PURE_LOG("\tBusinessUnit [%s]", pParkedOrder->BusinessUnit);
        PURE_LOG("\tExchangeID [%s]", pParkedOrder->ExchangeID);
        PURE_LOG("\tParkedOrderID [%s]", pParkedOrder->ParkedOrderID);
        PURE_LOG("\tErrorMsg [%s]", pParkedOrder->ErrorMsg);
        PURE_LOG("\tAccountID [%s]", pParkedOrder->AccountID);
        PURE_LOG("\tCurrencyID [%s]", pParkedOrder->CurrencyID);
        PURE_LOG("\tClientID [%s]", pParkedOrder->ClientID);
        PURE_LOG("\tInvestUnitID [%s]", pParkedOrder->InvestUnitID);
        PURE_LOG("\tIPAddress [%s]", pParkedOrder->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pParkedOrder->MacAddress);
        PURE_LOG("\tVolumeTotalOriginal [%d]", pParkedOrder->VolumeTotalOriginal);
        PURE_LOG("\tMinVolume [%d]", pParkedOrder->MinVolume);
        PURE_LOG("\tIsAutoSuspend [%d]", pParkedOrder->IsAutoSuspend);
        PURE_LOG("\tRequestID [%d]", pParkedOrder->RequestID);
        PURE_LOG("\tUserForceClose [%d]", pParkedOrder->UserForceClose);
        PURE_LOG("\tErrorID [%d]", pParkedOrder->ErrorID);
        PURE_LOG("\tIsSwapOrder [%d]", pParkedOrder->IsSwapOrder);
        PURE_LOG("\tOrderPriceType [%c]", pParkedOrder->OrderPriceType);
        PURE_LOG("\tDirection [%c]", pParkedOrder->Direction);
        PURE_LOG("\tTimeCondition [%c]", pParkedOrder->TimeCondition);
        PURE_LOG("\tVolumeCondition [%c]", pParkedOrder->VolumeCondition);
        PURE_LOG("\tContingentCondition [%c]", pParkedOrder->ContingentCondition);
        PURE_LOG("\tForceCloseReason [%c]", pParkedOrder->ForceCloseReason);
        PURE_LOG("\tUserType [%c]", pParkedOrder->UserType);
        PURE_LOG("\tStatus [%c]", pParkedOrder->Status);
        PURE_LOG("\tLimitPrice [%.8lf]", pParkedOrder->LimitPrice);
        PURE_LOG("\tStopPrice [%.8lf]", pParkedOrder->StopPrice);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryParkedOrder>");
};

void CTraderSpi::OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryParkedOrderAction>");
	if (pParkedOrderAction)
	{
        PURE_LOG("\tBrokerID [%s]", pParkedOrderAction->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pParkedOrderAction->InvestorID);
        PURE_LOG("\tOrderRef [%s]", pParkedOrderAction->OrderRef);
        PURE_LOG("\tExchangeID [%s]", pParkedOrderAction->ExchangeID);
        PURE_LOG("\tOrderSysID [%s]", pParkedOrderAction->OrderSysID);
        PURE_LOG("\tUserID [%s]", pParkedOrderAction->UserID);
        PURE_LOG("\tInstrumentID [%s]", pParkedOrderAction->InstrumentID);
        PURE_LOG("\tParkedOrderActionID [%s]", pParkedOrderAction->ParkedOrderActionID);
        PURE_LOG("\tErrorMsg [%s]", pParkedOrderAction->ErrorMsg);
        PURE_LOG("\tInvestUnitID [%s]", pParkedOrderAction->InvestUnitID);
        PURE_LOG("\tIPAddress [%s]", pParkedOrderAction->IPAddress);
        PURE_LOG("\tMacAddress [%s]", pParkedOrderAction->MacAddress);
        PURE_LOG("\tOrderActionRef [%d]", pParkedOrderAction->OrderActionRef);
        PURE_LOG("\tRequestID [%d]", pParkedOrderAction->RequestID);
        PURE_LOG("\tFrontID [%d]", pParkedOrderAction->FrontID);
        PURE_LOG("\tSessionID [%d]", pParkedOrderAction->SessionID);
        PURE_LOG("\tVolumeChange [%d]", pParkedOrderAction->VolumeChange);
        PURE_LOG("\tErrorID [%d]", pParkedOrderAction->ErrorID);
        PURE_LOG("\tActionFlag [%c]", pParkedOrderAction->ActionFlag);
        PURE_LOG("\tUserType [%c]", pParkedOrderAction->UserType);
        PURE_LOG("\tStatus [%c]", pParkedOrderAction->Status);
        PURE_LOG("\tLimitPrice [%.8lf]", pParkedOrderAction->LimitPrice);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryParkedOrderAction>");
};

void CTraderSpi::OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryTradingNotice>");
	if (pTradingNotice)
	{
        PURE_LOG("\tBrokerID [%s]", pTradingNotice->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pTradingNotice->InvestorID);
        PURE_LOG("\tUserID [%s]", pTradingNotice->UserID);
        PURE_LOG("\tSendTime [%s]", pTradingNotice->SendTime);
        PURE_LOG("\tFieldContent [%s]", pTradingNotice->FieldContent);
        PURE_LOG("\tInvestUnitID [%s]", pTradingNotice->InvestUnitID);
        PURE_LOG("\tSequenceNo [%d]", pTradingNotice->SequenceNo);
        PURE_LOG("\tInvestorRange [%c]", pTradingNotice->InvestorRange);
        PURE_LOG("\tSequenceSeries [%d]", pTradingNotice->SequenceSeries);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryTradingNotice>");
};

void CTraderSpi::OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryBrokerTradingParams>");
	if (pBrokerTradingParams)
	{
        PURE_LOG("\tBrokerID [%s]", pBrokerTradingParams->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pBrokerTradingParams->InvestorID);
        PURE_LOG("\tCurrencyID [%s]", pBrokerTradingParams->CurrencyID);
        PURE_LOG("\tAccountID [%s]", pBrokerTradingParams->AccountID);
        PURE_LOG("\tMarginPriceType [%c]", pBrokerTradingParams->MarginPriceType);
        PURE_LOG("\tAlgorithm [%c]", pBrokerTradingParams->Algorithm);
        PURE_LOG("\tAvailIncludeCloseProfit [%c]", pBrokerTradingParams->AvailIncludeCloseProfit);
        PURE_LOG("\tOptionRoyaltyPriceType [%c]", pBrokerTradingParams->OptionRoyaltyPriceType);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryBrokerTradingParams>");
};

void CTraderSpi::OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQryBrokerTradingAlgos>");
	if (pBrokerTradingAlgos)
	{
        PURE_LOG("\tBrokerID [%s]", pBrokerTradingAlgos->BrokerID);
        PURE_LOG("\tExchangeID [%s]", pBrokerTradingAlgos->ExchangeID);
        PURE_LOG("\tInstrumentID [%s]", pBrokerTradingAlgos->InstrumentID);
        PURE_LOG("\tHandlePositionAlgoID [%c]", pBrokerTradingAlgos->HandlePositionAlgoID);
        PURE_LOG("\tFindMarginRateAlgoID [%c]", pBrokerTradingAlgos->FindMarginRateAlgoID);
        PURE_LOG("\tHandleTradingAccountAlgoID [%c]", pBrokerTradingAlgos->HandleTradingAccountAlgoID);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQryBrokerTradingAlgos>");
};

void CTraderSpi::OnRspQueryCFMMCTradingAccountToken(CThostFtdcQueryCFMMCTradingAccountTokenField *pQueryCFMMCTradingAccountToken, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQueryCFMMCTradingAccountToken>");
	if (pQueryCFMMCTradingAccountToken)
	{
        PURE_LOG("\tBrokerID [%s]", pQueryCFMMCTradingAccountToken->BrokerID);
        PURE_LOG("\tInvestorID [%s]", pQueryCFMMCTradingAccountToken->InvestorID);
        PURE_LOG("\tInvestUnitID [%s]", pQueryCFMMCTradingAccountToken->InvestUnitID);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQueryCFMMCTradingAccountToken>");
};

void CTraderSpi::OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer)
{
    PURE_LOG("<OnRtnFromBankToFutureByBank>");
	if (pRspTransfer)
	{
        PURE_LOG("\tTradeCode [%s]", pRspTransfer->TradeCode);
        PURE_LOG("\tBankID [%s]", pRspTransfer->BankID);
        PURE_LOG("\tBankBranchID [%s]", pRspTransfer->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pRspTransfer->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pRspTransfer->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pRspTransfer->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pRspTransfer->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pRspTransfer->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pRspTransfer->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pRspTransfer->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pRspTransfer->IdentifiedCardNo);
        PURE_LOG("\tBankAccount [%s]", pRspTransfer->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pRspTransfer->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pRspTransfer->AccountID);
        PURE_LOG("\tPassword [%s]", pRspTransfer->Password);
        PURE_LOG("\tUserID [%s]", pRspTransfer->UserID);
        PURE_LOG("\tCurrencyID [%s]", pRspTransfer->CurrencyID);
        PURE_LOG("\tMessage [%s]", pRspTransfer->Message);
        PURE_LOG("\tDigest [%s]", pRspTransfer->Digest);
        PURE_LOG("\tDeviceID [%s]", pRspTransfer->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pRspTransfer->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pRspTransfer->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pRspTransfer->OperNo);
        PURE_LOG("\tErrorMsg [%s]", pRspTransfer->ErrorMsg);
        PURE_LOG("\tLongCustomerName [%s]", pRspTransfer->LongCustomerName);
        PURE_LOG("\tPlateSerial [%d]", pRspTransfer->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pRspTransfer->SessionID);
        PURE_LOG("\tInstallID [%d]", pRspTransfer->InstallID);
        PURE_LOG("\tFutureSerial [%d]", pRspTransfer->FutureSerial);
        PURE_LOG("\tRequestID [%d]", pRspTransfer->RequestID);
        PURE_LOG("\tTID [%d]", pRspTransfer->TID);
        PURE_LOG("\tErrorID [%d]", pRspTransfer->ErrorID);
        PURE_LOG("\tLastFragment [%c]", pRspTransfer->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pRspTransfer->IdCardType);
        PURE_LOG("\tCustType [%c]", pRspTransfer->CustType);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pRspTransfer->VerifyCertNoFlag);
        PURE_LOG("\tFeePayFlag [%c]", pRspTransfer->FeePayFlag);
        PURE_LOG("\tBankAccType [%c]", pRspTransfer->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pRspTransfer->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pRspTransfer->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pRspTransfer->SecuPwdFlag);
        PURE_LOG("\tTransferStatus [%c]", pRspTransfer->TransferStatus);
        PURE_LOG("\tTradeAmount [%.8lf]", pRspTransfer->TradeAmount);
        PURE_LOG("\tFutureFetchAmount [%.8lf]", pRspTransfer->FutureFetchAmount);
        PURE_LOG("\tCustFee [%.8lf]", pRspTransfer->CustFee);
        PURE_LOG("\tBrokerFee [%.8lf]", pRspTransfer->BrokerFee);
	}
    PURE_LOG("</OnRtnFromBankToFutureByBank>");
};
void CTraderSpi::OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer)
{
    PURE_LOG("<OnRtnFromFutureToBankByBank>");
	if (pRspTransfer)
	{
        PURE_LOG("\tTradeCode [%s]", pRspTransfer->TradeCode);
        PURE_LOG("\tBankID [%s]", pRspTransfer->BankID);
        PURE_LOG("\tBankBranchID [%s]", pRspTransfer->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pRspTransfer->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pRspTransfer->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pRspTransfer->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pRspTransfer->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pRspTransfer->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pRspTransfer->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pRspTransfer->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pRspTransfer->IdentifiedCardNo);
        PURE_LOG("\tBankAccount [%s]", pRspTransfer->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pRspTransfer->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pRspTransfer->AccountID);
        PURE_LOG("\tPassword [%s]", pRspTransfer->Password);
        PURE_LOG("\tUserID [%s]", pRspTransfer->UserID);
        PURE_LOG("\tCurrencyID [%s]", pRspTransfer->CurrencyID);
        PURE_LOG("\tMessage [%s]", pRspTransfer->Message);
        PURE_LOG("\tDigest [%s]", pRspTransfer->Digest);
        PURE_LOG("\tDeviceID [%s]", pRspTransfer->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pRspTransfer->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pRspTransfer->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pRspTransfer->OperNo);
        PURE_LOG("\tErrorMsg [%s]", pRspTransfer->ErrorMsg);
        PURE_LOG("\tLongCustomerName [%s]", pRspTransfer->LongCustomerName);
        PURE_LOG("\tPlateSerial [%d]", pRspTransfer->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pRspTransfer->SessionID);
        PURE_LOG("\tInstallID [%d]", pRspTransfer->InstallID);
        PURE_LOG("\tFutureSerial [%d]", pRspTransfer->FutureSerial);
        PURE_LOG("\tRequestID [%d]", pRspTransfer->RequestID);
        PURE_LOG("\tTID [%d]", pRspTransfer->TID);
        PURE_LOG("\tErrorID [%d]", pRspTransfer->ErrorID);
        PURE_LOG("\tLastFragment [%c]", pRspTransfer->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pRspTransfer->IdCardType);
        PURE_LOG("\tCustType [%c]", pRspTransfer->CustType);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pRspTransfer->VerifyCertNoFlag);
        PURE_LOG("\tFeePayFlag [%c]", pRspTransfer->FeePayFlag);
        PURE_LOG("\tBankAccType [%c]", pRspTransfer->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pRspTransfer->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pRspTransfer->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pRspTransfer->SecuPwdFlag);
        PURE_LOG("\tTransferStatus [%c]", pRspTransfer->TransferStatus);
        PURE_LOG("\tTradeAmount [%.8lf]", pRspTransfer->TradeAmount);
        PURE_LOG("\tFutureFetchAmount [%.8lf]", pRspTransfer->FutureFetchAmount);
        PURE_LOG("\tCustFee [%.8lf]", pRspTransfer->CustFee);
        PURE_LOG("\tBrokerFee [%.8lf]", pRspTransfer->BrokerFee);
	}
    PURE_LOG("</OnRtnFromFutureToBankByBank>");
};
void CTraderSpi::OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal)
{
    PURE_LOG("<OnRtnRepealFromBankToFutureByBank>");
	if (pRspRepeal)
	{
        PURE_LOG("\tBankRepealSerial [%s]", pRspRepeal->BankRepealSerial);
        PURE_LOG("\tTradeCode [%s]", pRspRepeal->TradeCode);
        PURE_LOG("\tBankID [%s]", pRspRepeal->BankID);
        PURE_LOG("\tBankBranchID [%s]", pRspRepeal->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pRspRepeal->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pRspRepeal->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pRspRepeal->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pRspRepeal->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pRspRepeal->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pRspRepeal->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pRspRepeal->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pRspRepeal->IdentifiedCardNo);
        PURE_LOG("\tBankAccount [%s]", pRspRepeal->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pRspRepeal->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pRspRepeal->AccountID);
        PURE_LOG("\tPassword [%s]", pRspRepeal->Password);
        PURE_LOG("\tUserID [%s]", pRspRepeal->UserID);
        PURE_LOG("\tCurrencyID [%s]", pRspRepeal->CurrencyID);
        PURE_LOG("\tMessage [%s]", pRspRepeal->Message);
        PURE_LOG("\tDigest [%s]", pRspRepeal->Digest);
        PURE_LOG("\tDeviceID [%s]", pRspRepeal->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pRspRepeal->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pRspRepeal->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pRspRepeal->OperNo);
        PURE_LOG("\tErrorMsg [%s]", pRspRepeal->ErrorMsg);
        PURE_LOG("\tLongCustomerName [%s]", pRspRepeal->LongCustomerName);
        PURE_LOG("\tRepealTimeInterval [%d]", pRspRepeal->RepealTimeInterval);
        PURE_LOG("\tRepealedTimes [%d]", pRspRepeal->RepealedTimes);
        PURE_LOG("\tPlateRepealSerial [%d]", pRspRepeal->PlateRepealSerial);
        PURE_LOG("\tFutureRepealSerial [%d]", pRspRepeal->FutureRepealSerial);
        PURE_LOG("\tPlateSerial [%d]", pRspRepeal->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pRspRepeal->SessionID);
        PURE_LOG("\tInstallID [%d]", pRspRepeal->InstallID);
        PURE_LOG("\tFutureSerial [%d]", pRspRepeal->FutureSerial);
        PURE_LOG("\tRequestID [%d]", pRspRepeal->RequestID);
        PURE_LOG("\tTID [%d]", pRspRepeal->TID);
        PURE_LOG("\tErrorID [%d]", pRspRepeal->ErrorID);
        PURE_LOG("\tBankRepealFlag [%c]", pRspRepeal->BankRepealFlag);
        PURE_LOG("\tBrokerRepealFlag [%c]", pRspRepeal->BrokerRepealFlag);
        PURE_LOG("\tLastFragment [%c]", pRspRepeal->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pRspRepeal->IdCardType);
        PURE_LOG("\tCustType [%c]", pRspRepeal->CustType);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pRspRepeal->VerifyCertNoFlag);
        PURE_LOG("\tFeePayFlag [%c]", pRspRepeal->FeePayFlag);
        PURE_LOG("\tBankAccType [%c]", pRspRepeal->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pRspRepeal->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pRspRepeal->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pRspRepeal->SecuPwdFlag);
        PURE_LOG("\tTransferStatus [%c]", pRspRepeal->TransferStatus);
        PURE_LOG("\tTradeAmount [%.8lf]", pRspRepeal->TradeAmount);
        PURE_LOG("\tFutureFetchAmount [%.8lf]", pRspRepeal->FutureFetchAmount);
        PURE_LOG("\tCustFee [%.8lf]", pRspRepeal->CustFee);
        PURE_LOG("\tBrokerFee [%.8lf]", pRspRepeal->BrokerFee);
	}
    PURE_LOG("</OnRtnRepealFromBankToFutureByBank>");
};
void CTraderSpi::OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal)
{
    PURE_LOG("<OnRtnRepealFromFutureToBankByBank>");
	if (pRspRepeal)
	{
        PURE_LOG("\tBankRepealSerial [%s]", pRspRepeal->BankRepealSerial);
        PURE_LOG("\tTradeCode [%s]", pRspRepeal->TradeCode);
        PURE_LOG("\tBankID [%s]", pRspRepeal->BankID);
        PURE_LOG("\tBankBranchID [%s]", pRspRepeal->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pRspRepeal->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pRspRepeal->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pRspRepeal->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pRspRepeal->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pRspRepeal->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pRspRepeal->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pRspRepeal->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pRspRepeal->IdentifiedCardNo);
        PURE_LOG("\tBankAccount [%s]", pRspRepeal->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pRspRepeal->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pRspRepeal->AccountID);
        PURE_LOG("\tPassword [%s]", pRspRepeal->Password);
        PURE_LOG("\tUserID [%s]", pRspRepeal->UserID);
        PURE_LOG("\tCurrencyID [%s]", pRspRepeal->CurrencyID);
        PURE_LOG("\tMessage [%s]", pRspRepeal->Message);
        PURE_LOG("\tDigest [%s]", pRspRepeal->Digest);
        PURE_LOG("\tDeviceID [%s]", pRspRepeal->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pRspRepeal->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pRspRepeal->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pRspRepeal->OperNo);
        PURE_LOG("\tErrorMsg [%s]", pRspRepeal->ErrorMsg);
        PURE_LOG("\tLongCustomerName [%s]", pRspRepeal->LongCustomerName);
        PURE_LOG("\tRepealTimeInterval [%d]", pRspRepeal->RepealTimeInterval);
        PURE_LOG("\tRepealedTimes [%d]", pRspRepeal->RepealedTimes);
        PURE_LOG("\tPlateRepealSerial [%d]", pRspRepeal->PlateRepealSerial);
        PURE_LOG("\tFutureRepealSerial [%d]", pRspRepeal->FutureRepealSerial);
        PURE_LOG("\tPlateSerial [%d]", pRspRepeal->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pRspRepeal->SessionID);
        PURE_LOG("\tInstallID [%d]", pRspRepeal->InstallID);
        PURE_LOG("\tFutureSerial [%d]", pRspRepeal->FutureSerial);
        PURE_LOG("\tRequestID [%d]", pRspRepeal->RequestID);
        PURE_LOG("\tTID [%d]", pRspRepeal->TID);
        PURE_LOG("\tErrorID [%d]", pRspRepeal->ErrorID);
        PURE_LOG("\tBankRepealFlag [%c]", pRspRepeal->BankRepealFlag);
        PURE_LOG("\tBrokerRepealFlag [%c]", pRspRepeal->BrokerRepealFlag);
        PURE_LOG("\tLastFragment [%c]", pRspRepeal->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pRspRepeal->IdCardType);
        PURE_LOG("\tCustType [%c]", pRspRepeal->CustType);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pRspRepeal->VerifyCertNoFlag);
        PURE_LOG("\tFeePayFlag [%c]", pRspRepeal->FeePayFlag);
        PURE_LOG("\tBankAccType [%c]", pRspRepeal->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pRspRepeal->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pRspRepeal->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pRspRepeal->SecuPwdFlag);
        PURE_LOG("\tTransferStatus [%c]", pRspRepeal->TransferStatus);
        PURE_LOG("\tTradeAmount [%.8lf]", pRspRepeal->TradeAmount);
        PURE_LOG("\tFutureFetchAmount [%.8lf]", pRspRepeal->FutureFetchAmount);
        PURE_LOG("\tCustFee [%.8lf]", pRspRepeal->CustFee);
        PURE_LOG("\tBrokerFee [%.8lf]", pRspRepeal->BrokerFee);
	}
    PURE_LOG("</OnRtnRepealFromFutureToBankByBank>");
};
void CTraderSpi::OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer)
{
    PURE_LOG("<OnRtnFromBankToFutureByFuture>");
	if (pRspTransfer)
	{
        PURE_LOG("\tTradeCode [%s]", pRspTransfer->TradeCode);
        PURE_LOG("\tBankID [%s]", pRspTransfer->BankID);
        PURE_LOG("\tBankBranchID [%s]", pRspTransfer->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pRspTransfer->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pRspTransfer->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pRspTransfer->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pRspTransfer->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pRspTransfer->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pRspTransfer->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pRspTransfer->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pRspTransfer->IdentifiedCardNo);
        PURE_LOG("\tBankAccount [%s]", pRspTransfer->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pRspTransfer->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pRspTransfer->AccountID);
        PURE_LOG("\tPassword [%s]", pRspTransfer->Password);
        PURE_LOG("\tUserID [%s]", pRspTransfer->UserID);
        PURE_LOG("\tCurrencyID [%s]", pRspTransfer->CurrencyID);
        PURE_LOG("\tMessage [%s]", pRspTransfer->Message);
        PURE_LOG("\tDigest [%s]", pRspTransfer->Digest);
        PURE_LOG("\tDeviceID [%s]", pRspTransfer->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pRspTransfer->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pRspTransfer->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pRspTransfer->OperNo);
        PURE_LOG("\tErrorMsg [%s]", pRspTransfer->ErrorMsg);
        PURE_LOG("\tLongCustomerName [%s]", pRspTransfer->LongCustomerName);
        PURE_LOG("\tPlateSerial [%d]", pRspTransfer->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pRspTransfer->SessionID);
        PURE_LOG("\tInstallID [%d]", pRspTransfer->InstallID);
        PURE_LOG("\tFutureSerial [%d]", pRspTransfer->FutureSerial);
        PURE_LOG("\tRequestID [%d]", pRspTransfer->RequestID);
        PURE_LOG("\tTID [%d]", pRspTransfer->TID);
        PURE_LOG("\tErrorID [%d]", pRspTransfer->ErrorID);
        PURE_LOG("\tLastFragment [%c]", pRspTransfer->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pRspTransfer->IdCardType);
        PURE_LOG("\tCustType [%c]", pRspTransfer->CustType);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pRspTransfer->VerifyCertNoFlag);
        PURE_LOG("\tFeePayFlag [%c]", pRspTransfer->FeePayFlag);
        PURE_LOG("\tBankAccType [%c]", pRspTransfer->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pRspTransfer->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pRspTransfer->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pRspTransfer->SecuPwdFlag);
        PURE_LOG("\tTransferStatus [%c]", pRspTransfer->TransferStatus);
        PURE_LOG("\tTradeAmount [%.8lf]", pRspTransfer->TradeAmount);
        PURE_LOG("\tFutureFetchAmount [%.8lf]", pRspTransfer->FutureFetchAmount);
        PURE_LOG("\tCustFee [%.8lf]", pRspTransfer->CustFee);
        PURE_LOG("\tBrokerFee [%.8lf]", pRspTransfer->BrokerFee);
	}
    PURE_LOG("</OnRtnFromBankToFutureByFuture>");
};
void CTraderSpi::OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer)
{
    PURE_LOG("<OnRtnFromFutureToBankByFuture>");
	if (pRspTransfer)
	{
        PURE_LOG("\tTradeCode [%s]", pRspTransfer->TradeCode);
        PURE_LOG("\tBankID [%s]", pRspTransfer->BankID);
        PURE_LOG("\tBankBranchID [%s]", pRspTransfer->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pRspTransfer->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pRspTransfer->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pRspTransfer->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pRspTransfer->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pRspTransfer->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pRspTransfer->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pRspTransfer->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pRspTransfer->IdentifiedCardNo);
        PURE_LOG("\tBankAccount [%s]", pRspTransfer->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pRspTransfer->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pRspTransfer->AccountID);
        PURE_LOG("\tPassword [%s]", pRspTransfer->Password);
        PURE_LOG("\tUserID [%s]", pRspTransfer->UserID);
        PURE_LOG("\tCurrencyID [%s]", pRspTransfer->CurrencyID);
        PURE_LOG("\tMessage [%s]", pRspTransfer->Message);
        PURE_LOG("\tDigest [%s]", pRspTransfer->Digest);
        PURE_LOG("\tDeviceID [%s]", pRspTransfer->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pRspTransfer->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pRspTransfer->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pRspTransfer->OperNo);
        PURE_LOG("\tErrorMsg [%s]", pRspTransfer->ErrorMsg);
        PURE_LOG("\tLongCustomerName [%s]", pRspTransfer->LongCustomerName);
        PURE_LOG("\tPlateSerial [%d]", pRspTransfer->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pRspTransfer->SessionID);
        PURE_LOG("\tInstallID [%d]", pRspTransfer->InstallID);
        PURE_LOG("\tFutureSerial [%d]", pRspTransfer->FutureSerial);
        PURE_LOG("\tRequestID [%d]", pRspTransfer->RequestID);
        PURE_LOG("\tTID [%d]", pRspTransfer->TID);
        PURE_LOG("\tErrorID [%d]", pRspTransfer->ErrorID);
        PURE_LOG("\tLastFragment [%c]", pRspTransfer->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pRspTransfer->IdCardType);
        PURE_LOG("\tCustType [%c]", pRspTransfer->CustType);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pRspTransfer->VerifyCertNoFlag);
        PURE_LOG("\tFeePayFlag [%c]", pRspTransfer->FeePayFlag);
        PURE_LOG("\tBankAccType [%c]", pRspTransfer->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pRspTransfer->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pRspTransfer->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pRspTransfer->SecuPwdFlag);
        PURE_LOG("\tTransferStatus [%c]", pRspTransfer->TransferStatus);
        PURE_LOG("\tTradeAmount [%.8lf]", pRspTransfer->TradeAmount);
        PURE_LOG("\tFutureFetchAmount [%.8lf]", pRspTransfer->FutureFetchAmount);
        PURE_LOG("\tCustFee [%.8lf]", pRspTransfer->CustFee);
        PURE_LOG("\tBrokerFee [%.8lf]", pRspTransfer->BrokerFee);
	}
    PURE_LOG("</OnRtnFromFutureToBankByFuture>");
};
void CTraderSpi::OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal)
{
    PURE_LOG("<OnRtnRepealFromBankToFutureByFutureManual>");
	if (pRspRepeal)
	{
        PURE_LOG("\tBankRepealSerial [%s]", pRspRepeal->BankRepealSerial);
        PURE_LOG("\tTradeCode [%s]", pRspRepeal->TradeCode);
        PURE_LOG("\tBankID [%s]", pRspRepeal->BankID);
        PURE_LOG("\tBankBranchID [%s]", pRspRepeal->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pRspRepeal->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pRspRepeal->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pRspRepeal->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pRspRepeal->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pRspRepeal->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pRspRepeal->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pRspRepeal->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pRspRepeal->IdentifiedCardNo);
        PURE_LOG("\tBankAccount [%s]", pRspRepeal->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pRspRepeal->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pRspRepeal->AccountID);
        PURE_LOG("\tPassword [%s]", pRspRepeal->Password);
        PURE_LOG("\tUserID [%s]", pRspRepeal->UserID);
        PURE_LOG("\tCurrencyID [%s]", pRspRepeal->CurrencyID);
        PURE_LOG("\tMessage [%s]", pRspRepeal->Message);
        PURE_LOG("\tDigest [%s]", pRspRepeal->Digest);
        PURE_LOG("\tDeviceID [%s]", pRspRepeal->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pRspRepeal->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pRspRepeal->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pRspRepeal->OperNo);
        PURE_LOG("\tErrorMsg [%s]", pRspRepeal->ErrorMsg);
        PURE_LOG("\tLongCustomerName [%s]", pRspRepeal->LongCustomerName);
        PURE_LOG("\tRepealTimeInterval [%d]", pRspRepeal->RepealTimeInterval);
        PURE_LOG("\tRepealedTimes [%d]", pRspRepeal->RepealedTimes);
        PURE_LOG("\tPlateRepealSerial [%d]", pRspRepeal->PlateRepealSerial);
        PURE_LOG("\tFutureRepealSerial [%d]", pRspRepeal->FutureRepealSerial);
        PURE_LOG("\tPlateSerial [%d]", pRspRepeal->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pRspRepeal->SessionID);
        PURE_LOG("\tInstallID [%d]", pRspRepeal->InstallID);
        PURE_LOG("\tFutureSerial [%d]", pRspRepeal->FutureSerial);
        PURE_LOG("\tRequestID [%d]", pRspRepeal->RequestID);
        PURE_LOG("\tTID [%d]", pRspRepeal->TID);
        PURE_LOG("\tErrorID [%d]", pRspRepeal->ErrorID);
        PURE_LOG("\tBankRepealFlag [%c]", pRspRepeal->BankRepealFlag);
        PURE_LOG("\tBrokerRepealFlag [%c]", pRspRepeal->BrokerRepealFlag);
        PURE_LOG("\tLastFragment [%c]", pRspRepeal->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pRspRepeal->IdCardType);
        PURE_LOG("\tCustType [%c]", pRspRepeal->CustType);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pRspRepeal->VerifyCertNoFlag);
        PURE_LOG("\tFeePayFlag [%c]", pRspRepeal->FeePayFlag);
        PURE_LOG("\tBankAccType [%c]", pRspRepeal->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pRspRepeal->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pRspRepeal->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pRspRepeal->SecuPwdFlag);
        PURE_LOG("\tTransferStatus [%c]", pRspRepeal->TransferStatus);
        PURE_LOG("\tTradeAmount [%.8lf]", pRspRepeal->TradeAmount);
        PURE_LOG("\tFutureFetchAmount [%.8lf]", pRspRepeal->FutureFetchAmount);
        PURE_LOG("\tCustFee [%.8lf]", pRspRepeal->CustFee);
        PURE_LOG("\tBrokerFee [%.8lf]", pRspRepeal->BrokerFee);
	}
    PURE_LOG("</OnRtnRepealFromBankToFutureByFutureManual>");
};
void CTraderSpi::OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal)
{
    PURE_LOG("<OnRtnRepealFromFutureToBankByFutureManual>");
	if (pRspRepeal)
	{
        PURE_LOG("\tBankRepealSerial [%s]", pRspRepeal->BankRepealSerial);
        PURE_LOG("\tTradeCode [%s]", pRspRepeal->TradeCode);
        PURE_LOG("\tBankID [%s]", pRspRepeal->BankID);
        PURE_LOG("\tBankBranchID [%s]", pRspRepeal->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pRspRepeal->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pRspRepeal->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pRspRepeal->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pRspRepeal->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pRspRepeal->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pRspRepeal->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pRspRepeal->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pRspRepeal->IdentifiedCardNo);
        PURE_LOG("\tBankAccount [%s]", pRspRepeal->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pRspRepeal->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pRspRepeal->AccountID);
        PURE_LOG("\tPassword [%s]", pRspRepeal->Password);
        PURE_LOG("\tUserID [%s]", pRspRepeal->UserID);
        PURE_LOG("\tCurrencyID [%s]", pRspRepeal->CurrencyID);
        PURE_LOG("\tMessage [%s]", pRspRepeal->Message);
        PURE_LOG("\tDigest [%s]", pRspRepeal->Digest);
        PURE_LOG("\tDeviceID [%s]", pRspRepeal->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pRspRepeal->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pRspRepeal->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pRspRepeal->OperNo);
        PURE_LOG("\tErrorMsg [%s]", pRspRepeal->ErrorMsg);
        PURE_LOG("\tLongCustomerName [%s]", pRspRepeal->LongCustomerName);
        PURE_LOG("\tRepealTimeInterval [%d]", pRspRepeal->RepealTimeInterval);
        PURE_LOG("\tRepealedTimes [%d]", pRspRepeal->RepealedTimes);
        PURE_LOG("\tPlateRepealSerial [%d]", pRspRepeal->PlateRepealSerial);
        PURE_LOG("\tFutureRepealSerial [%d]", pRspRepeal->FutureRepealSerial);
        PURE_LOG("\tPlateSerial [%d]", pRspRepeal->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pRspRepeal->SessionID);
        PURE_LOG("\tInstallID [%d]", pRspRepeal->InstallID);
        PURE_LOG("\tFutureSerial [%d]", pRspRepeal->FutureSerial);
        PURE_LOG("\tRequestID [%d]", pRspRepeal->RequestID);
        PURE_LOG("\tTID [%d]", pRspRepeal->TID);
        PURE_LOG("\tErrorID [%d]", pRspRepeal->ErrorID);
        PURE_LOG("\tBankRepealFlag [%c]", pRspRepeal->BankRepealFlag);
        PURE_LOG("\tBrokerRepealFlag [%c]", pRspRepeal->BrokerRepealFlag);
        PURE_LOG("\tLastFragment [%c]", pRspRepeal->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pRspRepeal->IdCardType);
        PURE_LOG("\tCustType [%c]", pRspRepeal->CustType);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pRspRepeal->VerifyCertNoFlag);
        PURE_LOG("\tFeePayFlag [%c]", pRspRepeal->FeePayFlag);
        PURE_LOG("\tBankAccType [%c]", pRspRepeal->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pRspRepeal->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pRspRepeal->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pRspRepeal->SecuPwdFlag);
        PURE_LOG("\tTransferStatus [%c]", pRspRepeal->TransferStatus);
        PURE_LOG("\tTradeAmount [%.8lf]", pRspRepeal->TradeAmount);
        PURE_LOG("\tFutureFetchAmount [%.8lf]", pRspRepeal->FutureFetchAmount);
        PURE_LOG("\tCustFee [%.8lf]", pRspRepeal->CustFee);
        PURE_LOG("\tBrokerFee [%.8lf]", pRspRepeal->BrokerFee);
	}
    PURE_LOG("</OnRtnRepealFromFutureToBankByFutureManual>");
};
void CTraderSpi::OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount)
{
    PURE_LOG("<OnRtnQueryBankBalanceByFuture>");
	if (pNotifyQueryAccount)
	{
        PURE_LOG("\tTradeCode [%s]", pNotifyQueryAccount->TradeCode);
        PURE_LOG("\tBankID [%s]", pNotifyQueryAccount->BankID);
        PURE_LOG("\tBankBranchID [%s]", pNotifyQueryAccount->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pNotifyQueryAccount->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pNotifyQueryAccount->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pNotifyQueryAccount->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pNotifyQueryAccount->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pNotifyQueryAccount->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pNotifyQueryAccount->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pNotifyQueryAccount->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pNotifyQueryAccount->IdentifiedCardNo);
        PURE_LOG("\tBankAccount [%s]", pNotifyQueryAccount->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pNotifyQueryAccount->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pNotifyQueryAccount->AccountID);
        PURE_LOG("\tPassword [%s]", pNotifyQueryAccount->Password);
        PURE_LOG("\tUserID [%s]", pNotifyQueryAccount->UserID);
        PURE_LOG("\tCurrencyID [%s]", pNotifyQueryAccount->CurrencyID);
        PURE_LOG("\tDigest [%s]", pNotifyQueryAccount->Digest);
        PURE_LOG("\tDeviceID [%s]", pNotifyQueryAccount->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pNotifyQueryAccount->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pNotifyQueryAccount->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pNotifyQueryAccount->OperNo);
        PURE_LOG("\tErrorMsg [%s]", pNotifyQueryAccount->ErrorMsg);
        PURE_LOG("\tLongCustomerName [%s]", pNotifyQueryAccount->LongCustomerName);
        PURE_LOG("\tPlateSerial [%d]", pNotifyQueryAccount->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pNotifyQueryAccount->SessionID);
        PURE_LOG("\tFutureSerial [%d]", pNotifyQueryAccount->FutureSerial);
        PURE_LOG("\tInstallID [%d]", pNotifyQueryAccount->InstallID);
        PURE_LOG("\tRequestID [%d]", pNotifyQueryAccount->RequestID);
        PURE_LOG("\tTID [%d]", pNotifyQueryAccount->TID);
        PURE_LOG("\tErrorID [%d]", pNotifyQueryAccount->ErrorID);
        PURE_LOG("\tLastFragment [%c]", pNotifyQueryAccount->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pNotifyQueryAccount->IdCardType);
        PURE_LOG("\tCustType [%c]", pNotifyQueryAccount->CustType);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pNotifyQueryAccount->VerifyCertNoFlag);
        PURE_LOG("\tBankAccType [%c]", pNotifyQueryAccount->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pNotifyQueryAccount->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pNotifyQueryAccount->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pNotifyQueryAccount->SecuPwdFlag);
        PURE_LOG("\tBankUseAmount [%.8lf]", pNotifyQueryAccount->BankUseAmount);
        PURE_LOG("\tBankFetchAmount [%.8lf]", pNotifyQueryAccount->BankFetchAmount);
	}
    PURE_LOG("</OnRtnQueryBankBalanceByFuture>");
};
void CTraderSpi::OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo)
{
    PURE_LOG("<OnErrRtnBankToFutureByFuture>");
	if (pReqTransfer)
	{
        PURE_LOG("\tTradeCode [%s]", pReqTransfer->TradeCode);
        PURE_LOG("\tBankID [%s]", pReqTransfer->BankID);
        PURE_LOG("\tBankBranchID [%s]", pReqTransfer->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pReqTransfer->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pReqTransfer->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pReqTransfer->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pReqTransfer->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pReqTransfer->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pReqTransfer->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pReqTransfer->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pReqTransfer->IdentifiedCardNo);
        PURE_LOG("\tBankAccount [%s]", pReqTransfer->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pReqTransfer->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pReqTransfer->AccountID);
        PURE_LOG("\tPassword [%s]", pReqTransfer->Password);
        PURE_LOG("\tUserID [%s]", pReqTransfer->UserID);
        PURE_LOG("\tCurrencyID [%s]", pReqTransfer->CurrencyID);
        PURE_LOG("\tMessage [%s]", pReqTransfer->Message);
        PURE_LOG("\tDigest [%s]", pReqTransfer->Digest);
        PURE_LOG("\tDeviceID [%s]", pReqTransfer->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pReqTransfer->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pReqTransfer->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pReqTransfer->OperNo);
        PURE_LOG("\tLongCustomerName [%s]", pReqTransfer->LongCustomerName);
        PURE_LOG("\tPlateSerial [%d]", pReqTransfer->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pReqTransfer->SessionID);
        PURE_LOG("\tInstallID [%d]", pReqTransfer->InstallID);
        PURE_LOG("\tFutureSerial [%d]", pReqTransfer->FutureSerial);
        PURE_LOG("\tRequestID [%d]", pReqTransfer->RequestID);
        PURE_LOG("\tTID [%d]", pReqTransfer->TID);
        PURE_LOG("\tLastFragment [%c]", pReqTransfer->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pReqTransfer->IdCardType);
        PURE_LOG("\tCustType [%c]", pReqTransfer->CustType);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pReqTransfer->VerifyCertNoFlag);
        PURE_LOG("\tFeePayFlag [%c]", pReqTransfer->FeePayFlag);
        PURE_LOG("\tBankAccType [%c]", pReqTransfer->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pReqTransfer->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pReqTransfer->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pReqTransfer->SecuPwdFlag);
        PURE_LOG("\tTransferStatus [%c]", pReqTransfer->TransferStatus);
        PURE_LOG("\tTradeAmount [%.8lf]", pReqTransfer->TradeAmount);
        PURE_LOG("\tFutureFetchAmount [%.8lf]", pReqTransfer->FutureFetchAmount);
        PURE_LOG("\tCustFee [%.8lf]", pReqTransfer->CustFee);
        PURE_LOG("\tBrokerFee [%.8lf]", pReqTransfer->BrokerFee);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("</OnErrRtnBankToFutureByFuture>");
};
void CTraderSpi::OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo)
{
    PURE_LOG("<OnErrRtnFutureToBankByFuture>");
	if (pReqTransfer)
	{
        PURE_LOG("\tTradeCode [%s]", pReqTransfer->TradeCode);
        PURE_LOG("\tBankID [%s]", pReqTransfer->BankID);
        PURE_LOG("\tBankBranchID [%s]", pReqTransfer->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pReqTransfer->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pReqTransfer->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pReqTransfer->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pReqTransfer->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pReqTransfer->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pReqTransfer->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pReqTransfer->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pReqTransfer->IdentifiedCardNo);
        PURE_LOG("\tBankAccount [%s]", pReqTransfer->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pReqTransfer->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pReqTransfer->AccountID);
        PURE_LOG("\tPassword [%s]", pReqTransfer->Password);
        PURE_LOG("\tUserID [%s]", pReqTransfer->UserID);
        PURE_LOG("\tCurrencyID [%s]", pReqTransfer->CurrencyID);
        PURE_LOG("\tMessage [%s]", pReqTransfer->Message);
        PURE_LOG("\tDigest [%s]", pReqTransfer->Digest);
        PURE_LOG("\tDeviceID [%s]", pReqTransfer->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pReqTransfer->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pReqTransfer->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pReqTransfer->OperNo);
        PURE_LOG("\tLongCustomerName [%s]", pReqTransfer->LongCustomerName);
        PURE_LOG("\tPlateSerial [%d]", pReqTransfer->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pReqTransfer->SessionID);
        PURE_LOG("\tInstallID [%d]", pReqTransfer->InstallID);
        PURE_LOG("\tFutureSerial [%d]", pReqTransfer->FutureSerial);
        PURE_LOG("\tRequestID [%d]", pReqTransfer->RequestID);
        PURE_LOG("\tTID [%d]", pReqTransfer->TID);
        PURE_LOG("\tLastFragment [%c]", pReqTransfer->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pReqTransfer->IdCardType);
        PURE_LOG("\tCustType [%c]", pReqTransfer->CustType);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pReqTransfer->VerifyCertNoFlag);
        PURE_LOG("\tFeePayFlag [%c]", pReqTransfer->FeePayFlag);
        PURE_LOG("\tBankAccType [%c]", pReqTransfer->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pReqTransfer->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pReqTransfer->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pReqTransfer->SecuPwdFlag);
        PURE_LOG("\tTransferStatus [%c]", pReqTransfer->TransferStatus);
        PURE_LOG("\tTradeAmount [%.8lf]", pReqTransfer->TradeAmount);
        PURE_LOG("\tFutureFetchAmount [%.8lf]", pReqTransfer->FutureFetchAmount);
        PURE_LOG("\tCustFee [%.8lf]", pReqTransfer->CustFee);
        PURE_LOG("\tBrokerFee [%.8lf]", pReqTransfer->BrokerFee);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("</OnErrRtnFutureToBankByFuture>");
};
void CTraderSpi::OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo)
{
    PURE_LOG("<OnErrRtnRepealBankToFutureByFutureManual>");
	if (pReqRepeal)
	{
        PURE_LOG("\tBankRepealSerial [%s]", pReqRepeal->BankRepealSerial);
        PURE_LOG("\tTradeCode [%s]", pReqRepeal->TradeCode);
        PURE_LOG("\tBankID [%s]", pReqRepeal->BankID);
        PURE_LOG("\tBankBranchID [%s]", pReqRepeal->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pReqRepeal->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pReqRepeal->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pReqRepeal->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pReqRepeal->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pReqRepeal->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pReqRepeal->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pReqRepeal->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pReqRepeal->IdentifiedCardNo);
        PURE_LOG("\tBankAccount [%s]", pReqRepeal->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pReqRepeal->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pReqRepeal->AccountID);
        PURE_LOG("\tPassword [%s]", pReqRepeal->Password);
        PURE_LOG("\tUserID [%s]", pReqRepeal->UserID);
        PURE_LOG("\tCurrencyID [%s]", pReqRepeal->CurrencyID);
        PURE_LOG("\tMessage [%s]", pReqRepeal->Message);
        PURE_LOG("\tDigest [%s]", pReqRepeal->Digest);
        PURE_LOG("\tDeviceID [%s]", pReqRepeal->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pReqRepeal->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pReqRepeal->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pReqRepeal->OperNo);
        PURE_LOG("\tLongCustomerName [%s]", pReqRepeal->LongCustomerName);
        PURE_LOG("\tRepealTimeInterval [%d]", pReqRepeal->RepealTimeInterval);
        PURE_LOG("\tRepealedTimes [%d]", pReqRepeal->RepealedTimes);
        PURE_LOG("\tPlateRepealSerial [%d]", pReqRepeal->PlateRepealSerial);
        PURE_LOG("\tFutureRepealSerial [%d]", pReqRepeal->FutureRepealSerial);
        PURE_LOG("\tPlateSerial [%d]", pReqRepeal->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pReqRepeal->SessionID);
        PURE_LOG("\tInstallID [%d]", pReqRepeal->InstallID);
        PURE_LOG("\tFutureSerial [%d]", pReqRepeal->FutureSerial);
        PURE_LOG("\tRequestID [%d]", pReqRepeal->RequestID);
        PURE_LOG("\tTID [%d]", pReqRepeal->TID);
        PURE_LOG("\tBankRepealFlag [%c]", pReqRepeal->BankRepealFlag);
        PURE_LOG("\tBrokerRepealFlag [%c]", pReqRepeal->BrokerRepealFlag);
        PURE_LOG("\tLastFragment [%c]", pReqRepeal->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pReqRepeal->IdCardType);
        PURE_LOG("\tCustType [%c]", pReqRepeal->CustType);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pReqRepeal->VerifyCertNoFlag);
        PURE_LOG("\tFeePayFlag [%c]", pReqRepeal->FeePayFlag);
        PURE_LOG("\tBankAccType [%c]", pReqRepeal->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pReqRepeal->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pReqRepeal->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pReqRepeal->SecuPwdFlag);
        PURE_LOG("\tTransferStatus [%c]", pReqRepeal->TransferStatus);
        PURE_LOG("\tTradeAmount [%.8lf]", pReqRepeal->TradeAmount);
        PURE_LOG("\tFutureFetchAmount [%.8lf]", pReqRepeal->FutureFetchAmount);
        PURE_LOG("\tCustFee [%.8lf]", pReqRepeal->CustFee);
        PURE_LOG("\tBrokerFee [%.8lf]", pReqRepeal->BrokerFee);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("</OnErrRtnRepealBankToFutureByFutureManual>");
};
void CTraderSpi::OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo)
{
    PURE_LOG("<OnErrRtnRepealFutureToBankByFutureManual>");
	if (pReqRepeal)
	{
        PURE_LOG("\tBankRepealSerial [%s]", pReqRepeal->BankRepealSerial);
        PURE_LOG("\tTradeCode [%s]", pReqRepeal->TradeCode);
        PURE_LOG("\tBankID [%s]", pReqRepeal->BankID);
        PURE_LOG("\tBankBranchID [%s]", pReqRepeal->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pReqRepeal->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pReqRepeal->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pReqRepeal->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pReqRepeal->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pReqRepeal->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pReqRepeal->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pReqRepeal->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pReqRepeal->IdentifiedCardNo);
        PURE_LOG("\tBankAccount [%s]", pReqRepeal->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pReqRepeal->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pReqRepeal->AccountID);
        PURE_LOG("\tPassword [%s]", pReqRepeal->Password);
        PURE_LOG("\tUserID [%s]", pReqRepeal->UserID);
        PURE_LOG("\tCurrencyID [%s]", pReqRepeal->CurrencyID);
        PURE_LOG("\tMessage [%s]", pReqRepeal->Message);
        PURE_LOG("\tDigest [%s]", pReqRepeal->Digest);
        PURE_LOG("\tDeviceID [%s]", pReqRepeal->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pReqRepeal->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pReqRepeal->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pReqRepeal->OperNo);
        PURE_LOG("\tLongCustomerName [%s]", pReqRepeal->LongCustomerName);
        PURE_LOG("\tRepealTimeInterval [%d]", pReqRepeal->RepealTimeInterval);
        PURE_LOG("\tRepealedTimes [%d]", pReqRepeal->RepealedTimes);
        PURE_LOG("\tPlateRepealSerial [%d]", pReqRepeal->PlateRepealSerial);
        PURE_LOG("\tFutureRepealSerial [%d]", pReqRepeal->FutureRepealSerial);
        PURE_LOG("\tPlateSerial [%d]", pReqRepeal->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pReqRepeal->SessionID);
        PURE_LOG("\tInstallID [%d]", pReqRepeal->InstallID);
        PURE_LOG("\tFutureSerial [%d]", pReqRepeal->FutureSerial);
        PURE_LOG("\tRequestID [%d]", pReqRepeal->RequestID);
        PURE_LOG("\tTID [%d]", pReqRepeal->TID);
        PURE_LOG("\tBankRepealFlag [%c]", pReqRepeal->BankRepealFlag);
        PURE_LOG("\tBrokerRepealFlag [%c]", pReqRepeal->BrokerRepealFlag);
        PURE_LOG("\tLastFragment [%c]", pReqRepeal->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pReqRepeal->IdCardType);
        PURE_LOG("\tCustType [%c]", pReqRepeal->CustType);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pReqRepeal->VerifyCertNoFlag);
        PURE_LOG("\tFeePayFlag [%c]", pReqRepeal->FeePayFlag);
        PURE_LOG("\tBankAccType [%c]", pReqRepeal->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pReqRepeal->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pReqRepeal->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pReqRepeal->SecuPwdFlag);
        PURE_LOG("\tTransferStatus [%c]", pReqRepeal->TransferStatus);
        PURE_LOG("\tTradeAmount [%.8lf]", pReqRepeal->TradeAmount);
        PURE_LOG("\tFutureFetchAmount [%.8lf]", pReqRepeal->FutureFetchAmount);
        PURE_LOG("\tCustFee [%.8lf]", pReqRepeal->CustFee);
        PURE_LOG("\tBrokerFee [%.8lf]", pReqRepeal->BrokerFee);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("</OnErrRtnRepealFutureToBankByFutureManual>");
};
void CTraderSpi::OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo)
{
    PURE_LOG("<OnErrRtnQueryBankBalanceByFuture>");
	if (pReqQueryAccount)
	{
        PURE_LOG("\tTradeCode [%s]", pReqQueryAccount->TradeCode);
        PURE_LOG("\tBankID [%s]", pReqQueryAccount->BankID);
        PURE_LOG("\tBankBranchID [%s]", pReqQueryAccount->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pReqQueryAccount->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pReqQueryAccount->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pReqQueryAccount->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pReqQueryAccount->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pReqQueryAccount->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pReqQueryAccount->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pReqQueryAccount->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pReqQueryAccount->IdentifiedCardNo);
        PURE_LOG("\tBankAccount [%s]", pReqQueryAccount->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pReqQueryAccount->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pReqQueryAccount->AccountID);
        PURE_LOG("\tPassword [%s]", pReqQueryAccount->Password);
        PURE_LOG("\tUserID [%s]", pReqQueryAccount->UserID);
        PURE_LOG("\tCurrencyID [%s]", pReqQueryAccount->CurrencyID);
        PURE_LOG("\tDigest [%s]", pReqQueryAccount->Digest);
        PURE_LOG("\tDeviceID [%s]", pReqQueryAccount->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pReqQueryAccount->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pReqQueryAccount->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pReqQueryAccount->OperNo);
        PURE_LOG("\tLongCustomerName [%s]", pReqQueryAccount->LongCustomerName);
        PURE_LOG("\tPlateSerial [%d]", pReqQueryAccount->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pReqQueryAccount->SessionID);
        PURE_LOG("\tFutureSerial [%d]", pReqQueryAccount->FutureSerial);
        PURE_LOG("\tInstallID [%d]", pReqQueryAccount->InstallID);
        PURE_LOG("\tRequestID [%d]", pReqQueryAccount->RequestID);
        PURE_LOG("\tTID [%d]", pReqQueryAccount->TID);
        PURE_LOG("\tLastFragment [%c]", pReqQueryAccount->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pReqQueryAccount->IdCardType);
        PURE_LOG("\tCustType [%c]", pReqQueryAccount->CustType);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pReqQueryAccount->VerifyCertNoFlag);
        PURE_LOG("\tBankAccType [%c]", pReqQueryAccount->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pReqQueryAccount->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pReqQueryAccount->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pReqQueryAccount->SecuPwdFlag);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("</OnErrRtnQueryBankBalanceByFuture>");
};
void CTraderSpi::OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal)
{
    PURE_LOG("<OnRtnRepealFromBankToFutureByFuture>");
	if (pRspRepeal)
	{
        PURE_LOG("\tBankRepealSerial [%s]", pRspRepeal->BankRepealSerial);
        PURE_LOG("\tTradeCode [%s]", pRspRepeal->TradeCode);
        PURE_LOG("\tBankID [%s]", pRspRepeal->BankID);
        PURE_LOG("\tBankBranchID [%s]", pRspRepeal->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pRspRepeal->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pRspRepeal->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pRspRepeal->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pRspRepeal->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pRspRepeal->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pRspRepeal->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pRspRepeal->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pRspRepeal->IdentifiedCardNo);
        PURE_LOG("\tBankAccount [%s]", pRspRepeal->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pRspRepeal->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pRspRepeal->AccountID);
        PURE_LOG("\tPassword [%s]", pRspRepeal->Password);
        PURE_LOG("\tUserID [%s]", pRspRepeal->UserID);
        PURE_LOG("\tCurrencyID [%s]", pRspRepeal->CurrencyID);
        PURE_LOG("\tMessage [%s]", pRspRepeal->Message);
        PURE_LOG("\tDigest [%s]", pRspRepeal->Digest);
        PURE_LOG("\tDeviceID [%s]", pRspRepeal->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pRspRepeal->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pRspRepeal->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pRspRepeal->OperNo);
        PURE_LOG("\tErrorMsg [%s]", pRspRepeal->ErrorMsg);
        PURE_LOG("\tLongCustomerName [%s]", pRspRepeal->LongCustomerName);
        PURE_LOG("\tRepealTimeInterval [%d]", pRspRepeal->RepealTimeInterval);
        PURE_LOG("\tRepealedTimes [%d]", pRspRepeal->RepealedTimes);
        PURE_LOG("\tPlateRepealSerial [%d]", pRspRepeal->PlateRepealSerial);
        PURE_LOG("\tFutureRepealSerial [%d]", pRspRepeal->FutureRepealSerial);
        PURE_LOG("\tPlateSerial [%d]", pRspRepeal->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pRspRepeal->SessionID);
        PURE_LOG("\tInstallID [%d]", pRspRepeal->InstallID);
        PURE_LOG("\tFutureSerial [%d]", pRspRepeal->FutureSerial);
        PURE_LOG("\tRequestID [%d]", pRspRepeal->RequestID);
        PURE_LOG("\tTID [%d]", pRspRepeal->TID);
        PURE_LOG("\tErrorID [%d]", pRspRepeal->ErrorID);
        PURE_LOG("\tBankRepealFlag [%c]", pRspRepeal->BankRepealFlag);
        PURE_LOG("\tBrokerRepealFlag [%c]", pRspRepeal->BrokerRepealFlag);
        PURE_LOG("\tLastFragment [%c]", pRspRepeal->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pRspRepeal->IdCardType);
        PURE_LOG("\tCustType [%c]", pRspRepeal->CustType);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pRspRepeal->VerifyCertNoFlag);
        PURE_LOG("\tFeePayFlag [%c]", pRspRepeal->FeePayFlag);
        PURE_LOG("\tBankAccType [%c]", pRspRepeal->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pRspRepeal->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pRspRepeal->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pRspRepeal->SecuPwdFlag);
        PURE_LOG("\tTransferStatus [%c]", pRspRepeal->TransferStatus);
        PURE_LOG("\tTradeAmount [%.8lf]", pRspRepeal->TradeAmount);
        PURE_LOG("\tFutureFetchAmount [%.8lf]", pRspRepeal->FutureFetchAmount);
        PURE_LOG("\tCustFee [%.8lf]", pRspRepeal->CustFee);
        PURE_LOG("\tBrokerFee [%.8lf]", pRspRepeal->BrokerFee);
	}
    PURE_LOG("</OnRtnRepealFromBankToFutureByFuture>");
};
void CTraderSpi::OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal)
{
    PURE_LOG("<OnRtnRepealFromFutureToBankByFuture>");
	if (pRspRepeal)
	{
        PURE_LOG("\tBankRepealSerial [%s]", pRspRepeal->BankRepealSerial);
        PURE_LOG("\tTradeCode [%s]", pRspRepeal->TradeCode);
        PURE_LOG("\tBankID [%s]", pRspRepeal->BankID);
        PURE_LOG("\tBankBranchID [%s]", pRspRepeal->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pRspRepeal->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pRspRepeal->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pRspRepeal->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pRspRepeal->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pRspRepeal->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pRspRepeal->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pRspRepeal->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pRspRepeal->IdentifiedCardNo);
        PURE_LOG("\tBankAccount [%s]", pRspRepeal->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pRspRepeal->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pRspRepeal->AccountID);
        PURE_LOG("\tPassword [%s]", pRspRepeal->Password);
        PURE_LOG("\tUserID [%s]", pRspRepeal->UserID);
        PURE_LOG("\tCurrencyID [%s]", pRspRepeal->CurrencyID);
        PURE_LOG("\tMessage [%s]", pRspRepeal->Message);
        PURE_LOG("\tDigest [%s]", pRspRepeal->Digest);
        PURE_LOG("\tDeviceID [%s]", pRspRepeal->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pRspRepeal->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pRspRepeal->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pRspRepeal->OperNo);
        PURE_LOG("\tErrorMsg [%s]", pRspRepeal->ErrorMsg);
        PURE_LOG("\tLongCustomerName [%s]", pRspRepeal->LongCustomerName);
        PURE_LOG("\tRepealTimeInterval [%d]", pRspRepeal->RepealTimeInterval);
        PURE_LOG("\tRepealedTimes [%d]", pRspRepeal->RepealedTimes);
        PURE_LOG("\tPlateRepealSerial [%d]", pRspRepeal->PlateRepealSerial);
        PURE_LOG("\tFutureRepealSerial [%d]", pRspRepeal->FutureRepealSerial);
        PURE_LOG("\tPlateSerial [%d]", pRspRepeal->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pRspRepeal->SessionID);
        PURE_LOG("\tInstallID [%d]", pRspRepeal->InstallID);
        PURE_LOG("\tFutureSerial [%d]", pRspRepeal->FutureSerial);
        PURE_LOG("\tRequestID [%d]", pRspRepeal->RequestID);
        PURE_LOG("\tTID [%d]", pRspRepeal->TID);
        PURE_LOG("\tErrorID [%d]", pRspRepeal->ErrorID);
        PURE_LOG("\tBankRepealFlag [%c]", pRspRepeal->BankRepealFlag);
        PURE_LOG("\tBrokerRepealFlag [%c]", pRspRepeal->BrokerRepealFlag);
        PURE_LOG("\tLastFragment [%c]", pRspRepeal->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pRspRepeal->IdCardType);
        PURE_LOG("\tCustType [%c]", pRspRepeal->CustType);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pRspRepeal->VerifyCertNoFlag);
        PURE_LOG("\tFeePayFlag [%c]", pRspRepeal->FeePayFlag);
        PURE_LOG("\tBankAccType [%c]", pRspRepeal->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pRspRepeal->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pRspRepeal->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pRspRepeal->SecuPwdFlag);
        PURE_LOG("\tTransferStatus [%c]", pRspRepeal->TransferStatus);
        PURE_LOG("\tTradeAmount [%.8lf]", pRspRepeal->TradeAmount);
        PURE_LOG("\tFutureFetchAmount [%.8lf]", pRspRepeal->FutureFetchAmount);
        PURE_LOG("\tCustFee [%.8lf]", pRspRepeal->CustFee);
        PURE_LOG("\tBrokerFee [%.8lf]", pRspRepeal->BrokerFee);
	}
    PURE_LOG("</OnRtnRepealFromFutureToBankByFuture>");
};
void CTraderSpi::OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspFromBankToFutureByFuture>");
	if (pReqTransfer)
	{
        PURE_LOG("\tTradeCode [%s]", pReqTransfer->TradeCode);
        PURE_LOG("\tBankID [%s]", pReqTransfer->BankID);
        PURE_LOG("\tBankBranchID [%s]", pReqTransfer->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pReqTransfer->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pReqTransfer->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pReqTransfer->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pReqTransfer->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pReqTransfer->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pReqTransfer->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pReqTransfer->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pReqTransfer->IdentifiedCardNo);
        PURE_LOG("\tBankAccount [%s]", pReqTransfer->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pReqTransfer->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pReqTransfer->AccountID);
        PURE_LOG("\tPassword [%s]", pReqTransfer->Password);
        PURE_LOG("\tUserID [%s]", pReqTransfer->UserID);
        PURE_LOG("\tCurrencyID [%s]", pReqTransfer->CurrencyID);
        PURE_LOG("\tMessage [%s]", pReqTransfer->Message);
        PURE_LOG("\tDigest [%s]", pReqTransfer->Digest);
        PURE_LOG("\tDeviceID [%s]", pReqTransfer->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pReqTransfer->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pReqTransfer->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pReqTransfer->OperNo);
        PURE_LOG("\tLongCustomerName [%s]", pReqTransfer->LongCustomerName);
        PURE_LOG("\tPlateSerial [%d]", pReqTransfer->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pReqTransfer->SessionID);
        PURE_LOG("\tInstallID [%d]", pReqTransfer->InstallID);
        PURE_LOG("\tFutureSerial [%d]", pReqTransfer->FutureSerial);
        PURE_LOG("\tRequestID [%d]", pReqTransfer->RequestID);
        PURE_LOG("\tTID [%d]", pReqTransfer->TID);
        PURE_LOG("\tLastFragment [%c]", pReqTransfer->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pReqTransfer->IdCardType);
        PURE_LOG("\tCustType [%c]", pReqTransfer->CustType);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pReqTransfer->VerifyCertNoFlag);
        PURE_LOG("\tFeePayFlag [%c]", pReqTransfer->FeePayFlag);
        PURE_LOG("\tBankAccType [%c]", pReqTransfer->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pReqTransfer->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pReqTransfer->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pReqTransfer->SecuPwdFlag);
        PURE_LOG("\tTransferStatus [%c]", pReqTransfer->TransferStatus);
        PURE_LOG("\tTradeAmount [%.8lf]", pReqTransfer->TradeAmount);
        PURE_LOG("\tFutureFetchAmount [%.8lf]", pReqTransfer->FutureFetchAmount);
        PURE_LOG("\tCustFee [%.8lf]", pReqTransfer->CustFee);
        PURE_LOG("\tBrokerFee [%.8lf]", pReqTransfer->BrokerFee);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspFromBankToFutureByFuture>");
};

void CTraderSpi::OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspFromFutureToBankByFuture>");
	if (pReqTransfer)
	{
        PURE_LOG("\tTradeCode [%s]", pReqTransfer->TradeCode);
        PURE_LOG("\tBankID [%s]", pReqTransfer->BankID);
        PURE_LOG("\tBankBranchID [%s]", pReqTransfer->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pReqTransfer->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pReqTransfer->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pReqTransfer->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pReqTransfer->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pReqTransfer->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pReqTransfer->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pReqTransfer->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pReqTransfer->IdentifiedCardNo);
        PURE_LOG("\tBankAccount [%s]", pReqTransfer->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pReqTransfer->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pReqTransfer->AccountID);
        PURE_LOG("\tPassword [%s]", pReqTransfer->Password);
        PURE_LOG("\tUserID [%s]", pReqTransfer->UserID);
        PURE_LOG("\tCurrencyID [%s]", pReqTransfer->CurrencyID);
        PURE_LOG("\tMessage [%s]", pReqTransfer->Message);
        PURE_LOG("\tDigest [%s]", pReqTransfer->Digest);
        PURE_LOG("\tDeviceID [%s]", pReqTransfer->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pReqTransfer->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pReqTransfer->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pReqTransfer->OperNo);
        PURE_LOG("\tLongCustomerName [%s]", pReqTransfer->LongCustomerName);
        PURE_LOG("\tPlateSerial [%d]", pReqTransfer->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pReqTransfer->SessionID);
        PURE_LOG("\tInstallID [%d]", pReqTransfer->InstallID);
        PURE_LOG("\tFutureSerial [%d]", pReqTransfer->FutureSerial);
        PURE_LOG("\tRequestID [%d]", pReqTransfer->RequestID);
        PURE_LOG("\tTID [%d]", pReqTransfer->TID);
        PURE_LOG("\tLastFragment [%c]", pReqTransfer->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pReqTransfer->IdCardType);
        PURE_LOG("\tCustType [%c]", pReqTransfer->CustType);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pReqTransfer->VerifyCertNoFlag);
        PURE_LOG("\tFeePayFlag [%c]", pReqTransfer->FeePayFlag);
        PURE_LOG("\tBankAccType [%c]", pReqTransfer->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pReqTransfer->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pReqTransfer->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pReqTransfer->SecuPwdFlag);
        PURE_LOG("\tTransferStatus [%c]", pReqTransfer->TransferStatus);
        PURE_LOG("\tTradeAmount [%.8lf]", pReqTransfer->TradeAmount);
        PURE_LOG("\tFutureFetchAmount [%.8lf]", pReqTransfer->FutureFetchAmount);
        PURE_LOG("\tCustFee [%.8lf]", pReqTransfer->CustFee);
        PURE_LOG("\tBrokerFee [%.8lf]", pReqTransfer->BrokerFee);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspFromFutureToBankByFuture>");
};

void CTraderSpi::OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    PURE_LOG("<OnRspQueryBankAccountMoneyByFuture>");
	if (pReqQueryAccount)
	{
        PURE_LOG("\tTradeCode [%s]", pReqQueryAccount->TradeCode);
        PURE_LOG("\tBankID [%s]", pReqQueryAccount->BankID);
        PURE_LOG("\tBankBranchID [%s]", pReqQueryAccount->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pReqQueryAccount->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pReqQueryAccount->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pReqQueryAccount->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pReqQueryAccount->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pReqQueryAccount->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pReqQueryAccount->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pReqQueryAccount->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pReqQueryAccount->IdentifiedCardNo);
        PURE_LOG("\tBankAccount [%s]", pReqQueryAccount->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pReqQueryAccount->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pReqQueryAccount->AccountID);
        PURE_LOG("\tPassword [%s]", pReqQueryAccount->Password);
        PURE_LOG("\tUserID [%s]", pReqQueryAccount->UserID);
        PURE_LOG("\tCurrencyID [%s]", pReqQueryAccount->CurrencyID);
        PURE_LOG("\tDigest [%s]", pReqQueryAccount->Digest);
        PURE_LOG("\tDeviceID [%s]", pReqQueryAccount->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pReqQueryAccount->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pReqQueryAccount->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pReqQueryAccount->OperNo);
        PURE_LOG("\tLongCustomerName [%s]", pReqQueryAccount->LongCustomerName);
        PURE_LOG("\tPlateSerial [%d]", pReqQueryAccount->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pReqQueryAccount->SessionID);
        PURE_LOG("\tFutureSerial [%d]", pReqQueryAccount->FutureSerial);
        PURE_LOG("\tInstallID [%d]", pReqQueryAccount->InstallID);
        PURE_LOG("\tRequestID [%d]", pReqQueryAccount->RequestID);
        PURE_LOG("\tTID [%d]", pReqQueryAccount->TID);
        PURE_LOG("\tLastFragment [%c]", pReqQueryAccount->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pReqQueryAccount->IdCardType);
        PURE_LOG("\tCustType [%c]", pReqQueryAccount->CustType);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pReqQueryAccount->VerifyCertNoFlag);
        PURE_LOG("\tBankAccType [%c]", pReqQueryAccount->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pReqQueryAccount->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pReqQueryAccount->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pReqQueryAccount->SecuPwdFlag);
	}
	if (pRspInfo)
	{
        TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
        PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
	}
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspQueryBankAccountMoneyByFuture>");
};

void CTraderSpi::OnRtnOpenAccountByBank(CThostFtdcOpenAccountField *pOpenAccount)
{
    PURE_LOG("<OnRtnOpenAccountByBank>");
	if (pOpenAccount)
	{
        PURE_LOG("\tTradeCode [%s]", pOpenAccount->TradeCode);
        PURE_LOG("\tBankID [%s]", pOpenAccount->BankID);
        PURE_LOG("\tBankBranchID [%s]", pOpenAccount->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pOpenAccount->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pOpenAccount->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pOpenAccount->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pOpenAccount->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pOpenAccount->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pOpenAccount->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pOpenAccount->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pOpenAccount->IdentifiedCardNo);
        PURE_LOG("\tCountryCode [%s]", pOpenAccount->CountryCode);
        PURE_LOG("\tAddress [%s]", pOpenAccount->Address);
        PURE_LOG("\tZipCode [%s]", pOpenAccount->ZipCode);
        PURE_LOG("\tTelephone [%s]", pOpenAccount->Telephone);
        PURE_LOG("\tMobilePhone [%s]", pOpenAccount->MobilePhone);
        PURE_LOG("\tFax [%s]", pOpenAccount->Fax);
        PURE_LOG("\tEMail [%s]", pOpenAccount->EMail);
        PURE_LOG("\tBankAccount [%s]", pOpenAccount->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pOpenAccount->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pOpenAccount->AccountID);
        PURE_LOG("\tPassword [%s]", pOpenAccount->Password);
        PURE_LOG("\tCurrencyID [%s]", pOpenAccount->CurrencyID);
        PURE_LOG("\tDigest [%s]", pOpenAccount->Digest);
        PURE_LOG("\tDeviceID [%s]", pOpenAccount->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pOpenAccount->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pOpenAccount->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pOpenAccount->OperNo);
        PURE_LOG("\tUserID [%s]", pOpenAccount->UserID);
        PURE_LOG("\tErrorMsg [%s]", pOpenAccount->ErrorMsg);
        PURE_LOG("\tLongCustomerName [%s]", pOpenAccount->LongCustomerName);
        PURE_LOG("\tPlateSerial [%d]", pOpenAccount->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pOpenAccount->SessionID);
        PURE_LOG("\tInstallID [%d]", pOpenAccount->InstallID);
        PURE_LOG("\tTID [%d]", pOpenAccount->TID);
        PURE_LOG("\tErrorID [%d]", pOpenAccount->ErrorID);
        PURE_LOG("\tLastFragment [%c]", pOpenAccount->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pOpenAccount->IdCardType);
        PURE_LOG("\tGender [%c]", pOpenAccount->Gender);
        PURE_LOG("\tCustType [%c]", pOpenAccount->CustType);
        PURE_LOG("\tMoneyAccountStatus [%c]", pOpenAccount->MoneyAccountStatus);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pOpenAccount->VerifyCertNoFlag);
        PURE_LOG("\tCashExchangeCode [%c]", pOpenAccount->CashExchangeCode);
        PURE_LOG("\tBankAccType [%c]", pOpenAccount->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pOpenAccount->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pOpenAccount->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pOpenAccount->SecuPwdFlag);
	}
    PURE_LOG("</OnRtnOpenAccountByBank>");
};
void CTraderSpi::OnRtnCancelAccountByBank(CThostFtdcCancelAccountField *pCancelAccount)
{
    PURE_LOG("<OnRtnCancelAccountByBank>");
	if (pCancelAccount)
	{
        PURE_LOG("\tTradeCode [%s]", pCancelAccount->TradeCode);
        PURE_LOG("\tBankID [%s]", pCancelAccount->BankID);
        PURE_LOG("\tBankBranchID [%s]", pCancelAccount->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pCancelAccount->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pCancelAccount->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pCancelAccount->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pCancelAccount->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pCancelAccount->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pCancelAccount->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pCancelAccount->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pCancelAccount->IdentifiedCardNo);
        PURE_LOG("\tCountryCode [%s]", pCancelAccount->CountryCode);
        PURE_LOG("\tAddress [%s]", pCancelAccount->Address);
        PURE_LOG("\tZipCode [%s]", pCancelAccount->ZipCode);
        PURE_LOG("\tTelephone [%s]", pCancelAccount->Telephone);
        PURE_LOG("\tMobilePhone [%s]", pCancelAccount->MobilePhone);
        PURE_LOG("\tFax [%s]", pCancelAccount->Fax);
        PURE_LOG("\tEMail [%s]", pCancelAccount->EMail);
        PURE_LOG("\tBankAccount [%s]", pCancelAccount->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pCancelAccount->BankPassWord);
        PURE_LOG("\tAccountID [%s]", pCancelAccount->AccountID);
        PURE_LOG("\tPassword [%s]", pCancelAccount->Password);
        PURE_LOG("\tCurrencyID [%s]", pCancelAccount->CurrencyID);
        PURE_LOG("\tDigest [%s]", pCancelAccount->Digest);
        PURE_LOG("\tDeviceID [%s]", pCancelAccount->DeviceID);
        PURE_LOG("\tBrokerIDByBank [%s]", pCancelAccount->BrokerIDByBank);
        PURE_LOG("\tBankSecuAcc [%s]", pCancelAccount->BankSecuAcc);
        PURE_LOG("\tOperNo [%s]", pCancelAccount->OperNo);
        PURE_LOG("\tUserID [%s]", pCancelAccount->UserID);
        PURE_LOG("\tErrorMsg [%s]", pCancelAccount->ErrorMsg);
        PURE_LOG("\tLongCustomerName [%s]", pCancelAccount->LongCustomerName);
        PURE_LOG("\tPlateSerial [%d]", pCancelAccount->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pCancelAccount->SessionID);
        PURE_LOG("\tInstallID [%d]", pCancelAccount->InstallID);
        PURE_LOG("\tTID [%d]", pCancelAccount->TID);
        PURE_LOG("\tErrorID [%d]", pCancelAccount->ErrorID);
        PURE_LOG("\tLastFragment [%c]", pCancelAccount->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pCancelAccount->IdCardType);
        PURE_LOG("\tGender [%c]", pCancelAccount->Gender);
        PURE_LOG("\tCustType [%c]", pCancelAccount->CustType);
        PURE_LOG("\tMoneyAccountStatus [%c]", pCancelAccount->MoneyAccountStatus);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pCancelAccount->VerifyCertNoFlag);
        PURE_LOG("\tCashExchangeCode [%c]", pCancelAccount->CashExchangeCode);
        PURE_LOG("\tBankAccType [%c]", pCancelAccount->BankAccType);
        PURE_LOG("\tBankSecuAccType [%c]", pCancelAccount->BankSecuAccType);
        PURE_LOG("\tBankPwdFlag [%c]", pCancelAccount->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pCancelAccount->SecuPwdFlag);
	}
    PURE_LOG("</OnRtnCancelAccountByBank>");
};
void CTraderSpi::OnRtnChangeAccountByBank(CThostFtdcChangeAccountField *pChangeAccount)
{
    PURE_LOG("<OnRtnChangeAccountByBank>");
	if (pChangeAccount)
	{
        PURE_LOG("\tTradeCode [%s]", pChangeAccount->TradeCode);
        PURE_LOG("\tBankID [%s]", pChangeAccount->BankID);
        PURE_LOG("\tBankBranchID [%s]", pChangeAccount->BankBranchID);
        PURE_LOG("\tBrokerID [%s]", pChangeAccount->BrokerID);
        PURE_LOG("\tBrokerBranchID [%s]", pChangeAccount->BrokerBranchID);
        PURE_LOG("\tTradeDate [%s]", pChangeAccount->TradeDate);
        PURE_LOG("\tTradeTime [%s]", pChangeAccount->TradeTime);
        PURE_LOG("\tBankSerial [%s]", pChangeAccount->BankSerial);
        PURE_LOG("\tTradingDay [%s]", pChangeAccount->TradingDay);
        PURE_LOG("\tCustomerName [%s]", pChangeAccount->CustomerName);
        PURE_LOG("\tIdentifiedCardNo [%s]", pChangeAccount->IdentifiedCardNo);
        PURE_LOG("\tCountryCode [%s]", pChangeAccount->CountryCode);
        PURE_LOG("\tAddress [%s]", pChangeAccount->Address);
        PURE_LOG("\tZipCode [%s]", pChangeAccount->ZipCode);
        PURE_LOG("\tTelephone [%s]", pChangeAccount->Telephone);
        PURE_LOG("\tMobilePhone [%s]", pChangeAccount->MobilePhone);
        PURE_LOG("\tFax [%s]", pChangeAccount->Fax);
        PURE_LOG("\tEMail [%s]", pChangeAccount->EMail);
        PURE_LOG("\tBankAccount [%s]", pChangeAccount->BankAccount);
        PURE_LOG("\tBankPassWord [%s]", pChangeAccount->BankPassWord);
        PURE_LOG("\tNewBankAccount [%s]", pChangeAccount->NewBankAccount);
        PURE_LOG("\tNewBankPassWord [%s]", pChangeAccount->NewBankPassWord);
        PURE_LOG("\tAccountID [%s]", pChangeAccount->AccountID);
        PURE_LOG("\tPassword [%s]", pChangeAccount->Password);
        PURE_LOG("\tCurrencyID [%s]", pChangeAccount->CurrencyID);
        PURE_LOG("\tBrokerIDByBank [%s]", pChangeAccount->BrokerIDByBank);
        PURE_LOG("\tDigest [%s]", pChangeAccount->Digest);
        PURE_LOG("\tErrorMsg [%s]", pChangeAccount->ErrorMsg);
        PURE_LOG("\tLongCustomerName [%s]", pChangeAccount->LongCustomerName);
        PURE_LOG("\tPlateSerial [%d]", pChangeAccount->PlateSerial);
        PURE_LOG("\tSessionID [%d]", pChangeAccount->SessionID);
        PURE_LOG("\tInstallID [%d]", pChangeAccount->InstallID);
        PURE_LOG("\tTID [%d]", pChangeAccount->TID);
        PURE_LOG("\tErrorID [%d]", pChangeAccount->ErrorID);
        PURE_LOG("\tLastFragment [%c]", pChangeAccount->LastFragment);
        PURE_LOG("\tIdCardType [%c]", pChangeAccount->IdCardType);
        PURE_LOG("\tGender [%c]", pChangeAccount->Gender);
        PURE_LOG("\tCustType [%c]", pChangeAccount->CustType);
        PURE_LOG("\tMoneyAccountStatus [%c]", pChangeAccount->MoneyAccountStatus);
        PURE_LOG("\tBankAccType [%c]", pChangeAccount->BankAccType);
        PURE_LOG("\tVerifyCertNoFlag [%c]", pChangeAccount->VerifyCertNoFlag);
        PURE_LOG("\tBankPwdFlag [%c]", pChangeAccount->BankPwdFlag);
        PURE_LOG("\tSecuPwdFlag [%c]", pChangeAccount->SecuPwdFlag);
	}
    PURE_LOG("</OnRtnChangeAccountByBank>");
};
