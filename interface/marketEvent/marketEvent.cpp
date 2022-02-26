/*
 * marketEvent.cpp
 *
 *  Created on: 2020年10月23日
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
#include "common/self/utils.h"

constexpr U32 MAX_INSTRUMENT_STATUS_SIZE = 5;
constexpr U32 WAITTIME_FOR_MARKET_HANDLE = 10000;
bool MarketEvent::init()
{
    regMsgFun();

    return true;
}


void MarketEvent::regMsgFun()
{
    int cnt = 0;
    msgFuncMap.clear();
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("QryInstrumentReq",   [this](MsgStruct& msg){QryInstrumentReqHandle(msg);}));

    for(auto iter : msgFuncMap)
    {
        INFO_LOG("msgFuncMap[%d] key is [%s]",cnt, iter.first.c_str());
        cnt++;
    }
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
    auto& traderSer = TraderSevice::getInstance();
    if(!traderSer.ROLE(Trader).ROLE(CtpTraderApi).isLogIN)
    {
        ERROR_LOG("ctp not login!");
        pubQryInstrumentRsp(nullptr, true, false);
        return;
    }
    auto& req = reqMsg.qry_instrument_req();
    if(req.identity() != std::string("all"))
    {
        ERROR_LOG("qry_instrument_req identity[%s] is not [all]",req.identity().c_str());
        return;
    }
    auto* traderApi = traderSer.ROLE(Trader).ROLE(CtpTraderApi).traderApi;

    utils::InstrumtntID ins_exch;
    ins_exch.exch = "";
    ins_exch.ins = "";
    traderApi->ReqQryInstrument(ins_exch);
}

void MarketEvent::pubQryInstrumentRsp(CThostFtdcInstrumentField *field, bool result, bool isFinish)
{
    market_trader::message rsp;
    auto* qryInstruments = rsp.mutable_qry_instrument_rsp();

    if (result == false)
    {
        qryInstruments->set_result(market_trader::Result::failed);
        qryInstruments->set_finish_flag(true);
    }
    else
    {
        qryInstruments->set_instrument_id(field->InstrumentID);
        qryInstruments->set_exchange_id(field->ExchangeID);
        qryInstruments->set_price_tick(utils::doubleToStringConvert(field->PriceTick).c_str());
        qryInstruments->set_result(market_trader::Result::success);
        qryInstruments->set_finish_flag(isFinish);
    }

    std::string strRsp = rsp.SerializeAsString();
    std::string head = "market_trader.QryInstrumentRsp";
    auto& recerSender = RecerSender::getInstance();
    bool sendRes = recerSender.ROLE(Sender).ROLE(ProxySender).send(head.c_str(), strRsp.c_str());

    if (sendRes == false)
    {
        ERROR_LOG("send OrderInsertRsp error");
    }
    return;
}
