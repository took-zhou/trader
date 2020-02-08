//#include "stdafx.h"
#include <stdio.h>
#include "vector_TraderApi.h"
#include "define.h"


void Vector::OnFrontConnected()
{
	LOGDEMO("<OnFrontConnected>\n");
	LOGDEMO("</OnFrontConnected>\n");
};

void Vector::OnFrontDisconnected(int nReason)
{
	LOGDEMO("<OnFrontDisconnected>\n");
	LOGDEMO("\tnReason [%d]\n", nReason);
	LOGDEMO("</OnFrontDisconnected>\n");
}

void Vector::OnHeartBeatWarning(int nTimeLapse)
{
	LOGDEMO("<OnHeartBeatWarning>\n");
	LOGDEMO("\tnTimeLapse [%d]\n", nTimeLapse);
	LOGDEMO("</OnHeartBeatWarning>\n");
}

void Vector::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspAuthenticate>\n");
	if (pRspAuthenticateField)
	{
		LOGDEMO("\tBrokerID [%s]\n", pRspAuthenticateField->BrokerID);
		LOGDEMO("\tUserID [%s]\n", pRspAuthenticateField->UserID);
		LOGDEMO("\tUserProductInfo [%s]\n", pRspAuthenticateField->UserProductInfo);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspAuthenticate>\n");
};

void Vector::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspUserLogin>\n");
	if (pRspUserLogin)
	{
		LOGDEMO("\tTradingDay [%s]\n", pRspUserLogin->TradingDay);
		LOGDEMO("\tLoginTime [%s]\n", pRspUserLogin->LoginTime);
		LOGDEMO("\tBrokerID [%s]\n", pRspUserLogin->BrokerID);
		LOGDEMO("\tUserID [%s]\n", pRspUserLogin->UserID);
		LOGDEMO("\tSystemName [%s]\n", pRspUserLogin->SystemName);
		LOGDEMO("\tMaxOrderRef [%s]\n", pRspUserLogin->MaxOrderRef);
		LOGDEMO("\tSHFETime [%s]\n", pRspUserLogin->SHFETime);
		LOGDEMO("\tDCETime [%s]\n", pRspUserLogin->DCETime);
		LOGDEMO("\tCZCETime [%s]\n", pRspUserLogin->CZCETime);
		LOGDEMO("\tFFEXTime [%s]\n", pRspUserLogin->FFEXTime);
		LOGDEMO("\tINETime [%s]\n", pRspUserLogin->INETime);
		LOGDEMO("\tFrontID [%d]\n", pRspUserLogin->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pRspUserLogin->SessionID);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspUserLogin>\n");
};

void Vector::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspUserLogout>\n");
	if (pUserLogout)
	{
		LOGDEMO("\tBrokerID [%s]\n", pUserLogout->BrokerID);
		LOGDEMO("\tUserID [%s]\n", pUserLogout->UserID);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspUserLogout>\n");
};

void Vector::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspUserPasswordUpdate>\n");
	if (pUserPasswordUpdate)
	{
		LOGDEMO("\tBrokerID [%s]\n", pUserPasswordUpdate->BrokerID);
		LOGDEMO("\tUserID [%s]\n", pUserPasswordUpdate->UserID);
		LOGDEMO("\tOldPassword [%s]\n", pUserPasswordUpdate->OldPassword);
		LOGDEMO("\tNewPassword [%s]\n", pUserPasswordUpdate->NewPassword);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspUserPasswordUpdate>\n");
};

void Vector::OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspTradingAccountPasswordUpdate>\n");
	if (pTradingAccountPasswordUpdate)
	{
		LOGDEMO("\tBrokerID [%s]\n", pTradingAccountPasswordUpdate->BrokerID);
		LOGDEMO("\tAccountID [%s]\n", pTradingAccountPasswordUpdate->AccountID);
		LOGDEMO("\tOldPassword [%s]\n", pTradingAccountPasswordUpdate->OldPassword);
		LOGDEMO("\tNewPassword [%s]\n", pTradingAccountPasswordUpdate->NewPassword);
		LOGDEMO("\tCurrencyID [%s]\n", pTradingAccountPasswordUpdate->CurrencyID);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspTradingAccountPasswordUpdate>\n");
};

void Vector::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspOrderInsert>\n");
	if (pInputOrder)
	{
		LOGDEMO("\tBrokerID [%s]\n", pInputOrder->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInputOrder->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pInputOrder->InstrumentID);
		LOGDEMO("\tOrderRef [%s]\n", pInputOrder->OrderRef);
		LOGDEMO("\tUserID [%s]\n", pInputOrder->UserID);
		LOGDEMO("\tCombOffsetFlag [%s]\n", pInputOrder->CombOffsetFlag);
		LOGDEMO("\tCombHedgeFlag [%s]\n", pInputOrder->CombHedgeFlag);
		LOGDEMO("\tGTDDate [%s]\n", pInputOrder->GTDDate);
		LOGDEMO("\tBusinessUnit [%s]\n", pInputOrder->BusinessUnit);
		LOGDEMO("\tExchangeID [%s]\n", pInputOrder->ExchangeID);
		LOGDEMO("\tInvestUnitID [%s]\n", pInputOrder->InvestUnitID);
		LOGDEMO("\tAccountID [%s]\n", pInputOrder->AccountID);
		LOGDEMO("\tCurrencyID [%s]\n", pInputOrder->CurrencyID);
		LOGDEMO("\tClientID [%s]\n", pInputOrder->ClientID);
		LOGDEMO("\tIPAddress [%s]\n", pInputOrder->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pInputOrder->MacAddress);
		LOGDEMO("\tVolumeTotalOriginal [%d]\n", pInputOrder->VolumeTotalOriginal);
		LOGDEMO("\tMinVolume [%d]\n", pInputOrder->MinVolume);
		LOGDEMO("\tIsAutoSuspend [%d]\n", pInputOrder->IsAutoSuspend);
		LOGDEMO("\tRequestID [%d]\n", pInputOrder->RequestID);
		LOGDEMO("\tUserForceClose [%d]\n", pInputOrder->UserForceClose);
		LOGDEMO("\tIsSwapOrder [%d]\n", pInputOrder->IsSwapOrder);
		LOGDEMO("\tOrderPriceType [%c]\n", pInputOrder->OrderPriceType);
		LOGDEMO("\tDirection [%c]\n", pInputOrder->Direction);
		LOGDEMO("\tTimeCondition [%c]\n", pInputOrder->TimeCondition);
		LOGDEMO("\tVolumeCondition [%c]\n", pInputOrder->VolumeCondition);
		LOGDEMO("\tContingentCondition [%c]\n", pInputOrder->ContingentCondition);
		LOGDEMO("\tForceCloseReason [%c]\n", pInputOrder->ForceCloseReason);
		LOGDEMO("\tLimitPrice [%.8lf]\n", pInputOrder->LimitPrice);
		LOGDEMO("\tStopPrice [%.8lf]\n", pInputOrder->StopPrice);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspOrderInsert>\n");
};

void Vector::OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspParkedOrderInsert>\n");
	if (pParkedOrder)
	{
		LOGDEMO("\tBrokerID [%s]\n", pParkedOrder->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pParkedOrder->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pParkedOrder->InstrumentID);
		LOGDEMO("\tOrderRef [%s]\n", pParkedOrder->OrderRef);
		LOGDEMO("\tUserID [%s]\n", pParkedOrder->UserID);
		LOGDEMO("\tCombOffsetFlag [%s]\n", pParkedOrder->CombOffsetFlag);
		LOGDEMO("\tCombHedgeFlag [%s]\n", pParkedOrder->CombHedgeFlag);
		LOGDEMO("\tGTDDate [%s]\n", pParkedOrder->GTDDate);
		LOGDEMO("\tBusinessUnit [%s]\n", pParkedOrder->BusinessUnit);
		LOGDEMO("\tExchangeID [%s]\n", pParkedOrder->ExchangeID);
		LOGDEMO("\tParkedOrderID [%s]\n", pParkedOrder->ParkedOrderID);
		LOGDEMO("\tErrorMsg [%s]\n", pParkedOrder->ErrorMsg);
		LOGDEMO("\tAccountID [%s]\n", pParkedOrder->AccountID);
		LOGDEMO("\tCurrencyID [%s]\n", pParkedOrder->CurrencyID);
		LOGDEMO("\tClientID [%s]\n", pParkedOrder->ClientID);
		LOGDEMO("\tInvestUnitID [%s]\n", pParkedOrder->InvestUnitID);
		LOGDEMO("\tIPAddress [%s]\n", pParkedOrder->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pParkedOrder->MacAddress);
		LOGDEMO("\tVolumeTotalOriginal [%d]\n", pParkedOrder->VolumeTotalOriginal);
		LOGDEMO("\tMinVolume [%d]\n", pParkedOrder->MinVolume);
		LOGDEMO("\tIsAutoSuspend [%d]\n", pParkedOrder->IsAutoSuspend);
		LOGDEMO("\tRequestID [%d]\n", pParkedOrder->RequestID);
		LOGDEMO("\tUserForceClose [%d]\n", pParkedOrder->UserForceClose);
		LOGDEMO("\tErrorID [%d]\n", pParkedOrder->ErrorID);
		LOGDEMO("\tIsSwapOrder [%d]\n", pParkedOrder->IsSwapOrder);
		LOGDEMO("\tOrderPriceType [%c]\n", pParkedOrder->OrderPriceType);
		LOGDEMO("\tDirection [%c]\n", pParkedOrder->Direction);
		LOGDEMO("\tTimeCondition [%c]\n", pParkedOrder->TimeCondition);
		LOGDEMO("\tVolumeCondition [%c]\n", pParkedOrder->VolumeCondition);
		LOGDEMO("\tContingentCondition [%c]\n", pParkedOrder->ContingentCondition);
		LOGDEMO("\tForceCloseReason [%c]\n", pParkedOrder->ForceCloseReason);
		LOGDEMO("\tUserType [%c]\n", pParkedOrder->UserType);
		LOGDEMO("\tStatus [%c]\n", pParkedOrder->Status);
		LOGDEMO("\tLimitPrice [%.8lf]\n", pParkedOrder->LimitPrice);
		LOGDEMO("\tStopPrice [%.8lf]\n", pParkedOrder->StopPrice);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspParkedOrderInsert>\n");
};

void Vector::OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspParkedOrderAction>\n");
	if (pParkedOrderAction)
	{
		LOGDEMO("\tBrokerID [%s]\n", pParkedOrderAction->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pParkedOrderAction->InvestorID);
		LOGDEMO("\tOrderRef [%s]\n", pParkedOrderAction->OrderRef);
		LOGDEMO("\tExchangeID [%s]\n", pParkedOrderAction->ExchangeID);
		LOGDEMO("\tOrderSysID [%s]\n", pParkedOrderAction->OrderSysID);
		LOGDEMO("\tUserID [%s]\n", pParkedOrderAction->UserID);
		LOGDEMO("\tInstrumentID [%s]\n", pParkedOrderAction->InstrumentID);
		LOGDEMO("\tParkedOrderActionID [%s]\n", pParkedOrderAction->ParkedOrderActionID);
		LOGDEMO("\tErrorMsg [%s]\n", pParkedOrderAction->ErrorMsg);
		LOGDEMO("\tInvestUnitID [%s]\n", pParkedOrderAction->InvestUnitID);
		LOGDEMO("\tIPAddress [%s]\n", pParkedOrderAction->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pParkedOrderAction->MacAddress);
		LOGDEMO("\tOrderActionRef [%d]\n", pParkedOrderAction->OrderActionRef);
		LOGDEMO("\tRequestID [%d]\n", pParkedOrderAction->RequestID);
		LOGDEMO("\tFrontID [%d]\n", pParkedOrderAction->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pParkedOrderAction->SessionID);
		LOGDEMO("\tVolumeChange [%d]\n", pParkedOrderAction->VolumeChange);
		LOGDEMO("\tErrorID [%d]\n", pParkedOrderAction->ErrorID);
		LOGDEMO("\tActionFlag [%c]\n", pParkedOrderAction->ActionFlag);
		LOGDEMO("\tUserType [%c]\n", pParkedOrderAction->UserType);
		LOGDEMO("\tStatus [%c]\n", pParkedOrderAction->Status);
		LOGDEMO("\tLimitPrice [%.8lf]\n", pParkedOrderAction->LimitPrice);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspParkedOrderAction>\n");
};

void Vector::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspOrderAction>\n");
	if (pInputOrderAction)
	{
		LOGDEMO("\tBrokerID [%s]\n", pInputOrderAction->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInputOrderAction->InvestorID);
		LOGDEMO("\tOrderRef [%s]\n", pInputOrderAction->OrderRef);
		LOGDEMO("\tExchangeID [%s]\n", pInputOrderAction->ExchangeID);
		LOGDEMO("\tOrderSysID [%s]\n", pInputOrderAction->OrderSysID);
		LOGDEMO("\tUserID [%s]\n", pInputOrderAction->UserID);
		LOGDEMO("\tInstrumentID [%s]\n", pInputOrderAction->InstrumentID);
		LOGDEMO("\tInvestUnitID [%s]\n", pInputOrderAction->InvestUnitID);
		LOGDEMO("\tIPAddress [%s]\n", pInputOrderAction->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pInputOrderAction->MacAddress);
		LOGDEMO("\tOrderActionRef [%d]\n", pInputOrderAction->OrderActionRef);
		LOGDEMO("\tRequestID [%d]\n", pInputOrderAction->RequestID);
		LOGDEMO("\tFrontID [%d]\n", pInputOrderAction->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pInputOrderAction->SessionID);
		LOGDEMO("\tVolumeChange [%d]\n", pInputOrderAction->VolumeChange);
		LOGDEMO("\tActionFlag [%c]\n", pInputOrderAction->ActionFlag);
		LOGDEMO("\tLimitPrice [%.8lf]\n", pInputOrderAction->LimitPrice);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspOrderAction>\n");
};

void Vector::OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQueryMaxOrderVolume>\n");
	if (pQueryMaxOrderVolume)
	{
		LOGDEMO("\tBrokerID [%s]\n", pQueryMaxOrderVolume->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pQueryMaxOrderVolume->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pQueryMaxOrderVolume->InstrumentID);
		LOGDEMO("\tMaxVolume [%d]\n", pQueryMaxOrderVolume->MaxVolume);
		LOGDEMO("\tDirection [%c]\n", pQueryMaxOrderVolume->Direction);
		LOGDEMO("\tOffsetFlag [%c]\n", pQueryMaxOrderVolume->OffsetFlag);
		LOGDEMO("\tHedgeFlag [%c]\n", pQueryMaxOrderVolume->HedgeFlag);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQueryMaxOrderVolume>\n");
};

void Vector::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspSettlementInfoConfirm>\n");
	if (pSettlementInfoConfirm)
	{
		LOGDEMO("\tBrokerID [%s]\n", pSettlementInfoConfirm->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pSettlementInfoConfirm->InvestorID);
		LOGDEMO("\tConfirmDate [%s]\n", pSettlementInfoConfirm->ConfirmDate);
		LOGDEMO("\tConfirmTime [%s]\n", pSettlementInfoConfirm->ConfirmTime);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspSettlementInfoConfirm>\n");
};

void Vector::OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspRemoveParkedOrder>\n");
	if (pRemoveParkedOrder)
	{
		LOGDEMO("\tBrokerID [%s]\n", pRemoveParkedOrder->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pRemoveParkedOrder->InvestorID);
		LOGDEMO("\tParkedOrderID [%s]\n", pRemoveParkedOrder->ParkedOrderID);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspRemoveParkedOrder>\n");
};

void Vector::OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspRemoveParkedOrderAction>\n");
	if (pRemoveParkedOrderAction)
	{
		LOGDEMO("\tBrokerID [%s]\n", pRemoveParkedOrderAction->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pRemoveParkedOrderAction->InvestorID);
		LOGDEMO("\tParkedOrderActionID [%s]\n", pRemoveParkedOrderAction->ParkedOrderActionID);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspRemoveParkedOrderAction>\n");
};

void Vector::OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspExecOrderInsert>\n");
	if (pInputExecOrder)
	{
		LOGDEMO("\tBrokerID [%s]\n", pInputExecOrder->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInputExecOrder->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pInputExecOrder->InstrumentID);
		LOGDEMO("\tExecOrderRef [%s]\n", pInputExecOrder->ExecOrderRef);
		LOGDEMO("\tUserID [%s]\n", pInputExecOrder->UserID);
		LOGDEMO("\tBusinessUnit [%s]\n", pInputExecOrder->BusinessUnit);
		LOGDEMO("\tExchangeID [%s]\n", pInputExecOrder->ExchangeID);
		LOGDEMO("\tInvestUnitID [%s]\n", pInputExecOrder->InvestUnitID);
		LOGDEMO("\tAccountID [%s]\n", pInputExecOrder->AccountID);
		LOGDEMO("\tCurrencyID [%s]\n", pInputExecOrder->CurrencyID);
		LOGDEMO("\tClientID [%s]\n", pInputExecOrder->ClientID);
		LOGDEMO("\tIPAddress [%s]\n", pInputExecOrder->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pInputExecOrder->MacAddress);
		LOGDEMO("\tVolume [%d]\n", pInputExecOrder->Volume);
		LOGDEMO("\tRequestID [%d]\n", pInputExecOrder->RequestID);
		LOGDEMO("\tOffsetFlag [%c]\n", pInputExecOrder->OffsetFlag);
		LOGDEMO("\tHedgeFlag [%c]\n", pInputExecOrder->HedgeFlag);
		LOGDEMO("\tActionType [%c]\n", pInputExecOrder->ActionType);
		LOGDEMO("\tPosiDirection [%c]\n", pInputExecOrder->PosiDirection);
		LOGDEMO("\tReservePositionFlag [%c]\n", pInputExecOrder->ReservePositionFlag);
		LOGDEMO("\tCloseFlag [%c]\n", pInputExecOrder->CloseFlag);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspExecOrderInsert>\n");
};

void Vector::OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspExecOrderAction>\n");
	if (pInputExecOrderAction)
	{
		LOGDEMO("\tBrokerID [%s]\n", pInputExecOrderAction->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInputExecOrderAction->InvestorID);
		LOGDEMO("\tExecOrderRef [%s]\n", pInputExecOrderAction->ExecOrderRef);
		LOGDEMO("\tExchangeID [%s]\n", pInputExecOrderAction->ExchangeID);
		LOGDEMO("\tExecOrderSysID [%s]\n", pInputExecOrderAction->ExecOrderSysID);
		LOGDEMO("\tUserID [%s]\n", pInputExecOrderAction->UserID);
		LOGDEMO("\tInstrumentID [%s]\n", pInputExecOrderAction->InstrumentID);
		LOGDEMO("\tInvestUnitID [%s]\n", pInputExecOrderAction->InvestUnitID);
		LOGDEMO("\tIPAddress [%s]\n", pInputExecOrderAction->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pInputExecOrderAction->MacAddress);
		LOGDEMO("\tExecOrderActionRef [%d]\n", pInputExecOrderAction->ExecOrderActionRef);
		LOGDEMO("\tRequestID [%d]\n", pInputExecOrderAction->RequestID);
		LOGDEMO("\tFrontID [%d]\n", pInputExecOrderAction->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pInputExecOrderAction->SessionID);
		LOGDEMO("\tActionFlag [%c]\n", pInputExecOrderAction->ActionFlag);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspExecOrderAction>\n");
};

void Vector::OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspForQuoteInsert>\n");
	if (pInputForQuote)
	{
		LOGDEMO("\tBrokerID [%s]\n", pInputForQuote->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInputForQuote->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pInputForQuote->InstrumentID);
		LOGDEMO("\tForQuoteRef [%s]\n", pInputForQuote->ForQuoteRef);
		LOGDEMO("\tUserID [%s]\n", pInputForQuote->UserID);
		LOGDEMO("\tExchangeID [%s]\n", pInputForQuote->ExchangeID);
		LOGDEMO("\tInvestUnitID [%s]\n", pInputForQuote->InvestUnitID);
		LOGDEMO("\tIPAddress [%s]\n", pInputForQuote->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pInputForQuote->MacAddress);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspForQuoteInsert>\n");
};

void Vector::OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQuoteInsert>\n");
	if (pInputQuote)
	{
		LOGDEMO("\tBrokerID [%s]\n", pInputQuote->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInputQuote->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pInputQuote->InstrumentID);
		LOGDEMO("\tQuoteRef [%s]\n", pInputQuote->QuoteRef);
		LOGDEMO("\tUserID [%s]\n", pInputQuote->UserID);
		LOGDEMO("\tBusinessUnit [%s]\n", pInputQuote->BusinessUnit);
		LOGDEMO("\tAskOrderRef [%s]\n", pInputQuote->AskOrderRef);
		LOGDEMO("\tBidOrderRef [%s]\n", pInputQuote->BidOrderRef);
		LOGDEMO("\tForQuoteSysID [%s]\n", pInputQuote->ForQuoteSysID);
		LOGDEMO("\tExchangeID [%s]\n", pInputQuote->ExchangeID);
		LOGDEMO("\tInvestUnitID [%s]\n", pInputQuote->InvestUnitID);
		LOGDEMO("\tClientID [%s]\n", pInputQuote->ClientID);
		LOGDEMO("\tIPAddress [%s]\n", pInputQuote->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pInputQuote->MacAddress);
		LOGDEMO("\tAskVolume [%d]\n", pInputQuote->AskVolume);
		LOGDEMO("\tBidVolume [%d]\n", pInputQuote->BidVolume);
		LOGDEMO("\tRequestID [%d]\n", pInputQuote->RequestID);
		LOGDEMO("\tAskOffsetFlag [%c]\n", pInputQuote->AskOffsetFlag);
		LOGDEMO("\tBidOffsetFlag [%c]\n", pInputQuote->BidOffsetFlag);
		LOGDEMO("\tAskHedgeFlag [%c]\n", pInputQuote->AskHedgeFlag);
		LOGDEMO("\tBidHedgeFlag [%c]\n", pInputQuote->BidHedgeFlag);
		LOGDEMO("\tAskPrice [%.8lf]\n", pInputQuote->AskPrice);
		LOGDEMO("\tBidPrice [%.8lf]\n", pInputQuote->BidPrice);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQuoteInsert>\n");
};

void Vector::OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQuoteAction>\n");
	if (pInputQuoteAction)
	{
		LOGDEMO("\tBrokerID [%s]\n", pInputQuoteAction->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInputQuoteAction->InvestorID);
		LOGDEMO("\tQuoteRef [%s]\n", pInputQuoteAction->QuoteRef);
		LOGDEMO("\tExchangeID [%s]\n", pInputQuoteAction->ExchangeID);
		LOGDEMO("\tQuoteSysID [%s]\n", pInputQuoteAction->QuoteSysID);
		LOGDEMO("\tUserID [%s]\n", pInputQuoteAction->UserID);
		LOGDEMO("\tInstrumentID [%s]\n", pInputQuoteAction->InstrumentID);
		LOGDEMO("\tInvestUnitID [%s]\n", pInputQuoteAction->InvestUnitID);
		LOGDEMO("\tClientID [%s]\n", pInputQuoteAction->ClientID);
		LOGDEMO("\tIPAddress [%s]\n", pInputQuoteAction->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pInputQuoteAction->MacAddress);
		LOGDEMO("\tQuoteActionRef [%d]\n", pInputQuoteAction->QuoteActionRef);
		LOGDEMO("\tRequestID [%d]\n", pInputQuoteAction->RequestID);
		LOGDEMO("\tFrontID [%d]\n", pInputQuoteAction->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pInputQuoteAction->SessionID);
		LOGDEMO("\tActionFlag [%c]\n", pInputQuoteAction->ActionFlag);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQuoteAction>\n");
};

