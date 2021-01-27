/*
 * strategyEvent.cpp
*/


#include "trader/interface/strategyEvent/strategyEvent.h"
#include "common/extern/log/log.h"
#include "trader/infra/define.h"
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/extern/google/protobuf/text_format.h"
#include "common/self/utils.h"
#include "trader/domain/traderService.h"
#include "common/self/fileUtil.h"
#include "trader/infra/recerSender.h"
#include "trader/domain/traderService.h"
#include "trader/domain/components/InsertResult.h"

#include "common/self/semaphorePart.h"
extern GlobalSem globalSem;

bool StrategyEvent::init()
{
    regMsgFun();

    return true;
}

void StrategyEvent::handle(MsgStruct& msg)
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

void StrategyEvent::regMsgFun()
{
    msgFuncMap.clear();
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("OrderInsertReq",     [this](MsgStruct& msg){OrderInsertReqHandle(msg);}));

    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("AccountStatusReq",   [this](MsgStruct& msg){AccountStatusReqHandle(msg);}));
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("OrderCancelReq",     [this](MsgStruct& msg){OrderCancelReqHandle(msg);}));
    return;
}

void StrategyEvent::OrderCancelReqHandle(MsgStruct& msg)
{
    strategy_trader::message reqMsg;
    reqMsg.ParseFromString(msg.pbMsg);
    utils::printProtoMsg(reqMsg);

    auto& orderCancelReq = reqMsg.order_cancel_req();
    std::string identity = orderCancelReq.identity();
    auto& traderSer = TraderSevice::getInstance();
    if(!traderSer.ROLE(Trader).ROLE(CtpTraderApi).isLogIN)
    {
        ERROR_LOG("ctp not login!");
        pubOrderCancelRsp(identity, false, "ctp_logout");
        return;
    }
    auto& orderManage = traderSer.ROLE(OrderManage);
    auto& orderContent = orderManage.getOrderCOntentByIdentityId(identity);
    if(!orderContent.isValid())
    {
        std::string reason = "invalidOrderIdentityId";
        pubOrderCancelRsp(identity, false, reason);
        return;
    }

    orderContent.activeCancleIndication = true;
    auto* traderApi = traderSer.ROLE(Trader).ROLE(CtpTraderApi).traderApi;
    CThostFtdcInputOrderActionField orderActionReq;

    traderApi->ReqOrderAction(orderContent);
}

void StrategyEvent::pubOrderCancelRsp(std::string identityId, bool result, const std::string& reason)
{
    strategy_trader::message rspMsg;
    auto* orderCancelRsp  = rspMsg.mutable_order_cancel_rsp();
    orderCancelRsp->set_identity(identityId);
    orderCancelRsp->set_result(result? strategy_trader::Result::success:strategy_trader::Result::failed);
    orderCancelRsp->set_failedreason(reason);

    std::string strRsp = rspMsg.SerializeAsString();
    std::string head = "strategy_trader.OrderCancelRsp";
    auto& recerSender = RecerSender::getInstance();
    bool sendRes = recerSender.ROLE(Sender).ROLE(ProxySender).send(head.c_str(), strRsp.c_str());
    utils::printProtoMsg(rspMsg);
    if(!sendRes)
    {
        ERROR_LOG("send OrderCancelRsp error");
        return;
    }
    return;
}

void StrategyEvent::AccountStatusReqHandle(MsgStruct& msg)
{
    strategy_trader::message reqMsg;
    reqMsg.ParseFromString(msg.pbMsg);
    utils::printProtoMsg(reqMsg);

    auto& traderSer = TraderSevice::getInstance();
    if(!traderSer.ROLE(Trader).ROLE(CtpTraderApi).isLogIN)
    {
        ERROR_LOG("ctp not login!");
        pubAccountStatusRsq(false,"ctp_logout");
        return;
    }
    auto* traderApi = traderSer.ROLE(Trader).ROLE(CtpTraderApi).traderApi;
    traderApi->ReqQryTradingAccount();
    std::string semName = "trader_ReqQryTradingAccount";
    globalSem.waitSemBySemName(semName);
    INFO_LOG("waitSemBySemName [%s] ok",semName.c_str());
    globalSem.delOrderSem(semName);
    pubAccountStatusRsq(true);
}

