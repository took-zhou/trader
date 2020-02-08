

#include "PrintStruct.h"

void printCThostFtdcInputOrderField(CThostFtdcInputOrderField *order){
    std::cout << "*******struct data of CThostFtdcInputOrderField*******" << std::endl;
    std::cout << "BrokerID:           " << order->BrokerID << std::endl;
    std::cout << "InvestorID:         " << order->InvestorID << std::endl;
    std::cout << "InstrumentID:       " << order->InstrumentID << std::endl;
    std::cout << "OrderRef:           " << order->OrderRef << std::endl;
    std::cout << "UserID:             " << order->UserID << std::endl;
    std::cout << "OrderPriceType:     " << order->OrderPriceType << std::endl;
    std::cout << "Direction:          " << order->Direction << std::endl;
    std::cout << "CombOffsetFlag:     " << order->CombOffsetFlag << std::endl;
    std::cout << "CombHedgeFlag:      " << order->CombHedgeFlag << std::endl;
    std::cout << "LimitPrice:         " << order->LimitPrice << std::endl;
    std::cout << "VolumeTotalOriginal:" << order->VolumeTotalOriginal << std::endl;
    std::cout << "TimeCondition:      " << order->TimeCondition << std::endl;
    std::cout << "GTDDate:            " << order->GTDDate << std::endl;
    std::cout << "VolumeCondition:    " << order->VolumeCondition << std::endl;
    std::cout << "MinVolume:          " << order->MinVolume << std::endl;
    std::cout << "ContingentCondition:" << order->ContingentCondition << std::endl;
    std::cout << "StopPrice:          " << order->StopPrice << std::endl;
    std::cout << "ForceCloseReason:   " << order->ForceCloseReason << std::endl;
    std::cout << "IsAutoSuspend:      " << order->IsAutoSuspend << std::endl;
    std::cout << "BusinessUnit:       " << order->BusinessUnit << std::endl;
    std::cout << "RequestID:          " << order->RequestID << std::endl;
    std::cout << "UserForceClose:     " << order->UserForceClose << std::endl;
    std::cout << "IsSwapOrder:        " << order->IsSwapOrder << std::endl;
    std::cout << "ExchangeID:         " << order->ExchangeID << std::endl;
    std::cout << "InvestUnitID:       " << order->InvestUnitID << std::endl;
    std::cout << "AccountID:          " << order->AccountID << std::endl;
    std::cout << "CurrencyID:         " << order->CurrencyID << std::endl;
    std::cout << "ClientID:           " << order->ClientID << std::endl;
    std::cout << "IPAddress:          " << order->IPAddress << std::endl;
    std::cout << "MacAddress:         " << order->MacAddress << std::endl;

    std::cout << "****************************************" << std::endl;


}

void printContract(Contract *order){
    std::cout << "*******struct data of Contract*******" << std::endl;
    std::cout << "InstrumentID:       " << order->InstrumentID << std::endl;
    std::cout << "LimitPrice:         " << order->LimitPrice << std::endl;
    std::cout << "VolumeTotalOriginal:" << order->VolumeTotalOriginal << std::endl;

    std::cout << "****************************************" << std::endl;
}

void printGDF_STRUCT(GDF_STRUCT *gdf) {
	std::cout << "*******struct data of GDF_STRUCT*******" << std::endl;
	std::cout << "合约代码:" << gdf->InstrumentID << std::endl;
	std::cout << "价格类型:" << gdf->OrderPriceType << std::endl;
	std::cout << "买卖方向:" << gdf->Direction << std::endl;
	std::cout << "价格:" << gdf->LimitPrice << std::endl;
	std::cout << "开平仓标志:" << gdf->CombOffsetFlag << std::endl;
	std::cout << "数量:" << gdf->VolumeTotalOriginal << std::endl;
	std::cout << "期货所代码:" << gdf->ExchangeID << std::endl;
	std::cout << "****************************************" << std::endl;

}
