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
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("OnRspQryInstrumentMarginRate",       [this](MsgStruct& msg){OnRspQryInstrumentMarginRateHandle(msg);}));
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("OnRspQryInstrumentCommissionRate",   [this](MsgStruct& msg){OnRspQryInstrumentCommissionRateHandle(msg);}));

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
    orderCancelRsp->set_identity(identityId.identity);
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
    orderCancelRsp->set_identity(identityId.identity);
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
    auto& traderSer = TraderSevice::getInstance();
    auto& tmpAccountInfo = traderSer.ROLE(Trader).ROLE(TmpStore).accountInfo;

    if (tradeAccount)
    {
        tmpAccountInfo.Available = utils::doubleToStringConvert(tradeAccount->Available);
        tmpAccountInfo.Balance = utils::doubleToStringConvert(tradeAccount->Balance);
        tmpAccountInfo.BizType = utils::doubleToStringConvert(tradeAccount->BizType);
        tmpAccountInfo.CashIn = utils::doubleToStringConvert(tradeAccount->CashIn);
        tmpAccountInfo.CloseProfit = utils::doubleToStringConvert(tradeAccount->CloseProfit);
        tmpAccountInfo.Commission = utils::doubleToStringConvert(tradeAccount->Commission);
        tmpAccountInfo.Credit = utils::doubleToStringConvert(tradeAccount->Credit);
        tmpAccountInfo.CurrMargin = utils::doubleToStringConvert(tradeAccount->CurrMargin);
        tmpAccountInfo.CurrencyID = tradeAccount->CurrencyID;
        tmpAccountInfo.DeliveryMargin = utils::doubleToStringConvert(tradeAccount->DeliveryMargin);
        tmpAccountInfo.Deposit = utils::doubleToStringConvert(tradeAccount->Deposit);
        tmpAccountInfo.ExchangeDeliveryMargin = utils::doubleToStringConvert(tradeAccount->ExchangeDeliveryMargin);
        tmpAccountInfo.ExchangeMargin = utils::doubleToStringConvert(tradeAccount->ExchangeMargin);
        tmpAccountInfo.FrozenCash = utils::doubleToStringConvert(tradeAccount->FrozenCash);
        tmpAccountInfo.FrozenCommission = utils::doubleToStringConvert(tradeAccount->FrozenCommission);
        tmpAccountInfo.FrozenMargin = utils::doubleToStringConvert(tradeAccount->FrozenMargin);
        tmpAccountInfo.FrozenSwap = utils::doubleToStringConvert(tradeAccount->FrozenSwap);
        tmpAccountInfo.FundMortgageAvailable = utils::doubleToStringConvert(tradeAccount->FundMortgageAvailable);
        tmpAccountInfo.FundMortgageIn = utils::doubleToStringConvert(tradeAccount->FundMortgageIn);
        tmpAccountInfo.FundMortgageOut = utils::doubleToStringConvert(tradeAccount->FundMortgageOut);
        tmpAccountInfo.Interest = utils::doubleToStringConvert(tradeAccount->Interest);
        tmpAccountInfo.InterestBase = utils::doubleToStringConvert(tradeAccount->InterestBase);
        tmpAccountInfo.Mortgage = utils::doubleToStringConvert(tradeAccount->Mortgage);
        tmpAccountInfo.MortgageableFund = utils::doubleToStringConvert(tradeAccount->MortgageableFund);
        tmpAccountInfo.PositionProfit = utils::doubleToStringConvert(tradeAccount->PositionProfit);
        tmpAccountInfo.PreBalance = utils::doubleToStringConvert(tradeAccount->PreBalance);
        tmpAccountInfo.PreCredit = utils::doubleToStringConvert(tradeAccount->PreCredit);
        tmpAccountInfo.PreDeposit = utils::doubleToStringConvert(tradeAccount->PreDeposit);
        tmpAccountInfo.PreFundMortgageIn = utils::doubleToStringConvert(tradeAccount->PreFundMortgageIn);
        tmpAccountInfo.PreFundMortgageOut = utils::doubleToStringConvert(tradeAccount->PreFundMortgageOut);
        tmpAccountInfo.PreMargin = utils::doubleToStringConvert(tradeAccount->PreMargin);
        tmpAccountInfo.PreMortgage = utils::doubleToStringConvert(tradeAccount->PreMortgage);
        tmpAccountInfo.RemainSwap = utils::doubleToStringConvert(tradeAccount->RemainSwap);
        tmpAccountInfo.Reserve = utils::doubleToStringConvert(tradeAccount->Reserve);
        tmpAccountInfo.ReserveBalance = utils::doubleToStringConvert(tradeAccount->ReserveBalance);
        tmpAccountInfo.SettlementID = utils::doubleToStringConvert(tradeAccount->SettlementID);
        tmpAccountInfo.SpecProductCloseProfit = utils::doubleToStringConvert(tradeAccount->SpecProductCloseProfit);
        tmpAccountInfo.SpecProductCommission = utils::doubleToStringConvert(tradeAccount->SpecProductCommission);
        tmpAccountInfo.SpecProductExchangeMargin = utils::doubleToStringConvert(tradeAccount->SpecProductExchangeMargin);
        tmpAccountInfo.SpecProductFrozenCommission = utils::doubleToStringConvert(tradeAccount->SpecProductFrozenCommission);
        tmpAccountInfo.SpecProductFrozenMargin = utils::doubleToStringConvert(tradeAccount->SpecProductFrozenMargin);
        tmpAccountInfo.SpecProductMargin = utils::doubleToStringConvert(tradeAccount->SpecProductMargin);
        tmpAccountInfo.SpecProductPositionProfit = utils::doubleToStringConvert(tradeAccount->SpecProductPositionProfit);
        tmpAccountInfo.SpecProductPositionProfitByAlg = utils::doubleToStringConvert(tradeAccount->SpecProductPositionProfitByAlg);
        tmpAccountInfo.TradingDay = tradeAccount->TradingDay;
        tmpAccountInfo.Withdraw = utils::doubleToStringConvert(tradeAccount->Withdraw);
        tmpAccountInfo.WithdrawQuota = utils::doubleToStringConvert(tradeAccount->WithdrawQuota);
        tmpAccountInfo.rsp_is_null = false;
        delete tradeAccount;
    }
    else
    {
        tmpAccountInfo.rsp_is_null = true;
    }

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

    OrderSave::saveSuccessOrderInsert(orderContent);
    OrderSave::saveSuccCancelOrder(orderContent,"deal");
    OrderSave::delOneRecordByOnRtnOrder(orderContent);
    ROLE(StrategyEvent).pubOrderInsertRsp(orderContent.identityId,true,"success");
