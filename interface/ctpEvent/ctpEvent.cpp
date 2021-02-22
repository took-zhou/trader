/*
 * ctpEvent.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#include "trader/interface/ctpEvent/ctpEvent.h"
#include "common/extern/log/log.h"
#include "trader/infra/define.h"
#include "trader/domain/components/orderstates.h"
#include "trader/domain/components/InsertResult.h"
#include "trader/domain/traderService.h"
#include "common/self/utils.h"
#include "trader/interface/marketEvent/marketEvent.h"
#include <algorithm>
#include <mutex>
#include "common/self/semaphorePart.h"
#include "trader/interface/strategyEvent/strategyEvent.h"
#include "trader/domain/components/order.h"
extern GlobalSem globalSem;
std::mutex m;
bool CtpEvent::init()
{
    regMsgFun();

    return true;
}


void CtpEvent::regMsgFun()
{
    int cnt = 0;
    msgFuncMap.clear();
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("OnRspSettlementInfoConfirm",   [this](MsgStruct& msg){OnRspSettlementInfoConfirmHandle(msg);}));
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("OnRspUserLogin",               [this](MsgStruct& msg){OnRspUserLoginHandle(msg);}));
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("OnRspAuthenticate",            [this](MsgStruct& msg){OnRspAuthenticateHandle(msg);}));
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("OnErrRtnOrderInsert",          [this](MsgStruct& msg){OnErrRtnOrderInsertHandle(msg);}));
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("OnRspOrderInsert",             [this](MsgStruct& msg){OnRspOrderInsertHandle(msg);}));
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("OnRtnOrder",                   [this](MsgStruct& msg){OnRtnOrderHandle(msg);}));
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("OnRtnTrade",                   [this](MsgStruct& msg){OnRtnTradeHandle(msg);}));
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("OnRspQryTradingAccount",       [this](MsgStruct& msg){OnRspQryTradingAccountHandle(msg);}));
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("OnRspQryInstrument",           [this](MsgStruct& msg){OnRspQryInstrumentHandle(msg);}));
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("OnRspOrderAction",             [this](MsgStruct& msg){OnRspOrderActionHandle(msg);}));
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("OnErrRtnOrderAction",          [this](MsgStruct& msg){OnErrRtnOrderActionHandle(msg);}));

    for(auto iter : msgFuncMap)
    {
        INFO_LOG("msgFuncMap[%d] key is [%s]",cnt, iter.first.c_str());
        cnt++;
    }
}

void CtpEvent::handle(MsgStruct& msg)
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

void CtpEvent::OnErrRtnOrderActionHandle(MsgStruct& msg)
{
    CThostFtdcOrderActionField orderActionRsp = *(CThostFtdcOrderActionField*)msg.ctpMsg;
    delete (CThostFtdcOrderActionField*)msg.ctpMsg;
    CThostFtdcRspInfoField msgInfo = *static_cast<CThostFtdcRspInfoField*>(msg.ctpMsgInfo);
    delete static_cast<CThostFtdcRspInfoField*>(msg.ctpMsgInfo);

    std::string orderRef = std::string(orderActionRsp.OrderRef);
    auto& traderSer = TraderSevice::getInstance();
    auto& orderManage = traderSer.ROLE(OrderManage);
    auto& orderContent = orderManage.getOrderContent(orderRef);

    auto identityId = orderContent.identityId;
    strategy_trader::message rspMsg;
    auto* orderCancelRsp  = rspMsg.mutable_order_cancel_rsp();
    orderCancelRsp->set_identity(identityId);
    orderCancelRsp->set_result(strategy_trader::Result::failed);
    TThostFtdcErrorMsgType errmsg;
    utils::gbk2utf8(msgInfo.ErrorMsg,errmsg,sizeof(errmsg));
    std::string reason = std::string("OnErrRtnOrderAction_")+utils::intToString(msgInfo.ErrorID)+"_"+std::string(errmsg);
    orderCancelRsp->set_failedreason(reason);
    ROLE(StrategyEvent).pubOrderCancelRsp(identityId, false,  reason);
}


void CtpEvent::OnRspOrderActionHandle(MsgStruct& msg)
{
    CThostFtdcInputOrderActionField orderActionRsp = *(CThostFtdcInputOrderActionField*)msg.ctpMsg;
    delete (CThostFtdcInputOrderActionField*)msg.ctpMsg;
    CThostFtdcRspInfoField msgInfo = *static_cast<CThostFtdcRspInfoField*>(msg.ctpMsgInfo);
    delete static_cast<CThostFtdcRspInfoField*>(msg.ctpMsgInfo);

    std::string orderRef = std::string(orderActionRsp.OrderRef);
    auto& traderSer = TraderSevice::getInstance();
    auto& orderManage = traderSer.ROLE(OrderManage);
    auto& orderContent = orderManage.getOrderContent(orderRef);
    auto identityId = orderContent.identityId;
    strategy_trader::message rspMsg;
    auto* orderCancelRsp  = rspMsg.mutable_order_cancel_rsp();
    orderCancelRsp->set_identity(identityId);
    orderCancelRsp->set_result(strategy_trader::Result::failed);
    TThostFtdcErrorMsgType errmsg;
    utils::gbk2utf8(msgInfo.ErrorMsg,errmsg,sizeof(errmsg));
    std::string reason = std::string("OnRspOrderAction_")+utils::intToString(msgInfo.ErrorID)+"_"+std::string(errmsg);
    orderCancelRsp->set_failedreason(reason);
    ROLE(StrategyEvent).pubOrderCancelRsp(identityId, false,  reason);
}

void CtpEvent::OnRspQryTradingAccountHandle(MsgStruct& msg)
{
    CThostFtdcTradingAccountField* tradeAccount = static_cast<CThostFtdcTradingAccountField*>(msg.ctpMsg);
    CThostFtdcTradingAccountField tmp = *tradeAccount;
    CThostFtdcTradingAccountField* pTradingAccount = &tmp;//(CThostFtdcTradingAccountField*)msg.ctpMsg;
    delete tradeAccount;
   // 目前只支持全量响应, 精确响应需要设置临时变量，以后根据需要再做适配

    auto& traderSer = TraderSevice::getInstance();
    auto& tmpAccountInfo = traderSer.ROLE(Trader).ROLE(TmpStore).accountInfo;

    tmpAccountInfo.Available = utils::doubleToStringConvert(pTradingAccount->Available);
    tmpAccountInfo.Balance = utils::doubleToStringConvert(pTradingAccount->Balance);
    tmpAccountInfo.BizType = utils::doubleToStringConvert(pTradingAccount->BizType);
    tmpAccountInfo.CashIn = utils::doubleToStringConvert(pTradingAccount->CashIn);
    tmpAccountInfo.CloseProfit = utils::doubleToStringConvert(pTradingAccount->CloseProfit);
    tmpAccountInfo.Commission = utils::doubleToStringConvert(pTradingAccount->Commission);
    tmpAccountInfo.Credit = utils::doubleToStringConvert(pTradingAccount->Credit);
    tmpAccountInfo.CurrMargin = utils::doubleToStringConvert(pTradingAccount->CurrMargin);
    tmpAccountInfo.CurrencyID = pTradingAccount->CurrencyID;
    tmpAccountInfo.DeliveryMargin = utils::doubleToStringConvert(pTradingAccount->DeliveryMargin);
    tmpAccountInfo.Deposit = utils::doubleToStringConvert(pTradingAccount->Deposit);
    tmpAccountInfo.ExchangeDeliveryMargin = utils::doubleToStringConvert(pTradingAccount->ExchangeDeliveryMargin);
    tmpAccountInfo.ExchangeMargin = utils::doubleToStringConvert(pTradingAccount->ExchangeMargin);
    tmpAccountInfo.FrozenCash = utils::doubleToStringConvert(pTradingAccount->FrozenCash);
    tmpAccountInfo.FrozenCommission = utils::doubleToStringConvert(pTradingAccount->FrozenCommission);
    tmpAccountInfo.FrozenMargin = utils::doubleToStringConvert(pTradingAccount->FrozenMargin);
    tmpAccountInfo.FrozenSwap = utils::doubleToStringConvert(pTradingAccount->FrozenSwap);
    tmpAccountInfo.FundMortgageAvailable = utils::doubleToStringConvert(pTradingAccount->FundMortgageAvailable);
    tmpAccountInfo.FundMortgageIn = utils::doubleToStringConvert(pTradingAccount->FundMortgageIn);
    tmpAccountInfo.FundMortgageOut = utils::doubleToStringConvert(pTradingAccount->FundMortgageOut);
    tmpAccountInfo.Interest = utils::doubleToStringConvert(pTradingAccount->Interest);
    tmpAccountInfo.InterestBase = utils::doubleToStringConvert(pTradingAccount->InterestBase);
    tmpAccountInfo.Mortgage = utils::doubleToStringConvert(pTradingAccount->Mortgage);
    tmpAccountInfo.MortgageableFund = utils::doubleToStringConvert(pTradingAccount->MortgageableFund);
    tmpAccountInfo.PositionProfit = utils::doubleToStringConvert(pTradingAccount->PositionProfit);
    tmpAccountInfo.PreBalance = utils::doubleToStringConvert(pTradingAccount->PreBalance);
    tmpAccountInfo.PreCredit = utils::doubleToStringConvert(pTradingAccount->PreCredit);
    tmpAccountInfo.PreDeposit = utils::doubleToStringConvert(pTradingAccount->PreDeposit);
    tmpAccountInfo.PreFundMortgageIn = utils::doubleToStringConvert(pTradingAccount->PreFundMortgageIn);
    tmpAccountInfo.PreFundMortgageOut = utils::doubleToStringConvert(pTradingAccount->PreFundMortgageOut);
    tmpAccountInfo.PreMargin = utils::doubleToStringConvert(pTradingAccount->PreMargin);
    tmpAccountInfo.PreMortgage = utils::doubleToStringConvert(pTradingAccount->PreMortgage);
    tmpAccountInfo.RemainSwap = utils::doubleToStringConvert(pTradingAccount->RemainSwap);
    tmpAccountInfo.Reserve = utils::doubleToStringConvert(pTradingAccount->Reserve);
    tmpAccountInfo.ReserveBalance = utils::doubleToStringConvert(pTradingAccount->ReserveBalance);
    tmpAccountInfo.SettlementID = utils::doubleToStringConvert(pTradingAccount->SettlementID);
    tmpAccountInfo.SpecProductCloseProfit = utils::doubleToStringConvert(pTradingAccount->SpecProductCloseProfit);
    tmpAccountInfo.SpecProductCommission = utils::doubleToStringConvert(pTradingAccount->SpecProductCommission);
    tmpAccountInfo.SpecProductExchangeMargin = utils::doubleToStringConvert(pTradingAccount->SpecProductExchangeMargin);
    tmpAccountInfo.SpecProductFrozenCommission = utils::doubleToStringConvert(pTradingAccount->SpecProductFrozenCommission);
    tmpAccountInfo.SpecProductFrozenMargin = utils::doubleToStringConvert(pTradingAccount->SpecProductFrozenMargin);
    tmpAccountInfo.SpecProductMargin = utils::doubleToStringConvert(pTradingAccount->SpecProductMargin);
    tmpAccountInfo.SpecProductPositionProfit = utils::doubleToStringConvert(pTradingAccount->SpecProductPositionProfit);
    tmpAccountInfo.SpecProductPositionProfitByAlg = utils::doubleToStringConvert(pTradingAccount->SpecProductPositionProfitByAlg);
    tmpAccountInfo.TradingDay = pTradingAccount->TradingDay;
    tmpAccountInfo.Withdraw = utils::doubleToStringConvert(pTradingAccount->Withdraw);
    tmpAccountInfo.WithdrawQuota = utils::doubleToStringConvert(pTradingAccount->WithdrawQuota);

    std::string semName = "trader_ReqQryTradingAccount";
    globalSem.postSemBySemName(semName);
    INFO_LOG("post sem of [%s]",semName.c_str());
}
void CtpEvent::OnRtnTradeHandle(MsgStruct& msg)
{
    CThostFtdcTradeField* pTrade = (CThostFtdcTradeField*) msg.ctpMsg;
    std::string orderKey = std::string(pTrade->OrderRef);
    auto& ctpRspResultMonitor = InsertResult::getInstance();
    ctpRspResultMonitor.setResultState(orderKey, InsertRspResult::Success);

    auto& traderSer = TraderSevice::getInstance();
    auto& orderManage = traderSer.ROLE(OrderManage);
    auto& orderContent = orderManage.getOrderContent(orderKey);
    if(!orderContent.isValid())
    {
        ERROR_LOG("OnRtnTrade can not find order in local, orderRef[%s]",orderKey.c_str());
        return;
    }
    orderContent.tradedOrder.price = pTrade->Price;
    orderContent.tradedOrder.volume = pTrade ->Volume;
    orderContent.tradedOrder.direction = utils::charToString(pTrade->Direction);
    orderContent.tradedOrder.date = pTrade->TradeDate;
    orderContent.tradedOrder.time = pTrade->TradeTime;

//    std::string semName = "trader_ReqOrderInsert" + std::string(pTrade->OrderRef);
//    globalSem.postSemBySemName(semName);
    OrderSave::saveSuccessOrderInsert(orderContent);
    OrderSave::saveSuccCancelOrder(orderContent,"deal");
    OrderSave::delOneRecordByOnRtnOrder(orderContent);
    ROLE(StrategyEvent).pubOrderInsertRsp(orderContent.identityId,true,"success");
    delete pTrade;
    INFO_LOG("insert order success");
}


namespace
{
    std::map<char, std::string> statusMap = {
            {THOST_FTDC_OST_AllTraded,              "AllTrade"},
            {THOST_FTDC_OST_PartTradedQueueing,     "PartTradedQueueing"},
            {THOST_FTDC_OST_PartTradedNotQueueing,  "PartTradedNotQueueing"},
            {THOST_FTDC_OST_NoTradeQueueing,        "NoTradeQueueing"},
            {THOST_FTDC_OST_NoTradeNotQueueing,     "NoTradeNotQueueing"},
            {THOST_FTDC_OST_Canceled,               "Canceled"},
            {THOST_FTDC_OST_Unknown,                "Unknown"},
            {THOST_FTDC_OST_NotTouched,             "NotTouched"},
            {THOST_FTDC_OST_Touched,                "Touched"}
    };
}
void CtpEvent::OnRtnOrderHandle(MsgStruct& msg)
{
    CThostFtdcOrderField* tmpPOrder = (CThostFtdcOrderField*) msg.ctpMsg;
    CThostFtdcOrderField tmp = *tmpPOrder;
    delete tmpPOrder;
    CThostFtdcOrderField* pOrder = &tmp;
    std::string orderKey = std::string(pOrder->OrderRef);

    auto& traderSer = TraderSevice::getInstance();
    auto& orderManage = traderSer.ROLE(OrderManage);
    auto& orderContent = orderManage.getOrderContent(orderKey);
    if(!orderContent.isValid())
    {
        ERROR_LOG("invalid order key[%s]",orderKey.c_str());
        return;
    }
    orderContent.frontId = pOrder->FrontID;
    orderContent.sessionId = pOrder->SessionID;
    orderContent.currentStateStr = statusMap.at(pOrder->OrderStatus);
    orderContent.currentStateChar = pOrder->OrderStatus;
    OrderSave::saveOnRtnOrderOrderState(orderContent);

    auto& orderStates = OrderStates::getInstance();
    if(! orderStates.insertState(orderKey, pOrder->OrderStatus))
    {
        ERROR_LOG("insertState ERROR!");
    }

    if (pOrder->OrderStatus == THOST_FTDC_OST_AllTraded)///全部成交
    {
        INFO_LOG("It's all done");
    }
    if (pOrder->OrderStatus == THOST_FTDC_OST_PartTradedQueueing)///部分成交还在队列中
    {
        INFO_LOG("Some deals are still in the queue");
    }
    if (pOrder->OrderStatus == THOST_FTDC_OST_PartTradedNotQueueing)///部分成交不在队列中
    {
        INFO_LOG("Some deals are not in the queue");
    }
    if (pOrder->OrderStatus == THOST_FTDC_OST_NoTradeQueueing)///未成交还在队列中
    {

        INFO_LOG("not deal is still in the queue");
    }
    if (pOrder->OrderStatus == THOST_FTDC_OST_NoTradeNotQueueing)///未成交不在队列中
    {
        INFO_LOG("not deal is not in the queue\n\n");
    }
    if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)///撤单
    {
        if(orderContent.activeCancleIndication)
        {
            auto identityId = orderContent.identityId;
            strategy_trader::message rspMsg;
            auto* orderCancelRsp  = rspMsg.mutable_order_cancel_rsp();
            orderCancelRsp->set_identity(identityId);
            orderCancelRsp->set_result(strategy_trader::Result::failed);
            TThostFtdcErrorMsgType errmsg;
            std::string reason = std::string("INVALID");
            orderCancelRsp->set_failedreason(reason);
            ROLE(StrategyEvent).pubOrderCancelRsp(identityId, true,  reason);
        }

        auto& traderSer = TraderSevice::getInstance();
        auto& orderManage = traderSer.ROLE(OrderManage);
        auto& orderContent = orderManage.getOrderContent(orderKey);
        if(!orderContent.isValid())
        {
            ERROR_LOG("can not find order in local, orderRef[%s]",orderKey.c_str());
            return;
        }
        OrderSave::saveSuccCancelOrder(orderContent,"cancel");
        OrderSave::delOneRecordByOnRtnOrder(orderContent);
        std::string reason = ORDER_CANCEL;
        ROLE(StrategyEvent).pubOrderInsertRsp(orderContent.identityId,false, reason);
        INFO_LOG("the order be canceled, ref[%s],identity[%s]",orderKey.c_str(), orderContent.identityId.c_str());
    }
    if (pOrder->OrderStatus == THOST_FTDC_OST_Unknown)///未知
    {
        INFO_LOG("unknow");
    }
    if (pOrder->OrderStatus == THOST_FTDC_OST_NotTouched)///尚未触发
    {
        INFO_LOG("Not triggered yet\n\n");
    }
    if (pOrder->OrderStatus == THOST_FTDC_OST_Touched)///已触发
    {
        INFO_LOG("have triggered");
    }

}

void CtpEvent::OnRspOrderInsertHandle(MsgStruct& msg)
{
    CThostFtdcInputOrderField* ctpRspField = static_cast<CThostFtdcInputOrderField*>(msg.ctpMsg);
    CThostFtdcRspInfoField* ctpMsgInfo = static_cast<CThostFtdcRspInfoField*>(msg.ctpMsgInfo);

    std::string orderKey = std::string(ctpRspField->OrderRef);

//    auto& ctpRspResultMonitor = InsertResult::getInstance();
//    ctpRspResultMonitor.setResultState(orderKey, InsertRspResult::Failed);

    auto& orderStates = OrderStates::getInstance();
    if(! orderStates.insertState(orderKey, 'E'))
    {
        ERROR_LOG("insertState ERROR!");
    }

    auto& traderSer = TraderSevice::getInstance();
    auto& orderManage = traderSer.ROLE(OrderManage);
    auto& orderContent = orderManage.getOrderContent(orderKey);
    if(!orderContent.isValid())
    {
        ERROR_LOG("can not find order in local, orderRef[%s]",orderKey.c_str());
        return;
    }
    std::string reason = ORDER_FILL_ERROR;
    ROLE(StrategyEvent).pubOrderInsertRsp(orderContent.identityId,false, reason);
    delete ctpRspField;
    delete ctpMsgInfo;
}

void CtpEvent::OnRspAuthenticateHandle(MsgStruct& msg)
{
    std::string semName = "trader_reqAuthenticate";
    globalSem.postSemBySemName(semName);
    INFO_LOG("post sem of [%s]",semName.c_str());
}

void CtpEvent::OnRspUserLoginHandle(MsgStruct& msg)
{
    CThostFtdcRspUserLoginField* pMsg = static_cast<CThostFtdcRspUserLoginField*>(msg.ctpMsg);
    delete pMsg;
    std::string semName = "trader_logIn";
    globalSem.postSemBySemName(semName);
    INFO_LOG("post sem of [%s]",semName.c_str());
}

void CtpEvent::OnRspSettlementInfoConfirmHandle(MsgStruct& msg)
{
    CThostFtdcSettlementInfoConfirmField* pMsg = static_cast<CThostFtdcSettlementInfoConfirmField*>(msg.ctpMsg);
    delete pMsg;
    std::string semName = "trader_ReqSettlementInfoConfirm";
    globalSem.postSemBySemName(semName);
    INFO_LOG("post sem of [%s]",semName.c_str());
}

void CtpEvent::OnErrRtnOrderInsertHandle(MsgStruct& msg)
{
    CThostFtdcInputOrderField* ctpRspField = (CThostFtdcInputOrderField*) msg.ctpMsg;
    CThostFtdcRspInfoField* rspInfoField = (CThostFtdcRspInfoField*)msg.ctpMsgInfo;

    std::string orderKey = std::string(ctpRspField->OrderRef);
    auto& orderStates = OrderStates::getInstance();
    if(! orderStates.insertState(orderKey, 'E'))
    {
        ERROR_LOG("insertState ERROR!");
    }

    auto& traderSer = TraderSevice::getInstance();
    auto& orderManage = traderSer.ROLE(OrderManage);
    auto& orderContent = orderManage.getOrderContent(orderKey);
    if(!orderContent.isValid())
    {
        ERROR_LOG("can not find order in local, orderRef[%s]",orderKey.c_str());
        return;
    }
    std::string reason = ORDER_FILL_ERROR;
    ROLE(StrategyEvent).pubOrderInsertRsp(orderContent.identityId,false, reason);
    delete ctpRspField;
    delete rspInfoField;
}

void CtpEvent::OnRspQryInstrumentHandle(MsgStruct& msg)
{
    U32 key{U32_MAX};
    bool result = getNotFullRspMap(key);
    if(! result)
    {
        key = addNewRspsList();
    }
    auto& InstrumentStatusList = qryRspsMap.at(key);
    //static std::vector<CThostFtdcInstrumentField> InstrumentStatusList;
    CThostFtdcInstrumentField ctpRspField =msg.specialMsg.instrumentField;
    //WARNING_LOG("OnRspQryInstrumentHandle ctpRspField.InstrumentID [%s]",ctpRspField.InstrumentID);
    if(strlen(ctpRspField.InstrumentID) <= 6)
    {
        m.lock();
        InstrumentStatusList.partRspList.push_back(ctpRspField);
        m.unlock();
    }
    //delete (CThostFtdcInstrumentField*)msg.ctpMsg;
    if(msg.bIsLast)
    {
        INFO_LOG("msg.bIsLast [%s]",msg.bIsLast?"true":"false");
        InstrumentStatusList.isOk = true;
        ROLE(MarketEvent).pubQryInstrumentRsq(key, msg.bIsLast);
        delRspsList(key);
        INFO_LOG("pub QryInstrumentRsq ok! bIsLast is [%s],mapKey[%u]", msg.bIsLast?"true":"false",key);
    }
}

U32 CtpEvent::addNewRspsList()
{
    InstrumentQryTmp tmp;
    U32 key = buildNewKey();
    qryRspsMap.insert(std::pair<U32, InstrumentQryTmp>(key,tmp));
    return key;
}

void CtpEvent::delRspsList(U32 key)
{
    auto iter = qryRspsMap.find(key);
    if(iter == qryRspsMap.end())
    {
        return;
    }
    iter->second.partRspList.clear();
    qryRspsMap.erase(key);
}

bool CtpEvent::getNotFullRspMap(U32& key)
{
    for(auto& iter : qryRspsMap)
    {
        if(iter.second.isOk == false)
        {
            key = iter.first;
            return true;
        }
    }
    return false;
}

U32 CtpEvent::buildNewKey()
{
    std::vector<U32> existKeys;
    for(auto& iter : qryRspsMap)
    {
        existKeys.push_back(iter.first);
    }
    if(existKeys.size() == 0)
    {
        return 0;
    }
    auto maxKey = *(std::max_element(existKeys.begin(),existKeys.end()));
    for(U32 i = 0; i < maxKey; i++)
    {
        if(qryRspsMap.find(i) != qryRspsMap.end())
        {
            continue;
        }
        return i;
    }
    return maxKey+1;
}


