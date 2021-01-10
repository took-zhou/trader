/*
 * marketEvent.cpp
 *
 *  Created on: 2020��8��30��
 *      Author: Administrator
 */


#include "trader/interface/marketEvent/marketEvent.h"
#include "common/self/protobuf/market-trader.pb.h"
#include "common/extern/log/log.h"
#include "trader/infra/define.h"
#include "common/self/utils.h"
#include "trader/domain/traderService.h"
#include "trader/infra/recerSender.h"
#include "trader/interface/ctpEvent/ctpEvent.h"
#include "common/self/basetype.h"

constexpr U32 MAX_INSTRUMENT_STATUS_SIZE = 5;
constexpr U32 WAITTIME_FOR_MARKET_HANDLE = 10000;
bool MarketEvent::init()
{
    regMsgFun();

    return true;
}


void MarketEvent::regMsgFun()
{
    msgFuncMap.clear();
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("QryInstrumentReq",   [this](MsgStruct& msg){QryInstrumentReqHandle(msg);}));
}

void MarketEvent::handle(MsgStruct& msg)
{
    auto iter = msgFuncMap.find(msg.msgName);
    if(iter != msgFuncMap.end())
    {
        iter->second(msg);
        return;
    }
    ERROR_LOG("can not find func for msgName [%s]!",msg.msgName.c_str());
    return;
}


void MarketEvent::QryInstrumentReqHandle(MsgStruct& msg)
{
    market_trader::message reqMsg;
    reqMsg.ParseFromString(msg.pbMsg);
    utils::printProtoMsg(reqMsg);

    auto& req = reqMsg.qry_instrument_req();
    if(req.identity() != std::string("all"))
    {
        ERROR_LOG("qry_instrument_req identity[%s] is not [all]",req.identity().c_str());
        return;
    }

    auto& traderSer = TraderSevice::getInstance();
    auto* traderApi = traderSer.ROLE(Trader).ROLE(CtpTraderApi).traderApi;
    traderApi->ReqQryInstrument();
}

void MarketEvent::pubQryInstrumentRsq(U32 mapKey, bool isFinish)
{
    auto instrumentFieldList = ROLE(CtpEvent).qryRspsMap.at(mapKey);
    market_trader::message rsp;
    auto* qryInstruments = rsp.mutable_qry_instrument_rsp();
    U32 cnt{0};
    U32 listSize = instrumentFieldList.partRspList.size();
    auto pubFunc = [](std::string strRsp){
        std::string head = "market_trader.QryInstrumentRsq";
        auto& recerSender = RecerSender::getInstance();
        bool sendRes = recerSender.ROLE(Sender).ROLE(ProxySender).send(head.c_str(), strRsp.c_str());
        if(!sendRes)
        {
            ERROR_LOG("send OrderInsertRsp error");
            return;
        }
        return;
    };
    for(U32 idx = 0; idx < listSize; idx++)
    {
        if(cnt == 0)
        {
            rsp.Clear();
            qryInstruments = rsp.mutable_qry_instrument_rsp();
        }
        auto& instrumentInfo = instrumentFieldList.partRspList.at(idx);
        auto* instrumentData = qryInstruments->add_instrument_data_list();
        instrumentData->set_instrumentid(instrumentInfo.InstrumentID);
        instrumentData->set_exchangeid(instrumentInfo.ExchangeID);
//        instrumentData->set_instrumentname(instrumentInfo.InstrumentName);
//        instrumentData->set_exchangeinstid(instrumentInfo.ExchangeInstID);
//        instrumentData->set_productid(instrumentInfo.ProductID);
//        instrumentData->set_productclass(utils::charToString(instrumentInfo.ProductClass).c_str());
//        instrumentData->set_deliveryyear(utils::intToString(instrumentInfo.DeliveryYear).c_str());
//        instrumentData->set_deliverymonth(utils::intToString(instrumentInfo.DeliveryMonth).c_str());
//        instrumentData->set_maxmarketordervolume(utils::intToString(instrumentInfo.MaxMarketOrderVolume).c_str());
//        instrumentData->set_minmarketordervolume(utils::intToString(instrumentInfo.MinMarketOrderVolume).c_str());
//        instrumentData->set_maxlimitordervolume(utils::intToString(instrumentInfo.MaxLimitOrderVolume).c_str());
//        instrumentData->set_minlimitordervolume(utils::intToString(instrumentInfo.MinLimitOrderVolume).c_str());
//        instrumentData->set_volumemultiple(utils::intToString(instrumentInfo.VolumeMultiple).c_str());
        instrumentData->set_pricetick(utils::doubleToStringConvert(instrumentInfo.PriceTick).c_str());
        instrumentData->set_createdate(instrumentInfo.CreateDate);
        instrumentData->set_opendate(instrumentInfo.OpenDate);
//        instrumentData->set_expiredate(instrumentInfo.ExpireDate);
//        instrumentData->set_startdelivdate(instrumentInfo.StartDelivDate);
//        instrumentData->set_enddelivdate(instrumentInfo.EndDelivDate);
//        instrumentData->set_instlifephase(utils::charToString(instrumentInfo.InstLifePhase).c_str());
        instrumentData->set_istrading(utils::intToString(instrumentInfo.IsTrading).c_str());
//        instrumentData->set_positiontype(utils::charToString(instrumentInfo.PositionType).c_str());
//        instrumentData->set_positiondatetype(utils::charToString(instrumentInfo.PositionDateType).c_str());
//        instrumentData->set_longmarginratio(utils::doubleToStringConvert(instrumentInfo.LongMarginRatio).c_str());
//        instrumentData->set_shortmarginratio(utils::doubleToStringConvert(instrumentInfo.ShortMarginRatio).c_str());
//        instrumentData->set_maxmarginsidealgorithm(utils::charToString(instrumentInfo.MaxMarginSideAlgorithm).c_str());
//        instrumentData->set_underlyinginstrid(instrumentInfo.UnderlyingInstrID);
//        instrumentData->set_strikeprice(utils::doubleToStringConvert(instrumentInfo.StrikePrice).c_str());
//        instrumentData->set_optionstype(utils::charToString(instrumentInfo.OptionsType).c_str());
//        instrumentData->set_underlyingmultiple(utils::doubleToStringConvert(instrumentInfo.UnderlyingMultiple).c_str());
//        instrumentData->set_combinationtype(utils::charToString(instrumentInfo.CombinationType).c_str());
        cnt++;
        if(cnt == MAX_INSTRUMENT_STATUS_SIZE)
        {
            qryInstruments->set_result(market_trader::Result::success);
            bool isFinishFlag = (idx == listSize-1) ? true : false;
            qryInstruments->set_finish_flag(isFinishFlag);
            std::string strRsp = rsp.SerializeAsString();
            pubFunc(strRsp);
            utils::printProtoMsg(rsp);
            cnt = 0;
        }
        if(idx == listSize-1)
        {
            qryInstruments->set_result(market_trader::Result::success);
            qryInstruments->set_finish_flag(true);
            std::string strRsp = rsp.SerializeAsString();
            pubFunc(strRsp);
            utils::printProtoMsg(rsp);
        }
        usleep(WAITTIME_FOR_MARKET_HANDLE);
    }
    return;
}