//    orderManage.delOrder(orderKey); // 暂时屏蔽 有内存泄漏， 暂时每周程序重启
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
            orderCancelRsp->set_identity(identityId.identity);
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
        orderManage.delOrder(orderKey);
        INFO_LOG("the order be canceled, ref[%s],identity[%s]",orderKey.c_str(), orderContent.identityId.identity.c_str());
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

    auto& traderSer = TraderSevice::getInstance();
    auto& orderManage = traderSer.ROLE(OrderManage);
    auto& orderContent = orderManage.getOrderContent(orderKey);
    if(!orderContent.isValid())
    {
        ERROR_LOG("can not find order in local, orderRef[%s]",orderKey.c_str());
        return;
    }
    if(orderContent.isFlowFinish)
    {
        INFO_LOG("insert flow has finished, ref[%s], identity[%s]",orderKey.c_str(), orderContent.identityId.identity.c_str());
        return;
    }
    orderContent.isFlowFinish = true;
    auto& orderStates = OrderStates::getInstance();
    if(! orderStates.insertState(orderKey, 'E'))
    {
        ERROR_LOG("insertState ERROR!");
    }

    std::string reason = ORDER_FILL_ERROR;
    ROLE(StrategyEvent).pubOrderInsertRsp(orderContent.identityId,false, reason);
    orderManage.delOrder(orderKey);
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
    auto& traderSer = TraderSevice::getInstance();
    auto& orderManage = traderSer.ROLE(OrderManage);
    auto& orderContent = orderManage.getOrderContent(orderKey);
    if(!orderContent.isValid())
    {
        ERROR_LOG("can not find order in local, orderRef[%s]",orderKey.c_str());
        return;
    }
    if(orderContent.isFlowFinish)
    {
        INFO_LOG("insert flow has finished, ref[%s], identity[%s]",orderKey.c_str(), orderContent.identityId.identity.c_str());
        return;
    }
    orderContent.isFlowFinish = true;
    auto& orderStates = OrderStates::getInstance();
    if(! orderStates.insertState(orderKey, 'E'))
    {
        ERROR_LOG("insertState ERROR!");
    }

    std::string reason = ORDER_FILL_ERROR;
    ROLE(StrategyEvent).pubOrderInsertRsp(orderContent.identityId,false, reason);
    orderManage.delOrder(orderKey);
    delete ctpRspField;
    delete rspInfoField;
}

