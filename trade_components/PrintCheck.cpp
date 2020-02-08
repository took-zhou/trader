#include "PrintCheck.h"
#include "json.h"
#include "log.h"
namespace{
constexpr const char* ORDERNAME = "order_1";
}
void PintCheck::printOrderInfo(const CThostFtdcInputOrderField& order, const string& orderName)
{
    json jorder ={
            {orderName.c_str(),{
                    {"BrokerID",    order.BrokerID},
                    {"ExchangeID",  order.ExchangeID},
                    {"InvestorID",  order.InvestorID},
                    {"AccountID",   order.AccountID},
                    {"ClientID",    order.ClientID},
                    {"GTDDate",     order.GTDDate},
                    {"UserID",      order.UserID},
                    {"RequestID",   order.RequestID},
                    {"InvestUnitID",order.InvestUnitID},
                    {"BusinessUnit",order.BusinessUnit},
                    {"ClientID",    order.ClientID},
                    {"OrderRef",    order.OrderRef},
                    {"IPAddress",   order.IPAddress},
                    {"MacAddress",  order.MacAddress},

                    {"InstrumentID",    order.InstrumentID},
                    {"Direction",       order.Direction},
                    {"CombOffsetFlag",  order.CombOffsetFlag},
                    {"LimitPrice",      order.LimitPrice},
                    {"OrderPriceType",  order.OrderPriceType},
                    {"TimeCondition",   order.TimeCondition},
                    {"ContingentCondition",order.ContingentCondition},
                    {"MinVolume",           order.MinVolume},
                    {"StopPrice",           order.StopPrice},
                    {"VolumeTotalOriginal", order.VolumeTotalOriginal},
                    {"VolumeCondition",     order.VolumeCondition},
                    {"CombHedgeFlag",       order.CombHedgeFlag},

                    {"CurrencyID",      order.CurrencyID},
                    {"UserForceClose",  order.UserForceClose},
                    {"ForceCloseReason",order.ForceCloseReason},
                    {"IsAutoSuspend",   order.IsAutoSuspend},
                    {"IsSwapOrder",     order.IsSwapOrder},
                    }
            }
    };
    JsonPrint(jorder);
}

void PintCheck::printMsgHead(const TradeMsgHead& msgHead)
{
    json jhead;
    jhead["dataTypeId"] = msgHead.dataTypeId;
    jhead["fromClientName"] = msgHead.fromClientName;
    jhead["toClientName"] = msgHead.toClientName;
    jhead["length"] = msgHead.length;
    JsonPrint(jhead);
}

void PintCheck::printInstrumentInfo(const CThostFtdcInstrumentField& instrumentInfo)
{
    json instrument;
    instrument["InstrumentID"] = instrumentInfo.InstrumentID;
    instrument["ExchangeID"] = instrumentInfo.ExchangeID;
    instrument["DeliveryYear"] = instrumentInfo.DeliveryYear;
    instrument["DeliveryMonth"] = instrumentInfo.DeliveryMonth;
    instrument["PriceTick"] = instrumentInfo.PriceTick;
    instrument["LongMarginRatio"] = instrumentInfo.LongMarginRatio;
    instrument["ShortMarginRatio"] = instrumentInfo.ShortMarginRatio;
    instrument["IsTrading"] = instrumentInfo.IsTrading;
}