void StrategyEvent::pubAccountStatusRsq(bool result, const std::string& reason)
{
    strategy_trader::message rsp;
    auto* accountRsp = rsp.mutable_account_status_rsq();
    auto& traderSer = TraderSevice::getInstance();
    accountRsp->set_result(result?strategy_trader::Result::success:strategy_trader::Result::failed);
    accountRsp->set_failedreason(reason);
    accountRsp->set_level(strategy_trader::Level::ALL);
    auto& tmpAccountInfo = traderSer.ROLE(Trader).ROLE(TmpStore).accountInfo;
    auto* filedContent = accountRsp->mutable_filed_content();

    filedContent->set_available(tmpAccountInfo.Available);
    filedContent->set_balance(tmpAccountInfo.Balance);
    filedContent->set_biztype(tmpAccountInfo.BizType);
    filedContent->set_cashin(tmpAccountInfo.CashIn);
    filedContent->set_closeprofit(tmpAccountInfo.CloseProfit);
    filedContent->set_commission(tmpAccountInfo.Commission);
    filedContent->set_credit(tmpAccountInfo.Credit);
    filedContent->set_currmargin(tmpAccountInfo.CurrMargin);
    filedContent->set_currencyid(tmpAccountInfo.CurrencyID);
    filedContent->set_deliverymargin(tmpAccountInfo.DeliveryMargin);
    filedContent->set_deposit(tmpAccountInfo.Deposit);
    filedContent->set_exchangedeliverymargin(tmpAccountInfo.ExchangeDeliveryMargin);
    filedContent->set_exchangemargin(tmpAccountInfo.ExchangeMargin);
    filedContent->set_frozencash(tmpAccountInfo.FrozenCash);
    filedContent->set_frozencommission(tmpAccountInfo.FrozenCommission);
    filedContent->set_frozenmargin(tmpAccountInfo.FrozenMargin);
    filedContent->set_frozenswap(tmpAccountInfo.FrozenSwap);
    filedContent->set_fundmortgageavailable(tmpAccountInfo.FundMortgageAvailable);
    filedContent->set_fundmortgagein(tmpAccountInfo.FundMortgageIn);
    filedContent->set_fundmortgageout(tmpAccountInfo.FundMortgageOut);
    filedContent->set_interest(tmpAccountInfo.Interest);
    filedContent->set_interestbase(tmpAccountInfo.InterestBase);
    filedContent->set_mortgage(tmpAccountInfo.Mortgage);
    filedContent->set_mortgageablefund(tmpAccountInfo.MortgageableFund);
    filedContent->set_positionprofit(tmpAccountInfo.PositionProfit);
    filedContent->set_prebalance(tmpAccountInfo.PreBalance);
    filedContent->set_precredit(tmpAccountInfo.PreCredit);
    filedContent->set_predeposit(tmpAccountInfo.PreDeposit);
    filedContent->set_prefundmortgagein(tmpAccountInfo.PreFundMortgageIn);
    filedContent->set_prefundmortgageout(tmpAccountInfo.PreFundMortgageOut);
    filedContent->set_premargin(tmpAccountInfo.PreMargin);
    filedContent->set_premortgage(tmpAccountInfo.PreMortgage);
    filedContent->set_remainswap(tmpAccountInfo.RemainSwap);
    filedContent->set_reserve(tmpAccountInfo.Reserve);
    filedContent->set_reservebalance(tmpAccountInfo.ReserveBalance);
    filedContent->set_settlementid(tmpAccountInfo.SettlementID);
    filedContent->set_specproductcloseprofit(tmpAccountInfo.SpecProductCloseProfit);
    filedContent->set_specproductcommission(tmpAccountInfo.SpecProductCommission);
    filedContent->set_specproductexchangemargin(tmpAccountInfo.SpecProductExchangeMargin);
    filedContent->set_specproductfrozencommission(tmpAccountInfo.SpecProductFrozenCommission);
    filedContent->set_specproductfrozenmargin(tmpAccountInfo.SpecProductFrozenMargin);
    filedContent->set_specproductmargin(tmpAccountInfo.SpecProductMargin);
    filedContent->set_specproductpositionprofit(tmpAccountInfo.SpecProductPositionProfit);
    filedContent->set_specproductpositionprofitbyalg(tmpAccountInfo.SpecProductPositionProfitByAlg);
    filedContent->set_tradingday(tmpAccountInfo.TradingDay);
    filedContent->set_withdraw(tmpAccountInfo.Withdraw);
    filedContent->set_withdrawquota(tmpAccountInfo.WithdrawQuota);

    std::string strRsp = rsp.SerializeAsString();
    std::string head = "strategy_trader.AccountStatusRsq";
    auto& recerSender = RecerSender::getInstance();
    bool sendRes = recerSender.ROLE(Sender).ROLE(ProxySender).send(head.c_str(), strRsp.c_str());
    utils::printProtoMsg(rsp);
    if(!sendRes)
    {
        ERROR_LOG("send OrderInsertRsp error");
        return;
    }
    return;
}

