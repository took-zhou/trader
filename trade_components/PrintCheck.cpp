#include "PrintCheck.h"
#include "json.h"
#include "log.h"
namespace{
constexpr const char* ORDERNAME = "order_1";
}
void PintCheck::printOrderInfo(const CThostFtdcInputOrderField& order, const string& orderName)
{
    string direction;
    direction.push_back(order.Direction);
    string forceCloseReason;
    forceCloseReason.push_back(order.ForceCloseReason);
    string orderPriceType;
    orderPriceType.push_back(order.OrderPriceType);
    string timeCondition;
    timeCondition.push_back(order.TimeCondition);
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
                    {"Direction",       direction},
                    {"CombOffsetFlag",  order.CombOffsetFlag},
                    {"LimitPrice",      order.LimitPrice},
                    {"OrderPriceType",  orderPriceType},
                    {"TimeCondition",   timeCondition},
                    {"ContingentCondition",order.ContingentCondition},
                    {"MinVolume",           order.MinVolume},
                    {"StopPrice",           order.StopPrice},
                    {"VolumeTotalOriginal", order.VolumeTotalOriginal},
                    {"VolumeCondition",     order.VolumeCondition},
                    {"CombHedgeFlag",       order.CombHedgeFlag},

                    {"CurrencyID",      order.CurrencyID},
                    {"UserForceClose",  order.UserForceClose},
                    {"ForceCloseReason",forceCloseReason},
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
