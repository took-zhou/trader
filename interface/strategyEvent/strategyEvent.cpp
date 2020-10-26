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
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("OrderInsertReq",   [this](MsgStruct& msg){OrderInsertReqHandle(msg);}));

    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("AccountStatusReq",   [this](MsgStruct& msg){AccountStatusReqHandle(msg);}));return;
}

void StrategyEvent::AccountStatusReqHandle(MsgStruct& msg)
{
    auto& traderSer = TraderSevice::getInstance();
    auto* traderApi = traderSer.ROLE(Trader).ROLE(CtpTraderApi).traderApi;
    traderApi->ReqQryTradingAccount();
    std::string semName = "trader_ReqQryTradingAccount";
    globalSem.waitSemBySemName(semName);
    INFO_LOG("waitSemBySemName [%s] ok",semName.c_str());
    globalSem.delOrderSem(semName);
    pubAccountStatusRsq();
}

void StrategyEvent::pubAccountStatusRsq()
{
    strategy_trader::message rsp;
    auto* accountRsp = rsp.mutable_account_status_rsq();
    auto& traderSer = TraderSevice::getInstance();

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
    if(sendRes)
    {
        ERROR_LOG("send OrderInsertRsp error");
        return;
    }
    return;
}

void StrategyEvent::OrderInsertReqHandle(MsgStruct& msg)
{
    static strategy_trader::message reqMsg;
    reqMsg.ParseFromString(msg.pbMsg);
    utils::printProtoMsg(reqMsg);

    const auto& orderInsertReq = reqMsg.order_insert_req();
    std::string identity = orderInsertReq.identity();
    auto& orderIndication = orderInsertReq.order();

    auto& traderSer = TraderSevice::getInstance();
    auto& orderManage = traderSer.ROLE(OrderManage);
    std::string newOrderRef = utils::genOrderRef();
    if(! orderManage.addOrder(newOrderRef))
    {
        ERROR_LOG("add order [%s] to OrderManage error", newOrderRef.c_str());
        pubOrderInsertRsp(identity,false);
        return;
    }
    if( ! orderManage.buildOrder(newOrderRef, orderInsertReq))
    {
        ERROR_LOG("build order failed, the orderRef is [%s]",newOrderRef.c_str());
        pubOrderInsertRsp(identity,false);
        return;
    }

    auto* traderApi = traderSer.ROLE(Trader).ROLE(CtpTraderApi).traderApi;
    auto* newOrder = orderManage.getOrder(newOrderRef);

    auto& ctpRspResultMonitor = InsertResult::getInstance();
    ctpRspResultMonitor.addResultMonitor(newOrderRef);

    int insertResult = traderApi->ReqOrderInsert(newOrder);

    std::string semName = "trader_ReqOrderInsert" + std::string(newOrder->OrderRef);;
    globalSem.waitSemBySemName(semName);
    INFO_LOG("waitSemBySemName [%s] ok",semName.c_str());
    globalSem.delOrderSem(semName);

    auto insertRes = ctpRspResultMonitor.getRspMonitorResult(newOrderRef);
    if(insertRes != InsertRspResult::Success)
    {
        ERROR_LOG("insert order failed");
        pubOrderInsertRsp(identity,false);
        return;
    }

    INFO_LOG("insert order success");
    pubOrderInsertRsp(identity,true);
    return;
}

void StrategyEvent::pubOrderInsertRsp(std::string identity, bool result)
{
    strategy_trader::message rsp;
    auto* insertRsp = rsp.mutable_order_insert_rsp();
    insertRsp->set_identity(identity);
    auto rspResult = result ? strategy_trader::Result::success : strategy_trader::Result::failed;
    insertRsp->set_result(rspResult);
    std::string strRsp = rsp.SerializeAsString();
    std::string head = "strategy_trader.OrderInsertRsp";
    auto& recerSender = RecerSender::getInstance();
    bool sendRes = recerSender.ROLE(Sender).ROLE(ProxySender).send(head.c_str(), strRsp.c_str());
    if(sendRes)
    {
        ERROR_LOG("send OrderInsertRsp error");
        return;
    }
    return;
}