void StrategyEvent::OrderInsertReqHandle(MsgStruct& msg)
{
    strategy_trader::message reqMsg;
    reqMsg.ParseFromString(msg.pbMsg);
    utils::printProtoMsg(reqMsg);

    const auto& orderInsertReq = reqMsg.order_insert_req();
    std::string identity = orderInsertReq.identity();
    auto& traderSer = TraderSevice::getInstance();
    if(!traderSer.ROLE(Trader).ROLE(CtpTraderApi).isLogIN)
    {
        ERROR_LOG("ctp not login!");
        pubOrderInsertRsp(identity,false, ORDER_BUILD_ERROR);
        return;
    }
    auto& orderIndication = orderInsertReq.order();

    auto& orderManage = traderSer.ROLE(OrderManage);
    std::string newOrderRef = utils::genOrderRef();
    if(! orderManage.addOrder(newOrderRef))
    {
        ERROR_LOG("add order [%s] to OrderManage error", newOrderRef.c_str());
        pubOrderInsertRsp(identity,false, ORDER_BUILD_ERROR);
        return;
    }


    if( ! orderManage.buildOrder(newOrderRef, orderInsertReq))
    {
        ERROR_LOG("build order failed, the orderRef is [%s]",newOrderRef.c_str());
        pubOrderInsertRsp(identity,false,ORDER_BUILD_ERROR);
        return;
    }
//    auto* newOrder = orderManage.getOrder(newOrderRef);
    auto& orderContent = orderManage.getOrderContent(newOrderRef);

    orderContent.orderRef = newOrderRef;
    orderContent.identityId = identity;
    orderContent.instrumentID = orderContent.ROLE(CThostFtdcInputOrderField).InstrumentID;
    orderContent.investorId  = orderContent.ROLE(CThostFtdcInputOrderField).InvestorID;
    orderContent.userId = orderContent.ROLE(CThostFtdcInputOrderField).UserID;
    auto* traderApi = traderSer.ROLE(Trader).ROLE(CtpTraderApi).traderApi;
    auto& ctpRspResultMonitor = InsertResult::getInstance();
    ctpRspResultMonitor.addResultMonitor(newOrderRef);

    auto* newOrder = &orderContent.ROLE(CThostFtdcInputOrderField);
    int insertResult = traderApi->ReqOrderInsert(newOrder);

    std::string semName = "trader_ReqOrderInsert" + std::string(newOrder->OrderRef);;
    globalSem.waitSemBySemName(semName);
    INFO_LOG("waitSemBySemName [%s] ok",semName.c_str());
    globalSem.delOrderSem(semName);

    auto insertRes = ctpRspResultMonitor.getRspMonitorResult(newOrderRef);
    if(insertRes != InsertRspResult::Success)
    {
        ERROR_LOG("insert order failed");
        std::string reason = insertRes == InsertRspResult::Failed ? ORDER_FILL_ERROR
                                                                  : ORDER_CANCEL;
        pubOrderInsertRsp(identity,false, reason);
        return;
    }

    INFO_LOG("insert order success");
    pubOrderInsertRsp(identity,true,"success");
    return;
}

void StrategyEvent::pubOrderInsertRsp(std::string identity, bool result, std::string reason)
{
    strategy_trader::message rsp;
    auto* insertRsp = rsp.mutable_order_insert_rsp();
    insertRsp->set_identity(identity);
    auto rspResult = result ? strategy_trader::Result::success : strategy_trader::Result::failed;
    insertRsp->set_result(rspResult);

    if(!result)
    {
        if(reason == std::string(ORDER_BUILD_ERROR))
        {
            insertRsp->set_reason(strategy_trader::FailedReason::Strategy_Ind_Error);
        }
        if(reason == std::string(ORDER_FILL_ERROR))
        {
            insertRsp->set_reason(strategy_trader::FailedReason::Order_Fill_Error);
        }
        if(reason == std::string(ORDER_CANCEL))
        {
            insertRsp->set_reason(strategy_trader::FailedReason::Order_Cancel);
        }
    }

    std::string strRsp = rsp.SerializeAsString();
    std::string head = "strategy_trader.OrderInsertRsp";
    auto& recerSender = RecerSender::getInstance();
    bool sendRes = recerSender.ROLE(Sender).ROLE(ProxySender).send(head.c_str(), strRsp.c_str());
    utils::printProtoMsg(rsp);
    if(!sendRes)
    {
        ERROR_LOG("send OrderInsertRsp error");
    }
    return;
}