void Vector::OnRspBatchOrderAction(CThostFtdcInputBatchOrderActionField *pInputBatchOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspBatchOrderAction>\n");
	if (pInputBatchOrderAction)
	{
		LOGDEMO("\tBrokerID [%s]\n", pInputBatchOrderAction->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInputBatchOrderAction->InvestorID);
		LOGDEMO("\tExchangeID [%s]\n", pInputBatchOrderAction->ExchangeID);
		LOGDEMO("\tUserID [%s]\n", pInputBatchOrderAction->UserID);
		LOGDEMO("\tInvestUnitID [%s]\n", pInputBatchOrderAction->InvestUnitID);
		LOGDEMO("\tIPAddress [%s]\n", pInputBatchOrderAction->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pInputBatchOrderAction->MacAddress);
		LOGDEMO("\tOrderActionRef [%d]\n", pInputBatchOrderAction->OrderActionRef);
		LOGDEMO("\tRequestID [%d]\n", pInputBatchOrderAction->RequestID);
		LOGDEMO("\tFrontID [%d]\n", pInputBatchOrderAction->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pInputBatchOrderAction->SessionID);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspBatchOrderAction>\n");
};

void Vector::OnRspCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspCombActionInsert>\n");
	if (pInputCombAction)
	{
		LOGDEMO("\tBrokerID [%s]\n", pInputCombAction->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInputCombAction->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pInputCombAction->InstrumentID);
		LOGDEMO("\tCombActionRef [%s]\n", pInputCombAction->CombActionRef);
		LOGDEMO("\tUserID [%s]\n", pInputCombAction->UserID);
		LOGDEMO("\tExchangeID [%s]\n", pInputCombAction->ExchangeID);
		LOGDEMO("\tIPAddress [%s]\n", pInputCombAction->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pInputCombAction->MacAddress);
		LOGDEMO("\tVolume [%d]\n", pInputCombAction->Volume);
		LOGDEMO("\tDirection [%c]\n", pInputCombAction->Direction);
		LOGDEMO("\tCombDirection [%c]\n", pInputCombAction->CombDirection);
		LOGDEMO("\tHedgeFlag [%c]\n", pInputCombAction->HedgeFlag);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspCombActionInsert>\n");
};

void Vector::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryOrder>\n");
	if (pOrder)
	{
		LOGDEMO("\tBrokerID [%s]\n", pOrder->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pOrder->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pOrder->InstrumentID);
		LOGDEMO("\tOrderRef [%s]\n", pOrder->OrderRef);
		LOGDEMO("\tUserID [%s]\n", pOrder->UserID);
		LOGDEMO("\tCombOffsetFlag [%s]\n", pOrder->CombOffsetFlag);
		LOGDEMO("\tCombHedgeFlag [%s]\n", pOrder->CombHedgeFlag);
		LOGDEMO("\tGTDDate [%s]\n", pOrder->GTDDate);
		LOGDEMO("\tBusinessUnit [%s]\n", pOrder->BusinessUnit);
		LOGDEMO("\tOrderLocalID [%s]\n", pOrder->OrderLocalID);
		LOGDEMO("\tExchangeID [%s]\n", pOrder->ExchangeID);
		LOGDEMO("\tParticipantID [%s]\n", pOrder->ParticipantID);
		LOGDEMO("\tClientID [%s]\n", pOrder->ClientID);
		LOGDEMO("\tExchangeInstID [%s]\n", pOrder->ExchangeInstID);
		LOGDEMO("\tTraderID [%s]\n", pOrder->TraderID);
		LOGDEMO("\tTradingDay [%s]\n", pOrder->TradingDay);
		LOGDEMO("\tOrderSysID [%s]\n", pOrder->OrderSysID);
		LOGDEMO("\tInsertDate [%s]\n", pOrder->InsertDate);
		LOGDEMO("\tInsertTime [%s]\n", pOrder->InsertTime);
		LOGDEMO("\tActiveTime [%s]\n", pOrder->ActiveTime);
		LOGDEMO("\tSuspendTime [%s]\n", pOrder->SuspendTime);
		LOGDEMO("\tUpdateTime [%s]\n", pOrder->UpdateTime);
		LOGDEMO("\tCancelTime [%s]\n", pOrder->CancelTime);
		LOGDEMO("\tActiveTraderID [%s]\n", pOrder->ActiveTraderID);
		LOGDEMO("\tClearingPartID [%s]\n", pOrder->ClearingPartID);
		LOGDEMO("\tUserProductInfo [%s]\n", pOrder->UserProductInfo);
		LOGDEMO("\tStatusMsg [%s]\n", pOrder->StatusMsg);
		LOGDEMO("\tActiveUserID [%s]\n", pOrder->ActiveUserID);
		LOGDEMO("\tRelativeOrderSysID [%s]\n", pOrder->RelativeOrderSysID);
		LOGDEMO("\tBranchID [%s]\n", pOrder->BranchID);
		LOGDEMO("\tInvestUnitID [%s]\n", pOrder->InvestUnitID);
		LOGDEMO("\tAccountID [%s]\n", pOrder->AccountID);
		LOGDEMO("\tCurrencyID [%s]\n", pOrder->CurrencyID);
		LOGDEMO("\tIPAddress [%s]\n", pOrder->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pOrder->MacAddress);
		LOGDEMO("\tVolumeTotalOriginal [%d]\n", pOrder->VolumeTotalOriginal);
		LOGDEMO("\tMinVolume [%d]\n", pOrder->MinVolume);
		LOGDEMO("\tIsAutoSuspend [%d]\n", pOrder->IsAutoSuspend);
		LOGDEMO("\tRequestID [%d]\n", pOrder->RequestID);
		LOGDEMO("\tInstallID [%d]\n", pOrder->InstallID);
		LOGDEMO("\tNotifySequence [%d]\n", pOrder->NotifySequence);
		LOGDEMO("\tSettlementID [%d]\n", pOrder->SettlementID);
		LOGDEMO("\tVolumeTraded [%d]\n", pOrder->VolumeTraded);
		LOGDEMO("\tVolumeTotal [%d]\n", pOrder->VolumeTotal);
		LOGDEMO("\tSequenceNo [%d]\n", pOrder->SequenceNo);
		LOGDEMO("\tFrontID [%d]\n", pOrder->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pOrder->SessionID);
		LOGDEMO("\tUserForceClose [%d]\n", pOrder->UserForceClose);
		LOGDEMO("\tBrokerOrderSeq [%d]\n", pOrder->BrokerOrderSeq);
		LOGDEMO("\tZCETotalTradedVolume [%d]\n", pOrder->ZCETotalTradedVolume);
		LOGDEMO("\tIsSwapOrder [%d]\n", pOrder->IsSwapOrder);
		LOGDEMO("\tOrderPriceType [%c]\n", pOrder->OrderPriceType);
		LOGDEMO("\tDirection [%c]\n", pOrder->Direction);
		LOGDEMO("\tTimeCondition [%c]\n", pOrder->TimeCondition);
		LOGDEMO("\tVolumeCondition [%c]\n", pOrder->VolumeCondition);
		LOGDEMO("\tContingentCondition [%c]\n", pOrder->ContingentCondition);
		LOGDEMO("\tForceCloseReason [%c]\n", pOrder->ForceCloseReason);
		LOGDEMO("\tOrderSubmitStatus [%c]\n", pOrder->OrderSubmitStatus);
		LOGDEMO("\tOrderSource [%c]\n", pOrder->OrderSource);
		LOGDEMO("\tOrderStatus [%c]\n", pOrder->OrderStatus);
		LOGDEMO("\tOrderType [%c]\n", pOrder->OrderType);
		LOGDEMO("\tLimitPrice [%.8lf]\n", pOrder->LimitPrice);
		LOGDEMO("\tStopPrice [%.8lf]\n", pOrder->StopPrice);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryOrder>\n");
};

void Vector::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryTrade>\n");
	if (pTrade)
	{
		LOGDEMO("\tBrokerID [%s]\n", pTrade->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pTrade->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pTrade->InstrumentID);
		LOGDEMO("\tOrderRef [%s]\n", pTrade->OrderRef);
		LOGDEMO("\tUserID [%s]\n", pTrade->UserID);
		LOGDEMO("\tExchangeID [%s]\n", pTrade->ExchangeID);
		LOGDEMO("\tTradeID [%s]\n", pTrade->TradeID);
		LOGDEMO("\tOrderSysID [%s]\n", pTrade->OrderSysID);
		LOGDEMO("\tParticipantID [%s]\n", pTrade->ParticipantID);
		LOGDEMO("\tClientID [%s]\n", pTrade->ClientID);
		LOGDEMO("\tExchangeInstID [%s]\n", pTrade->ExchangeInstID);
		LOGDEMO("\tTradeDate [%s]\n", pTrade->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pTrade->TradeTime);
		LOGDEMO("\tTraderID [%s]\n", pTrade->TraderID);
		LOGDEMO("\tOrderLocalID [%s]\n", pTrade->OrderLocalID);
		LOGDEMO("\tClearingPartID [%s]\n", pTrade->ClearingPartID);
		LOGDEMO("\tBusinessUnit [%s]\n", pTrade->BusinessUnit);
		LOGDEMO("\tTradingDay [%s]\n", pTrade->TradingDay);
		LOGDEMO("\tVolume [%d]\n", pTrade->Volume);
		LOGDEMO("\tSequenceNo [%d]\n", pTrade->SequenceNo);
		LOGDEMO("\tSettlementID [%d]\n", pTrade->SettlementID);
		LOGDEMO("\tBrokerOrderSeq [%d]\n", pTrade->BrokerOrderSeq);
		LOGDEMO("\tDirection [%c]\n", pTrade->Direction);
		LOGDEMO("\tTradingRole [%c]\n", pTrade->TradingRole);
		LOGDEMO("\tOffsetFlag [%c]\n", pTrade->OffsetFlag);
		LOGDEMO("\tHedgeFlag [%c]\n", pTrade->HedgeFlag);
		LOGDEMO("\tTradeType [%c]\n", pTrade->TradeType);
		LOGDEMO("\tPriceSource [%c]\n", pTrade->PriceSource);
		LOGDEMO("\tTradeSource [%c]\n", pTrade->TradeSource);
		LOGDEMO("\tPrice [%.8lf]\n", pTrade->Price);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryTrade>\n");
};

void Vector::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryInvestorPosition>\n");
	if (pInvestorPosition)
	{
		LOGDEMO("\tInstrumentID [%s]\n", pInvestorPosition->InstrumentID);
		LOGDEMO("\tBrokerID [%s]\n", pInvestorPosition->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInvestorPosition->InvestorID);
		LOGDEMO("\tTradingDay [%s]\n", pInvestorPosition->TradingDay);
		LOGDEMO("\tYdPosition [%d]\n", pInvestorPosition->YdPosition);
		LOGDEMO("\tPosition [%d]\n", pInvestorPosition->Position);
		LOGDEMO("\tLongFrozen [%d]\n", pInvestorPosition->LongFrozen);
		LOGDEMO("\tShortFrozen [%d]\n", pInvestorPosition->ShortFrozen);
		LOGDEMO("\tOpenVolume [%d]\n", pInvestorPosition->OpenVolume);
		LOGDEMO("\tCloseVolume [%d]\n", pInvestorPosition->CloseVolume);
		LOGDEMO("\tSettlementID [%d]\n", pInvestorPosition->SettlementID);
		LOGDEMO("\tCombPosition [%d]\n", pInvestorPosition->CombPosition);
		LOGDEMO("\tCombLongFrozen [%d]\n", pInvestorPosition->CombLongFrozen);
		LOGDEMO("\tCombShortFrozen [%d]\n", pInvestorPosition->CombShortFrozen);
		LOGDEMO("\tTodayPosition [%d]\n", pInvestorPosition->TodayPosition);
		LOGDEMO("\tStrikeFrozen [%d]\n", pInvestorPosition->StrikeFrozen);
		LOGDEMO("\tAbandonFrozen [%d]\n", pInvestorPosition->AbandonFrozen);
		LOGDEMO("\tPosiDirection [%c]\n", pInvestorPosition->PosiDirection);
		LOGDEMO("\tHedgeFlag [%c]\n", pInvestorPosition->HedgeFlag);
		LOGDEMO("\tPositionDate [%c]\n", pInvestorPosition->PositionDate);
		LOGDEMO("\tLongFrozenAmount [%.8lf]\n", pInvestorPosition->LongFrozenAmount);
		LOGDEMO("\tShortFrozenAmount [%.8lf]\n", pInvestorPosition->ShortFrozenAmount);
		LOGDEMO("\tOpenAmount [%.8lf]\n", pInvestorPosition->OpenAmount);
		LOGDEMO("\tCloseAmount [%.8lf]\n", pInvestorPosition->CloseAmount);
		LOGDEMO("\tPositionCost [%.8lf]\n", pInvestorPosition->PositionCost);
		LOGDEMO("\tPreMargin [%.8lf]\n", pInvestorPosition->PreMargin);
		LOGDEMO("\tUseMargin [%.8lf]\n", pInvestorPosition->UseMargin);
		LOGDEMO("\tFrozenMargin [%.8lf]\n", pInvestorPosition->FrozenMargin);
		LOGDEMO("\tFrozenCash [%.8lf]\n", pInvestorPosition->FrozenCash);
		LOGDEMO("\tFrozenCommission [%.8lf]\n", pInvestorPosition->FrozenCommission);
		LOGDEMO("\tCashIn [%.8lf]\n", pInvestorPosition->CashIn);
		LOGDEMO("\tCommission [%.8lf]\n", pInvestorPosition->Commission);
		LOGDEMO("\tCloseProfit [%.8lf]\n", pInvestorPosition->CloseProfit);
		LOGDEMO("\tPositionProfit [%.8lf]\n", pInvestorPosition->PositionProfit);
		LOGDEMO("\tPreSettlementPrice [%.8lf]\n", pInvestorPosition->PreSettlementPrice);
		LOGDEMO("\tSettlementPrice [%.8lf]\n", pInvestorPosition->SettlementPrice);
		LOGDEMO("\tOpenCost [%.8lf]\n", pInvestorPosition->OpenCost);
		LOGDEMO("\tExchangeMargin [%.8lf]\n", pInvestorPosition->ExchangeMargin);
		LOGDEMO("\tCloseProfitByDate [%.8lf]\n", pInvestorPosition->CloseProfitByDate);
		LOGDEMO("\tCloseProfitByTrade [%.8lf]\n", pInvestorPosition->CloseProfitByTrade);
		LOGDEMO("\tMarginRateByMoney [%.8lf]\n", pInvestorPosition->MarginRateByMoney);
		LOGDEMO("\tMarginRateByVolume [%.8lf]\n", pInvestorPosition->MarginRateByVolume);
		LOGDEMO("\tStrikeFrozenAmount [%.8lf]\n", pInvestorPosition->StrikeFrozenAmount);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryInvestorPosition>\n");
};

void Vector::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryTradingAccount>\n");
	if (pTradingAccount)
	{
		LOGDEMO("\tBrokerID [%s]\n", pTradingAccount->BrokerID);
		LOGDEMO("\tAccountID [%s]\n", pTradingAccount->AccountID);
		LOGDEMO("\tTradingDay [%s]\n", pTradingAccount->TradingDay);
		LOGDEMO("\tCurrencyID [%s]\n", pTradingAccount->CurrencyID);
		LOGDEMO("\tSettlementID [%d]\n", pTradingAccount->SettlementID);
		LOGDEMO("\tPreMortgage [%.8lf]\n", pTradingAccount->PreMortgage);
		LOGDEMO("\tPreCredit [%.8lf]\n", pTradingAccount->PreCredit);
		LOGDEMO("\tPreDeposit [%.8lf]\n", pTradingAccount->PreDeposit);
		LOGDEMO("\tPreBalance [%.8lf]\n", pTradingAccount->PreBalance);
		LOGDEMO("\tPreMargin [%.8lf]\n", pTradingAccount->PreMargin);
		LOGDEMO("\tInterestBase [%.8lf]\n", pTradingAccount->InterestBase);
		LOGDEMO("\tInterest [%.8lf]\n", pTradingAccount->Interest);
		LOGDEMO("\tDeposit [%.8lf]\n", pTradingAccount->Deposit);
		LOGDEMO("\tWithdraw [%.8lf]\n", pTradingAccount->Withdraw);
		LOGDEMO("\tFrozenMargin [%.8lf]\n", pTradingAccount->FrozenMargin);
		LOGDEMO("\tFrozenCash [%.8lf] 冻结的资金\n", pTradingAccount->FrozenCash);
		LOGDEMO("\tFrozenCommission [%.8lf] 冻结的手续费\n", pTradingAccount->FrozenCommission);
		LOGDEMO("\tCurrMargin [%.8lf] 当前保证金总额\n", pTradingAccount->CurrMargin);
		LOGDEMO("\tCashIn [%.8lf]\n", pTradingAccount->CashIn);
		LOGDEMO("\tCommission [%.8lf] 手续费\n", pTradingAccount->Commission);
		LOGDEMO("\tCloseProfit [%.8lf]\n", pTradingAccount->CloseProfit);
		LOGDEMO("\tPositionProfit [%.8lf]\n", pTradingAccount->PositionProfit);
		LOGDEMO("\tBalance [%.8lf]\n", pTradingAccount->Balance);
		LOGDEMO("\tAvailable [%.8lf]\n", pTradingAccount->Available);
		LOGDEMO("\tWithdrawQuota [%.8lf]\n", pTradingAccount->WithdrawQuota);
		LOGDEMO("\tReserve [%.8lf]\n", pTradingAccount->Reserve);
		LOGDEMO("\tCredit [%.8lf]\n", pTradingAccount->Credit);
		LOGDEMO("\tMortgage [%.8lf]\n", pTradingAccount->Mortgage);
		LOGDEMO("\tExchangeMargin [%.8lf]\n", pTradingAccount->ExchangeMargin);
		LOGDEMO("\tDeliveryMargin [%.8lf]\n", pTradingAccount->DeliveryMargin);
		LOGDEMO("\tExchangeDeliveryMargin [%.8lf]\n", pTradingAccount->ExchangeDeliveryMargin);
		LOGDEMO("\tReserveBalance [%.8lf]\n", pTradingAccount->ReserveBalance);
		LOGDEMO("\tPreFundMortgageIn [%.8lf]\n", pTradingAccount->PreFundMortgageIn);
		LOGDEMO("\tPreFundMortgageOut [%.8lf]\n", pTradingAccount->PreFundMortgageOut);
		LOGDEMO("\tFundMortgageIn [%.8lf]\n", pTradingAccount->FundMortgageIn);
		LOGDEMO("\tFundMortgageOut [%.8lf]\n", pTradingAccount->FundMortgageOut);
		LOGDEMO("\tFundMortgageAvailable [%.8lf]\n", pTradingAccount->FundMortgageAvailable);
		LOGDEMO("\tMortgageableFund [%.8lf]\n", pTradingAccount->MortgageableFund);
		LOGDEMO("\tSpecProductMargin [%.8lf]\n", pTradingAccount->SpecProductMargin);
		LOGDEMO("\tSpecProductFrozenMargin [%.8lf]\n", pTradingAccount->SpecProductFrozenMargin);
		LOGDEMO("\tSpecProductCommission [%.8lf]\n", pTradingAccount->SpecProductCommission);
		LOGDEMO("\tSpecProductFrozenCommission [%.8lf]\n", pTradingAccount->SpecProductFrozenCommission);
		LOGDEMO("\tSpecProductPositionProfit [%.8lf]\n", pTradingAccount->SpecProductPositionProfit);
		LOGDEMO("\tSpecProductCloseProfit [%.8lf]\n", pTradingAccount->SpecProductCloseProfit);
		LOGDEMO("\tSpecProductPositionProfitByAlg [%.8lf]\n", pTradingAccount->SpecProductPositionProfitByAlg);
		LOGDEMO("\tSpecProductExchangeMargin [%.8lf]\n", pTradingAccount->SpecProductExchangeMargin);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryTradingAccount>\n");
};

void Vector::OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryInvestor>\n");
	if (pInvestor)
	{
		LOGDEMO("\tInvestorID [%s]\n", pInvestor->InvestorID);
		LOGDEMO("\tBrokerID [%s]\n", pInvestor->BrokerID);
		LOGDEMO("\tInvestorGroupID [%s]\n", pInvestor->InvestorGroupID);
		LOGDEMO("\tInvestorName [%s]\n", pInvestor->InvestorName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pInvestor->IdentifiedCardNo);
		LOGDEMO("\tTelephone [%s]\n", pInvestor->Telephone);
		LOGDEMO("\tAddress [%s]\n", pInvestor->Address);
		LOGDEMO("\tOpenDate [%s]\n", pInvestor->OpenDate);
		LOGDEMO("\tMobile [%s]\n", pInvestor->Mobile);
		LOGDEMO("\tCommModelID [%s]\n", pInvestor->CommModelID);
		LOGDEMO("\tMarginModelID [%s]\n", pInvestor->MarginModelID);
		LOGDEMO("\tIsActive [%d]\n", pInvestor->IsActive);
		LOGDEMO("\tIdentifiedCardType [%c]\n", pInvestor->IdentifiedCardType);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryInvestor>\n");
};

void Vector::OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryTradingCode>\n");
	if (pTradingCode)
	{
		LOGDEMO("\tInvestorID [%s]\n", pTradingCode->InvestorID);
		LOGDEMO("\tBrokerID [%s]\n", pTradingCode->BrokerID);
		LOGDEMO("\tExchangeID [%s]\n", pTradingCode->ExchangeID);
		LOGDEMO("\tClientID [%s]\n", pTradingCode->ClientID);
		LOGDEMO("\tIsActive [%d]\n", pTradingCode->IsActive);
		LOGDEMO("\tClientIDType [%c]\n", pTradingCode->ClientIDType);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryTradingCode>\n");
};

void Vector::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryInstrumentMarginRate>\n");
	if (pInstrumentMarginRate)
	{
		LOGDEMO("\tInstrumentID [%s]\n", pInstrumentMarginRate->InstrumentID);
		LOGDEMO("\tBrokerID [%s]\n", pInstrumentMarginRate->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInstrumentMarginRate->InvestorID);
		LOGDEMO("\tIsRelative [%d]\n", pInstrumentMarginRate->IsRelative);
		LOGDEMO("\tInvestorRange [%c]\n", pInstrumentMarginRate->InvestorRange);
		LOGDEMO("\tHedgeFlag [%c]\n", pInstrumentMarginRate->HedgeFlag);
		LOGDEMO("\tLongMarginRatioByMoney [%.8lf]\n", pInstrumentMarginRate->LongMarginRatioByMoney);
		LOGDEMO("\tLongMarginRatioByVolume [%.8lf]\n", pInstrumentMarginRate->LongMarginRatioByVolume);
		LOGDEMO("\tShortMarginRatioByMoney [%.8lf]\n", pInstrumentMarginRate->ShortMarginRatioByMoney);
		LOGDEMO("\tShortMarginRatioByVolume [%.8lf]\n", pInstrumentMarginRate->ShortMarginRatioByVolume);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryInstrumentMarginRate>\n");
};

void Vector::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryInstrumentCommissionRate>\n");
	if (pInstrumentCommissionRate)
	{
		LOGDEMO("\tInstrumentID [%s]\n", pInstrumentCommissionRate->InstrumentID);
		LOGDEMO("\tBrokerID [%s]\n", pInstrumentCommissionRate->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInstrumentCommissionRate->InvestorID);
		LOGDEMO("\tInvestorRange [%c]\n", pInstrumentCommissionRate->InvestorRange);
		LOGDEMO("\tOpenRatioByMoney [%.8lf] 开仓手续费率\n", pInstrumentCommissionRate->OpenRatioByMoney);
		LOGDEMO("\tOpenRatioByVolume [%.8lf] 开仓手续费\n", pInstrumentCommissionRate->OpenRatioByVolume);
		LOGDEMO("\tCloseRatioByMoney [%.8lf] 平仓手续费率\n", pInstrumentCommissionRate->CloseRatioByMoney);
		LOGDEMO("\tCloseRatioByVolume [%.8lf] 平仓手续费\n", pInstrumentCommissionRate->CloseRatioByVolume);
		LOGDEMO("\tCloseTodayRatioByMoney [%.8lf] 平今手续费率\n", pInstrumentCommissionRate->CloseTodayRatioByMoney);
		LOGDEMO("\tCloseTodayRatioByVolume [%.8lf] 平今手续费\n", pInstrumentCommissionRate->CloseTodayRatioByVolume);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryInstrumentCommissionRate>\n");
};

void Vector::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryExchange>\n");
	if (pExchange)
	{
		LOGDEMO("\tExchangeID [%s]\n", pExchange->ExchangeID);
		LOGDEMO("\tExchangeName [%s]\n", pExchange->ExchangeName);
		LOGDEMO("\tExchangeProperty [%c]\n", pExchange->ExchangeProperty);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryExchange>\n");
};

void Vector::OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryProduct>\n");
	if (pProduct)
	{
		LOGDEMO("\tProductID [%s]\n", pProduct->ProductID);
		LOGDEMO("\tProductName [%s]\n", pProduct->ProductName);
		LOGDEMO("\tExchangeID [%s]\n", pProduct->ExchangeID);
		LOGDEMO("\tTradeCurrencyID [%s]\n", pProduct->TradeCurrencyID);
		LOGDEMO("\tExchangeProductID [%s]\n", pProduct->ExchangeProductID);
		LOGDEMO("\tVolumeMultiple [%d]\n", pProduct->VolumeMultiple);
		LOGDEMO("\tMaxMarketOrderVolume [%d]\n", pProduct->MaxMarketOrderVolume);
		LOGDEMO("\tMinMarketOrderVolume [%d]\n", pProduct->MinMarketOrderVolume);
		LOGDEMO("\tMaxLimitOrderVolume [%d]\n", pProduct->MaxLimitOrderVolume);
		LOGDEMO("\tMinLimitOrderVolume [%d]\n", pProduct->MinLimitOrderVolume);
		LOGDEMO("\tProductClass [%c]\n", pProduct->ProductClass);
		LOGDEMO("\tPositionType [%c]\n", pProduct->PositionType);
		LOGDEMO("\tPositionDateType [%c]\n", pProduct->PositionDateType);
		LOGDEMO("\tCloseDealType [%c]\n", pProduct->CloseDealType);
		LOGDEMO("\tMortgageFundUseRange [%c]\n", pProduct->MortgageFundUseRange);
		LOGDEMO("\tPriceTick [%.8lf]\n", pProduct->PriceTick);
		LOGDEMO("\tUnderlyingMultiple [%.8lf]\n", pProduct->UnderlyingMultiple);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryProduct>\n");
};

void Vector::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryInstrument>\n");
	if (pInstrument)
	{
		LOGDEMO("\tInstrumentID [%s]\n", pInstrument->InstrumentID);
		LOGDEMO("\tExchangeID [%s]\n", pInstrument->ExchangeID);
		LOGDEMO("\tInstrumentName [%s]\n", pInstrument->InstrumentName);
		LOGDEMO("\tExchangeInstID [%s]\n", pInstrument->ExchangeInstID);
		LOGDEMO("\tProductID [%s]\n", pInstrument->ProductID);
		LOGDEMO("\tCreateDate [%s]\n", pInstrument->CreateDate);
		LOGDEMO("\tOpenDate [%s]\n", pInstrument->OpenDate);
		LOGDEMO("\tExpireDate [%s]\n", pInstrument->ExpireDate);
		LOGDEMO("\tStartDelivDate [%s]\n", pInstrument->StartDelivDate);
		LOGDEMO("\tEndDelivDate [%s]\n", pInstrument->EndDelivDate);
		LOGDEMO("\tUnderlyingInstrID [%s]\n", pInstrument->UnderlyingInstrID);
		LOGDEMO("\tDeliveryYear [%d]\n", pInstrument->DeliveryYear);
		LOGDEMO("\tDeliveryMonth [%d]\n", pInstrument->DeliveryMonth);
		LOGDEMO("\tMaxMarketOrderVolume [%d]\n", pInstrument->MaxMarketOrderVolume);
		LOGDEMO("\tMinMarketOrderVolume [%d]\n", pInstrument->MinMarketOrderVolume);
		LOGDEMO("\tMaxLimitOrderVolume [%d]\n", pInstrument->MaxLimitOrderVolume);
		LOGDEMO("\tMinLimitOrderVolume [%d]\n", pInstrument->MinLimitOrderVolume);
		LOGDEMO("\tVolumeMultiple [%d]\n", pInstrument->VolumeMultiple);
		LOGDEMO("\tIsTrading [%d]\n", pInstrument->IsTrading);
		LOGDEMO("\tProductClass [%c]\n", pInstrument->ProductClass);
		LOGDEMO("\tInstLifePhase [%c]\n", pInstrument->InstLifePhase);
		LOGDEMO("\tPositionType [%c]\n", pInstrument->PositionType);
		LOGDEMO("\tPositionDateType [%c]\n", pInstrument->PositionDateType);
		LOGDEMO("\tMaxMarginSideAlgorithm [%c]\n", pInstrument->MaxMarginSideAlgorithm);
		LOGDEMO("\tOptionsType [%c]\n", pInstrument->OptionsType);
		LOGDEMO("\tCombinationType [%c]\n", pInstrument->CombinationType);
		LOGDEMO("\tPriceTick [%.8lf]\n", pInstrument->PriceTick);
		LOGDEMO("\tLongMarginRatio [%.8lf]\n", pInstrument->LongMarginRatio);
		LOGDEMO("\tShortMarginRatio [%.8lf]\n", pInstrument->ShortMarginRatio);
		LOGDEMO("\tStrikePrice [%.8lf]\n", pInstrument->StrikePrice);
		LOGDEMO("\tUnderlyingMultiple [%.8lf]\n", pInstrument->UnderlyingMultiple);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryInstrument>\n");
};

void Vector::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryDepthMarketData>\n");
	if (pDepthMarketData)
	{
		LOGDEMO("\tTradingDay [%s]\n", pDepthMarketData->TradingDay);
		LOGDEMO("\tInstrumentID [%s]\n", pDepthMarketData->InstrumentID);
		LOGDEMO("\tExchangeID [%s]\n", pDepthMarketData->ExchangeID);
		LOGDEMO("\tExchangeInstID [%s]\n", pDepthMarketData->ExchangeInstID);
		LOGDEMO("\tUpdateTime [%s]\n", pDepthMarketData->UpdateTime);
		LOGDEMO("\tActionDay [%s]\n", pDepthMarketData->ActionDay);
		LOGDEMO("\tVolume [%d]\n", pDepthMarketData->Volume);
		LOGDEMO("\tUpdateMillisec [%d]\n", pDepthMarketData->UpdateMillisec);
		LOGDEMO("\tBidVolume1 [%d]\n", pDepthMarketData->BidVolume1);
		LOGDEMO("\tAskVolume1 [%d]\n", pDepthMarketData->AskVolume1);
		LOGDEMO("\tBidVolume2 [%d]\n", pDepthMarketData->BidVolume2);
		LOGDEMO("\tAskVolume2 [%d]\n", pDepthMarketData->AskVolume2);
		LOGDEMO("\tBidVolume3 [%d]\n", pDepthMarketData->BidVolume3);
		LOGDEMO("\tAskVolume3 [%d]\n", pDepthMarketData->AskVolume3);
		LOGDEMO("\tBidVolume4 [%d]\n", pDepthMarketData->BidVolume4);
		LOGDEMO("\tAskVolume4 [%d]\n", pDepthMarketData->AskVolume4);
		LOGDEMO("\tBidVolume5 [%d]\n", pDepthMarketData->BidVolume5);
		LOGDEMO("\tAskVolume5 [%d]\n", pDepthMarketData->AskVolume5);
		LOGDEMO("\tLastPrice [%.8lf]\n", pDepthMarketData->LastPrice);
		LOGDEMO("\tPreSettlementPrice [%.8lf]\n", pDepthMarketData->PreSettlementPrice);
		LOGDEMO("\tPreClosePrice [%.8lf]\n", pDepthMarketData->PreClosePrice);
		LOGDEMO("\tPreOpenInterest [%.8lf]\n", pDepthMarketData->PreOpenInterest);
		LOGDEMO("\tOpenPrice [%.8lf]\n", pDepthMarketData->OpenPrice);
		LOGDEMO("\tHighestPrice [%.8lf]\n", pDepthMarketData->HighestPrice);
		LOGDEMO("\tLowestPrice [%.8lf]\n", pDepthMarketData->LowestPrice);
		LOGDEMO("\tTurnover [%.8lf]\n", pDepthMarketData->Turnover);
		LOGDEMO("\tOpenInterest [%.8lf]\n", pDepthMarketData->OpenInterest);
		LOGDEMO("\tClosePrice [%.8lf]\n", pDepthMarketData->ClosePrice);
		LOGDEMO("\tSettlementPrice [%.8lf]\n", pDepthMarketData->SettlementPrice);
		LOGDEMO("\tUpperLimitPrice [%.8lf]\n", pDepthMarketData->UpperLimitPrice);
		LOGDEMO("\tLowerLimitPrice [%.8lf]\n", pDepthMarketData->LowerLimitPrice);
		LOGDEMO("\tPreDelta [%.8lf]\n", pDepthMarketData->PreDelta);
		LOGDEMO("\tCurrDelta [%.8lf]\n", pDepthMarketData->CurrDelta);
		LOGDEMO("\tBidPrice1 [%.8lf]\n", pDepthMarketData->BidPrice1);
		LOGDEMO("\tAskPrice1 [%.8lf]\n", pDepthMarketData->AskPrice1);
		LOGDEMO("\tBidPrice2 [%.8lf]\n", pDepthMarketData->BidPrice2);
		LOGDEMO("\tAskPrice2 [%.8lf]\n", pDepthMarketData->AskPrice2);
		LOGDEMO("\tBidPrice3 [%.8lf]\n", pDepthMarketData->BidPrice3);
		LOGDEMO("\tAskPrice3 [%.8lf]\n", pDepthMarketData->AskPrice3);
		LOGDEMO("\tBidPrice4 [%.8lf]\n", pDepthMarketData->BidPrice4);
		LOGDEMO("\tAskPrice4 [%.8lf]\n", pDepthMarketData->AskPrice4);
		LOGDEMO("\tBidPrice5 [%.8lf]\n", pDepthMarketData->BidPrice5);
		LOGDEMO("\tAskPrice5 [%.8lf]\n", pDepthMarketData->AskPrice5);
		LOGDEMO("\tAveragePrice [%.8lf]\n", pDepthMarketData->AveragePrice);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryDepthMarketData>\n");
};

void Vector::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQrySettlementInfo>\n");
	if (pSettlementInfo)
	{
		LOGDEMO("\tTradingDay [%s]\n", pSettlementInfo->TradingDay);
		LOGDEMO("\tBrokerID [%s]\n", pSettlementInfo->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pSettlementInfo->InvestorID);
		LOGDEMO("\tContent [%s]\n", pSettlementInfo->Content);
		LOGDEMO("\tSettlementID [%d]\n", pSettlementInfo->SettlementID);
		LOGDEMO("\tSequenceNo [%d]\n", pSettlementInfo->SequenceNo);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQrySettlementInfo>\n");
};

void Vector::OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryTransferBank>\n");
	if (pTransferBank)
	{
		LOGDEMO("\tBankID [%s]\n", pTransferBank->BankID);
		LOGDEMO("\tBankBrchID [%s]\n", pTransferBank->BankBrchID);
		LOGDEMO("\tBankName [%s]\n", pTransferBank->BankName);
		LOGDEMO("\tIsActive [%d]\n", pTransferBank->IsActive);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryTransferBank>\n");
};

void Vector::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryInvestorPositionDetail>\n");
	if (pInvestorPositionDetail)
	{
		LOGDEMO("\tInstrumentID [%s]\n", pInvestorPositionDetail->InstrumentID);
		LOGDEMO("\tBrokerID [%s]\n", pInvestorPositionDetail->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInvestorPositionDetail->InvestorID);
		LOGDEMO("\tOpenDate [%s]\n", pInvestorPositionDetail->OpenDate);
		LOGDEMO("\tTradeID [%s]\n", pInvestorPositionDetail->TradeID);
		LOGDEMO("\tTradingDay [%s]\n", pInvestorPositionDetail->TradingDay);
		LOGDEMO("\tCombInstrumentID [%s]\n", pInvestorPositionDetail->CombInstrumentID);
		LOGDEMO("\tExchangeID [%s]\n", pInvestorPositionDetail->ExchangeID);
		LOGDEMO("\tVolume [%d]\n", pInvestorPositionDetail->Volume);
		LOGDEMO("\tSettlementID [%d]\n", pInvestorPositionDetail->SettlementID);
		LOGDEMO("\tCloseVolume [%d]\n", pInvestorPositionDetail->CloseVolume);
		LOGDEMO("\tHedgeFlag [%c]\n", pInvestorPositionDetail->HedgeFlag);
		LOGDEMO("\tDirection [%c]\n", pInvestorPositionDetail->Direction);
		LOGDEMO("\tTradeType [%c]\n", pInvestorPositionDetail->TradeType);
		LOGDEMO("\tOpenPrice [%.8lf]\n", pInvestorPositionDetail->OpenPrice);
		LOGDEMO("\tCloseProfitByDate [%.8lf]\n", pInvestorPositionDetail->CloseProfitByDate);
		LOGDEMO("\tCloseProfitByTrade [%.8lf]\n", pInvestorPositionDetail->CloseProfitByTrade);
		LOGDEMO("\tPositionProfitByDate [%.8lf]\n", pInvestorPositionDetail->PositionProfitByDate);
		LOGDEMO("\tPositionProfitByTrade [%.8lf]\n", pInvestorPositionDetail->PositionProfitByTrade);
		LOGDEMO("\tMargin [%.8lf]\n", pInvestorPositionDetail->Margin);
		LOGDEMO("\tExchMargin [%.8lf]\n", pInvestorPositionDetail->ExchMargin);
		LOGDEMO("\tMarginRateByMoney [%.8lf]\n", pInvestorPositionDetail->MarginRateByMoney);
		LOGDEMO("\tMarginRateByVolume [%.8lf]\n", pInvestorPositionDetail->MarginRateByVolume);
		LOGDEMO("\tLastSettlementPrice [%.8lf]\n", pInvestorPositionDetail->LastSettlementPrice);
		LOGDEMO("\tSettlementPrice [%.8lf]\n", pInvestorPositionDetail->SettlementPrice);
		LOGDEMO("\tCloseAmount [%.8lf]\n", pInvestorPositionDetail->CloseAmount);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryInvestorPositionDetail>\n");
};

void Vector::OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryNotice>\n");
	if (pNotice)
	{
		LOGDEMO("\tBrokerID [%s]\n", pNotice->BrokerID);
		LOGDEMO("\tContent [%s]\n", pNotice->Content);
		LOGDEMO("\tSequenceLabel [%s]\n", pNotice->SequenceLabel);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryNotice>\n");
};

void Vector::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQrySettlementInfoConfirm>\n");
	if (pSettlementInfoConfirm)
	{
		LOGDEMO("\tBrokerID [%s]\n", pSettlementInfoConfirm->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pSettlementInfoConfirm->InvestorID);
		LOGDEMO("\tConfirmDate [%s]\n", pSettlementInfoConfirm->ConfirmDate);
		LOGDEMO("\tConfirmTime [%s]\n", pSettlementInfoConfirm->ConfirmTime);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQrySettlementInfoConfirm>\n");
};

void Vector::OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryInvestorPositionCombineDetail>\n");
	if (pInvestorPositionCombineDetail)
	{
		LOGDEMO("\tTradingDay [%s]\n", pInvestorPositionCombineDetail->TradingDay);
		LOGDEMO("\tOpenDate [%s]\n", pInvestorPositionCombineDetail->OpenDate);
		LOGDEMO("\tExchangeID [%s]\n", pInvestorPositionCombineDetail->ExchangeID);
		LOGDEMO("\tBrokerID [%s]\n", pInvestorPositionCombineDetail->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInvestorPositionCombineDetail->InvestorID);
		LOGDEMO("\tComTradeID [%s]\n", pInvestorPositionCombineDetail->ComTradeID);
		LOGDEMO("\tTradeID [%s]\n", pInvestorPositionCombineDetail->TradeID);
		LOGDEMO("\tInstrumentID [%s]\n", pInvestorPositionCombineDetail->InstrumentID);
		LOGDEMO("\tCombInstrumentID [%s]\n", pInvestorPositionCombineDetail->CombInstrumentID);
		LOGDEMO("\tSettlementID [%d]\n", pInvestorPositionCombineDetail->SettlementID);
		LOGDEMO("\tTotalAmt [%d]\n", pInvestorPositionCombineDetail->TotalAmt);
		LOGDEMO("\tLegID [%d]\n", pInvestorPositionCombineDetail->LegID);
		LOGDEMO("\tLegMultiple [%d]\n", pInvestorPositionCombineDetail->LegMultiple);
		LOGDEMO("\tTradeGroupID [%d]\n", pInvestorPositionCombineDetail->TradeGroupID);
		LOGDEMO("\tHedgeFlag [%c]\n", pInvestorPositionCombineDetail->HedgeFlag);
		LOGDEMO("\tDirection [%c]\n", pInvestorPositionCombineDetail->Direction);
		LOGDEMO("\tMargin [%.8lf]\n", pInvestorPositionCombineDetail->Margin);
		LOGDEMO("\tExchMargin [%.8lf]\n", pInvestorPositionCombineDetail->ExchMargin);
		LOGDEMO("\tMarginRateByMoney [%.8lf]\n", pInvestorPositionCombineDetail->MarginRateByMoney);
		LOGDEMO("\tMarginRateByVolume [%.8lf]\n", pInvestorPositionCombineDetail->MarginRateByVolume);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryInvestorPositionCombineDetail>\n");
};

void Vector::OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryCFMMCTradingAccountKey>\n");
	if (pCFMMCTradingAccountKey)
	{
		LOGDEMO("\tBrokerID [%s]\n", pCFMMCTradingAccountKey->BrokerID);
		LOGDEMO("\tParticipantID [%s]\n", pCFMMCTradingAccountKey->ParticipantID);
		LOGDEMO("\tAccountID [%s]\n", pCFMMCTradingAccountKey->AccountID);
		LOGDEMO("\tCurrentKey [%s]\n", pCFMMCTradingAccountKey->CurrentKey);
		LOGDEMO("\tKeyID [%d]\n", pCFMMCTradingAccountKey->KeyID);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryCFMMCTradingAccountKey>\n");
};

void Vector::OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField *pEWarrantOffset, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryEWarrantOffset>\n");
	if (pEWarrantOffset)
	{
		LOGDEMO("\tTradingDay [%s]\n", pEWarrantOffset->TradingDay);
		LOGDEMO("\tBrokerID [%s]\n", pEWarrantOffset->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pEWarrantOffset->InvestorID);
		LOGDEMO("\tExchangeID [%s]\n", pEWarrantOffset->ExchangeID);
		LOGDEMO("\tInstrumentID [%s]\n", pEWarrantOffset->InstrumentID);
		LOGDEMO("\tVolume [%d]\n", pEWarrantOffset->Volume);
		LOGDEMO("\tDirection [%c]\n", pEWarrantOffset->Direction);
		LOGDEMO("\tHedgeFlag [%c]\n", pEWarrantOffset->HedgeFlag);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryEWarrantOffset>\n");
};

void Vector::OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField *pInvestorProductGroupMargin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryInvestorProductGroupMargin>\n");
	if (pInvestorProductGroupMargin)
	{
		LOGDEMO("\tProductGroupID [%s]\n", pInvestorProductGroupMargin->ProductGroupID);
		LOGDEMO("\tBrokerID [%s]\n", pInvestorProductGroupMargin->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInvestorProductGroupMargin->InvestorID);
		LOGDEMO("\tTradingDay [%s]\n", pInvestorProductGroupMargin->TradingDay);
		LOGDEMO("\tSettlementID [%d]\n", pInvestorProductGroupMargin->SettlementID);
		LOGDEMO("\tHedgeFlag [%c]\n", pInvestorProductGroupMargin->HedgeFlag);
		LOGDEMO("\tFrozenMargin [%.8lf]\n", pInvestorProductGroupMargin->FrozenMargin);
		LOGDEMO("\tLongFrozenMargin [%.8lf]\n", pInvestorProductGroupMargin->LongFrozenMargin);
		LOGDEMO("\tShortFrozenMargin [%.8lf]\n", pInvestorProductGroupMargin->ShortFrozenMargin);
		LOGDEMO("\tUseMargin [%.8lf]\n", pInvestorProductGroupMargin->UseMargin);
		LOGDEMO("\tLongUseMargin [%.8lf]\n", pInvestorProductGroupMargin->LongUseMargin);
		LOGDEMO("\tShortUseMargin [%.8lf]\n", pInvestorProductGroupMargin->ShortUseMargin);
		LOGDEMO("\tExchMargin [%.8lf]\n", pInvestorProductGroupMargin->ExchMargin);
		LOGDEMO("\tLongExchMargin [%.8lf]\n", pInvestorProductGroupMargin->LongExchMargin);
		LOGDEMO("\tShortExchMargin [%.8lf]\n", pInvestorProductGroupMargin->ShortExchMargin);
		LOGDEMO("\tCloseProfit [%.8lf]\n", pInvestorProductGroupMargin->CloseProfit);
		LOGDEMO("\tFrozenCommission [%.8lf]\n", pInvestorProductGroupMargin->FrozenCommission);
		LOGDEMO("\tCommission [%.8lf]\n", pInvestorProductGroupMargin->Commission);
		LOGDEMO("\tFrozenCash [%.8lf]\n", pInvestorProductGroupMargin->FrozenCash);
		LOGDEMO("\tCashIn [%.8lf]\n", pInvestorProductGroupMargin->CashIn);
		LOGDEMO("\tPositionProfit [%.8lf]\n", pInvestorProductGroupMargin->PositionProfit);
		LOGDEMO("\tOffsetAmount [%.8lf]\n", pInvestorProductGroupMargin->OffsetAmount);
		LOGDEMO("\tLongOffsetAmount [%.8lf]\n", pInvestorProductGroupMargin->LongOffsetAmount);
		LOGDEMO("\tShortOffsetAmount [%.8lf]\n", pInvestorProductGroupMargin->ShortOffsetAmount);
		LOGDEMO("\tExchOffsetAmount [%.8lf]\n", pInvestorProductGroupMargin->ExchOffsetAmount);
		LOGDEMO("\tLongExchOffsetAmount [%.8lf]\n", pInvestorProductGroupMargin->LongExchOffsetAmount);
		LOGDEMO("\tShortExchOffsetAmount [%.8lf]\n", pInvestorProductGroupMargin->ShortExchOffsetAmount);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryInvestorProductGroupMargin>\n");
};

void Vector::OnRspQryExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryExchangeMarginRate>\n");
	if (pExchangeMarginRate)
	{
		LOGDEMO("\tBrokerID [%s]\n", pExchangeMarginRate->BrokerID);
		LOGDEMO("\tInstrumentID [%s]\n", pExchangeMarginRate->InstrumentID);
		LOGDEMO("\tHedgeFlag [%c]\n", pExchangeMarginRate->HedgeFlag);
		LOGDEMO("\tLongMarginRatioByMoney [%.8lf]\n", pExchangeMarginRate->LongMarginRatioByMoney);
		LOGDEMO("\tLongMarginRatioByVolume [%.8lf]\n", pExchangeMarginRate->LongMarginRatioByVolume);
		LOGDEMO("\tShortMarginRatioByMoney [%.8lf]\n", pExchangeMarginRate->ShortMarginRatioByMoney);
		LOGDEMO("\tShortMarginRatioByVolume [%.8lf]\n", pExchangeMarginRate->ShortMarginRatioByVolume);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryExchangeMarginRate>\n");
};

void Vector::OnRspQryExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryExchangeMarginRateAdjust>\n");
	if (pExchangeMarginRateAdjust)
	{
		LOGDEMO("\tBrokerID [%s]\n", pExchangeMarginRateAdjust->BrokerID);
		LOGDEMO("\tInstrumentID [%s]\n", pExchangeMarginRateAdjust->InstrumentID);
		LOGDEMO("\tHedgeFlag [%c]\n", pExchangeMarginRateAdjust->HedgeFlag);
		LOGDEMO("\tLongMarginRatioByMoney [%.8lf]\n", pExchangeMarginRateAdjust->LongMarginRatioByMoney);
		LOGDEMO("\tLongMarginRatioByVolume [%.8lf]\n", pExchangeMarginRateAdjust->LongMarginRatioByVolume);
		LOGDEMO("\tShortMarginRatioByMoney [%.8lf]\n", pExchangeMarginRateAdjust->ShortMarginRatioByMoney);
		LOGDEMO("\tShortMarginRatioByVolume [%.8lf]\n", pExchangeMarginRateAdjust->ShortMarginRatioByVolume);
		LOGDEMO("\tExchLongMarginRatioByMoney [%.8lf]\n", pExchangeMarginRateAdjust->ExchLongMarginRatioByMoney);
		LOGDEMO("\tExchLongMarginRatioByVolume [%.8lf]\n", pExchangeMarginRateAdjust->ExchLongMarginRatioByVolume);
		LOGDEMO("\tExchShortMarginRatioByMoney [%.8lf]\n", pExchangeMarginRateAdjust->ExchShortMarginRatioByMoney);
		LOGDEMO("\tExchShortMarginRatioByVolume [%.8lf]\n", pExchangeMarginRateAdjust->ExchShortMarginRatioByVolume);
		LOGDEMO("\tNoLongMarginRatioByMoney [%.8lf]\n", pExchangeMarginRateAdjust->NoLongMarginRatioByMoney);
		LOGDEMO("\tNoLongMarginRatioByVolume [%.8lf]\n", pExchangeMarginRateAdjust->NoLongMarginRatioByVolume);
		LOGDEMO("\tNoShortMarginRatioByMoney [%.8lf]\n", pExchangeMarginRateAdjust->NoShortMarginRatioByMoney);
		LOGDEMO("\tNoShortMarginRatioByVolume [%.8lf]\n", pExchangeMarginRateAdjust->NoShortMarginRatioByVolume);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryExchangeMarginRateAdjust>\n");
};

void Vector::OnRspQryExchangeRate(CThostFtdcExchangeRateField *pExchangeRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryExchangeRate>\n");
	if (pExchangeRate)
	{
		LOGDEMO("\tBrokerID [%s]\n", pExchangeRate->BrokerID);
		LOGDEMO("\tFromCurrencyID [%s]\n", pExchangeRate->FromCurrencyID);
		LOGDEMO("\tToCurrencyID [%s]\n", pExchangeRate->ToCurrencyID);
		LOGDEMO("\tFromCurrencyUnit [%.8lf]\n", pExchangeRate->FromCurrencyUnit);
		LOGDEMO("\tExchangeRate [%.8lf]\n", pExchangeRate->ExchangeRate);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryExchangeRate>\n");
};

void Vector::OnRspQrySecAgentACIDMap(CThostFtdcSecAgentACIDMapField *pSecAgentACIDMap, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQrySecAgentACIDMap>\n");
	if (pSecAgentACIDMap)
	{
		LOGDEMO("\tBrokerID [%s]\n", pSecAgentACIDMap->BrokerID);
		LOGDEMO("\tUserID [%s]\n", pSecAgentACIDMap->UserID);
		LOGDEMO("\tAccountID [%s]\n", pSecAgentACIDMap->AccountID);
		LOGDEMO("\tCurrencyID [%s]\n", pSecAgentACIDMap->CurrencyID);
		LOGDEMO("\tBrokerSecAgentID [%s]\n", pSecAgentACIDMap->BrokerSecAgentID);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQrySecAgentACIDMap>\n");
};

void Vector::OnRspQryProductExchRate(CThostFtdcProductExchRateField *pProductExchRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryProductExchRate>\n");
	if (pProductExchRate)
	{
		LOGDEMO("\tProductID [%s]\n", pProductExchRate->ProductID);
		LOGDEMO("\tQuoteCurrencyID [%s]\n", pProductExchRate->QuoteCurrencyID);
		LOGDEMO("\tExchangeRate [%.8lf]\n", pProductExchRate->ExchangeRate);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryProductExchRate>\n");
};

void Vector::OnRspQryProductGroup(CThostFtdcProductGroupField *pProductGroup, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryProductGroup>\n");
	if (pProductGroup)
	{
		LOGDEMO("\tProductID [%s]\n", pProductGroup->ProductID);
		LOGDEMO("\tExchangeID [%s]\n", pProductGroup->ExchangeID);
		LOGDEMO("\tProductGroupID [%s]\n", pProductGroup->ProductGroupID);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryProductGroup>\n");
};

void Vector::OnRspQryMMInstrumentCommissionRate(CThostFtdcMMInstrumentCommissionRateField *pMMInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryMMInstrumentCommissionRate>\n");
	if (pMMInstrumentCommissionRate)
	{
		LOGDEMO("\tInstrumentID [%s]\n", pMMInstrumentCommissionRate->InstrumentID);
		LOGDEMO("\tBrokerID [%s]\n", pMMInstrumentCommissionRate->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pMMInstrumentCommissionRate->InvestorID);
		LOGDEMO("\tInvestorRange [%c]\n", pMMInstrumentCommissionRate->InvestorRange);
		LOGDEMO("\tOpenRatioByMoney [%.8lf]\n", pMMInstrumentCommissionRate->OpenRatioByMoney);
		LOGDEMO("\tOpenRatioByVolume [%.8lf]\n", pMMInstrumentCommissionRate->OpenRatioByVolume);
		LOGDEMO("\tCloseRatioByMoney [%.8lf]\n", pMMInstrumentCommissionRate->CloseRatioByMoney);
		LOGDEMO("\tCloseRatioByVolume [%.8lf]\n", pMMInstrumentCommissionRate->CloseRatioByVolume);
		LOGDEMO("\tCloseTodayRatioByMoney [%.8lf]\n", pMMInstrumentCommissionRate->CloseTodayRatioByMoney);
		LOGDEMO("\tCloseTodayRatioByVolume [%.8lf]\n", pMMInstrumentCommissionRate->CloseTodayRatioByVolume);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryMMInstrumentCommissionRate>\n");
};

void Vector::OnRspQryMMOptionInstrCommRate(CThostFtdcMMOptionInstrCommRateField *pMMOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryMMOptionInstrCommRate>\n");
	if (pMMOptionInstrCommRate)
	{
		LOGDEMO("\tInstrumentID [%s]\n", pMMOptionInstrCommRate->InstrumentID);
		LOGDEMO("\tBrokerID [%s]\n", pMMOptionInstrCommRate->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pMMOptionInstrCommRate->InvestorID);
		LOGDEMO("\tInvestorRange [%c]\n", pMMOptionInstrCommRate->InvestorRange);
		LOGDEMO("\tOpenRatioByMoney [%.8lf]\n", pMMOptionInstrCommRate->OpenRatioByMoney);
		LOGDEMO("\tOpenRatioByVolume [%.8lf]\n", pMMOptionInstrCommRate->OpenRatioByVolume);
		LOGDEMO("\tCloseRatioByMoney [%.8lf]\n", pMMOptionInstrCommRate->CloseRatioByMoney);
		LOGDEMO("\tCloseRatioByVolume [%.8lf]\n", pMMOptionInstrCommRate->CloseRatioByVolume);
		LOGDEMO("\tCloseTodayRatioByMoney [%.8lf]\n", pMMOptionInstrCommRate->CloseTodayRatioByMoney);
		LOGDEMO("\tCloseTodayRatioByVolume [%.8lf]\n", pMMOptionInstrCommRate->CloseTodayRatioByVolume);
		LOGDEMO("\tStrikeRatioByMoney [%.8lf]\n", pMMOptionInstrCommRate->StrikeRatioByMoney);
		LOGDEMO("\tStrikeRatioByVolume [%.8lf]\n", pMMOptionInstrCommRate->StrikeRatioByVolume);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryMMOptionInstrCommRate>\n");
};

void Vector::OnRspQryInstrumentOrderCommRate(CThostFtdcInstrumentOrderCommRateField *pInstrumentOrderCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryInstrumentOrderCommRate>\n");
	if (pInstrumentOrderCommRate)
	{
		LOGDEMO("\tInstrumentID [%s]\n", pInstrumentOrderCommRate->InstrumentID);
		LOGDEMO("\tBrokerID [%s]\n", pInstrumentOrderCommRate->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInstrumentOrderCommRate->InvestorID);
		LOGDEMO("\tInvestorRange [%c]\n", pInstrumentOrderCommRate->InvestorRange);
		LOGDEMO("\tHedgeFlag [%c]\n", pInstrumentOrderCommRate->HedgeFlag);
		LOGDEMO("\tOrderCommByVolume [%.8lf]\n", pInstrumentOrderCommRate->OrderCommByVolume);
		LOGDEMO("\tOrderActionCommByVolume [%.8lf]\n", pInstrumentOrderCommRate->OrderActionCommByVolume);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryInstrumentOrderCommRate>\n");
};

void Vector::OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField *pOptionInstrTradeCost, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryOptionInstrTradeCost>\n");
	if (pOptionInstrTradeCost)
	{
		LOGDEMO("\tBrokerID [%s]\n", pOptionInstrTradeCost->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pOptionInstrTradeCost->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pOptionInstrTradeCost->InstrumentID);
		LOGDEMO("\tHedgeFlag [%c]\n", pOptionInstrTradeCost->HedgeFlag);
		LOGDEMO("\tFixedMargin [%.8lf]\n", pOptionInstrTradeCost->FixedMargin);
		LOGDEMO("\tMiniMargin [%.8lf]\n", pOptionInstrTradeCost->MiniMargin);
		LOGDEMO("\tRoyalty [%.8lf]\n", pOptionInstrTradeCost->Royalty);
		LOGDEMO("\tExchFixedMargin [%.8lf]\n", pOptionInstrTradeCost->ExchFixedMargin);
		LOGDEMO("\tExchMiniMargin [%.8lf]\n", pOptionInstrTradeCost->ExchMiniMargin);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryOptionInstrTradeCost>\n");
};

void Vector::OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField *pOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryOptionInstrCommRate>\n");
	if (pOptionInstrCommRate)
	{
		LOGDEMO("\tInstrumentID [%s]\n", pOptionInstrCommRate->InstrumentID);
		LOGDEMO("\tBrokerID [%s]\n", pOptionInstrCommRate->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pOptionInstrCommRate->InvestorID);
		LOGDEMO("\tInvestorRange [%c]\n", pOptionInstrCommRate->InvestorRange);
		LOGDEMO("\tOpenRatioByMoney [%.8lf]\n", pOptionInstrCommRate->OpenRatioByMoney);
		LOGDEMO("\tOpenRatioByVolume [%.8lf]\n", pOptionInstrCommRate->OpenRatioByVolume);
		LOGDEMO("\tCloseRatioByMoney [%.8lf]\n", pOptionInstrCommRate->CloseRatioByMoney);
		LOGDEMO("\tCloseRatioByVolume [%.8lf]\n", pOptionInstrCommRate->CloseRatioByVolume);
		LOGDEMO("\tCloseTodayRatioByMoney [%.8lf]\n", pOptionInstrCommRate->CloseTodayRatioByMoney);
		LOGDEMO("\tCloseTodayRatioByVolume [%.8lf]\n", pOptionInstrCommRate->CloseTodayRatioByVolume);
		LOGDEMO("\tStrikeRatioByMoney [%.8lf]\n", pOptionInstrCommRate->StrikeRatioByMoney);
		LOGDEMO("\tStrikeRatioByVolume [%.8lf]\n", pOptionInstrCommRate->StrikeRatioByVolume);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryOptionInstrCommRate>\n");
};

void Vector::OnRspQryExecOrder(CThostFtdcExecOrderField *pExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryExecOrder>\n");
	if (pExecOrder)
	{
		LOGDEMO("\tBrokerID [%s]\n", pExecOrder->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pExecOrder->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pExecOrder->InstrumentID);
		LOGDEMO("\tExecOrderRef [%s]\n", pExecOrder->ExecOrderRef);
		LOGDEMO("\tUserID [%s]\n", pExecOrder->UserID);
		LOGDEMO("\tBusinessUnit [%s]\n", pExecOrder->BusinessUnit);
		LOGDEMO("\tExecOrderLocalID [%s]\n", pExecOrder->ExecOrderLocalID);
		LOGDEMO("\tExchangeID [%s]\n", pExecOrder->ExchangeID);
		LOGDEMO("\tParticipantID [%s]\n", pExecOrder->ParticipantID);
		LOGDEMO("\tClientID [%s]\n", pExecOrder->ClientID);
		LOGDEMO("\tExchangeInstID [%s]\n", pExecOrder->ExchangeInstID);
		LOGDEMO("\tTraderID [%s]\n", pExecOrder->TraderID);
		LOGDEMO("\tTradingDay [%s]\n", pExecOrder->TradingDay);
		LOGDEMO("\tExecOrderSysID [%s]\n", pExecOrder->ExecOrderSysID);
		LOGDEMO("\tInsertDate [%s]\n", pExecOrder->InsertDate);
		LOGDEMO("\tInsertTime [%s]\n", pExecOrder->InsertTime);
		LOGDEMO("\tCancelTime [%s]\n", pExecOrder->CancelTime);
		LOGDEMO("\tClearingPartID [%s]\n", pExecOrder->ClearingPartID);
		LOGDEMO("\tUserProductInfo [%s]\n", pExecOrder->UserProductInfo);
		LOGDEMO("\tStatusMsg [%s]\n", pExecOrder->StatusMsg);
		LOGDEMO("\tActiveUserID [%s]\n", pExecOrder->ActiveUserID);
		LOGDEMO("\tBranchID [%s]\n", pExecOrder->BranchID);
		LOGDEMO("\tInvestUnitID [%s]\n", pExecOrder->InvestUnitID);
		LOGDEMO("\tAccountID [%s]\n", pExecOrder->AccountID);
		LOGDEMO("\tCurrencyID [%s]\n", pExecOrder->CurrencyID);
		LOGDEMO("\tIPAddress [%s]\n", pExecOrder->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pExecOrder->MacAddress);
		LOGDEMO("\tVolume [%d]\n", pExecOrder->Volume);
		LOGDEMO("\tRequestID [%d]\n", pExecOrder->RequestID);
		LOGDEMO("\tInstallID [%d]\n", pExecOrder->InstallID);
		LOGDEMO("\tNotifySequence [%d]\n", pExecOrder->NotifySequence);
		LOGDEMO("\tSettlementID [%d]\n", pExecOrder->SettlementID);
		LOGDEMO("\tSequenceNo [%d]\n", pExecOrder->SequenceNo);
		LOGDEMO("\tFrontID [%d]\n", pExecOrder->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pExecOrder->SessionID);
		LOGDEMO("\tBrokerExecOrderSeq [%d]\n", pExecOrder->BrokerExecOrderSeq);
		LOGDEMO("\tOffsetFlag [%c]\n", pExecOrder->OffsetFlag);
		LOGDEMO("\tHedgeFlag [%c]\n", pExecOrder->HedgeFlag);
		LOGDEMO("\tActionType [%c]\n", pExecOrder->ActionType);
		LOGDEMO("\tPosiDirection [%c]\n", pExecOrder->PosiDirection);
		LOGDEMO("\tReservePositionFlag [%c]\n", pExecOrder->ReservePositionFlag);
		LOGDEMO("\tCloseFlag [%c]\n", pExecOrder->CloseFlag);
		LOGDEMO("\tOrderSubmitStatus [%c]\n", pExecOrder->OrderSubmitStatus);
		LOGDEMO("\tExecResult [%c]\n", pExecOrder->ExecResult);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryExecOrder>\n");
};

void Vector::OnRspQryForQuote(CThostFtdcForQuoteField *pForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryForQuote>\n");
	if (pForQuote)
	{
		LOGDEMO("\tBrokerID [%s]\n", pForQuote->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pForQuote->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pForQuote->InstrumentID);
		LOGDEMO("\tForQuoteRef [%s]\n", pForQuote->ForQuoteRef);
		LOGDEMO("\tUserID [%s]\n", pForQuote->UserID);
		LOGDEMO("\tForQuoteLocalID [%s]\n", pForQuote->ForQuoteLocalID);
		LOGDEMO("\tExchangeID [%s]\n", pForQuote->ExchangeID);
		LOGDEMO("\tParticipantID [%s]\n", pForQuote->ParticipantID);
		LOGDEMO("\tClientID [%s]\n", pForQuote->ClientID);
		LOGDEMO("\tExchangeInstID [%s]\n", pForQuote->ExchangeInstID);
		LOGDEMO("\tTraderID [%s]\n", pForQuote->TraderID);
		LOGDEMO("\tInsertDate [%s]\n", pForQuote->InsertDate);
		LOGDEMO("\tInsertTime [%s]\n", pForQuote->InsertTime);
		LOGDEMO("\tStatusMsg [%s]\n", pForQuote->StatusMsg);
		LOGDEMO("\tActiveUserID [%s]\n", pForQuote->ActiveUserID);
		LOGDEMO("\tInvestUnitID [%s]\n", pForQuote->InvestUnitID);
		LOGDEMO("\tIPAddress [%s]\n", pForQuote->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pForQuote->MacAddress);
		LOGDEMO("\tInstallID [%d]\n", pForQuote->InstallID);
		LOGDEMO("\tFrontID [%d]\n", pForQuote->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pForQuote->SessionID);
		LOGDEMO("\tBrokerForQutoSeq [%d]\n", pForQuote->BrokerForQutoSeq);
		LOGDEMO("\tForQuoteStatus [%c]\n", pForQuote->ForQuoteStatus);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryForQuote>\n");
};

void Vector::OnRspQryQuote(CThostFtdcQuoteField *pQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryQuote>\n");
	if (pQuote)
	{
		LOGDEMO("\tBrokerID [%s]\n", pQuote->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pQuote->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pQuote->InstrumentID);
		LOGDEMO("\tQuoteRef [%s]\n", pQuote->QuoteRef);
		LOGDEMO("\tUserID [%s]\n", pQuote->UserID);
		LOGDEMO("\tBusinessUnit [%s]\n", pQuote->BusinessUnit);
		LOGDEMO("\tQuoteLocalID [%s]\n", pQuote->QuoteLocalID);
		LOGDEMO("\tExchangeID [%s]\n", pQuote->ExchangeID);
		LOGDEMO("\tParticipantID [%s]\n", pQuote->ParticipantID);
		LOGDEMO("\tClientID [%s]\n", pQuote->ClientID);
		LOGDEMO("\tExchangeInstID [%s]\n", pQuote->ExchangeInstID);
		LOGDEMO("\tTraderID [%s]\n", pQuote->TraderID);
		LOGDEMO("\tTradingDay [%s]\n", pQuote->TradingDay);
		LOGDEMO("\tQuoteSysID [%s]\n", pQuote->QuoteSysID);
		LOGDEMO("\tInsertDate [%s]\n", pQuote->InsertDate);
		LOGDEMO("\tInsertTime [%s]\n", pQuote->InsertTime);
		LOGDEMO("\tCancelTime [%s]\n", pQuote->CancelTime);
		LOGDEMO("\tClearingPartID [%s]\n", pQuote->ClearingPartID);
		LOGDEMO("\tAskOrderSysID [%s]\n", pQuote->AskOrderSysID);
		LOGDEMO("\tBidOrderSysID [%s]\n", pQuote->BidOrderSysID);
		LOGDEMO("\tUserProductInfo [%s]\n", pQuote->UserProductInfo);
		LOGDEMO("\tStatusMsg [%s]\n", pQuote->StatusMsg);
		LOGDEMO("\tActiveUserID [%s]\n", pQuote->ActiveUserID);
		LOGDEMO("\tAskOrderRef [%s]\n", pQuote->AskOrderRef);
		LOGDEMO("\tBidOrderRef [%s]\n", pQuote->BidOrderRef);
		LOGDEMO("\tForQuoteSysID [%s]\n", pQuote->ForQuoteSysID);
		LOGDEMO("\tBranchID [%s]\n", pQuote->BranchID);
		LOGDEMO("\tInvestUnitID [%s]\n", pQuote->InvestUnitID);
		LOGDEMO("\tAccountID [%s]\n", pQuote->AccountID);
		LOGDEMO("\tCurrencyID [%s]\n", pQuote->CurrencyID);
		LOGDEMO("\tIPAddress [%s]\n", pQuote->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pQuote->MacAddress);
		LOGDEMO("\tAskVolume [%d]\n", pQuote->AskVolume);
		LOGDEMO("\tBidVolume [%d]\n", pQuote->BidVolume);
		LOGDEMO("\tRequestID [%d]\n", pQuote->RequestID);
		LOGDEMO("\tInstallID [%d]\n", pQuote->InstallID);
		LOGDEMO("\tNotifySequence [%d]\n", pQuote->NotifySequence);
		LOGDEMO("\tSettlementID [%d]\n", pQuote->SettlementID);
		LOGDEMO("\tSequenceNo [%d]\n", pQuote->SequenceNo);
		LOGDEMO("\tFrontID [%d]\n", pQuote->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pQuote->SessionID);
		LOGDEMO("\tBrokerQuoteSeq [%d]\n", pQuote->BrokerQuoteSeq);
		LOGDEMO("\tAskOffsetFlag [%c]\n", pQuote->AskOffsetFlag);
		LOGDEMO("\tBidOffsetFlag [%c]\n", pQuote->BidOffsetFlag);
		LOGDEMO("\tAskHedgeFlag [%c]\n", pQuote->AskHedgeFlag);
		LOGDEMO("\tBidHedgeFlag [%c]\n", pQuote->BidHedgeFlag);
		LOGDEMO("\tOrderSubmitStatus [%c]\n", pQuote->OrderSubmitStatus);
		LOGDEMO("\tQuoteStatus [%c]\n", pQuote->QuoteStatus);
		LOGDEMO("\tAskPrice [%.8lf]\n", pQuote->AskPrice);
		LOGDEMO("\tBidPrice [%.8lf]\n", pQuote->BidPrice);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryQuote>\n");
};

void Vector::OnRspQryCombInstrumentGuard(CThostFtdcCombInstrumentGuardField *pCombInstrumentGuard, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryCombInstrumentGuard>\n");
	if (pCombInstrumentGuard)
	{
		LOGDEMO("\tBrokerID [%s]\n", pCombInstrumentGuard->BrokerID);
		LOGDEMO("\tInstrumentID [%s]\n", pCombInstrumentGuard->InstrumentID);
		LOGDEMO("\tGuarantRatio [%.8lf]\n", pCombInstrumentGuard->GuarantRatio);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryCombInstrumentGuard>\n");
};

void Vector::OnRspQryCombAction(CThostFtdcCombActionField *pCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryCombAction>\n");
	if (pCombAction)
	{
		LOGDEMO("\tBrokerID [%s]\n", pCombAction->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pCombAction->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pCombAction->InstrumentID);
		LOGDEMO("\tCombActionRef [%s]\n", pCombAction->CombActionRef);
		LOGDEMO("\tUserID [%s]\n", pCombAction->UserID);
		LOGDEMO("\tActionLocalID [%s]\n", pCombAction->ActionLocalID);
		LOGDEMO("\tExchangeID [%s]\n", pCombAction->ExchangeID);
		LOGDEMO("\tParticipantID [%s]\n", pCombAction->ParticipantID);
		LOGDEMO("\tClientID [%s]\n", pCombAction->ClientID);
		LOGDEMO("\tExchangeInstID [%s]\n", pCombAction->ExchangeInstID);
		LOGDEMO("\tTraderID [%s]\n", pCombAction->TraderID);
		LOGDEMO("\tTradingDay [%s]\n", pCombAction->TradingDay);
		LOGDEMO("\tUserProductInfo [%s]\n", pCombAction->UserProductInfo);
		LOGDEMO("\tStatusMsg [%s]\n", pCombAction->StatusMsg);
		LOGDEMO("\tIPAddress [%s]\n", pCombAction->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pCombAction->MacAddress);
		LOGDEMO("\tVolume [%d]\n", pCombAction->Volume);
		LOGDEMO("\tInstallID [%d]\n", pCombAction->InstallID);
		LOGDEMO("\tNotifySequence [%d]\n", pCombAction->NotifySequence);
		LOGDEMO("\tSettlementID [%d]\n", pCombAction->SettlementID);
		LOGDEMO("\tSequenceNo [%d]\n", pCombAction->SequenceNo);
		LOGDEMO("\tFrontID [%d]\n", pCombAction->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pCombAction->SessionID);
		LOGDEMO("\tDirection [%c]\n", pCombAction->Direction);
		LOGDEMO("\tCombDirection [%c]\n", pCombAction->CombDirection);
		LOGDEMO("\tHedgeFlag [%c]\n", pCombAction->HedgeFlag);
		LOGDEMO("\tActionStatus [%c]\n", pCombAction->ActionStatus);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryCombAction>\n");
};

void Vector::OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryTransferSerial>\n");
	if (pTransferSerial)
	{
		LOGDEMO("\tTradeDate [%s]\n", pTransferSerial->TradeDate);
		LOGDEMO("\tTradingDay [%s]\n", pTransferSerial->TradingDay);
		LOGDEMO("\tTradeTime [%s]\n", pTransferSerial->TradeTime);
		LOGDEMO("\tTradeCode [%s]\n", pTransferSerial->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pTransferSerial->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pTransferSerial->BankBranchID);
		LOGDEMO("\tBankAccount [%s]\n", pTransferSerial->BankAccount);
		LOGDEMO("\tBankSerial [%s]\n", pTransferSerial->BankSerial);
		LOGDEMO("\tBrokerID [%s]\n", pTransferSerial->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pTransferSerial->BrokerBranchID);
		LOGDEMO("\tAccountID [%s]\n", pTransferSerial->AccountID);
		LOGDEMO("\tInvestorID [%s]\n", pTransferSerial->InvestorID);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pTransferSerial->IdentifiedCardNo);
		LOGDEMO("\tCurrencyID [%s]\n", pTransferSerial->CurrencyID);
		LOGDEMO("\tOperatorCode [%s]\n", pTransferSerial->OperatorCode);
		LOGDEMO("\tBankNewAccount [%s]\n", pTransferSerial->BankNewAccount);
		LOGDEMO("\tErrorMsg [%s]\n", pTransferSerial->ErrorMsg);
		LOGDEMO("\tPlateSerial [%d]\n", pTransferSerial->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pTransferSerial->SessionID);
		LOGDEMO("\tFutureSerial [%d]\n", pTransferSerial->FutureSerial);
		LOGDEMO("\tErrorID [%d]\n", pTransferSerial->ErrorID);
		LOGDEMO("\tBankAccType [%c]\n", pTransferSerial->BankAccType);
		LOGDEMO("\tFutureAccType [%c]\n", pTransferSerial->FutureAccType);
		LOGDEMO("\tIdCardType [%c]\n", pTransferSerial->IdCardType);
		LOGDEMO("\tAvailabilityFlag [%c]\n", pTransferSerial->AvailabilityFlag);
		LOGDEMO("\tTradeAmount [%.8lf]\n", pTransferSerial->TradeAmount);
		LOGDEMO("\tCustFee [%.8lf]\n", pTransferSerial->CustFee);
		LOGDEMO("\tBrokerFee [%.8lf]\n", pTransferSerial->BrokerFee);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryTransferSerial>\n");
};

void Vector::OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryAccountregister>\n");
	if (pAccountregister)
	{
		LOGDEMO("\tTradeDay [%s]\n", pAccountregister->TradeDay);
		LOGDEMO("\tBankID [%s]\n", pAccountregister->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pAccountregister->BankBranchID);
		LOGDEMO("\tBankAccount [%s]\n", pAccountregister->BankAccount);
		LOGDEMO("\tBrokerID [%s]\n", pAccountregister->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pAccountregister->BrokerBranchID);
		LOGDEMO("\tAccountID [%s]\n", pAccountregister->AccountID);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pAccountregister->IdentifiedCardNo);
		LOGDEMO("\tCustomerName [%s]\n", pAccountregister->CustomerName);
		LOGDEMO("\tCurrencyID [%s]\n", pAccountregister->CurrencyID);
		LOGDEMO("\tRegDate [%s]\n", pAccountregister->RegDate);
		LOGDEMO("\tOutDate [%s]\n", pAccountregister->OutDate);
		LOGDEMO("\tLongCustomerName [%s]\n", pAccountregister->LongCustomerName);
		LOGDEMO("\tTID [%d]\n", pAccountregister->TID);
		LOGDEMO("\tIdCardType [%c]\n", pAccountregister->IdCardType);
		LOGDEMO("\tOpenOrDestroy [%c]\n", pAccountregister->OpenOrDestroy);
		LOGDEMO("\tCustType [%c]\n", pAccountregister->CustType);
		LOGDEMO("\tBankAccType [%c]\n", pAccountregister->BankAccType);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryAccountregister>\n");
};

void Vector::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspError>\n");
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspError>\n");
};
void Vector::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	LOGDEMO("<OnRtnOrder>\n");
	if (pOrder)
	{
		LOGDEMO("\tBrokerID [%s]\n", pOrder->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pOrder->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pOrder->InstrumentID);
		LOGDEMO("\tOrderRef [%s]\n", pOrder->OrderRef);
		LOGDEMO("\tUserID [%s]\n", pOrder->UserID);
		LOGDEMO("\tCombOffsetFlag [%s]\n", pOrder->CombOffsetFlag);
		LOGDEMO("\tCombHedgeFlag [%s]\n", pOrder->CombHedgeFlag);
		LOGDEMO("\tGTDDate [%s]\n", pOrder->GTDDate);
		LOGDEMO("\tBusinessUnit [%s]\n", pOrder->BusinessUnit);
		LOGDEMO("\tOrderLocalID [%s]\n", pOrder->OrderLocalID);
		LOGDEMO("\tExchangeID [%s]\n", pOrder->ExchangeID);
		LOGDEMO("\tParticipantID [%s]\n", pOrder->ParticipantID);
		LOGDEMO("\tClientID [%s]\n", pOrder->ClientID);
		LOGDEMO("\tExchangeInstID [%s]\n", pOrder->ExchangeInstID);
		LOGDEMO("\tTraderID [%s]\n", pOrder->TraderID);
		LOGDEMO("\tTradingDay [%s]\n", pOrder->TradingDay);
		LOGDEMO("\tOrderSysID [%s]\n", pOrder->OrderSysID);
		LOGDEMO("\tInsertDate [%s]\n", pOrder->InsertDate);
		LOGDEMO("\tInsertTime [%s]\n", pOrder->InsertTime);
		LOGDEMO("\tActiveTime [%s]\n", pOrder->ActiveTime);
		LOGDEMO("\tSuspendTime [%s]\n", pOrder->SuspendTime);
		LOGDEMO("\tUpdateTime [%s]\n", pOrder->UpdateTime);
		LOGDEMO("\tCancelTime [%s]\n", pOrder->CancelTime);
		LOGDEMO("\tActiveTraderID [%s]\n", pOrder->ActiveTraderID);
		LOGDEMO("\tClearingPartID [%s]\n", pOrder->ClearingPartID);
		LOGDEMO("\tUserProductInfo [%s]\n", pOrder->UserProductInfo);
		LOGDEMO("\tStatusMsg [%s]\n", pOrder->StatusMsg);
		LOGDEMO("\tActiveUserID [%s]\n", pOrder->ActiveUserID);
		LOGDEMO("\tRelativeOrderSysID [%s]\n", pOrder->RelativeOrderSysID);
		LOGDEMO("\tBranchID [%s]\n", pOrder->BranchID);
		LOGDEMO("\tInvestUnitID [%s]\n", pOrder->InvestUnitID);
		LOGDEMO("\tAccountID [%s]\n", pOrder->AccountID);
		LOGDEMO("\tCurrencyID [%s]\n", pOrder->CurrencyID);
		LOGDEMO("\tIPAddress [%s]\n", pOrder->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pOrder->MacAddress);
		LOGDEMO("\tVolumeTotalOriginal [%d]\n", pOrder->VolumeTotalOriginal);
		LOGDEMO("\tMinVolume [%d]\n", pOrder->MinVolume);
		LOGDEMO("\tIsAutoSuspend [%d]\n", pOrder->IsAutoSuspend);
		LOGDEMO("\tRequestID [%d]\n", pOrder->RequestID);
		LOGDEMO("\tInstallID [%d]\n", pOrder->InstallID);
		LOGDEMO("\tNotifySequence [%d]\n", pOrder->NotifySequence);
		LOGDEMO("\tSettlementID [%d]\n", pOrder->SettlementID);
		LOGDEMO("\tVolumeTraded [%d]\n", pOrder->VolumeTraded);
		LOGDEMO("\tVolumeTotal [%d]\n", pOrder->VolumeTotal);
		LOGDEMO("\tSequenceNo [%d]\n", pOrder->SequenceNo);
		LOGDEMO("\tFrontID [%d]\n", pOrder->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pOrder->SessionID);
		LOGDEMO("\tUserForceClose [%d]\n", pOrder->UserForceClose);
		LOGDEMO("\tBrokerOrderSeq [%d]\n", pOrder->BrokerOrderSeq);
		LOGDEMO("\tZCETotalTradedVolume [%d]\n", pOrder->ZCETotalTradedVolume);
		LOGDEMO("\tIsSwapOrder [%d]\n", pOrder->IsSwapOrder);
		LOGDEMO("\tOrderPriceType [%c]\n", pOrder->OrderPriceType);
		LOGDEMO("\tDirection [%c]\n", pOrder->Direction);
		LOGDEMO("\tTimeCondition [%c]\n", pOrder->TimeCondition);
		LOGDEMO("\tVolumeCondition [%c]\n", pOrder->VolumeCondition);
		LOGDEMO("\tContingentCondition [%c]\n", pOrder->ContingentCondition);
		LOGDEMO("\tForceCloseReason [%c]\n", pOrder->ForceCloseReason);
		LOGDEMO("\tOrderSubmitStatus [%c]\n", pOrder->OrderSubmitStatus);
		LOGDEMO("\tOrderSource [%c]\n", pOrder->OrderSource);
		LOGDEMO("\tOrderStatus [%c]\n", pOrder->OrderStatus);
		LOGDEMO("\tOrderType [%c]\n", pOrder->OrderType);
		LOGDEMO("\tLimitPrice [%.8lf]\n", pOrder->LimitPrice);
		LOGDEMO("\tStopPrice [%.8lf]\n", pOrder->StopPrice);
	}
	LOGDEMO("</OnRtnOrder>\n");
};
void Vector::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	LOGDEMO("<OnRtnTrade>\n");
	if (pTrade)
	{
		LOGDEMO("\tBrokerID [%s]\n", pTrade->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pTrade->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pTrade->InstrumentID);
		LOGDEMO("\tOrderRef [%s]\n", pTrade->OrderRef);
		LOGDEMO("\tUserID [%s]\n", pTrade->UserID);
		LOGDEMO("\tExchangeID [%s]\n", pTrade->ExchangeID);
		LOGDEMO("\tTradeID [%s]\n", pTrade->TradeID);
		LOGDEMO("\tOrderSysID [%s]\n", pTrade->OrderSysID);
		LOGDEMO("\tParticipantID [%s]\n", pTrade->ParticipantID);
		LOGDEMO("\tClientID [%s]\n", pTrade->ClientID);
		LOGDEMO("\tExchangeInstID [%s]\n", pTrade->ExchangeInstID);
		LOGDEMO("\tTradeDate [%s]\n", pTrade->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pTrade->TradeTime);
		LOGDEMO("\tTraderID [%s]\n", pTrade->TraderID);
		LOGDEMO("\tOrderLocalID [%s]\n", pTrade->OrderLocalID);
		LOGDEMO("\tClearingPartID [%s]\n", pTrade->ClearingPartID);
		LOGDEMO("\tBusinessUnit [%s]\n", pTrade->BusinessUnit);
		LOGDEMO("\tTradingDay [%s]\n", pTrade->TradingDay);
		LOGDEMO("\tVolume [%d]\n", pTrade->Volume);
		LOGDEMO("\tSequenceNo [%d]\n", pTrade->SequenceNo);
		LOGDEMO("\tSettlementID [%d]\n", pTrade->SettlementID);
		LOGDEMO("\tBrokerOrderSeq [%d]\n", pTrade->BrokerOrderSeq);
		LOGDEMO("\tDirection [%c]\n", pTrade->Direction);
		LOGDEMO("\tTradingRole [%c]\n", pTrade->TradingRole);
		LOGDEMO("\tOffsetFlag [%c]\n", pTrade->OffsetFlag);
		LOGDEMO("\tHedgeFlag [%c]\n", pTrade->HedgeFlag);
		LOGDEMO("\tTradeType [%c]\n", pTrade->TradeType);
		LOGDEMO("\tPriceSource [%c]\n", pTrade->PriceSource);
		LOGDEMO("\tTradeSource [%c]\n", pTrade->TradeSource);
		LOGDEMO("\tPrice [%.8lf]\n", pTrade->Price);
	}
	LOGDEMO("</OnRtnTrade>\n");
};
void Vector::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	LOGDEMO("<OnErrRtnOrderInsert>\n");
	if (pInputOrder)
	{
		LOGDEMO("\tBrokerID [%s]\n", pInputOrder->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInputOrder->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pInputOrder->InstrumentID);
		LOGDEMO("\tOrderRef [%s]\n", pInputOrder->OrderRef);
		LOGDEMO("\tUserID [%s]\n", pInputOrder->UserID);
		LOGDEMO("\tCombOffsetFlag [%s]\n", pInputOrder->CombOffsetFlag);
		LOGDEMO("\tCombHedgeFlag [%s]\n", pInputOrder->CombHedgeFlag);
		LOGDEMO("\tGTDDate [%s]\n", pInputOrder->GTDDate);
		LOGDEMO("\tBusinessUnit [%s]\n", pInputOrder->BusinessUnit);
		LOGDEMO("\tExchangeID [%s]\n", pInputOrder->ExchangeID);
		LOGDEMO("\tInvestUnitID [%s]\n", pInputOrder->InvestUnitID);
		LOGDEMO("\tAccountID [%s]\n", pInputOrder->AccountID);
		LOGDEMO("\tCurrencyID [%s]\n", pInputOrder->CurrencyID);
		LOGDEMO("\tClientID [%s]\n", pInputOrder->ClientID);
		LOGDEMO("\tIPAddress [%s]\n", pInputOrder->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pInputOrder->MacAddress);
		LOGDEMO("\tVolumeTotalOriginal [%d]\n", pInputOrder->VolumeTotalOriginal);
		LOGDEMO("\tMinVolume [%d]\n", pInputOrder->MinVolume);
		LOGDEMO("\tIsAutoSuspend [%d]\n", pInputOrder->IsAutoSuspend);
		LOGDEMO("\tRequestID [%d]\n", pInputOrder->RequestID);
		LOGDEMO("\tUserForceClose [%d]\n", pInputOrder->UserForceClose);
		LOGDEMO("\tIsSwapOrder [%d]\n", pInputOrder->IsSwapOrder);
		LOGDEMO("\tOrderPriceType [%c]\n", pInputOrder->OrderPriceType);
		LOGDEMO("\tDirection [%c]\n", pInputOrder->Direction);
		LOGDEMO("\tTimeCondition [%c]\n", pInputOrder->TimeCondition);
		LOGDEMO("\tVolumeCondition [%c]\n", pInputOrder->VolumeCondition);
		LOGDEMO("\tContingentCondition [%c]\n", pInputOrder->ContingentCondition);
		LOGDEMO("\tForceCloseReason [%c]\n", pInputOrder->ForceCloseReason);
		LOGDEMO("\tLimitPrice [%.8lf]\n", pInputOrder->LimitPrice);
		LOGDEMO("\tStopPrice [%.8lf]\n", pInputOrder->StopPrice);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("</OnErrRtnOrderInsert>\n");
};
void Vector::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
	LOGDEMO("<OnErrRtnOrderAction>\n");
	if (pOrderAction)
	{
		LOGDEMO("\tBrokerID [%s]\n", pOrderAction->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pOrderAction->InvestorID);
		LOGDEMO("\tOrderRef [%s]\n", pOrderAction->OrderRef);
		LOGDEMO("\tExchangeID [%s]\n", pOrderAction->ExchangeID);
		LOGDEMO("\tOrderSysID [%s]\n", pOrderAction->OrderSysID);
		LOGDEMO("\tActionDate [%s]\n", pOrderAction->ActionDate);
		LOGDEMO("\tActionTime [%s]\n", pOrderAction->ActionTime);
		LOGDEMO("\tTraderID [%s]\n", pOrderAction->TraderID);
		LOGDEMO("\tOrderLocalID [%s]\n", pOrderAction->OrderLocalID);
		LOGDEMO("\tActionLocalID [%s]\n", pOrderAction->ActionLocalID);
		LOGDEMO("\tParticipantID [%s]\n", pOrderAction->ParticipantID);
		LOGDEMO("\tClientID [%s]\n", pOrderAction->ClientID);
		LOGDEMO("\tBusinessUnit [%s]\n", pOrderAction->BusinessUnit);
		LOGDEMO("\tUserID [%s]\n", pOrderAction->UserID);
		LOGDEMO("\tStatusMsg [%s]\n", pOrderAction->StatusMsg);
		LOGDEMO("\tInstrumentID [%s]\n", pOrderAction->InstrumentID);
		LOGDEMO("\tBranchID [%s]\n", pOrderAction->BranchID);
		LOGDEMO("\tInvestUnitID [%s]\n", pOrderAction->InvestUnitID);
		LOGDEMO("\tIPAddress [%s]\n", pOrderAction->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pOrderAction->MacAddress);
		LOGDEMO("\tOrderActionRef [%d]\n", pOrderAction->OrderActionRef);
		LOGDEMO("\tRequestID [%d]\n", pOrderAction->RequestID);
		LOGDEMO("\tFrontID [%d]\n", pOrderAction->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pOrderAction->SessionID);
		LOGDEMO("\tVolumeChange [%d]\n", pOrderAction->VolumeChange);
		LOGDEMO("\tInstallID [%d]\n", pOrderAction->InstallID);
		LOGDEMO("\tActionFlag [%c]\n", pOrderAction->ActionFlag);
		LOGDEMO("\tOrderActionStatus [%c]\n", pOrderAction->OrderActionStatus);
		LOGDEMO("\tLimitPrice [%.8lf]\n", pOrderAction->LimitPrice);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("</OnErrRtnOrderAction>\n");
};
void Vector::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
	LOGDEMO("<OnRtnInstrumentStatus>\n");
	if (pInstrumentStatus)
	{
		LOGDEMO("\tExchangeID [%s]\n", pInstrumentStatus->ExchangeID);
		LOGDEMO("\tExchangeInstID [%s]\n", pInstrumentStatus->ExchangeInstID);
		LOGDEMO("\tSettlementGroupID [%s]\n", pInstrumentStatus->SettlementGroupID);
		LOGDEMO("\tInstrumentID [%s]\n", pInstrumentStatus->InstrumentID);
		LOGDEMO("\tEnterTime [%s]\n", pInstrumentStatus->EnterTime);
		LOGDEMO("\tTradingSegmentSN [%d]\n", pInstrumentStatus->TradingSegmentSN);
		LOGDEMO("\tInstrumentStatus [%c]\n", pInstrumentStatus->InstrumentStatus);
		LOGDEMO("\tEnterReason [%c]\n", pInstrumentStatus->EnterReason);
	}
	LOGDEMO("</OnRtnInstrumentStatus>\n");
};
void Vector::OnRtnBulletin(CThostFtdcBulletinField *pBulletin)
{
	LOGDEMO("<OnRtnBulletin>\n");
	if (pBulletin)
	{
		LOGDEMO("\tExchangeID [%s]\n", pBulletin->ExchangeID);
		LOGDEMO("\tTradingDay [%s]\n", pBulletin->TradingDay);
		LOGDEMO("\tNewsType [%s]\n", pBulletin->NewsType);
		LOGDEMO("\tSendTime [%s]\n", pBulletin->SendTime);
		LOGDEMO("\tAbstract [%s]\n", pBulletin->Abstract);
		LOGDEMO("\tComeFrom [%s]\n", pBulletin->ComeFrom);
		LOGDEMO("\tContent [%s]\n", pBulletin->Content);
		LOGDEMO("\tURLLink [%s]\n", pBulletin->URLLink);
		LOGDEMO("\tMarketID [%s]\n", pBulletin->MarketID);
		LOGDEMO("\tBulletinID [%d]\n", pBulletin->BulletinID);
		LOGDEMO("\tSequenceNo [%d]\n", pBulletin->SequenceNo);
		LOGDEMO("\tNewsUrgency [%c]\n", pBulletin->NewsUrgency);
	}
	LOGDEMO("</OnRtnBulletin>\n");
};
void Vector::OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo)
{
	LOGDEMO("<OnRtnTradingNotice>\n");
	if (pTradingNoticeInfo)
	{
		LOGDEMO("\tBrokerID [%s]\n", pTradingNoticeInfo->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pTradingNoticeInfo->InvestorID);
		LOGDEMO("\tSendTime [%s]\n", pTradingNoticeInfo->SendTime);
		LOGDEMO("\tFieldContent [%s]\n", pTradingNoticeInfo->FieldContent);
		LOGDEMO("\tSequenceNo [%d]\n", pTradingNoticeInfo->SequenceNo);
		LOGDEMO("\tSequenceSeries [%d]\n", pTradingNoticeInfo->SequenceSeries);
	}
	LOGDEMO("</OnRtnTradingNotice>\n");
};
void Vector::OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder)
{
	LOGDEMO("<OnRtnErrorConditionalOrder>\n");
	if (pErrorConditionalOrder)
	{
		LOGDEMO("\tBrokerID [%s]\n", pErrorConditionalOrder->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pErrorConditionalOrder->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pErrorConditionalOrder->InstrumentID);
		LOGDEMO("\tOrderRef [%s]\n", pErrorConditionalOrder->OrderRef);
		LOGDEMO("\tUserID [%s]\n", pErrorConditionalOrder->UserID);
		LOGDEMO("\tCombOffsetFlag [%s]\n", pErrorConditionalOrder->CombOffsetFlag);
		LOGDEMO("\tCombHedgeFlag [%s]\n", pErrorConditionalOrder->CombHedgeFlag);
		LOGDEMO("\tGTDDate [%s]\n", pErrorConditionalOrder->GTDDate);
		LOGDEMO("\tBusinessUnit [%s]\n", pErrorConditionalOrder->BusinessUnit);
		LOGDEMO("\tOrderLocalID [%s]\n", pErrorConditionalOrder->OrderLocalID);
		LOGDEMO("\tExchangeID [%s]\n", pErrorConditionalOrder->ExchangeID);
		LOGDEMO("\tParticipantID [%s]\n", pErrorConditionalOrder->ParticipantID);
		LOGDEMO("\tClientID [%s]\n", pErrorConditionalOrder->ClientID);
		LOGDEMO("\tExchangeInstID [%s]\n", pErrorConditionalOrder->ExchangeInstID);
		LOGDEMO("\tTraderID [%s]\n", pErrorConditionalOrder->TraderID);
		LOGDEMO("\tTradingDay [%s]\n", pErrorConditionalOrder->TradingDay);
		LOGDEMO("\tOrderSysID [%s]\n", pErrorConditionalOrder->OrderSysID);
		LOGDEMO("\tInsertDate [%s]\n", pErrorConditionalOrder->InsertDate);
		LOGDEMO("\tInsertTime [%s]\n", pErrorConditionalOrder->InsertTime);
		LOGDEMO("\tActiveTime [%s]\n", pErrorConditionalOrder->ActiveTime);
		LOGDEMO("\tSuspendTime [%s]\n", pErrorConditionalOrder->SuspendTime);
		LOGDEMO("\tUpdateTime [%s]\n", pErrorConditionalOrder->UpdateTime);
		LOGDEMO("\tCancelTime [%s]\n", pErrorConditionalOrder->CancelTime);
		LOGDEMO("\tActiveTraderID [%s]\n", pErrorConditionalOrder->ActiveTraderID);
		LOGDEMO("\tClearingPartID [%s]\n", pErrorConditionalOrder->ClearingPartID);
		LOGDEMO("\tUserProductInfo [%s]\n", pErrorConditionalOrder->UserProductInfo);
		LOGDEMO("\tStatusMsg [%s]\n", pErrorConditionalOrder->StatusMsg);
		LOGDEMO("\tActiveUserID [%s]\n", pErrorConditionalOrder->ActiveUserID);
		LOGDEMO("\tRelativeOrderSysID [%s]\n", pErrorConditionalOrder->RelativeOrderSysID);
		LOGDEMO("\tErrorMsg [%s]\n", pErrorConditionalOrder->ErrorMsg);
		LOGDEMO("\tBranchID [%s]\n", pErrorConditionalOrder->BranchID);
		LOGDEMO("\tInvestUnitID [%s]\n", pErrorConditionalOrder->InvestUnitID);
		LOGDEMO("\tAccountID [%s]\n", pErrorConditionalOrder->AccountID);
		LOGDEMO("\tCurrencyID [%s]\n", pErrorConditionalOrder->CurrencyID);
		LOGDEMO("\tIPAddress [%s]\n", pErrorConditionalOrder->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pErrorConditionalOrder->MacAddress);
		LOGDEMO("\tVolumeTotalOriginal [%d]\n", pErrorConditionalOrder->VolumeTotalOriginal);
		LOGDEMO("\tMinVolume [%d]\n", pErrorConditionalOrder->MinVolume);
		LOGDEMO("\tIsAutoSuspend [%d]\n", pErrorConditionalOrder->IsAutoSuspend);
		LOGDEMO("\tRequestID [%d]\n", pErrorConditionalOrder->RequestID);
		LOGDEMO("\tInstallID [%d]\n", pErrorConditionalOrder->InstallID);
		LOGDEMO("\tNotifySequence [%d]\n", pErrorConditionalOrder->NotifySequence);
		LOGDEMO("\tSettlementID [%d]\n", pErrorConditionalOrder->SettlementID);
		LOGDEMO("\tVolumeTraded [%d]\n", pErrorConditionalOrder->VolumeTraded);
		LOGDEMO("\tVolumeTotal [%d]\n", pErrorConditionalOrder->VolumeTotal);
		LOGDEMO("\tSequenceNo [%d]\n", pErrorConditionalOrder->SequenceNo);
		LOGDEMO("\tFrontID [%d]\n", pErrorConditionalOrder->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pErrorConditionalOrder->SessionID);
		LOGDEMO("\tUserForceClose [%d]\n", pErrorConditionalOrder->UserForceClose);
		LOGDEMO("\tBrokerOrderSeq [%d]\n", pErrorConditionalOrder->BrokerOrderSeq);
		LOGDEMO("\tZCETotalTradedVolume [%d]\n", pErrorConditionalOrder->ZCETotalTradedVolume);
		LOGDEMO("\tErrorID [%d]\n", pErrorConditionalOrder->ErrorID);
		LOGDEMO("\tIsSwapOrder [%d]\n", pErrorConditionalOrder->IsSwapOrder);
		LOGDEMO("\tOrderPriceType [%c]\n", pErrorConditionalOrder->OrderPriceType);
		LOGDEMO("\tDirection [%c]\n", pErrorConditionalOrder->Direction);
		LOGDEMO("\tTimeCondition [%c]\n", pErrorConditionalOrder->TimeCondition);
		LOGDEMO("\tVolumeCondition [%c]\n", pErrorConditionalOrder->VolumeCondition);
		LOGDEMO("\tContingentCondition [%c]\n", pErrorConditionalOrder->ContingentCondition);
		LOGDEMO("\tForceCloseReason [%c]\n", pErrorConditionalOrder->ForceCloseReason);
		LOGDEMO("\tOrderSubmitStatus [%c]\n", pErrorConditionalOrder->OrderSubmitStatus);
		LOGDEMO("\tOrderSource [%c]\n", pErrorConditionalOrder->OrderSource);
		LOGDEMO("\tOrderStatus [%c]\n", pErrorConditionalOrder->OrderStatus);
		LOGDEMO("\tOrderType [%c]\n", pErrorConditionalOrder->OrderType);
		LOGDEMO("\tLimitPrice [%.8lf]\n", pErrorConditionalOrder->LimitPrice);
		LOGDEMO("\tStopPrice [%.8lf]\n", pErrorConditionalOrder->StopPrice);
	}
	LOGDEMO("</OnRtnErrorConditionalOrder>\n");
};
void Vector::OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder)
{
	LOGDEMO("<OnRtnExecOrder>\n");
	if (pExecOrder)
	{
		LOGDEMO("\tBrokerID [%s]\n", pExecOrder->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pExecOrder->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pExecOrder->InstrumentID);
		LOGDEMO("\tExecOrderRef [%s]\n", pExecOrder->ExecOrderRef);
		LOGDEMO("\tUserID [%s]\n", pExecOrder->UserID);
		LOGDEMO("\tBusinessUnit [%s]\n", pExecOrder->BusinessUnit);
		LOGDEMO("\tExecOrderLocalID [%s]\n", pExecOrder->ExecOrderLocalID);
		LOGDEMO("\tExchangeID [%s]\n", pExecOrder->ExchangeID);
		LOGDEMO("\tParticipantID [%s]\n", pExecOrder->ParticipantID);
		LOGDEMO("\tClientID [%s]\n", pExecOrder->ClientID);
		LOGDEMO("\tExchangeInstID [%s]\n", pExecOrder->ExchangeInstID);
		LOGDEMO("\tTraderID [%s]\n", pExecOrder->TraderID);
		LOGDEMO("\tTradingDay [%s]\n", pExecOrder->TradingDay);
		LOGDEMO("\tExecOrderSysID [%s]\n", pExecOrder->ExecOrderSysID);
		LOGDEMO("\tInsertDate [%s]\n", pExecOrder->InsertDate);
		LOGDEMO("\tInsertTime [%s]\n", pExecOrder->InsertTime);
		LOGDEMO("\tCancelTime [%s]\n", pExecOrder->CancelTime);
		LOGDEMO("\tClearingPartID [%s]\n", pExecOrder->ClearingPartID);
		LOGDEMO("\tUserProductInfo [%s]\n", pExecOrder->UserProductInfo);
		LOGDEMO("\tStatusMsg [%s]\n", pExecOrder->StatusMsg);
		LOGDEMO("\tActiveUserID [%s]\n", pExecOrder->ActiveUserID);
		LOGDEMO("\tBranchID [%s]\n", pExecOrder->BranchID);
		LOGDEMO("\tInvestUnitID [%s]\n", pExecOrder->InvestUnitID);
		LOGDEMO("\tAccountID [%s]\n", pExecOrder->AccountID);
		LOGDEMO("\tCurrencyID [%s]\n", pExecOrder->CurrencyID);
		LOGDEMO("\tIPAddress [%s]\n", pExecOrder->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pExecOrder->MacAddress);
		LOGDEMO("\tVolume [%d]\n", pExecOrder->Volume);
		LOGDEMO("\tRequestID [%d]\n", pExecOrder->RequestID);
		LOGDEMO("\tInstallID [%d]\n", pExecOrder->InstallID);
		LOGDEMO("\tNotifySequence [%d]\n", pExecOrder->NotifySequence);
		LOGDEMO("\tSettlementID [%d]\n", pExecOrder->SettlementID);
		LOGDEMO("\tSequenceNo [%d]\n", pExecOrder->SequenceNo);
		LOGDEMO("\tFrontID [%d]\n", pExecOrder->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pExecOrder->SessionID);
		LOGDEMO("\tBrokerExecOrderSeq [%d]\n", pExecOrder->BrokerExecOrderSeq);
		LOGDEMO("\tOffsetFlag [%c]\n", pExecOrder->OffsetFlag);
		LOGDEMO("\tHedgeFlag [%c]\n", pExecOrder->HedgeFlag);
		LOGDEMO("\tActionType [%c]\n", pExecOrder->ActionType);
		LOGDEMO("\tPosiDirection [%c]\n", pExecOrder->PosiDirection);
		LOGDEMO("\tReservePositionFlag [%c]\n", pExecOrder->ReservePositionFlag);
		LOGDEMO("\tCloseFlag [%c]\n", pExecOrder->CloseFlag);
		LOGDEMO("\tOrderSubmitStatus [%c]\n", pExecOrder->OrderSubmitStatus);
		LOGDEMO("\tExecResult [%c]\n", pExecOrder->ExecResult);
	}
	LOGDEMO("</OnRtnExecOrder>\n");
};
void Vector::OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo)
{
	LOGDEMO("<OnErrRtnExecOrderInsert>\n");
	if (pInputExecOrder)
	{
		LOGDEMO("\tBrokerID [%s]\n", pInputExecOrder->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInputExecOrder->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pInputExecOrder->InstrumentID);
		LOGDEMO("\tExecOrderRef [%s]\n", pInputExecOrder->ExecOrderRef);
		LOGDEMO("\tUserID [%s]\n", pInputExecOrder->UserID);
		LOGDEMO("\tBusinessUnit [%s]\n", pInputExecOrder->BusinessUnit);
		LOGDEMO("\tExchangeID [%s]\n", pInputExecOrder->ExchangeID);
		LOGDEMO("\tInvestUnitID [%s]\n", pInputExecOrder->InvestUnitID);
		LOGDEMO("\tAccountID [%s]\n", pInputExecOrder->AccountID);
		LOGDEMO("\tCurrencyID [%s]\n", pInputExecOrder->CurrencyID);
		LOGDEMO("\tClientID [%s]\n", pInputExecOrder->ClientID);
		LOGDEMO("\tIPAddress [%s]\n", pInputExecOrder->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pInputExecOrder->MacAddress);
		LOGDEMO("\tVolume [%d]\n", pInputExecOrder->Volume);
		LOGDEMO("\tRequestID [%d]\n", pInputExecOrder->RequestID);
		LOGDEMO("\tOffsetFlag [%c]\n", pInputExecOrder->OffsetFlag);
		LOGDEMO("\tHedgeFlag [%c]\n", pInputExecOrder->HedgeFlag);
		LOGDEMO("\tActionType [%c]\n", pInputExecOrder->ActionType);
		LOGDEMO("\tPosiDirection [%c]\n", pInputExecOrder->PosiDirection);
		LOGDEMO("\tReservePositionFlag [%c]\n", pInputExecOrder->ReservePositionFlag);
		LOGDEMO("\tCloseFlag [%c]\n", pInputExecOrder->CloseFlag);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("</OnErrRtnExecOrderInsert>\n");
};
void Vector::OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
	LOGDEMO("<OnErrRtnExecOrderAction>\n");
	if (pExecOrderAction)
	{
		LOGDEMO("\tBrokerID [%s]\n", pExecOrderAction->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pExecOrderAction->InvestorID);
		LOGDEMO("\tExecOrderRef [%s]\n", pExecOrderAction->ExecOrderRef);
		LOGDEMO("\tExchangeID [%s]\n", pExecOrderAction->ExchangeID);
		LOGDEMO("\tExecOrderSysID [%s]\n", pExecOrderAction->ExecOrderSysID);
		LOGDEMO("\tActionDate [%s]\n", pExecOrderAction->ActionDate);
		LOGDEMO("\tActionTime [%s]\n", pExecOrderAction->ActionTime);
		LOGDEMO("\tTraderID [%s]\n", pExecOrderAction->TraderID);
		LOGDEMO("\tExecOrderLocalID [%s]\n", pExecOrderAction->ExecOrderLocalID);
		LOGDEMO("\tActionLocalID [%s]\n", pExecOrderAction->ActionLocalID);
		LOGDEMO("\tParticipantID [%s]\n", pExecOrderAction->ParticipantID);
		LOGDEMO("\tClientID [%s]\n", pExecOrderAction->ClientID);
		LOGDEMO("\tBusinessUnit [%s]\n", pExecOrderAction->BusinessUnit);
		LOGDEMO("\tUserID [%s]\n", pExecOrderAction->UserID);
		LOGDEMO("\tStatusMsg [%s]\n", pExecOrderAction->StatusMsg);
		LOGDEMO("\tInstrumentID [%s]\n", pExecOrderAction->InstrumentID);
		LOGDEMO("\tBranchID [%s]\n", pExecOrderAction->BranchID);
		LOGDEMO("\tInvestUnitID [%s]\n", pExecOrderAction->InvestUnitID);
		LOGDEMO("\tIPAddress [%s]\n", pExecOrderAction->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pExecOrderAction->MacAddress);
		LOGDEMO("\tExecOrderActionRef [%d]\n", pExecOrderAction->ExecOrderActionRef);
		LOGDEMO("\tRequestID [%d]\n", pExecOrderAction->RequestID);
		LOGDEMO("\tFrontID [%d]\n", pExecOrderAction->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pExecOrderAction->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pExecOrderAction->InstallID);
		LOGDEMO("\tActionFlag [%c]\n", pExecOrderAction->ActionFlag);
		LOGDEMO("\tOrderActionStatus [%c]\n", pExecOrderAction->OrderActionStatus);
		LOGDEMO("\tActionType [%c]\n", pExecOrderAction->ActionType);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("</OnErrRtnExecOrderAction>\n");
};
void Vector::OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo)
{
	LOGDEMO("<OnErrRtnForQuoteInsert>\n");
	if (pInputForQuote)
	{
		LOGDEMO("\tBrokerID [%s]\n", pInputForQuote->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInputForQuote->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pInputForQuote->InstrumentID);
		LOGDEMO("\tForQuoteRef [%s]\n", pInputForQuote->ForQuoteRef);
		LOGDEMO("\tUserID [%s]\n", pInputForQuote->UserID);
		LOGDEMO("\tExchangeID [%s]\n", pInputForQuote->ExchangeID);
		LOGDEMO("\tInvestUnitID [%s]\n", pInputForQuote->InvestUnitID);
		LOGDEMO("\tIPAddress [%s]\n", pInputForQuote->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pInputForQuote->MacAddress);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("</OnErrRtnForQuoteInsert>\n");
};
void Vector::OnRtnQuote(CThostFtdcQuoteField *pQuote)
{
	LOGDEMO("<OnRtnQuote>\n");
	if (pQuote)
	{
		LOGDEMO("\tBrokerID [%s]\n", pQuote->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pQuote->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pQuote->InstrumentID);
		LOGDEMO("\tQuoteRef [%s]\n", pQuote->QuoteRef);
		LOGDEMO("\tUserID [%s]\n", pQuote->UserID);
		LOGDEMO("\tBusinessUnit [%s]\n", pQuote->BusinessUnit);
		LOGDEMO("\tQuoteLocalID [%s]\n", pQuote->QuoteLocalID);
		LOGDEMO("\tExchangeID [%s]\n", pQuote->ExchangeID);
		LOGDEMO("\tParticipantID [%s]\n", pQuote->ParticipantID);
		LOGDEMO("\tClientID [%s]\n", pQuote->ClientID);
		LOGDEMO("\tExchangeInstID [%s]\n", pQuote->ExchangeInstID);
		LOGDEMO("\tTraderID [%s]\n", pQuote->TraderID);
		LOGDEMO("\tTradingDay [%s]\n", pQuote->TradingDay);
		LOGDEMO("\tQuoteSysID [%s]\n", pQuote->QuoteSysID);
		LOGDEMO("\tInsertDate [%s]\n", pQuote->InsertDate);
		LOGDEMO("\tInsertTime [%s]\n", pQuote->InsertTime);
		LOGDEMO("\tCancelTime [%s]\n", pQuote->CancelTime);
		LOGDEMO("\tClearingPartID [%s]\n", pQuote->ClearingPartID);
		LOGDEMO("\tAskOrderSysID [%s]\n", pQuote->AskOrderSysID);
		LOGDEMO("\tBidOrderSysID [%s]\n", pQuote->BidOrderSysID);
		LOGDEMO("\tUserProductInfo [%s]\n", pQuote->UserProductInfo);
		LOGDEMO("\tStatusMsg [%s]\n", pQuote->StatusMsg);
		LOGDEMO("\tActiveUserID [%s]\n", pQuote->ActiveUserID);
		LOGDEMO("\tAskOrderRef [%s]\n", pQuote->AskOrderRef);
		LOGDEMO("\tBidOrderRef [%s]\n", pQuote->BidOrderRef);
		LOGDEMO("\tForQuoteSysID [%s]\n", pQuote->ForQuoteSysID);
		LOGDEMO("\tBranchID [%s]\n", pQuote->BranchID);
		LOGDEMO("\tInvestUnitID [%s]\n", pQuote->InvestUnitID);
		LOGDEMO("\tAccountID [%s]\n", pQuote->AccountID);
		LOGDEMO("\tCurrencyID [%s]\n", pQuote->CurrencyID);
		LOGDEMO("\tIPAddress [%s]\n", pQuote->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pQuote->MacAddress);
		LOGDEMO("\tAskVolume [%d]\n", pQuote->AskVolume);
		LOGDEMO("\tBidVolume [%d]\n", pQuote->BidVolume);
		LOGDEMO("\tRequestID [%d]\n", pQuote->RequestID);
		LOGDEMO("\tInstallID [%d]\n", pQuote->InstallID);
		LOGDEMO("\tNotifySequence [%d]\n", pQuote->NotifySequence);
		LOGDEMO("\tSettlementID [%d]\n", pQuote->SettlementID);
		LOGDEMO("\tSequenceNo [%d]\n", pQuote->SequenceNo);
		LOGDEMO("\tFrontID [%d]\n", pQuote->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pQuote->SessionID);
		LOGDEMO("\tBrokerQuoteSeq [%d]\n", pQuote->BrokerQuoteSeq);
		LOGDEMO("\tAskOffsetFlag [%c]\n", pQuote->AskOffsetFlag);
		LOGDEMO("\tBidOffsetFlag [%c]\n", pQuote->BidOffsetFlag);
		LOGDEMO("\tAskHedgeFlag [%c]\n", pQuote->AskHedgeFlag);
		LOGDEMO("\tBidHedgeFlag [%c]\n", pQuote->BidHedgeFlag);
		LOGDEMO("\tOrderSubmitStatus [%c]\n", pQuote->OrderSubmitStatus);
		LOGDEMO("\tQuoteStatus [%c]\n", pQuote->QuoteStatus);
		LOGDEMO("\tAskPrice [%.8lf]\n", pQuote->AskPrice);
		LOGDEMO("\tBidPrice [%.8lf]\n", pQuote->BidPrice);
	}
	LOGDEMO("</OnRtnQuote>\n");
};
void Vector::OnErrRtnQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo)
{
	LOGDEMO("<OnErrRtnQuoteInsert>\n");
	if (pInputQuote)
	{
		LOGDEMO("\tBrokerID [%s]\n", pInputQuote->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInputQuote->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pInputQuote->InstrumentID);
		LOGDEMO("\tQuoteRef [%s]\n", pInputQuote->QuoteRef);
		LOGDEMO("\tUserID [%s]\n", pInputQuote->UserID);
		LOGDEMO("\tBusinessUnit [%s]\n", pInputQuote->BusinessUnit);
		LOGDEMO("\tAskOrderRef [%s]\n", pInputQuote->AskOrderRef);
		LOGDEMO("\tBidOrderRef [%s]\n", pInputQuote->BidOrderRef);
		LOGDEMO("\tForQuoteSysID [%s]\n", pInputQuote->ForQuoteSysID);
		LOGDEMO("\tExchangeID [%s]\n", pInputQuote->ExchangeID);
		LOGDEMO("\tInvestUnitID [%s]\n", pInputQuote->InvestUnitID);
		LOGDEMO("\tClientID [%s]\n", pInputQuote->ClientID);
		LOGDEMO("\tIPAddress [%s]\n", pInputQuote->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pInputQuote->MacAddress);
		LOGDEMO("\tAskVolume [%d]\n", pInputQuote->AskVolume);
		LOGDEMO("\tBidVolume [%d]\n", pInputQuote->BidVolume);
		LOGDEMO("\tRequestID [%d]\n", pInputQuote->RequestID);
		LOGDEMO("\tAskOffsetFlag [%c]\n", pInputQuote->AskOffsetFlag);
		LOGDEMO("\tBidOffsetFlag [%c]\n", pInputQuote->BidOffsetFlag);
		LOGDEMO("\tAskHedgeFlag [%c]\n", pInputQuote->AskHedgeFlag);
		LOGDEMO("\tBidHedgeFlag [%c]\n", pInputQuote->BidHedgeFlag);
		LOGDEMO("\tAskPrice [%.8lf]\n", pInputQuote->AskPrice);
		LOGDEMO("\tBidPrice [%.8lf]\n", pInputQuote->BidPrice);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("</OnErrRtnQuoteInsert>\n");
};
void Vector::OnErrRtnQuoteAction(CThostFtdcQuoteActionField *pQuoteAction, CThostFtdcRspInfoField *pRspInfo)
{
	LOGDEMO("<OnErrRtnQuoteAction>\n");
	if (pQuoteAction)
	{
		LOGDEMO("\tBrokerID [%s]\n", pQuoteAction->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pQuoteAction->InvestorID);
		LOGDEMO("\tQuoteRef [%s]\n", pQuoteAction->QuoteRef);
		LOGDEMO("\tExchangeID [%s]\n", pQuoteAction->ExchangeID);
		LOGDEMO("\tQuoteSysID [%s]\n", pQuoteAction->QuoteSysID);
		LOGDEMO("\tActionDate [%s]\n", pQuoteAction->ActionDate);
		LOGDEMO("\tActionTime [%s]\n", pQuoteAction->ActionTime);
		LOGDEMO("\tTraderID [%s]\n", pQuoteAction->TraderID);
		LOGDEMO("\tQuoteLocalID [%s]\n", pQuoteAction->QuoteLocalID);
		LOGDEMO("\tActionLocalID [%s]\n", pQuoteAction->ActionLocalID);
		LOGDEMO("\tParticipantID [%s]\n", pQuoteAction->ParticipantID);
		LOGDEMO("\tClientID [%s]\n", pQuoteAction->ClientID);
		LOGDEMO("\tBusinessUnit [%s]\n", pQuoteAction->BusinessUnit);
		LOGDEMO("\tUserID [%s]\n", pQuoteAction->UserID);
		LOGDEMO("\tStatusMsg [%s]\n", pQuoteAction->StatusMsg);
		LOGDEMO("\tInstrumentID [%s]\n", pQuoteAction->InstrumentID);
		LOGDEMO("\tBranchID [%s]\n", pQuoteAction->BranchID);
		LOGDEMO("\tInvestUnitID [%s]\n", pQuoteAction->InvestUnitID);
		LOGDEMO("\tIPAddress [%s]\n", pQuoteAction->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pQuoteAction->MacAddress);
		LOGDEMO("\tQuoteActionRef [%d]\n", pQuoteAction->QuoteActionRef);
		LOGDEMO("\tRequestID [%d]\n", pQuoteAction->RequestID);
		LOGDEMO("\tFrontID [%d]\n", pQuoteAction->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pQuoteAction->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pQuoteAction->InstallID);
		LOGDEMO("\tActionFlag [%c]\n", pQuoteAction->ActionFlag);
		LOGDEMO("\tOrderActionStatus [%c]\n", pQuoteAction->OrderActionStatus);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("</OnErrRtnQuoteAction>\n");
};
void Vector::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
	LOGDEMO("<OnRtnForQuoteRsp>\n");
	if (pForQuoteRsp)
	{
		LOGDEMO("\tTradingDay [%s]\n", pForQuoteRsp->TradingDay);
		LOGDEMO("\tInstrumentID [%s]\n", pForQuoteRsp->InstrumentID);
		LOGDEMO("\tForQuoteSysID [%s]\n", pForQuoteRsp->ForQuoteSysID);
		LOGDEMO("\tForQuoteTime [%s]\n", pForQuoteRsp->ForQuoteTime);
		LOGDEMO("\tActionDay [%s]\n", pForQuoteRsp->ActionDay);
		LOGDEMO("\tExchangeID [%s]\n", pForQuoteRsp->ExchangeID);
	}
	LOGDEMO("</OnRtnForQuoteRsp>\n");
};
void Vector::OnRtnCFMMCTradingAccountToken(CThostFtdcCFMMCTradingAccountTokenField *pCFMMCTradingAccountToken)
{
	LOGDEMO("<OnRtnCFMMCTradingAccountToken>\n");
	if (pCFMMCTradingAccountToken)
	{
		LOGDEMO("\tBrokerID [%s]\n", pCFMMCTradingAccountToken->BrokerID);
		LOGDEMO("\tParticipantID [%s]\n", pCFMMCTradingAccountToken->ParticipantID);
		LOGDEMO("\tAccountID [%s]\n", pCFMMCTradingAccountToken->AccountID);
		LOGDEMO("\tToken [%s]\n", pCFMMCTradingAccountToken->Token);
		LOGDEMO("\tKeyID [%d]\n", pCFMMCTradingAccountToken->KeyID);
	}
	LOGDEMO("</OnRtnCFMMCTradingAccountToken>\n");
};
void Vector::OnErrRtnBatchOrderAction(CThostFtdcBatchOrderActionField *pBatchOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
	LOGDEMO("<OnErrRtnBatchOrderAction>\n");
	if (pBatchOrderAction)
	{
		LOGDEMO("\tBrokerID [%s]\n", pBatchOrderAction->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pBatchOrderAction->InvestorID);
		LOGDEMO("\tExchangeID [%s]\n", pBatchOrderAction->ExchangeID);
		LOGDEMO("\tActionDate [%s]\n", pBatchOrderAction->ActionDate);
		LOGDEMO("\tActionTime [%s]\n", pBatchOrderAction->ActionTime);
		LOGDEMO("\tTraderID [%s]\n", pBatchOrderAction->TraderID);
		LOGDEMO("\tActionLocalID [%s]\n", pBatchOrderAction->ActionLocalID);
		LOGDEMO("\tParticipantID [%s]\n", pBatchOrderAction->ParticipantID);
		LOGDEMO("\tClientID [%s]\n", pBatchOrderAction->ClientID);
		LOGDEMO("\tBusinessUnit [%s]\n", pBatchOrderAction->BusinessUnit);
		LOGDEMO("\tUserID [%s]\n", pBatchOrderAction->UserID);
		LOGDEMO("\tStatusMsg [%s]\n", pBatchOrderAction->StatusMsg);
		LOGDEMO("\tInvestUnitID [%s]\n", pBatchOrderAction->InvestUnitID);
		LOGDEMO("\tIPAddress [%s]\n", pBatchOrderAction->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pBatchOrderAction->MacAddress);
		LOGDEMO("\tOrderActionRef [%d]\n", pBatchOrderAction->OrderActionRef);
		LOGDEMO("\tRequestID [%d]\n", pBatchOrderAction->RequestID);
		LOGDEMO("\tFrontID [%d]\n", pBatchOrderAction->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pBatchOrderAction->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pBatchOrderAction->InstallID);
		LOGDEMO("\tOrderActionStatus [%c]\n", pBatchOrderAction->OrderActionStatus);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("</OnErrRtnBatchOrderAction>\n");
};
void Vector::OnRtnCombAction(CThostFtdcCombActionField *pCombAction)
{
	LOGDEMO("<OnRtnCombAction>\n");
	if (pCombAction)
	{
		LOGDEMO("\tBrokerID [%s]\n", pCombAction->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pCombAction->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pCombAction->InstrumentID);
		LOGDEMO("\tCombActionRef [%s]\n", pCombAction->CombActionRef);
		LOGDEMO("\tUserID [%s]\n", pCombAction->UserID);
		LOGDEMO("\tActionLocalID [%s]\n", pCombAction->ActionLocalID);
		LOGDEMO("\tExchangeID [%s]\n", pCombAction->ExchangeID);
		LOGDEMO("\tParticipantID [%s]\n", pCombAction->ParticipantID);
		LOGDEMO("\tClientID [%s]\n", pCombAction->ClientID);
		LOGDEMO("\tExchangeInstID [%s]\n", pCombAction->ExchangeInstID);
		LOGDEMO("\tTraderID [%s]\n", pCombAction->TraderID);
		LOGDEMO("\tTradingDay [%s]\n", pCombAction->TradingDay);
		LOGDEMO("\tUserProductInfo [%s]\n", pCombAction->UserProductInfo);
		LOGDEMO("\tStatusMsg [%s]\n", pCombAction->StatusMsg);
		LOGDEMO("\tIPAddress [%s]\n", pCombAction->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pCombAction->MacAddress);
		LOGDEMO("\tVolume [%d]\n", pCombAction->Volume);
		LOGDEMO("\tInstallID [%d]\n", pCombAction->InstallID);
		LOGDEMO("\tNotifySequence [%d]\n", pCombAction->NotifySequence);
		LOGDEMO("\tSettlementID [%d]\n", pCombAction->SettlementID);
		LOGDEMO("\tSequenceNo [%d]\n", pCombAction->SequenceNo);
		LOGDEMO("\tFrontID [%d]\n", pCombAction->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pCombAction->SessionID);
		LOGDEMO("\tDirection [%c]\n", pCombAction->Direction);
		LOGDEMO("\tCombDirection [%c]\n", pCombAction->CombDirection);
		LOGDEMO("\tHedgeFlag [%c]\n", pCombAction->HedgeFlag);
		LOGDEMO("\tActionStatus [%c]\n", pCombAction->ActionStatus);
	}
	LOGDEMO("</OnRtnCombAction>\n");
};
void Vector::OnErrRtnCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo)
{
	LOGDEMO("<OnErrRtnCombActionInsert>\n");
	if (pInputCombAction)
	{
		LOGDEMO("\tBrokerID [%s]\n", pInputCombAction->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pInputCombAction->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pInputCombAction->InstrumentID);
		LOGDEMO("\tCombActionRef [%s]\n", pInputCombAction->CombActionRef);
		LOGDEMO("\tUserID [%s]\n", pInputCombAction->UserID);
		LOGDEMO("\tExchangeID [%s]\n", pInputCombAction->ExchangeID);
		LOGDEMO("\tIPAddress [%s]\n", pInputCombAction->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pInputCombAction->MacAddress);
		LOGDEMO("\tVolume [%d]\n", pInputCombAction->Volume);
		LOGDEMO("\tDirection [%c]\n", pInputCombAction->Direction);
		LOGDEMO("\tCombDirection [%c]\n", pInputCombAction->CombDirection);
		LOGDEMO("\tHedgeFlag [%c]\n", pInputCombAction->HedgeFlag);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("</OnErrRtnCombActionInsert>\n");
};
void Vector::OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryContractBank>\n");
	if (pContractBank)
	{
		LOGDEMO("\tBrokerID [%s]\n", pContractBank->BrokerID);
		LOGDEMO("\tBankID [%s]\n", pContractBank->BankID);
		LOGDEMO("\tBankBrchID [%s]\n", pContractBank->BankBrchID);
		LOGDEMO("\tBankName [%s]\n", pContractBank->BankName);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryContractBank>\n");
};

void Vector::OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryParkedOrder>\n");
	if (pParkedOrder)
	{
		LOGDEMO("\tBrokerID [%s]\n", pParkedOrder->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pParkedOrder->InvestorID);
		LOGDEMO("\tInstrumentID [%s]\n", pParkedOrder->InstrumentID);
		LOGDEMO("\tOrderRef [%s]\n", pParkedOrder->OrderRef);
		LOGDEMO("\tUserID [%s]\n", pParkedOrder->UserID);
		LOGDEMO("\tCombOffsetFlag [%s]\n", pParkedOrder->CombOffsetFlag);
		LOGDEMO("\tCombHedgeFlag [%s]\n", pParkedOrder->CombHedgeFlag);
		LOGDEMO("\tGTDDate [%s]\n", pParkedOrder->GTDDate);
		LOGDEMO("\tBusinessUnit [%s]\n", pParkedOrder->BusinessUnit);
		LOGDEMO("\tExchangeID [%s]\n", pParkedOrder->ExchangeID);
		LOGDEMO("\tParkedOrderID [%s]\n", pParkedOrder->ParkedOrderID);
		LOGDEMO("\tErrorMsg [%s]\n", pParkedOrder->ErrorMsg);
		LOGDEMO("\tAccountID [%s]\n", pParkedOrder->AccountID);
		LOGDEMO("\tCurrencyID [%s]\n", pParkedOrder->CurrencyID);
		LOGDEMO("\tClientID [%s]\n", pParkedOrder->ClientID);
		LOGDEMO("\tInvestUnitID [%s]\n", pParkedOrder->InvestUnitID);
		LOGDEMO("\tIPAddress [%s]\n", pParkedOrder->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pParkedOrder->MacAddress);
		LOGDEMO("\tVolumeTotalOriginal [%d]\n", pParkedOrder->VolumeTotalOriginal);
		LOGDEMO("\tMinVolume [%d]\n", pParkedOrder->MinVolume);
		LOGDEMO("\tIsAutoSuspend [%d]\n", pParkedOrder->IsAutoSuspend);
		LOGDEMO("\tRequestID [%d]\n", pParkedOrder->RequestID);
		LOGDEMO("\tUserForceClose [%d]\n", pParkedOrder->UserForceClose);
		LOGDEMO("\tErrorID [%d]\n", pParkedOrder->ErrorID);
		LOGDEMO("\tIsSwapOrder [%d]\n", pParkedOrder->IsSwapOrder);
		LOGDEMO("\tOrderPriceType [%c]\n", pParkedOrder->OrderPriceType);
		LOGDEMO("\tDirection [%c]\n", pParkedOrder->Direction);
		LOGDEMO("\tTimeCondition [%c]\n", pParkedOrder->TimeCondition);
		LOGDEMO("\tVolumeCondition [%c]\n", pParkedOrder->VolumeCondition);
		LOGDEMO("\tContingentCondition [%c]\n", pParkedOrder->ContingentCondition);
		LOGDEMO("\tForceCloseReason [%c]\n", pParkedOrder->ForceCloseReason);
		LOGDEMO("\tUserType [%c]\n", pParkedOrder->UserType);
		LOGDEMO("\tStatus [%c]\n", pParkedOrder->Status);
		LOGDEMO("\tLimitPrice [%.8lf]\n", pParkedOrder->LimitPrice);
		LOGDEMO("\tStopPrice [%.8lf]\n", pParkedOrder->StopPrice);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryParkedOrder>\n");
};

void Vector::OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryParkedOrderAction>\n");
	if (pParkedOrderAction)
	{
		LOGDEMO("\tBrokerID [%s]\n", pParkedOrderAction->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pParkedOrderAction->InvestorID);
		LOGDEMO("\tOrderRef [%s]\n", pParkedOrderAction->OrderRef);
		LOGDEMO("\tExchangeID [%s]\n", pParkedOrderAction->ExchangeID);
		LOGDEMO("\tOrderSysID [%s]\n", pParkedOrderAction->OrderSysID);
		LOGDEMO("\tUserID [%s]\n", pParkedOrderAction->UserID);
		LOGDEMO("\tInstrumentID [%s]\n", pParkedOrderAction->InstrumentID);
		LOGDEMO("\tParkedOrderActionID [%s]\n", pParkedOrderAction->ParkedOrderActionID);
		LOGDEMO("\tErrorMsg [%s]\n", pParkedOrderAction->ErrorMsg);
		LOGDEMO("\tInvestUnitID [%s]\n", pParkedOrderAction->InvestUnitID);
		LOGDEMO("\tIPAddress [%s]\n", pParkedOrderAction->IPAddress);
		LOGDEMO("\tMacAddress [%s]\n", pParkedOrderAction->MacAddress);
		LOGDEMO("\tOrderActionRef [%d]\n", pParkedOrderAction->OrderActionRef);
		LOGDEMO("\tRequestID [%d]\n", pParkedOrderAction->RequestID);
		LOGDEMO("\tFrontID [%d]\n", pParkedOrderAction->FrontID);
		LOGDEMO("\tSessionID [%d]\n", pParkedOrderAction->SessionID);
		LOGDEMO("\tVolumeChange [%d]\n", pParkedOrderAction->VolumeChange);
		LOGDEMO("\tErrorID [%d]\n", pParkedOrderAction->ErrorID);
		LOGDEMO("\tActionFlag [%c]\n", pParkedOrderAction->ActionFlag);
		LOGDEMO("\tUserType [%c]\n", pParkedOrderAction->UserType);
		LOGDEMO("\tStatus [%c]\n", pParkedOrderAction->Status);
		LOGDEMO("\tLimitPrice [%.8lf]\n", pParkedOrderAction->LimitPrice);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryParkedOrderAction>\n");
};

void Vector::OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryTradingNotice>\n");
	if (pTradingNotice)
	{
		LOGDEMO("\tBrokerID [%s]\n", pTradingNotice->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pTradingNotice->InvestorID);
		LOGDEMO("\tUserID [%s]\n", pTradingNotice->UserID);
		LOGDEMO("\tSendTime [%s]\n", pTradingNotice->SendTime);
		LOGDEMO("\tFieldContent [%s]\n", pTradingNotice->FieldContent);
		LOGDEMO("\tSequenceNo [%d]\n", pTradingNotice->SequenceNo);
		LOGDEMO("\tInvestorRange [%c]\n", pTradingNotice->InvestorRange);
		LOGDEMO("\tSequenceSeries [%d]\n", pTradingNotice->SequenceSeries);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryTradingNotice>\n");
};

void Vector::OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryBrokerTradingParams>\n");
	if (pBrokerTradingParams)
	{
		LOGDEMO("\tBrokerID [%s]\n", pBrokerTradingParams->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pBrokerTradingParams->InvestorID);
		LOGDEMO("\tCurrencyID [%s]\n", pBrokerTradingParams->CurrencyID);
		LOGDEMO("\tMarginPriceType [%c]\n", pBrokerTradingParams->MarginPriceType);
		LOGDEMO("\tAlgorithm [%c]\n", pBrokerTradingParams->Algorithm);
		LOGDEMO("\tAvailIncludeCloseProfit [%c]\n", pBrokerTradingParams->AvailIncludeCloseProfit);
		LOGDEMO("\tOptionRoyaltyPriceType [%c]\n", pBrokerTradingParams->OptionRoyaltyPriceType);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryBrokerTradingParams>\n");
};

void Vector::OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQryBrokerTradingAlgos>\n");
	if (pBrokerTradingAlgos)
	{
		LOGDEMO("\tBrokerID [%s]\n", pBrokerTradingAlgos->BrokerID);
		LOGDEMO("\tExchangeID [%s]\n", pBrokerTradingAlgos->ExchangeID);
		LOGDEMO("\tInstrumentID [%s]\n", pBrokerTradingAlgos->InstrumentID);
		LOGDEMO("\tHandlePositionAlgoID [%c]\n", pBrokerTradingAlgos->HandlePositionAlgoID);
		LOGDEMO("\tFindMarginRateAlgoID [%c]\n", pBrokerTradingAlgos->FindMarginRateAlgoID);
		LOGDEMO("\tHandleTradingAccountAlgoID [%c]\n", pBrokerTradingAlgos->HandleTradingAccountAlgoID);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQryBrokerTradingAlgos>\n");
};

void Vector::OnRspQueryCFMMCTradingAccountToken(CThostFtdcQueryCFMMCTradingAccountTokenField *pQueryCFMMCTradingAccountToken, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQueryCFMMCTradingAccountToken>\n");
	if (pQueryCFMMCTradingAccountToken)
	{
		LOGDEMO("\tBrokerID [%s]\n", pQueryCFMMCTradingAccountToken->BrokerID);
		LOGDEMO("\tInvestorID [%s]\n", pQueryCFMMCTradingAccountToken->InvestorID);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQueryCFMMCTradingAccountToken>\n");
};

void Vector::OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer)
{
	LOGDEMO("<OnRtnFromBankToFutureByBank>\n");
	if (pRspTransfer)
	{
		LOGDEMO("\tTradeCode [%s]\n", pRspTransfer->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pRspTransfer->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pRspTransfer->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pRspTransfer->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pRspTransfer->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pRspTransfer->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pRspTransfer->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pRspTransfer->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pRspTransfer->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pRspTransfer->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pRspTransfer->IdentifiedCardNo);
		LOGDEMO("\tBankAccount [%s]\n", pRspTransfer->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pRspTransfer->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pRspTransfer->AccountID);
		LOGDEMO("\tPassword [%s]\n", pRspTransfer->Password);
		LOGDEMO("\tUserID [%s]\n", pRspTransfer->UserID);
		LOGDEMO("\tCurrencyID [%s]\n", pRspTransfer->CurrencyID);
		LOGDEMO("\tMessage [%s]\n", pRspTransfer->Message);
		LOGDEMO("\tDigest [%s]\n", pRspTransfer->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pRspTransfer->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pRspTransfer->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pRspTransfer->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pRspTransfer->OperNo);
		LOGDEMO("\tErrorMsg [%s]\n", pRspTransfer->ErrorMsg);
		LOGDEMO("\tLongCustomerName [%s]\n", pRspTransfer->LongCustomerName);
		LOGDEMO("\tPlateSerial [%d]\n", pRspTransfer->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pRspTransfer->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pRspTransfer->InstallID);
		LOGDEMO("\tFutureSerial [%d]\n", pRspTransfer->FutureSerial);
		LOGDEMO("\tRequestID [%d]\n", pRspTransfer->RequestID);
		LOGDEMO("\tTID [%d]\n", pRspTransfer->TID);
		LOGDEMO("\tErrorID [%d]\n", pRspTransfer->ErrorID);
		LOGDEMO("\tLastFragment [%c]\n", pRspTransfer->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pRspTransfer->IdCardType);
		LOGDEMO("\tCustType [%c]\n", pRspTransfer->CustType);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pRspTransfer->VerifyCertNoFlag);
		LOGDEMO("\tFeePayFlag [%c]\n", pRspTransfer->FeePayFlag);
		LOGDEMO("\tBankAccType [%c]\n", pRspTransfer->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pRspTransfer->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pRspTransfer->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pRspTransfer->SecuPwdFlag);
		LOGDEMO("\tTransferStatus [%c]\n", pRspTransfer->TransferStatus);
		LOGDEMO("\tTradeAmount [%.8lf]\n", pRspTransfer->TradeAmount);
		LOGDEMO("\tFutureFetchAmount [%.8lf]\n", pRspTransfer->FutureFetchAmount);
		LOGDEMO("\tCustFee [%.8lf]\n", pRspTransfer->CustFee);
		LOGDEMO("\tBrokerFee [%.8lf]\n", pRspTransfer->BrokerFee);
	}
	LOGDEMO("</OnRtnFromBankToFutureByBank>\n");
};
void Vector::OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer)
{
	LOGDEMO("<OnRtnFromFutureToBankByBank>\n");
	if (pRspTransfer)
	{
		LOGDEMO("\tTradeCode [%s]\n", pRspTransfer->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pRspTransfer->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pRspTransfer->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pRspTransfer->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pRspTransfer->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pRspTransfer->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pRspTransfer->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pRspTransfer->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pRspTransfer->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pRspTransfer->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pRspTransfer->IdentifiedCardNo);
		LOGDEMO("\tBankAccount [%s]\n", pRspTransfer->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pRspTransfer->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pRspTransfer->AccountID);
		LOGDEMO("\tPassword [%s]\n", pRspTransfer->Password);
		LOGDEMO("\tUserID [%s]\n", pRspTransfer->UserID);
		LOGDEMO("\tCurrencyID [%s]\n", pRspTransfer->CurrencyID);
		LOGDEMO("\tMessage [%s]\n", pRspTransfer->Message);
		LOGDEMO("\tDigest [%s]\n", pRspTransfer->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pRspTransfer->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pRspTransfer->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pRspTransfer->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pRspTransfer->OperNo);
		LOGDEMO("\tErrorMsg [%s]\n", pRspTransfer->ErrorMsg);
		LOGDEMO("\tLongCustomerName [%s]\n", pRspTransfer->LongCustomerName);
		LOGDEMO("\tPlateSerial [%d]\n", pRspTransfer->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pRspTransfer->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pRspTransfer->InstallID);
		LOGDEMO("\tFutureSerial [%d]\n", pRspTransfer->FutureSerial);
		LOGDEMO("\tRequestID [%d]\n", pRspTransfer->RequestID);
		LOGDEMO("\tTID [%d]\n", pRspTransfer->TID);
		LOGDEMO("\tErrorID [%d]\n", pRspTransfer->ErrorID);
		LOGDEMO("\tLastFragment [%c]\n", pRspTransfer->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pRspTransfer->IdCardType);
		LOGDEMO("\tCustType [%c]\n", pRspTransfer->CustType);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pRspTransfer->VerifyCertNoFlag);
		LOGDEMO("\tFeePayFlag [%c]\n", pRspTransfer->FeePayFlag);
		LOGDEMO("\tBankAccType [%c]\n", pRspTransfer->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pRspTransfer->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pRspTransfer->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pRspTransfer->SecuPwdFlag);
		LOGDEMO("\tTransferStatus [%c]\n", pRspTransfer->TransferStatus);
		LOGDEMO("\tTradeAmount [%.8lf]\n", pRspTransfer->TradeAmount);
		LOGDEMO("\tFutureFetchAmount [%.8lf]\n", pRspTransfer->FutureFetchAmount);
		LOGDEMO("\tCustFee [%.8lf]\n", pRspTransfer->CustFee);
		LOGDEMO("\tBrokerFee [%.8lf]\n", pRspTransfer->BrokerFee);
	}
	LOGDEMO("</OnRtnFromFutureToBankByBank>\n");
};
void Vector::OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal)
{
	LOGDEMO("<OnRtnRepealFromBankToFutureByBank>\n");
	if (pRspRepeal)
	{
		LOGDEMO("\tBankRepealSerial [%s]\n", pRspRepeal->BankRepealSerial);
		LOGDEMO("\tTradeCode [%s]\n", pRspRepeal->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pRspRepeal->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pRspRepeal->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pRspRepeal->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pRspRepeal->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pRspRepeal->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pRspRepeal->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pRspRepeal->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pRspRepeal->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pRspRepeal->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pRspRepeal->IdentifiedCardNo);
		LOGDEMO("\tBankAccount [%s]\n", pRspRepeal->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pRspRepeal->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pRspRepeal->AccountID);
		LOGDEMO("\tPassword [%s]\n", pRspRepeal->Password);
		LOGDEMO("\tUserID [%s]\n", pRspRepeal->UserID);
		LOGDEMO("\tCurrencyID [%s]\n", pRspRepeal->CurrencyID);
		LOGDEMO("\tMessage [%s]\n", pRspRepeal->Message);
		LOGDEMO("\tDigest [%s]\n", pRspRepeal->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pRspRepeal->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pRspRepeal->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pRspRepeal->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pRspRepeal->OperNo);
		LOGDEMO("\tErrorMsg [%s]\n", pRspRepeal->ErrorMsg);
		LOGDEMO("\tLongCustomerName [%s]\n", pRspRepeal->LongCustomerName);
		LOGDEMO("\tRepealTimeInterval [%d]\n", pRspRepeal->RepealTimeInterval);
		LOGDEMO("\tRepealedTimes [%d]\n", pRspRepeal->RepealedTimes);
		LOGDEMO("\tPlateRepealSerial [%d]\n", pRspRepeal->PlateRepealSerial);
		LOGDEMO("\tFutureRepealSerial [%d]\n", pRspRepeal->FutureRepealSerial);
		LOGDEMO("\tPlateSerial [%d]\n", pRspRepeal->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pRspRepeal->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pRspRepeal->InstallID);
		LOGDEMO("\tFutureSerial [%d]\n", pRspRepeal->FutureSerial);
		LOGDEMO("\tRequestID [%d]\n", pRspRepeal->RequestID);
		LOGDEMO("\tTID [%d]\n", pRspRepeal->TID);
		LOGDEMO("\tErrorID [%d]\n", pRspRepeal->ErrorID);
		LOGDEMO("\tBankRepealFlag [%c]\n", pRspRepeal->BankRepealFlag);
		LOGDEMO("\tBrokerRepealFlag [%c]\n", pRspRepeal->BrokerRepealFlag);
		LOGDEMO("\tLastFragment [%c]\n", pRspRepeal->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pRspRepeal->IdCardType);
		LOGDEMO("\tCustType [%c]\n", pRspRepeal->CustType);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pRspRepeal->VerifyCertNoFlag);
		LOGDEMO("\tFeePayFlag [%c]\n", pRspRepeal->FeePayFlag);
		LOGDEMO("\tBankAccType [%c]\n", pRspRepeal->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pRspRepeal->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pRspRepeal->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pRspRepeal->SecuPwdFlag);
		LOGDEMO("\tTransferStatus [%c]\n", pRspRepeal->TransferStatus);
		LOGDEMO("\tTradeAmount [%.8lf]\n", pRspRepeal->TradeAmount);
		LOGDEMO("\tFutureFetchAmount [%.8lf]\n", pRspRepeal->FutureFetchAmount);
		LOGDEMO("\tCustFee [%.8lf]\n", pRspRepeal->CustFee);
		LOGDEMO("\tBrokerFee [%.8lf]\n", pRspRepeal->BrokerFee);
	}
	LOGDEMO("</OnRtnRepealFromBankToFutureByBank>\n");
};
void Vector::OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal)
{
	LOGDEMO("<OnRtnRepealFromFutureToBankByBank>\n");
	if (pRspRepeal)
	{
		LOGDEMO("\tBankRepealSerial [%s]\n", pRspRepeal->BankRepealSerial);
		LOGDEMO("\tTradeCode [%s]\n", pRspRepeal->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pRspRepeal->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pRspRepeal->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pRspRepeal->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pRspRepeal->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pRspRepeal->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pRspRepeal->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pRspRepeal->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pRspRepeal->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pRspRepeal->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pRspRepeal->IdentifiedCardNo);
		LOGDEMO("\tBankAccount [%s]\n", pRspRepeal->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pRspRepeal->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pRspRepeal->AccountID);
		LOGDEMO("\tPassword [%s]\n", pRspRepeal->Password);
		LOGDEMO("\tUserID [%s]\n", pRspRepeal->UserID);
		LOGDEMO("\tCurrencyID [%s]\n", pRspRepeal->CurrencyID);
		LOGDEMO("\tMessage [%s]\n", pRspRepeal->Message);
		LOGDEMO("\tDigest [%s]\n", pRspRepeal->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pRspRepeal->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pRspRepeal->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pRspRepeal->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pRspRepeal->OperNo);
		LOGDEMO("\tErrorMsg [%s]\n", pRspRepeal->ErrorMsg);
		LOGDEMO("\tLongCustomerName [%s]\n", pRspRepeal->LongCustomerName);
		LOGDEMO("\tRepealTimeInterval [%d]\n", pRspRepeal->RepealTimeInterval);
		LOGDEMO("\tRepealedTimes [%d]\n", pRspRepeal->RepealedTimes);
		LOGDEMO("\tPlateRepealSerial [%d]\n", pRspRepeal->PlateRepealSerial);
		LOGDEMO("\tFutureRepealSerial [%d]\n", pRspRepeal->FutureRepealSerial);
		LOGDEMO("\tPlateSerial [%d]\n", pRspRepeal->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pRspRepeal->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pRspRepeal->InstallID);
		LOGDEMO("\tFutureSerial [%d]\n", pRspRepeal->FutureSerial);
		LOGDEMO("\tRequestID [%d]\n", pRspRepeal->RequestID);
		LOGDEMO("\tTID [%d]\n", pRspRepeal->TID);
		LOGDEMO("\tErrorID [%d]\n", pRspRepeal->ErrorID);
		LOGDEMO("\tBankRepealFlag [%c]\n", pRspRepeal->BankRepealFlag);
		LOGDEMO("\tBrokerRepealFlag [%c]\n", pRspRepeal->BrokerRepealFlag);
		LOGDEMO("\tLastFragment [%c]\n", pRspRepeal->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pRspRepeal->IdCardType);
		LOGDEMO("\tCustType [%c]\n", pRspRepeal->CustType);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pRspRepeal->VerifyCertNoFlag);
		LOGDEMO("\tFeePayFlag [%c]\n", pRspRepeal->FeePayFlag);
		LOGDEMO("\tBankAccType [%c]\n", pRspRepeal->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pRspRepeal->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pRspRepeal->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pRspRepeal->SecuPwdFlag);
		LOGDEMO("\tTransferStatus [%c]\n", pRspRepeal->TransferStatus);
		LOGDEMO("\tTradeAmount [%.8lf]\n", pRspRepeal->TradeAmount);
		LOGDEMO("\tFutureFetchAmount [%.8lf]\n", pRspRepeal->FutureFetchAmount);
		LOGDEMO("\tCustFee [%.8lf]\n", pRspRepeal->CustFee);
		LOGDEMO("\tBrokerFee [%.8lf]\n", pRspRepeal->BrokerFee);
	}
	LOGDEMO("</OnRtnRepealFromFutureToBankByBank>\n");
};
void Vector::OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer)
{
	LOGDEMO("<OnRtnFromBankToFutureByFuture>\n");
	if (pRspTransfer)
	{
		LOGDEMO("\tTradeCode [%s]\n", pRspTransfer->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pRspTransfer->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pRspTransfer->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pRspTransfer->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pRspTransfer->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pRspTransfer->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pRspTransfer->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pRspTransfer->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pRspTransfer->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pRspTransfer->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pRspTransfer->IdentifiedCardNo);
		LOGDEMO("\tBankAccount [%s]\n", pRspTransfer->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pRspTransfer->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pRspTransfer->AccountID);
		LOGDEMO("\tPassword [%s]\n", pRspTransfer->Password);
		LOGDEMO("\tUserID [%s]\n", pRspTransfer->UserID);
		LOGDEMO("\tCurrencyID [%s]\n", pRspTransfer->CurrencyID);
		LOGDEMO("\tMessage [%s]\n", pRspTransfer->Message);
		LOGDEMO("\tDigest [%s]\n", pRspTransfer->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pRspTransfer->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pRspTransfer->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pRspTransfer->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pRspTransfer->OperNo);
		LOGDEMO("\tErrorMsg [%s]\n", pRspTransfer->ErrorMsg);
		LOGDEMO("\tLongCustomerName [%s]\n", pRspTransfer->LongCustomerName);
		LOGDEMO("\tPlateSerial [%d]\n", pRspTransfer->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pRspTransfer->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pRspTransfer->InstallID);
		LOGDEMO("\tFutureSerial [%d]\n", pRspTransfer->FutureSerial);
		LOGDEMO("\tRequestID [%d]\n", pRspTransfer->RequestID);
		LOGDEMO("\tTID [%d]\n", pRspTransfer->TID);
		LOGDEMO("\tErrorID [%d]\n", pRspTransfer->ErrorID);
		LOGDEMO("\tLastFragment [%c]\n", pRspTransfer->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pRspTransfer->IdCardType);
		LOGDEMO("\tCustType [%c]\n", pRspTransfer->CustType);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pRspTransfer->VerifyCertNoFlag);
		LOGDEMO("\tFeePayFlag [%c]\n", pRspTransfer->FeePayFlag);
		LOGDEMO("\tBankAccType [%c]\n", pRspTransfer->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pRspTransfer->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pRspTransfer->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pRspTransfer->SecuPwdFlag);
		LOGDEMO("\tTransferStatus [%c]\n", pRspTransfer->TransferStatus);
		LOGDEMO("\tTradeAmount [%.8lf]\n", pRspTransfer->TradeAmount);
		LOGDEMO("\tFutureFetchAmount [%.8lf]\n", pRspTransfer->FutureFetchAmount);
		LOGDEMO("\tCustFee [%.8lf]\n", pRspTransfer->CustFee);
		LOGDEMO("\tBrokerFee [%.8lf]\n", pRspTransfer->BrokerFee);
	}
	LOGDEMO("</OnRtnFromBankToFutureByFuture>\n");
};
void Vector::OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer)
{
	LOGDEMO("<OnRtnFromFutureToBankByFuture>\n");
	if (pRspTransfer)
	{
		LOGDEMO("\tTradeCode [%s]\n", pRspTransfer->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pRspTransfer->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pRspTransfer->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pRspTransfer->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pRspTransfer->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pRspTransfer->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pRspTransfer->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pRspTransfer->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pRspTransfer->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pRspTransfer->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pRspTransfer->IdentifiedCardNo);
		LOGDEMO("\tBankAccount [%s]\n", pRspTransfer->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pRspTransfer->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pRspTransfer->AccountID);
		LOGDEMO("\tPassword [%s]\n", pRspTransfer->Password);
		LOGDEMO("\tUserID [%s]\n", pRspTransfer->UserID);
		LOGDEMO("\tCurrencyID [%s]\n", pRspTransfer->CurrencyID);
		LOGDEMO("\tMessage [%s]\n", pRspTransfer->Message);
		LOGDEMO("\tDigest [%s]\n", pRspTransfer->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pRspTransfer->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pRspTransfer->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pRspTransfer->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pRspTransfer->OperNo);
		LOGDEMO("\tErrorMsg [%s]\n", pRspTransfer->ErrorMsg);
		LOGDEMO("\tLongCustomerName [%s]\n", pRspTransfer->LongCustomerName);
		LOGDEMO("\tPlateSerial [%d]\n", pRspTransfer->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pRspTransfer->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pRspTransfer->InstallID);
		LOGDEMO("\tFutureSerial [%d]\n", pRspTransfer->FutureSerial);
		LOGDEMO("\tRequestID [%d]\n", pRspTransfer->RequestID);
		LOGDEMO("\tTID [%d]\n", pRspTransfer->TID);
		LOGDEMO("\tErrorID [%d]\n", pRspTransfer->ErrorID);
		LOGDEMO("\tLastFragment [%c]\n", pRspTransfer->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pRspTransfer->IdCardType);
		LOGDEMO("\tCustType [%c]\n", pRspTransfer->CustType);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pRspTransfer->VerifyCertNoFlag);
		LOGDEMO("\tFeePayFlag [%c]\n", pRspTransfer->FeePayFlag);
		LOGDEMO("\tBankAccType [%c]\n", pRspTransfer->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pRspTransfer->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pRspTransfer->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pRspTransfer->SecuPwdFlag);
		LOGDEMO("\tTransferStatus [%c]\n", pRspTransfer->TransferStatus);
		LOGDEMO("\tTradeAmount [%.8lf]\n", pRspTransfer->TradeAmount);
		LOGDEMO("\tFutureFetchAmount [%.8lf]\n", pRspTransfer->FutureFetchAmount);
		LOGDEMO("\tCustFee [%.8lf]\n", pRspTransfer->CustFee);
		LOGDEMO("\tBrokerFee [%.8lf]\n", pRspTransfer->BrokerFee);
	}
	LOGDEMO("</OnRtnFromFutureToBankByFuture>\n");
};
void Vector::OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal)
{
	LOGDEMO("<OnRtnRepealFromBankToFutureByFutureManual>\n");
	if (pRspRepeal)
	{
		LOGDEMO("\tBankRepealSerial [%s]\n", pRspRepeal->BankRepealSerial);
		LOGDEMO("\tTradeCode [%s]\n", pRspRepeal->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pRspRepeal->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pRspRepeal->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pRspRepeal->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pRspRepeal->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pRspRepeal->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pRspRepeal->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pRspRepeal->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pRspRepeal->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pRspRepeal->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pRspRepeal->IdentifiedCardNo);
		LOGDEMO("\tBankAccount [%s]\n", pRspRepeal->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pRspRepeal->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pRspRepeal->AccountID);
		LOGDEMO("\tPassword [%s]\n", pRspRepeal->Password);
		LOGDEMO("\tUserID [%s]\n", pRspRepeal->UserID);
		LOGDEMO("\tCurrencyID [%s]\n", pRspRepeal->CurrencyID);
		LOGDEMO("\tMessage [%s]\n", pRspRepeal->Message);
		LOGDEMO("\tDigest [%s]\n", pRspRepeal->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pRspRepeal->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pRspRepeal->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pRspRepeal->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pRspRepeal->OperNo);
		LOGDEMO("\tErrorMsg [%s]\n", pRspRepeal->ErrorMsg);
		LOGDEMO("\tLongCustomerName [%s]\n", pRspRepeal->LongCustomerName);
		LOGDEMO("\tRepealTimeInterval [%d]\n", pRspRepeal->RepealTimeInterval);
		LOGDEMO("\tRepealedTimes [%d]\n", pRspRepeal->RepealedTimes);
		LOGDEMO("\tPlateRepealSerial [%d]\n", pRspRepeal->PlateRepealSerial);
		LOGDEMO("\tFutureRepealSerial [%d]\n", pRspRepeal->FutureRepealSerial);
		LOGDEMO("\tPlateSerial [%d]\n", pRspRepeal->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pRspRepeal->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pRspRepeal->InstallID);
		LOGDEMO("\tFutureSerial [%d]\n", pRspRepeal->FutureSerial);
		LOGDEMO("\tRequestID [%d]\n", pRspRepeal->RequestID);
		LOGDEMO("\tTID [%d]\n", pRspRepeal->TID);
		LOGDEMO("\tErrorID [%d]\n", pRspRepeal->ErrorID);
		LOGDEMO("\tBankRepealFlag [%c]\n", pRspRepeal->BankRepealFlag);
		LOGDEMO("\tBrokerRepealFlag [%c]\n", pRspRepeal->BrokerRepealFlag);
		LOGDEMO("\tLastFragment [%c]\n", pRspRepeal->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pRspRepeal->IdCardType);
		LOGDEMO("\tCustType [%c]\n", pRspRepeal->CustType);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pRspRepeal->VerifyCertNoFlag);
		LOGDEMO("\tFeePayFlag [%c]\n", pRspRepeal->FeePayFlag);
		LOGDEMO("\tBankAccType [%c]\n", pRspRepeal->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pRspRepeal->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pRspRepeal->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pRspRepeal->SecuPwdFlag);
		LOGDEMO("\tTransferStatus [%c]\n", pRspRepeal->TransferStatus);
		LOGDEMO("\tTradeAmount [%.8lf]\n", pRspRepeal->TradeAmount);
		LOGDEMO("\tFutureFetchAmount [%.8lf]\n", pRspRepeal->FutureFetchAmount);
		LOGDEMO("\tCustFee [%.8lf]\n", pRspRepeal->CustFee);
		LOGDEMO("\tBrokerFee [%.8lf]\n", pRspRepeal->BrokerFee);
	}
	LOGDEMO("</OnRtnRepealFromBankToFutureByFutureManual>\n");
};
void Vector::OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal)
{
	LOGDEMO("<OnRtnRepealFromFutureToBankByFutureManual>\n");
	if (pRspRepeal)
	{
		LOGDEMO("\tBankRepealSerial [%s]\n", pRspRepeal->BankRepealSerial);
		LOGDEMO("\tTradeCode [%s]\n", pRspRepeal->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pRspRepeal->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pRspRepeal->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pRspRepeal->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pRspRepeal->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pRspRepeal->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pRspRepeal->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pRspRepeal->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pRspRepeal->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pRspRepeal->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pRspRepeal->IdentifiedCardNo);
		LOGDEMO("\tBankAccount [%s]\n", pRspRepeal->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pRspRepeal->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pRspRepeal->AccountID);
		LOGDEMO("\tPassword [%s]\n", pRspRepeal->Password);
		LOGDEMO("\tUserID [%s]\n", pRspRepeal->UserID);
		LOGDEMO("\tCurrencyID [%s]\n", pRspRepeal->CurrencyID);
		LOGDEMO("\tMessage [%s]\n", pRspRepeal->Message);
		LOGDEMO("\tDigest [%s]\n", pRspRepeal->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pRspRepeal->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pRspRepeal->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pRspRepeal->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pRspRepeal->OperNo);
		LOGDEMO("\tErrorMsg [%s]\n", pRspRepeal->ErrorMsg);
		LOGDEMO("\tLongCustomerName [%s]\n", pRspRepeal->LongCustomerName);
		LOGDEMO("\tRepealTimeInterval [%d]\n", pRspRepeal->RepealTimeInterval);
		LOGDEMO("\tRepealedTimes [%d]\n", pRspRepeal->RepealedTimes);
		LOGDEMO("\tPlateRepealSerial [%d]\n", pRspRepeal->PlateRepealSerial);
		LOGDEMO("\tFutureRepealSerial [%d]\n", pRspRepeal->FutureRepealSerial);
		LOGDEMO("\tPlateSerial [%d]\n", pRspRepeal->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pRspRepeal->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pRspRepeal->InstallID);
		LOGDEMO("\tFutureSerial [%d]\n", pRspRepeal->FutureSerial);
		LOGDEMO("\tRequestID [%d]\n", pRspRepeal->RequestID);
		LOGDEMO("\tTID [%d]\n", pRspRepeal->TID);
		LOGDEMO("\tErrorID [%d]\n", pRspRepeal->ErrorID);
		LOGDEMO("\tBankRepealFlag [%c]\n", pRspRepeal->BankRepealFlag);
		LOGDEMO("\tBrokerRepealFlag [%c]\n", pRspRepeal->BrokerRepealFlag);
		LOGDEMO("\tLastFragment [%c]\n", pRspRepeal->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pRspRepeal->IdCardType);
		LOGDEMO("\tCustType [%c]\n", pRspRepeal->CustType);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pRspRepeal->VerifyCertNoFlag);
		LOGDEMO("\tFeePayFlag [%c]\n", pRspRepeal->FeePayFlag);
		LOGDEMO("\tBankAccType [%c]\n", pRspRepeal->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pRspRepeal->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pRspRepeal->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pRspRepeal->SecuPwdFlag);
		LOGDEMO("\tTransferStatus [%c]\n", pRspRepeal->TransferStatus);
		LOGDEMO("\tTradeAmount [%.8lf]\n", pRspRepeal->TradeAmount);
		LOGDEMO("\tFutureFetchAmount [%.8lf]\n", pRspRepeal->FutureFetchAmount);
		LOGDEMO("\tCustFee [%.8lf]\n", pRspRepeal->CustFee);
		LOGDEMO("\tBrokerFee [%.8lf]\n", pRspRepeal->BrokerFee);
	}
	LOGDEMO("</OnRtnRepealFromFutureToBankByFutureManual>\n");
};
void Vector::OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount)
{
	LOGDEMO("<OnRtnQueryBankBalanceByFuture>\n");
	if (pNotifyQueryAccount)
	{
		LOGDEMO("\tTradeCode [%s]\n", pNotifyQueryAccount->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pNotifyQueryAccount->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pNotifyQueryAccount->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pNotifyQueryAccount->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pNotifyQueryAccount->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pNotifyQueryAccount->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pNotifyQueryAccount->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pNotifyQueryAccount->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pNotifyQueryAccount->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pNotifyQueryAccount->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pNotifyQueryAccount->IdentifiedCardNo);
		LOGDEMO("\tBankAccount [%s]\n", pNotifyQueryAccount->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pNotifyQueryAccount->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pNotifyQueryAccount->AccountID);
		LOGDEMO("\tPassword [%s]\n", pNotifyQueryAccount->Password);
		LOGDEMO("\tUserID [%s]\n", pNotifyQueryAccount->UserID);
		LOGDEMO("\tCurrencyID [%s]\n", pNotifyQueryAccount->CurrencyID);
		LOGDEMO("\tDigest [%s]\n", pNotifyQueryAccount->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pNotifyQueryAccount->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pNotifyQueryAccount->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pNotifyQueryAccount->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pNotifyQueryAccount->OperNo);
		LOGDEMO("\tErrorMsg [%s]\n", pNotifyQueryAccount->ErrorMsg);
		LOGDEMO("\tLongCustomerName [%s]\n", pNotifyQueryAccount->LongCustomerName);
		LOGDEMO("\tPlateSerial [%d]\n", pNotifyQueryAccount->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pNotifyQueryAccount->SessionID);
		LOGDEMO("\tFutureSerial [%d]\n", pNotifyQueryAccount->FutureSerial);
		LOGDEMO("\tInstallID [%d]\n", pNotifyQueryAccount->InstallID);
		LOGDEMO("\tRequestID [%d]\n", pNotifyQueryAccount->RequestID);
		LOGDEMO("\tTID [%d]\n", pNotifyQueryAccount->TID);
		LOGDEMO("\tErrorID [%d]\n", pNotifyQueryAccount->ErrorID);
		LOGDEMO("\tLastFragment [%c]\n", pNotifyQueryAccount->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pNotifyQueryAccount->IdCardType);
		LOGDEMO("\tCustType [%c]\n", pNotifyQueryAccount->CustType);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pNotifyQueryAccount->VerifyCertNoFlag);
		LOGDEMO("\tBankAccType [%c]\n", pNotifyQueryAccount->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pNotifyQueryAccount->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pNotifyQueryAccount->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pNotifyQueryAccount->SecuPwdFlag);
		LOGDEMO("\tBankUseAmount [%.8lf]\n", pNotifyQueryAccount->BankUseAmount);
		LOGDEMO("\tBankFetchAmount [%.8lf]\n", pNotifyQueryAccount->BankFetchAmount);
	}
	LOGDEMO("</OnRtnQueryBankBalanceByFuture>\n");
};
void Vector::OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo)
{
	LOGDEMO("<OnErrRtnBankToFutureByFuture>\n");
	if (pReqTransfer)
	{
		LOGDEMO("\tTradeCode [%s]\n", pReqTransfer->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pReqTransfer->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pReqTransfer->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pReqTransfer->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pReqTransfer->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pReqTransfer->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pReqTransfer->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pReqTransfer->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pReqTransfer->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pReqTransfer->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pReqTransfer->IdentifiedCardNo);
		LOGDEMO("\tBankAccount [%s]\n", pReqTransfer->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pReqTransfer->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pReqTransfer->AccountID);
		LOGDEMO("\tPassword [%s]\n", pReqTransfer->Password);
		LOGDEMO("\tUserID [%s]\n", pReqTransfer->UserID);
		LOGDEMO("\tCurrencyID [%s]\n", pReqTransfer->CurrencyID);
		LOGDEMO("\tMessage [%s]\n", pReqTransfer->Message);
		LOGDEMO("\tDigest [%s]\n", pReqTransfer->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pReqTransfer->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pReqTransfer->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pReqTransfer->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pReqTransfer->OperNo);
		LOGDEMO("\tLongCustomerName [%s]\n", pReqTransfer->LongCustomerName);
		LOGDEMO("\tPlateSerial [%d]\n", pReqTransfer->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pReqTransfer->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pReqTransfer->InstallID);
		LOGDEMO("\tFutureSerial [%d]\n", pReqTransfer->FutureSerial);
		LOGDEMO("\tRequestID [%d]\n", pReqTransfer->RequestID);
		LOGDEMO("\tTID [%d]\n", pReqTransfer->TID);
		LOGDEMO("\tLastFragment [%c]\n", pReqTransfer->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pReqTransfer->IdCardType);
		LOGDEMO("\tCustType [%c]\n", pReqTransfer->CustType);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pReqTransfer->VerifyCertNoFlag);
		LOGDEMO("\tFeePayFlag [%c]\n", pReqTransfer->FeePayFlag);
		LOGDEMO("\tBankAccType [%c]\n", pReqTransfer->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pReqTransfer->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pReqTransfer->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pReqTransfer->SecuPwdFlag);
		LOGDEMO("\tTransferStatus [%c]\n", pReqTransfer->TransferStatus);
		LOGDEMO("\tTradeAmount [%.8lf]\n", pReqTransfer->TradeAmount);
		LOGDEMO("\tFutureFetchAmount [%.8lf]\n", pReqTransfer->FutureFetchAmount);
		LOGDEMO("\tCustFee [%.8lf]\n", pReqTransfer->CustFee);
		LOGDEMO("\tBrokerFee [%.8lf]\n", pReqTransfer->BrokerFee);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("</OnErrRtnBankToFutureByFuture>\n");
};
void Vector::OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo)
{
	LOGDEMO("<OnErrRtnFutureToBankByFuture>\n");
	if (pReqTransfer)
	{
		LOGDEMO("\tTradeCode [%s]\n", pReqTransfer->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pReqTransfer->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pReqTransfer->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pReqTransfer->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pReqTransfer->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pReqTransfer->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pReqTransfer->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pReqTransfer->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pReqTransfer->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pReqTransfer->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pReqTransfer->IdentifiedCardNo);
		LOGDEMO("\tBankAccount [%s]\n", pReqTransfer->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pReqTransfer->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pReqTransfer->AccountID);
		LOGDEMO("\tPassword [%s]\n", pReqTransfer->Password);
		LOGDEMO("\tUserID [%s]\n", pReqTransfer->UserID);
		LOGDEMO("\tCurrencyID [%s]\n", pReqTransfer->CurrencyID);
		LOGDEMO("\tMessage [%s]\n", pReqTransfer->Message);
		LOGDEMO("\tDigest [%s]\n", pReqTransfer->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pReqTransfer->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pReqTransfer->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pReqTransfer->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pReqTransfer->OperNo);
		LOGDEMO("\tLongCustomerName [%s]\n", pReqTransfer->LongCustomerName);
		LOGDEMO("\tPlateSerial [%d]\n", pReqTransfer->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pReqTransfer->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pReqTransfer->InstallID);
		LOGDEMO("\tFutureSerial [%d]\n", pReqTransfer->FutureSerial);
		LOGDEMO("\tRequestID [%d]\n", pReqTransfer->RequestID);
		LOGDEMO("\tTID [%d]\n", pReqTransfer->TID);
		LOGDEMO("\tLastFragment [%c]\n", pReqTransfer->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pReqTransfer->IdCardType);
		LOGDEMO("\tCustType [%c]\n", pReqTransfer->CustType);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pReqTransfer->VerifyCertNoFlag);
		LOGDEMO("\tFeePayFlag [%c]\n", pReqTransfer->FeePayFlag);
		LOGDEMO("\tBankAccType [%c]\n", pReqTransfer->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pReqTransfer->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pReqTransfer->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pReqTransfer->SecuPwdFlag);
		LOGDEMO("\tTransferStatus [%c]\n", pReqTransfer->TransferStatus);
		LOGDEMO("\tTradeAmount [%.8lf]\n", pReqTransfer->TradeAmount);
		LOGDEMO("\tFutureFetchAmount [%.8lf]\n", pReqTransfer->FutureFetchAmount);
		LOGDEMO("\tCustFee [%.8lf]\n", pReqTransfer->CustFee);
		LOGDEMO("\tBrokerFee [%.8lf]\n", pReqTransfer->BrokerFee);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("</OnErrRtnFutureToBankByFuture>\n");
};
void Vector::OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo)
{
	LOGDEMO("<OnErrRtnRepealBankToFutureByFutureManual>\n");
	if (pReqRepeal)
	{
		LOGDEMO("\tBankRepealSerial [%s]\n", pReqRepeal->BankRepealSerial);
		LOGDEMO("\tTradeCode [%s]\n", pReqRepeal->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pReqRepeal->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pReqRepeal->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pReqRepeal->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pReqRepeal->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pReqRepeal->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pReqRepeal->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pReqRepeal->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pReqRepeal->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pReqRepeal->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pReqRepeal->IdentifiedCardNo);
		LOGDEMO("\tBankAccount [%s]\n", pReqRepeal->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pReqRepeal->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pReqRepeal->AccountID);
		LOGDEMO("\tPassword [%s]\n", pReqRepeal->Password);
		LOGDEMO("\tUserID [%s]\n", pReqRepeal->UserID);
		LOGDEMO("\tCurrencyID [%s]\n", pReqRepeal->CurrencyID);
		LOGDEMO("\tMessage [%s]\n", pReqRepeal->Message);
		LOGDEMO("\tDigest [%s]\n", pReqRepeal->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pReqRepeal->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pReqRepeal->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pReqRepeal->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pReqRepeal->OperNo);
		LOGDEMO("\tLongCustomerName [%s]\n", pReqRepeal->LongCustomerName);
		LOGDEMO("\tRepealTimeInterval [%d]\n", pReqRepeal->RepealTimeInterval);
		LOGDEMO("\tRepealedTimes [%d]\n", pReqRepeal->RepealedTimes);
		LOGDEMO("\tPlateRepealSerial [%d]\n", pReqRepeal->PlateRepealSerial);
		LOGDEMO("\tFutureRepealSerial [%d]\n", pReqRepeal->FutureRepealSerial);
		LOGDEMO("\tPlateSerial [%d]\n", pReqRepeal->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pReqRepeal->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pReqRepeal->InstallID);
		LOGDEMO("\tFutureSerial [%d]\n", pReqRepeal->FutureSerial);
		LOGDEMO("\tRequestID [%d]\n", pReqRepeal->RequestID);
		LOGDEMO("\tTID [%d]\n", pReqRepeal->TID);
		LOGDEMO("\tBankRepealFlag [%c]\n", pReqRepeal->BankRepealFlag);
		LOGDEMO("\tBrokerRepealFlag [%c]\n", pReqRepeal->BrokerRepealFlag);
		LOGDEMO("\tLastFragment [%c]\n", pReqRepeal->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pReqRepeal->IdCardType);
		LOGDEMO("\tCustType [%c]\n", pReqRepeal->CustType);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pReqRepeal->VerifyCertNoFlag);
		LOGDEMO("\tFeePayFlag [%c]\n", pReqRepeal->FeePayFlag);
		LOGDEMO("\tBankAccType [%c]\n", pReqRepeal->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pReqRepeal->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pReqRepeal->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pReqRepeal->SecuPwdFlag);
		LOGDEMO("\tTransferStatus [%c]\n", pReqRepeal->TransferStatus);
		LOGDEMO("\tTradeAmount [%.8lf]\n", pReqRepeal->TradeAmount);
		LOGDEMO("\tFutureFetchAmount [%.8lf]\n", pReqRepeal->FutureFetchAmount);
		LOGDEMO("\tCustFee [%.8lf]\n", pReqRepeal->CustFee);
		LOGDEMO("\tBrokerFee [%.8lf]\n", pReqRepeal->BrokerFee);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("</OnErrRtnRepealBankToFutureByFutureManual>\n");
};
void Vector::OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo)
{
	LOGDEMO("<OnErrRtnRepealFutureToBankByFutureManual>\n");
	if (pReqRepeal)
	{
		LOGDEMO("\tBankRepealSerial [%s]\n", pReqRepeal->BankRepealSerial);
		LOGDEMO("\tTradeCode [%s]\n", pReqRepeal->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pReqRepeal->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pReqRepeal->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pReqRepeal->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pReqRepeal->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pReqRepeal->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pReqRepeal->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pReqRepeal->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pReqRepeal->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pReqRepeal->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pReqRepeal->IdentifiedCardNo);
		LOGDEMO("\tBankAccount [%s]\n", pReqRepeal->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pReqRepeal->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pReqRepeal->AccountID);
		LOGDEMO("\tPassword [%s]\n", pReqRepeal->Password);
		LOGDEMO("\tUserID [%s]\n", pReqRepeal->UserID);
		LOGDEMO("\tCurrencyID [%s]\n", pReqRepeal->CurrencyID);
		LOGDEMO("\tMessage [%s]\n", pReqRepeal->Message);
		LOGDEMO("\tDigest [%s]\n", pReqRepeal->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pReqRepeal->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pReqRepeal->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pReqRepeal->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pReqRepeal->OperNo);
		LOGDEMO("\tLongCustomerName [%s]\n", pReqRepeal->LongCustomerName);
		LOGDEMO("\tRepealTimeInterval [%d]\n", pReqRepeal->RepealTimeInterval);
		LOGDEMO("\tRepealedTimes [%d]\n", pReqRepeal->RepealedTimes);
		LOGDEMO("\tPlateRepealSerial [%d]\n", pReqRepeal->PlateRepealSerial);
		LOGDEMO("\tFutureRepealSerial [%d]\n", pReqRepeal->FutureRepealSerial);
		LOGDEMO("\tPlateSerial [%d]\n", pReqRepeal->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pReqRepeal->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pReqRepeal->InstallID);
		LOGDEMO("\tFutureSerial [%d]\n", pReqRepeal->FutureSerial);
		LOGDEMO("\tRequestID [%d]\n", pReqRepeal->RequestID);
		LOGDEMO("\tTID [%d]\n", pReqRepeal->TID);
		LOGDEMO("\tBankRepealFlag [%c]\n", pReqRepeal->BankRepealFlag);
		LOGDEMO("\tBrokerRepealFlag [%c]\n", pReqRepeal->BrokerRepealFlag);
		LOGDEMO("\tLastFragment [%c]\n", pReqRepeal->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pReqRepeal->IdCardType);
		LOGDEMO("\tCustType [%c]\n", pReqRepeal->CustType);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pReqRepeal->VerifyCertNoFlag);
		LOGDEMO("\tFeePayFlag [%c]\n", pReqRepeal->FeePayFlag);
		LOGDEMO("\tBankAccType [%c]\n", pReqRepeal->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pReqRepeal->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pReqRepeal->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pReqRepeal->SecuPwdFlag);
		LOGDEMO("\tTransferStatus [%c]\n", pReqRepeal->TransferStatus);
		LOGDEMO("\tTradeAmount [%.8lf]\n", pReqRepeal->TradeAmount);
		LOGDEMO("\tFutureFetchAmount [%.8lf]\n", pReqRepeal->FutureFetchAmount);
		LOGDEMO("\tCustFee [%.8lf]\n", pReqRepeal->CustFee);
		LOGDEMO("\tBrokerFee [%.8lf]\n", pReqRepeal->BrokerFee);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("</OnErrRtnRepealFutureToBankByFutureManual>\n");
};
void Vector::OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo)
{
	LOGDEMO("<OnErrRtnQueryBankBalanceByFuture>\n");
	if (pReqQueryAccount)
	{
		LOGDEMO("\tTradeCode [%s]\n", pReqQueryAccount->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pReqQueryAccount->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pReqQueryAccount->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pReqQueryAccount->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pReqQueryAccount->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pReqQueryAccount->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pReqQueryAccount->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pReqQueryAccount->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pReqQueryAccount->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pReqQueryAccount->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pReqQueryAccount->IdentifiedCardNo);
		LOGDEMO("\tBankAccount [%s]\n", pReqQueryAccount->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pReqQueryAccount->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pReqQueryAccount->AccountID);
		LOGDEMO("\tPassword [%s]\n", pReqQueryAccount->Password);
		LOGDEMO("\tUserID [%s]\n", pReqQueryAccount->UserID);
		LOGDEMO("\tCurrencyID [%s]\n", pReqQueryAccount->CurrencyID);
		LOGDEMO("\tDigest [%s]\n", pReqQueryAccount->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pReqQueryAccount->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pReqQueryAccount->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pReqQueryAccount->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pReqQueryAccount->OperNo);
		LOGDEMO("\tLongCustomerName [%s]\n", pReqQueryAccount->LongCustomerName);
		LOGDEMO("\tPlateSerial [%d]\n", pReqQueryAccount->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pReqQueryAccount->SessionID);
		LOGDEMO("\tFutureSerial [%d]\n", pReqQueryAccount->FutureSerial);
		LOGDEMO("\tInstallID [%d]\n", pReqQueryAccount->InstallID);
		LOGDEMO("\tRequestID [%d]\n", pReqQueryAccount->RequestID);
		LOGDEMO("\tTID [%d]\n", pReqQueryAccount->TID);
		LOGDEMO("\tLastFragment [%c]\n", pReqQueryAccount->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pReqQueryAccount->IdCardType);
		LOGDEMO("\tCustType [%c]\n", pReqQueryAccount->CustType);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pReqQueryAccount->VerifyCertNoFlag);
		LOGDEMO("\tBankAccType [%c]\n", pReqQueryAccount->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pReqQueryAccount->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pReqQueryAccount->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pReqQueryAccount->SecuPwdFlag);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("</OnErrRtnQueryBankBalanceByFuture>\n");
};
void Vector::OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal)
{
	LOGDEMO("<OnRtnRepealFromBankToFutureByFuture>\n");
	if (pRspRepeal)
	{
		LOGDEMO("\tBankRepealSerial [%s]\n", pRspRepeal->BankRepealSerial);
		LOGDEMO("\tTradeCode [%s]\n", pRspRepeal->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pRspRepeal->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pRspRepeal->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pRspRepeal->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pRspRepeal->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pRspRepeal->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pRspRepeal->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pRspRepeal->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pRspRepeal->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pRspRepeal->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pRspRepeal->IdentifiedCardNo);
		LOGDEMO("\tBankAccount [%s]\n", pRspRepeal->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pRspRepeal->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pRspRepeal->AccountID);
		LOGDEMO("\tPassword [%s]\n", pRspRepeal->Password);
		LOGDEMO("\tUserID [%s]\n", pRspRepeal->UserID);
		LOGDEMO("\tCurrencyID [%s]\n", pRspRepeal->CurrencyID);
		LOGDEMO("\tMessage [%s]\n", pRspRepeal->Message);
		LOGDEMO("\tDigest [%s]\n", pRspRepeal->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pRspRepeal->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pRspRepeal->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pRspRepeal->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pRspRepeal->OperNo);
		LOGDEMO("\tErrorMsg [%s]\n", pRspRepeal->ErrorMsg);
		LOGDEMO("\tLongCustomerName [%s]\n", pRspRepeal->LongCustomerName);
		LOGDEMO("\tRepealTimeInterval [%d]\n", pRspRepeal->RepealTimeInterval);
		LOGDEMO("\tRepealedTimes [%d]\n", pRspRepeal->RepealedTimes);
		LOGDEMO("\tPlateRepealSerial [%d]\n", pRspRepeal->PlateRepealSerial);
		LOGDEMO("\tFutureRepealSerial [%d]\n", pRspRepeal->FutureRepealSerial);
		LOGDEMO("\tPlateSerial [%d]\n", pRspRepeal->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pRspRepeal->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pRspRepeal->InstallID);
		LOGDEMO("\tFutureSerial [%d]\n", pRspRepeal->FutureSerial);
		LOGDEMO("\tRequestID [%d]\n", pRspRepeal->RequestID);
		LOGDEMO("\tTID [%d]\n", pRspRepeal->TID);
		LOGDEMO("\tErrorID [%d]\n", pRspRepeal->ErrorID);
		LOGDEMO("\tBankRepealFlag [%c]\n", pRspRepeal->BankRepealFlag);
		LOGDEMO("\tBrokerRepealFlag [%c]\n", pRspRepeal->BrokerRepealFlag);
		LOGDEMO("\tLastFragment [%c]\n", pRspRepeal->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pRspRepeal->IdCardType);
		LOGDEMO("\tCustType [%c]\n", pRspRepeal->CustType);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pRspRepeal->VerifyCertNoFlag);
		LOGDEMO("\tFeePayFlag [%c]\n", pRspRepeal->FeePayFlag);
		LOGDEMO("\tBankAccType [%c]\n", pRspRepeal->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pRspRepeal->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pRspRepeal->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pRspRepeal->SecuPwdFlag);
		LOGDEMO("\tTransferStatus [%c]\n", pRspRepeal->TransferStatus);
		LOGDEMO("\tTradeAmount [%.8lf]\n", pRspRepeal->TradeAmount);
		LOGDEMO("\tFutureFetchAmount [%.8lf]\n", pRspRepeal->FutureFetchAmount);
		LOGDEMO("\tCustFee [%.8lf]\n", pRspRepeal->CustFee);
		LOGDEMO("\tBrokerFee [%.8lf]\n", pRspRepeal->BrokerFee);
	}
	LOGDEMO("</OnRtnRepealFromBankToFutureByFuture>\n");
};
void Vector::OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal)
{
	LOGDEMO("<OnRtnRepealFromFutureToBankByFuture>\n");
	if (pRspRepeal)
	{
		LOGDEMO("\tBankRepealSerial [%s]\n", pRspRepeal->BankRepealSerial);
		LOGDEMO("\tTradeCode [%s]\n", pRspRepeal->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pRspRepeal->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pRspRepeal->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pRspRepeal->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pRspRepeal->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pRspRepeal->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pRspRepeal->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pRspRepeal->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pRspRepeal->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pRspRepeal->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pRspRepeal->IdentifiedCardNo);
		LOGDEMO("\tBankAccount [%s]\n", pRspRepeal->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pRspRepeal->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pRspRepeal->AccountID);
		LOGDEMO("\tPassword [%s]\n", pRspRepeal->Password);
		LOGDEMO("\tUserID [%s]\n", pRspRepeal->UserID);
		LOGDEMO("\tCurrencyID [%s]\n", pRspRepeal->CurrencyID);
		LOGDEMO("\tMessage [%s]\n", pRspRepeal->Message);
		LOGDEMO("\tDigest [%s]\n", pRspRepeal->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pRspRepeal->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pRspRepeal->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pRspRepeal->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pRspRepeal->OperNo);
		LOGDEMO("\tErrorMsg [%s]\n", pRspRepeal->ErrorMsg);
		LOGDEMO("\tLongCustomerName [%s]\n", pRspRepeal->LongCustomerName);
		LOGDEMO("\tRepealTimeInterval [%d]\n", pRspRepeal->RepealTimeInterval);
		LOGDEMO("\tRepealedTimes [%d]\n", pRspRepeal->RepealedTimes);
		LOGDEMO("\tPlateRepealSerial [%d]\n", pRspRepeal->PlateRepealSerial);
		LOGDEMO("\tFutureRepealSerial [%d]\n", pRspRepeal->FutureRepealSerial);
		LOGDEMO("\tPlateSerial [%d]\n", pRspRepeal->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pRspRepeal->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pRspRepeal->InstallID);
		LOGDEMO("\tFutureSerial [%d]\n", pRspRepeal->FutureSerial);
		LOGDEMO("\tRequestID [%d]\n", pRspRepeal->RequestID);
		LOGDEMO("\tTID [%d]\n", pRspRepeal->TID);
		LOGDEMO("\tErrorID [%d]\n", pRspRepeal->ErrorID);
		LOGDEMO("\tBankRepealFlag [%c]\n", pRspRepeal->BankRepealFlag);
		LOGDEMO("\tBrokerRepealFlag [%c]\n", pRspRepeal->BrokerRepealFlag);
		LOGDEMO("\tLastFragment [%c]\n", pRspRepeal->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pRspRepeal->IdCardType);
		LOGDEMO("\tCustType [%c]\n", pRspRepeal->CustType);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pRspRepeal->VerifyCertNoFlag);
		LOGDEMO("\tFeePayFlag [%c]\n", pRspRepeal->FeePayFlag);
		LOGDEMO("\tBankAccType [%c]\n", pRspRepeal->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pRspRepeal->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pRspRepeal->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pRspRepeal->SecuPwdFlag);
		LOGDEMO("\tTransferStatus [%c]\n", pRspRepeal->TransferStatus);
		LOGDEMO("\tTradeAmount [%.8lf]\n", pRspRepeal->TradeAmount);
		LOGDEMO("\tFutureFetchAmount [%.8lf]\n", pRspRepeal->FutureFetchAmount);
		LOGDEMO("\tCustFee [%.8lf]\n", pRspRepeal->CustFee);
		LOGDEMO("\tBrokerFee [%.8lf]\n", pRspRepeal->BrokerFee);
	}
	LOGDEMO("</OnRtnRepealFromFutureToBankByFuture>\n");
};
void Vector::OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspFromBankToFutureByFuture>\n");
	if (pReqTransfer)
	{
		LOGDEMO("\tTradeCode [%s]\n", pReqTransfer->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pReqTransfer->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pReqTransfer->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pReqTransfer->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pReqTransfer->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pReqTransfer->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pReqTransfer->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pReqTransfer->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pReqTransfer->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pReqTransfer->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pReqTransfer->IdentifiedCardNo);
		LOGDEMO("\tBankAccount [%s]\n", pReqTransfer->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pReqTransfer->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pReqTransfer->AccountID);
		LOGDEMO("\tPassword [%s]\n", pReqTransfer->Password);
		LOGDEMO("\tUserID [%s]\n", pReqTransfer->UserID);
		LOGDEMO("\tCurrencyID [%s]\n", pReqTransfer->CurrencyID);
		LOGDEMO("\tMessage [%s]\n", pReqTransfer->Message);
		LOGDEMO("\tDigest [%s]\n", pReqTransfer->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pReqTransfer->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pReqTransfer->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pReqTransfer->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pReqTransfer->OperNo);
		LOGDEMO("\tLongCustomerName [%s]\n", pReqTransfer->LongCustomerName);
		LOGDEMO("\tPlateSerial [%d]\n", pReqTransfer->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pReqTransfer->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pReqTransfer->InstallID);
		LOGDEMO("\tFutureSerial [%d]\n", pReqTransfer->FutureSerial);
		LOGDEMO("\tRequestID [%d]\n", pReqTransfer->RequestID);
		LOGDEMO("\tTID [%d]\n", pReqTransfer->TID);
		LOGDEMO("\tLastFragment [%c]\n", pReqTransfer->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pReqTransfer->IdCardType);
		LOGDEMO("\tCustType [%c]\n", pReqTransfer->CustType);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pReqTransfer->VerifyCertNoFlag);
		LOGDEMO("\tFeePayFlag [%c]\n", pReqTransfer->FeePayFlag);
		LOGDEMO("\tBankAccType [%c]\n", pReqTransfer->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pReqTransfer->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pReqTransfer->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pReqTransfer->SecuPwdFlag);
		LOGDEMO("\tTransferStatus [%c]\n", pReqTransfer->TransferStatus);
		LOGDEMO("\tTradeAmount [%.8lf]\n", pReqTransfer->TradeAmount);
		LOGDEMO("\tFutureFetchAmount [%.8lf]\n", pReqTransfer->FutureFetchAmount);
		LOGDEMO("\tCustFee [%.8lf]\n", pReqTransfer->CustFee);
		LOGDEMO("\tBrokerFee [%.8lf]\n", pReqTransfer->BrokerFee);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspFromBankToFutureByFuture>\n");
};