void CtpEvent::OnRspQryInstrumentHandle(MsgStruct& msg)
{
    if(strlen(msg.specialMsg.instrumentField.InstrumentID) <= 6 || msg.bIsLast == true)
    {
        ROLE(MarketEvent).pubQryInstrumentRsq(&msg.specialMsg.instrumentField, true, msg.bIsLast);
    }
}

void CtpEvent::OnRspQryInstrumentMarginRateHandle(MsgStruct& msg)
{
    CThostFtdcInstrumentMarginRateField* marginRateField = static_cast<CThostFtdcInstrumentMarginRateField*>(msg.ctpMsg);
    auto& traderSer = TraderSevice::getInstance();
    auto& marginRate = traderSer.ROLE(Trader).ROLE(TmpStore).marginRate;

    if (marginRateField)
    {
        marginRate.LongMarginRatioByMoney = utils::doubleToStringConvert(marginRateField->LongMarginRatioByMoney);
        marginRate.LongMarginRatioByVolume = utils::doubleToStringConvert(marginRateField->LongMarginRatioByVolume);
        marginRate.ShortMarginRatioByMoney = utils::doubleToStringConvert(marginRateField->ShortMarginRatioByMoney);
        marginRate.ShortMarginRatioByVolume = utils::doubleToStringConvert(marginRateField->ShortMarginRatioByVolume);
        marginRate.rsp_is_null = false;
        delete marginRateField;
    }
    else
    {
        marginRate.rsp_is_null = true;
    }

    std::string semName = "margin_rate";
    globalSem.postSemBySemName(semName);
    INFO_LOG("post sem of [%s]",semName.c_str());
}

void CtpEvent::OnRspQryInstrumentCommissionRateHandle(MsgStruct& msg)
{
    CThostFtdcInstrumentCommissionRateField* commissionRateField = static_cast<CThostFtdcInstrumentCommissionRateField*>(msg.ctpMsg);
    auto& traderSer = TraderSevice::getInstance();
    auto& commissionRate = traderSer.ROLE(Trader).ROLE(TmpStore).commissionRate;

    if (commissionRateField)
    {
        commissionRate.CloseRatioByMoney = utils::doubleToStringConvert(commissionRateField->CloseRatioByMoney);
        commissionRate.CloseRatioByVolume = utils::doubleToStringConvert(commissionRateField->CloseRatioByVolume);
        commissionRate.CloseTodayRatioByMoney = utils::doubleToStringConvert(commissionRateField->CloseTodayRatioByMoney);
        commissionRate.CloseTodayRatioByVolume = utils::doubleToStringConvert(commissionRateField->CloseTodayRatioByVolume);
        commissionRate.OpenRatioByMoney = utils::doubleToStringConvert(commissionRateField->OpenRatioByMoney);
        commissionRate.OpenRatioByVolume = utils::doubleToStringConvert(commissionRateField->OpenRatioByVolume);
        commissionRate.rsp_is_null = false;

        delete commissionRateField;
    }
    else
    {
        commissionRate.rsp_is_null = true;
    }

    std::string semName = "commission_rate";
    globalSem.postSemBySemName(semName);
    INFO_LOG("post sem of [%s]",semName.c_str());
}