void Vector::OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspFromFutureToBankByFuture>\n");
	if (pReqTransfer)
	{
		LOGDEMO("\tTradeCode [%s]\n", pReqTransfer->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pReqTransfer->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pReqTransfer->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pReqTransfer->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pReqTransfer->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pReqTransfer->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pReqTransfer->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pReqTransfer->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pReqTransfer->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pReqTransfer->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pReqTransfer->IdentifiedCardNo);
		LOGDEMO("\tBankAccount [%s]\n", pReqTransfer->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pReqTransfer->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pReqTransfer->AccountID);
		LOGDEMO("\tPassword [%s]\n", pReqTransfer->Password);
		LOGDEMO("\tUserID [%s]\n", pReqTransfer->UserID);
		LOGDEMO("\tCurrencyID [%s]\n", pReqTransfer->CurrencyID);
		LOGDEMO("\tMessage [%s]\n", pReqTransfer->Message);
		LOGDEMO("\tDigest [%s]\n", pReqTransfer->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pReqTransfer->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pReqTransfer->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pReqTransfer->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pReqTransfer->OperNo);
		LOGDEMO("\tLongCustomerName [%s]\n", pReqTransfer->LongCustomerName);
		LOGDEMO("\tPlateSerial [%d]\n", pReqTransfer->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pReqTransfer->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pReqTransfer->InstallID);
		LOGDEMO("\tFutureSerial [%d]\n", pReqTransfer->FutureSerial);
		LOGDEMO("\tRequestID [%d]\n", pReqTransfer->RequestID);
		LOGDEMO("\tTID [%d]\n", pReqTransfer->TID);
		LOGDEMO("\tLastFragment [%c]\n", pReqTransfer->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pReqTransfer->IdCardType);
		LOGDEMO("\tCustType [%c]\n", pReqTransfer->CustType);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pReqTransfer->VerifyCertNoFlag);
		LOGDEMO("\tFeePayFlag [%c]\n", pReqTransfer->FeePayFlag);
		LOGDEMO("\tBankAccType [%c]\n", pReqTransfer->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pReqTransfer->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pReqTransfer->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pReqTransfer->SecuPwdFlag);
		LOGDEMO("\tTransferStatus [%c]\n", pReqTransfer->TransferStatus);
		LOGDEMO("\tTradeAmount [%.8lf]\n", pReqTransfer->TradeAmount);
		LOGDEMO("\tFutureFetchAmount [%.8lf]\n", pReqTransfer->FutureFetchAmount);
		LOGDEMO("\tCustFee [%.8lf]\n", pReqTransfer->CustFee);
		LOGDEMO("\tBrokerFee [%.8lf]\n", pReqTransfer->BrokerFee);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspFromFutureToBankByFuture>\n");
};

void Vector::OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOGDEMO("<OnRspQueryBankAccountMoneyByFuture>\n");
	if (pReqQueryAccount)
	{
		LOGDEMO("\tTradeCode [%s]\n", pReqQueryAccount->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pReqQueryAccount->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pReqQueryAccount->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pReqQueryAccount->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pReqQueryAccount->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pReqQueryAccount->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pReqQueryAccount->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pReqQueryAccount->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pReqQueryAccount->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pReqQueryAccount->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pReqQueryAccount->IdentifiedCardNo);
		LOGDEMO("\tBankAccount [%s]\n", pReqQueryAccount->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pReqQueryAccount->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pReqQueryAccount->AccountID);
		LOGDEMO("\tPassword [%s]\n", pReqQueryAccount->Password);
		LOGDEMO("\tUserID [%s]\n", pReqQueryAccount->UserID);
		LOGDEMO("\tCurrencyID [%s]\n", pReqQueryAccount->CurrencyID);
		LOGDEMO("\tDigest [%s]\n", pReqQueryAccount->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pReqQueryAccount->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pReqQueryAccount->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pReqQueryAccount->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pReqQueryAccount->OperNo);
		LOGDEMO("\tLongCustomerName [%s]\n", pReqQueryAccount->LongCustomerName);
		LOGDEMO("\tPlateSerial [%d]\n", pReqQueryAccount->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pReqQueryAccount->SessionID);
		LOGDEMO("\tFutureSerial [%d]\n", pReqQueryAccount->FutureSerial);
		LOGDEMO("\tInstallID [%d]\n", pReqQueryAccount->InstallID);
		LOGDEMO("\tRequestID [%d]\n", pReqQueryAccount->RequestID);
		LOGDEMO("\tTID [%d]\n", pReqQueryAccount->TID);
		LOGDEMO("\tLastFragment [%c]\n", pReqQueryAccount->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pReqQueryAccount->IdCardType);
		LOGDEMO("\tCustType [%c]\n", pReqQueryAccount->CustType);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pReqQueryAccount->VerifyCertNoFlag);
		LOGDEMO("\tBankAccType [%c]\n", pReqQueryAccount->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pReqQueryAccount->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pReqQueryAccount->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pReqQueryAccount->SecuPwdFlag);
	}
	if (pRspInfo)
	{
		LOGDEMO("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
		LOGDEMO("\tErrorID [%d]\n", pRspInfo->ErrorID);
	}
	LOGDEMO("\tnRequestID [%d]\n", nRequestID);
	LOGDEMO("\tbIsLast [%d]\n", bIsLast);
	LOGDEMO("</OnRspQueryBankAccountMoneyByFuture>\n");
};

void Vector::OnRtnOpenAccountByBank(CThostFtdcOpenAccountField *pOpenAccount)
{
	LOGDEMO("<OnRtnOpenAccountByBank>\n");
	if (pOpenAccount)
	{
		LOGDEMO("\tTradeCode [%s]\n", pOpenAccount->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pOpenAccount->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pOpenAccount->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pOpenAccount->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pOpenAccount->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pOpenAccount->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pOpenAccount->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pOpenAccount->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pOpenAccount->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pOpenAccount->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pOpenAccount->IdentifiedCardNo);
		LOGDEMO("\tCountryCode [%s]\n", pOpenAccount->CountryCode);
		LOGDEMO("\tAddress [%s]\n", pOpenAccount->Address);
		LOGDEMO("\tZipCode [%s]\n", pOpenAccount->ZipCode);
		LOGDEMO("\tTelephone [%s]\n", pOpenAccount->Telephone);
		LOGDEMO("\tMobilePhone [%s]\n", pOpenAccount->MobilePhone);
		LOGDEMO("\tFax [%s]\n", pOpenAccount->Fax);
		LOGDEMO("\tEMail [%s]\n", pOpenAccount->EMail);
		LOGDEMO("\tBankAccount [%s]\n", pOpenAccount->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pOpenAccount->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pOpenAccount->AccountID);
		LOGDEMO("\tPassword [%s]\n", pOpenAccount->Password);
		LOGDEMO("\tCurrencyID [%s]\n", pOpenAccount->CurrencyID);
		LOGDEMO("\tDigest [%s]\n", pOpenAccount->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pOpenAccount->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pOpenAccount->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pOpenAccount->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pOpenAccount->OperNo);
		LOGDEMO("\tUserID [%s]\n", pOpenAccount->UserID);
		LOGDEMO("\tErrorMsg [%s]\n", pOpenAccount->ErrorMsg);
		LOGDEMO("\tLongCustomerName [%s]\n", pOpenAccount->LongCustomerName);
		LOGDEMO("\tPlateSerial [%d]\n", pOpenAccount->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pOpenAccount->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pOpenAccount->InstallID);
		LOGDEMO("\tTID [%d]\n", pOpenAccount->TID);
		LOGDEMO("\tErrorID [%d]\n", pOpenAccount->ErrorID);
		LOGDEMO("\tLastFragment [%c]\n", pOpenAccount->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pOpenAccount->IdCardType);
		LOGDEMO("\tGender [%c]\n", pOpenAccount->Gender);
		LOGDEMO("\tCustType [%c]\n", pOpenAccount->CustType);
		LOGDEMO("\tMoneyAccountStatus [%c]\n", pOpenAccount->MoneyAccountStatus);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pOpenAccount->VerifyCertNoFlag);
		LOGDEMO("\tCashExchangeCode [%c]\n", pOpenAccount->CashExchangeCode);
		LOGDEMO("\tBankAccType [%c]\n", pOpenAccount->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pOpenAccount->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pOpenAccount->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pOpenAccount->SecuPwdFlag);
	}
	LOGDEMO("</OnRtnOpenAccountByBank>\n");
};
void Vector::OnRtnCancelAccountByBank(CThostFtdcCancelAccountField *pCancelAccount)
{
	LOGDEMO("<OnRtnCancelAccountByBank>\n");
	if (pCancelAccount)
	{
		LOGDEMO("\tTradeCode [%s]\n", pCancelAccount->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pCancelAccount->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pCancelAccount->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pCancelAccount->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pCancelAccount->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pCancelAccount->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pCancelAccount->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pCancelAccount->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pCancelAccount->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pCancelAccount->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pCancelAccount->IdentifiedCardNo);
		LOGDEMO("\tCountryCode [%s]\n", pCancelAccount->CountryCode);
		LOGDEMO("\tAddress [%s]\n", pCancelAccount->Address);
		LOGDEMO("\tZipCode [%s]\n", pCancelAccount->ZipCode);
		LOGDEMO("\tTelephone [%s]\n", pCancelAccount->Telephone);
		LOGDEMO("\tMobilePhone [%s]\n", pCancelAccount->MobilePhone);
		LOGDEMO("\tFax [%s]\n", pCancelAccount->Fax);
		LOGDEMO("\tEMail [%s]\n", pCancelAccount->EMail);
		LOGDEMO("\tBankAccount [%s]\n", pCancelAccount->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pCancelAccount->BankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pCancelAccount->AccountID);
		LOGDEMO("\tPassword [%s]\n", pCancelAccount->Password);
		LOGDEMO("\tCurrencyID [%s]\n", pCancelAccount->CurrencyID);
		LOGDEMO("\tDigest [%s]\n", pCancelAccount->Digest);
		LOGDEMO("\tDeviceID [%s]\n", pCancelAccount->DeviceID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pCancelAccount->BrokerIDByBank);
		LOGDEMO("\tBankSecuAcc [%s]\n", pCancelAccount->BankSecuAcc);
		LOGDEMO("\tOperNo [%s]\n", pCancelAccount->OperNo);
		LOGDEMO("\tUserID [%s]\n", pCancelAccount->UserID);
		LOGDEMO("\tErrorMsg [%s]\n", pCancelAccount->ErrorMsg);
		LOGDEMO("\tLongCustomerName [%s]\n", pCancelAccount->LongCustomerName);
		LOGDEMO("\tPlateSerial [%d]\n", pCancelAccount->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pCancelAccount->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pCancelAccount->InstallID);
		LOGDEMO("\tTID [%d]\n", pCancelAccount->TID);
		LOGDEMO("\tErrorID [%d]\n", pCancelAccount->ErrorID);
		LOGDEMO("\tLastFragment [%c]\n", pCancelAccount->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pCancelAccount->IdCardType);
		LOGDEMO("\tGender [%c]\n", pCancelAccount->Gender);
		LOGDEMO("\tCustType [%c]\n", pCancelAccount->CustType);
		LOGDEMO("\tMoneyAccountStatus [%c]\n", pCancelAccount->MoneyAccountStatus);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pCancelAccount->VerifyCertNoFlag);
		LOGDEMO("\tCashExchangeCode [%c]\n", pCancelAccount->CashExchangeCode);
		LOGDEMO("\tBankAccType [%c]\n", pCancelAccount->BankAccType);
		LOGDEMO("\tBankSecuAccType [%c]\n", pCancelAccount->BankSecuAccType);
		LOGDEMO("\tBankPwdFlag [%c]\n", pCancelAccount->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pCancelAccount->SecuPwdFlag);
	}
	LOGDEMO("</OnRtnCancelAccountByBank>\n");
};
void Vector::OnRtnChangeAccountByBank(CThostFtdcChangeAccountField *pChangeAccount)
{
	LOGDEMO("<OnRtnChangeAccountByBank>\n");
	if (pChangeAccount)
	{
		LOGDEMO("\tTradeCode [%s]\n", pChangeAccount->TradeCode);
		LOGDEMO("\tBankID [%s]\n", pChangeAccount->BankID);
		LOGDEMO("\tBankBranchID [%s]\n", pChangeAccount->BankBranchID);
		LOGDEMO("\tBrokerID [%s]\n", pChangeAccount->BrokerID);
		LOGDEMO("\tBrokerBranchID [%s]\n", pChangeAccount->BrokerBranchID);
		LOGDEMO("\tTradeDate [%s]\n", pChangeAccount->TradeDate);
		LOGDEMO("\tTradeTime [%s]\n", pChangeAccount->TradeTime);
		LOGDEMO("\tBankSerial [%s]\n", pChangeAccount->BankSerial);
		LOGDEMO("\tTradingDay [%s]\n", pChangeAccount->TradingDay);
		LOGDEMO("\tCustomerName [%s]\n", pChangeAccount->CustomerName);
		LOGDEMO("\tIdentifiedCardNo [%s]\n", pChangeAccount->IdentifiedCardNo);
		LOGDEMO("\tCountryCode [%s]\n", pChangeAccount->CountryCode);
		LOGDEMO("\tAddress [%s]\n", pChangeAccount->Address);
		LOGDEMO("\tZipCode [%s]\n", pChangeAccount->ZipCode);
		LOGDEMO("\tTelephone [%s]\n", pChangeAccount->Telephone);
		LOGDEMO("\tMobilePhone [%s]\n", pChangeAccount->MobilePhone);
		LOGDEMO("\tFax [%s]\n", pChangeAccount->Fax);
		LOGDEMO("\tEMail [%s]\n", pChangeAccount->EMail);
		LOGDEMO("\tBankAccount [%s]\n", pChangeAccount->BankAccount);
		LOGDEMO("\tBankPassWord [%s]\n", pChangeAccount->BankPassWord);
		LOGDEMO("\tNewBankAccount [%s]\n", pChangeAccount->NewBankAccount);
		LOGDEMO("\tNewBankPassWord [%s]\n", pChangeAccount->NewBankPassWord);
		LOGDEMO("\tAccountID [%s]\n", pChangeAccount->AccountID);
		LOGDEMO("\tPassword [%s]\n", pChangeAccount->Password);
		LOGDEMO("\tCurrencyID [%s]\n", pChangeAccount->CurrencyID);
		LOGDEMO("\tBrokerIDByBank [%s]\n", pChangeAccount->BrokerIDByBank);
		LOGDEMO("\tDigest [%s]\n", pChangeAccount->Digest);
		LOGDEMO("\tErrorMsg [%s]\n", pChangeAccount->ErrorMsg);
		LOGDEMO("\tLongCustomerName [%s]\n", pChangeAccount->LongCustomerName);
		LOGDEMO("\tPlateSerial [%d]\n", pChangeAccount->PlateSerial);
		LOGDEMO("\tSessionID [%d]\n", pChangeAccount->SessionID);
		LOGDEMO("\tInstallID [%d]\n", pChangeAccount->InstallID);
		LOGDEMO("\tTID [%d]\n", pChangeAccount->TID);
		LOGDEMO("\tErrorID [%d]\n", pChangeAccount->ErrorID);
		LOGDEMO("\tLastFragment [%c]\n", pChangeAccount->LastFragment);
		LOGDEMO("\tIdCardType [%c]\n", pChangeAccount->IdCardType);
		LOGDEMO("\tGender [%c]\n", pChangeAccount->Gender);
		LOGDEMO("\tCustType [%c]\n", pChangeAccount->CustType);
		LOGDEMO("\tMoneyAccountStatus [%c]\n", pChangeAccount->MoneyAccountStatus);
		LOGDEMO("\tBankAccType [%c]\n", pChangeAccount->BankAccType);
		LOGDEMO("\tVerifyCertNoFlag [%c]\n", pChangeAccount->VerifyCertNoFlag);
		LOGDEMO("\tBankPwdFlag [%c]\n", pChangeAccount->BankPwdFlag);
		LOGDEMO("\tSecuPwdFlag [%c]\n", pChangeAccount->SecuPwdFlag);
	}
	LOGDEMO("</OnRtnChangeAccountByBank>\n");
};
