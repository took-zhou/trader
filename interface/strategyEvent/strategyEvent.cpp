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
#include "trader/domain/components/orderstates.h"
#include <fstream>
#include <iomanip>
#include<iostream>

#include "common/self/semaphorePart.h"
extern GlobalSem globalSem;

namespace{
    void sendEail(const std::string& fileName, const std::string& instrumentId)
    {
        // INFO_LOG("order insert success, begin send emails");
        auto& jsonCfg = utils::JsonConfig::getInstance();
        const auto& jsonCfgPath = jsonCfg.fileName;
        std::string command = jsonCfg.getConfig("emailbox", "command").get<std::string>();
        command += std::string(" " + jsonCfgPath + " " + fileName + " " + instrumentId);
        system(command.c_str());
        // INFO_LOG("exec [%s] ok",command.c_str());
    }

    bool saveAttachment(const std::string& identity, std::string& fileName, std::string& instrumentId)
    {
        const auto& traderSer = TraderSevice::getInstance();
        auto& orderManage = traderSer.ROLE(OrderManage);
        const auto& orderContent = orderManage.getOrderCOntentByIdentityId(identity);
        if(!orderContent.isValid())
        {
            ERROR_LOG("saveAttachment can not find order in local, identity[%s]",identity.c_str());
            return false;
        }

        auto& jsonCfg = utils::JsonConfig::getInstance();
        std::string fileRootPath = jsonCfg.getConfig("emailbox", "EmailAttachmentPath").get<std::string>();
        fileName = fileRootPath + orderContent.instrumentID +identity + utils::genEmailId()+std::string(".txt");
        if(!utils::isFileExist(fileName))
        {
            utils::creatFile(fileName);
        }
        std::string saveContent = "";
        ofstream outfile(fileName, ios::app);
        if(!outfile.is_open())
        {
            ERROR_LOG("open file[%s] failed",fileName.c_str());
            return false;
        }
        saveContent += "账户：" +orderContent.userId+"\n";
        saveContent += "合约：       "    +orderContent.instrumentID+"\n";
        saveContent += "下单价格："  +utils::doubleToStringConvert(orderContent.LimitPrice)+"\n";
        saveContent += "成交价格："  +utils::doubleToStringConvert(orderContent.tradedOrder.price)+"\n";
        saveContent += "成交日期："+orderContent.tradedOrder.date+"\n";
        saveContent += "成交时间："+orderContent.tradedOrder.time+"\n";
        saveContent += "方向：       "    +orderContent.tradedOrder.direction+"\n";
        saveContent += "下单数量："+utils::intToString(orderContent.VolumeTotalOriginal)+"\n";
        saveContent += "本批成交数量："+utils::intToString(orderContent.tradedOrder.volume)+"\n";
        outfile << saveContent;
        outfile.close();
        instrumentId = orderContent.instrumentID;
        // INFO_LOG("save attachment ok");
        return true;
    }
}

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
    int cnt = 0;
    msgFuncMap.clear();
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("OrderInsertReq",     [this](MsgStruct& msg){OrderInsertReqHandle(msg);}));
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("AccountStatusReq",   [this](MsgStruct& msg){AccountStatusReqHandle(msg);}));
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("OrderCancelReq",     [this](MsgStruct& msg){OrderCancelReqHandle(msg);}));
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("MarginRateReq",     [this](MsgStruct& msg){MarginRateReqHandle(msg);}));
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("CommissionRateReq",     [this](MsgStruct& msg){CommissionRateReqHandle(msg);}));
    msgFuncMap.insert(std::pair<std::string, std::function<void(MsgStruct& msg)>>("InstrumentReq",     [this](MsgStruct& msg){InstrumentReqHandle(msg);}));

    for(auto iter : msgFuncMap)
    {
        INFO_LOG("sessionFuncMap[%d] key is [%s]",cnt, iter.first.c_str());
        cnt++;
    }
    return;
}

void StrategyEvent::OrderCancelReqHandle(MsgStruct& msg)
{
    strategy_trader::message reqMsg;
    reqMsg.ParseFromString(msg.pbMsg);
    utils::printProtoMsg(reqMsg);

    auto& orderCancelReq = reqMsg.order_cancel_req();
    OrderIdentify IdentifyId;
    IdentifyId.prid = orderCancelReq.process_random_id();
    IdentifyId.identity = orderCancelReq.identity();
    auto& traderSer = TraderSevice::getInstance();
    if(!traderSer.ROLE(Trader).ROLE(CtpTraderApi).isLogIN)
    {
        ERROR_LOG("ctp not login!");
        pubOrderCancelRsp(IdentifyId, false, "ctp_logout");
        return;
    }
    auto& orderManage = traderSer.ROLE(OrderManage);
    auto& orderContent = orderManage.getOrderCOntentByIdentityId(IdentifyId.identity);
    if(!orderContent.isValid())
    {
        std::string reason = "invalidOrderIdentityId";
        pubOrderCancelRsp(IdentifyId, false, reason);
        return;
    }

    orderContent.activeCancleIndication = true;
    auto* traderApi = traderSer.ROLE(Trader).ROLE(CtpTraderApi).traderApi;
    CThostFtdcInputOrderActionField orderActionReq;

    traderApi->ReqOrderAction(orderContent);
}

void StrategyEvent::pubOrderCancelRsp(OrderIdentify identityId, bool result, const std::string& reason)
{
    auto& traderSer = TraderSevice::getInstance();
    auto& orderManage = traderSer.ROLE(OrderManage);
    auto& orderContent = orderManage.getOrderCOntentByIdentityId(identityId.identity);
    if(!orderContent.isValid())
    {
        ERROR_LOG("invalid order, identityId[%s]",identityId.identity.c_str());
        return;
    }
    if(orderContent.isFlowFinish)
    {
        INFO_LOG("flow has finished identityId[%s]",identityId.identity.c_str());
        return;
    }
    orderContent.isFlowFinish = true;
    strategy_trader::message rspMsg;
    auto* orderCancelRsp  = rspMsg.mutable_order_cancel_rsp();
    orderCancelRsp->set_identity(identityId.identity);
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
    auto reqInfo = reqMsg.account_status_req();
    int field  = reqInfo.field();
    auto identify = reqInfo.process_random_id();
    auto& traderSer = TraderSevice::getInstance();
    if(!traderSer.ROLE(Trader).ROLE(CtpTraderApi).isLogIN)
    {
        ERROR_LOG("ctp not login!");
        pubAccountStatusRsp(identify, field, false, "ctp_logout");
        return;
    }

    traderSer.ROLE(Trader).ROLE(TmpStore).accountInfo.ProcessRandomId = identify;
    auto* traderApi = traderSer.ROLE(Trader).ROLE(CtpTraderApi).traderApi;

    if (traderApi->ReqQryTradingAccount() != 0)
    {
        ERROR_LOG("req error!");
        pubAccountStatusRsp(identify, field, false, "req error");
        return;
    }

    std::string semName = "trader_ReqQryTradingAccount";
    globalSem.waitSemBySemName(semName);
    INFO_LOG("waitSemBySemName [%s] ok",semName.c_str());
    globalSem.delOrderSem(semName);

    if (traderSer.ROLE(Trader).ROLE(TmpStore).accountInfo.rsp_is_null == true)
    {
        pubAccountStatusRsp(identify, field, false, "rsp is null");
        return;
    }
    pubAccountStatusRsp(identify, field, true);
}

void StrategyEvent::pubAccountStatusRsp(std::string identity, int field, bool result, const std::string& reason)
{
    strategy_trader::message rsp;
    auto* accountRsp = rsp.mutable_account_status_rsp();
    auto& traderSer = TraderSevice::getInstance();
    accountRsp->set_result(result?strategy_trader::Result::success:strategy_trader::Result::failed);
    accountRsp->set_field_name((strategy_trader::AccountFiledReq)field);

    if (result == true)
    {
        auto& tmpAccountInfo = traderSer.ROLE(Trader).ROLE(TmpStore).accountInfo;
        auto* filedContent = accountRsp->mutable_filed_content();

        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::Available)
        {
            filedContent->set_available(tmpAccountInfo.Available);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::Balance)
        {
            filedContent->set_balance(tmpAccountInfo.Balance);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::BizType)
        {
            filedContent->set_biztype(tmpAccountInfo.BizType);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::CashIn)
        {
            filedContent->set_cashin(tmpAccountInfo.CashIn);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::CloseProfit)
        {
            filedContent->set_closeprofit(tmpAccountInfo.CloseProfit);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::Commission)
        {
            filedContent->set_commission(tmpAccountInfo.Commission);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::Credit)
        {
            filedContent->set_credit(tmpAccountInfo.Credit);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::CurrMargin)
        {
            filedContent->set_currmargin(tmpAccountInfo.CurrMargin);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::CurrencyID)
        {
            filedContent->set_currencyid(tmpAccountInfo.CurrencyID);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::DeliveryMargin)
        {
            filedContent->set_deliverymargin(tmpAccountInfo.DeliveryMargin);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::Deposit)
        {
            filedContent->set_deposit(tmpAccountInfo.Deposit);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::ExchangeDeliveryMargin)
        {
            filedContent->set_exchangedeliverymargin(tmpAccountInfo.ExchangeDeliveryMargin);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::ExchangeMargin)
        {
            filedContent->set_exchangemargin(tmpAccountInfo.ExchangeMargin);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::FrozenCash)
        {
            filedContent->set_frozencash(tmpAccountInfo.FrozenCash);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::FrozenCommission)
        {
            filedContent->set_frozencommission(tmpAccountInfo.FrozenCommission);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::FrozenMargin)
        {
            filedContent->set_frozenmargin(tmpAccountInfo.FrozenMargin);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::FrozenSwap)
        {
            filedContent->set_frozenswap(tmpAccountInfo.FrozenSwap);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::FundMortgageAvailable)
        {
            filedContent->set_fundmortgageavailable(tmpAccountInfo.FundMortgageAvailable);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::FundMortgageIn)
        {
            filedContent->set_fundmortgagein(tmpAccountInfo.FundMortgageIn);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::FundMortgageOut)
        {
            filedContent->set_fundmortgageout(tmpAccountInfo.FundMortgageOut);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::Interest)
        {
            filedContent->set_interest(tmpAccountInfo.Interest);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::InterestBase)
        {
            filedContent->set_interestbase(tmpAccountInfo.InterestBase);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::Mortgage)
        {
            filedContent->set_mortgage(tmpAccountInfo.Mortgage);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::MortgageableFund)
        {
            filedContent->set_mortgageablefund(tmpAccountInfo.MortgageableFund);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::PositionProfit)
        {
            filedContent->set_positionprofit(tmpAccountInfo.PositionProfit);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::PreBalance)
        {
            filedContent->set_prebalance(tmpAccountInfo.PreBalance);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::PreCredit)
        {
            filedContent->set_precredit(tmpAccountInfo.PreCredit);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::PreDeposit)
        {
            filedContent->set_predeposit(tmpAccountInfo.PreDeposit);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::PreFundMortgageIn)
        {
            filedContent->set_prefundmortgagein(tmpAccountInfo.PreFundMortgageIn);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::PreFundMortgageOut)
        {
            filedContent->set_prefundmortgageout(tmpAccountInfo.PreFundMortgageOut);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::PreMargin)
        {
            filedContent->set_premargin(tmpAccountInfo.PreMargin);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::PreMortgage)
        {
            filedContent->set_premortgage(tmpAccountInfo.PreMortgage);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::RemainSwap)
        {
            filedContent->set_remainswap(tmpAccountInfo.RemainSwap);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::Reserve)
        {
            filedContent->set_reserve(tmpAccountInfo.Reserve);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::ReserveBalance)
        {
            filedContent->set_reservebalance(tmpAccountInfo.ReserveBalance);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::SettlementID)
        {
            filedContent->set_settlementid(tmpAccountInfo.SettlementID);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::SpecProductCloseProfit)
        {
            filedContent->set_specproductcloseprofit(tmpAccountInfo.SpecProductCloseProfit);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::SpecProductCommission)
        {
            filedContent->set_specproductcommission(tmpAccountInfo.SpecProductCommission);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::SpecProductExchangeMargin)
        {
            filedContent->set_specproductexchangemargin(tmpAccountInfo.SpecProductExchangeMargin);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::SpecProductFrozenCommission)
        {
            filedContent->set_specproductfrozencommission(tmpAccountInfo.SpecProductFrozenCommission);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::SpecProductFrozenMargin)
        {
            filedContent->set_specproductfrozenmargin(tmpAccountInfo.SpecProductFrozenMargin);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::SpecProductMargin)
        {
            filedContent->set_specproductmargin(tmpAccountInfo.SpecProductMargin);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::SpecProductPositionProfit)
        {
            filedContent->set_specproductpositionprofit(tmpAccountInfo.SpecProductPositionProfit);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::SpecProductPositionProfitByAlg)
        {
            filedContent->set_specproductpositionprofitbyalg(tmpAccountInfo.SpecProductPositionProfitByAlg);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::TradingDay)
        {
            filedContent->set_tradingday(tmpAccountInfo.TradingDay);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::Withdraw)
        {
            filedContent->set_withdraw(tmpAccountInfo.Withdraw);
        }
        if (field == strategy_trader::AccountFiledReq::AllInfo || field == strategy_trader::AccountFiledReq::WithdrawQuota)
        {
            filedContent->set_withdrawquota(tmpAccountInfo.WithdrawQuota);
        }
    }
    else
    {
        accountRsp->set_failedreason(reason);
    }

    std::string strRsp = rsp.SerializeAsString();
    std::string head = "strategy_trader.AccountStatusRsp." + identity;
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
    // utils::printProtoMsg(reqMsg);

    const auto& orderInsertReq = reqMsg.order_insert_req();
    OrderIdentify identity;
    identity.identity = orderInsertReq.identity();
    identity.prid = orderInsertReq.process_random_id();
    auto& traderSer = TraderSevice::getInstance();
    if(!traderSer.ROLE(Trader).ROLE(CtpTraderApi).isLogIN)
    {
        ERROR_LOG("ctp not login!");
        pubOrderInsertRsp(identity, false, ORDER_BUILD_ERROR);
        return;
    }

    // ("identity[%s] insert begin",identity.identity.c_str());
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
        orderManage.delOrder(newOrderRef);
        return;
    }

    auto& orderContent = orderManage.getOrderContent(newOrderRef);
    orderContent.orderRef = newOrderRef;
    orderContent.identityId.prid  = identity.prid;
    orderContent.identityId.identity = identity.identity;
    orderContent.instrumentID = orderContent.ROLE(CThostFtdcInputOrderField).InstrumentID;
    orderContent.investorId  = orderContent.ROLE(CThostFtdcInputOrderField).InvestorID;
    orderContent.userId = orderContent.ROLE(CThostFtdcInputOrderField).UserID;
    auto* traderApi = traderSer.ROLE(Trader).ROLE(CtpTraderApi).traderApi;
    auto& ctpRspResultMonitor = InsertResult::getInstance();
    ctpRspResultMonitor.addResultMonitor(newOrderRef);
    auto* newOrder = orderManage.getOrder(newOrderRef);
    traderApi->ReqOrderInsert(newOrder);

    return;
}

void StrategyEvent::pubOrderInsertRsp(OrderIdentify identity, bool result, std::string reason)
{
    strategy_trader::message rsp;
    auto* insertRsp = rsp.mutable_order_insert_rsp();
    insertRsp->set_identity(identity.identity);
    auto rspResult = result ? strategy_trader::Result::success : strategy_trader::Result::failed;
    insertRsp->set_result(rspResult);
    if(result)
    {
        auto* succInfo = insertRsp->mutable_info();
        if(succInfo == nullptr)
        {
            ERROR_LOG("mutable_info error! identity[%s]", identity.identity.c_str());
            return;
        }
        auto& traderSer = TraderSevice::getInstance();
        auto& orderManage = traderSer.ROLE(OrderManage);
        auto& orderContent = orderManage.getOrderCOntentByIdentityId(identity.identity);
        if(!orderContent.isValid())
        {
            ERROR_LOG("OnRtnTrade can not find order in local, identity[%s]",identity.identity.c_str());
            return;
        }
        succInfo->set_orderprice(utils::doubleToStringConvert(orderContent.tradedOrder.price));
        succInfo->set_ordervolume(orderContent.tradedOrder.volume);
    }

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
    std::string head = "strategy_trader.OrderInsertRsp." + identity.prid;
    auto& recerSender = RecerSender::getInstance();
    bool sendRes = recerSender.ROLE(Sender).ROLE(ProxySender).send(head.c_str(), strRsp.c_str());
    utils::printProtoMsg(rsp);
    if(!sendRes)
    {
        ERROR_LOG("send OrderInsertRsp error");
    }
    auto& orderStates = OrderStates::getInstance();
    orderStates.showAllOrderStates();
    if(result)
    {
        std::string fileName{""}, instrumentId{""};
        if(saveAttachment(identity.identity, fileName, instrumentId))
        {
            auto sendfunc = [=](){
                sendEail(fileName, instrumentId);
            };
            std::thread(sendfunc).detach();
        }
    }
    return;
}

void StrategyEvent::MarginRateReqHandle(MsgStruct& msg)
{
    strategy_trader::message reqMsg;
    reqMsg.ParseFromString(msg.pbMsg);
    utils::printProtoMsg(reqMsg);
    auto identify = reqMsg.margin_rate_req().process_random_id();
    auto& traderSer = TraderSevice::getInstance();
    if(!traderSer.ROLE(Trader).ROLE(CtpTraderApi).isLogIN)
    {
        ERROR_LOG("ctp not login!");
        pubMarginRateRsp(identify, false, "ctp_logout");
        return;
    }

    traderSer.ROLE(Trader).ROLE(TmpStore).marginRate.ProcessRandomId = identify;
    utils::InstrumtntID ins_exch;
    ins_exch.ins = reqMsg.margin_rate_req().instrument_info().instrument_id();
    ins_exch.exch = reqMsg.margin_rate_req().instrument_info().exchange_id();
    auto* traderApi = traderSer.ROLE(Trader).ROLE(CtpTraderApi).traderApi;

    if (traderApi->ReqQryInstrumentMarginRate(ins_exch) != 0)
    {
        ERROR_LOG("req error!");
        pubMarginRateRsp(identify, false, "req error");
        return;
    }

    std::string semName = "margin_rate";
    globalSem.waitSemBySemName(semName);
    INFO_LOG("waitSemBySemName [%s] ok",semName.c_str());
    globalSem.delOrderSem(semName);

    if (traderSer.ROLE(Trader).ROLE(TmpStore).marginRate.rsp_is_null == true)
    {
        pubMarginRateRsp(identify, false, "rsp is null");
        return;
    }

    pubMarginRateRsp(identify, true);
}

void StrategyEvent::pubMarginRateRsp(std::string identity, bool result, const std::string& reason)
{
    strategy_trader::message rsp;
    auto* marginRateRsp = rsp.mutable_margin_rate_rsp();
    auto& traderSer = TraderSevice::getInstance();
    marginRateRsp->set_result(result?strategy_trader::Result::success:strategy_trader::Result::failed);

    if (result == true)
    {
        auto& marginRateInfo = traderSer.ROLE(Trader).ROLE(TmpStore).marginRate;
        marginRateRsp->set_longmarginratiobymoney(marginRateInfo.LongMarginRatioByMoney);
        marginRateRsp->set_longmarginratiobyvolume(marginRateInfo.LongMarginRatioByVolume);
        marginRateRsp->set_shortmarginratiobymoney(marginRateInfo.ShortMarginRatioByMoney);
        marginRateRsp->set_shortmarginratiobyvolume(marginRateInfo.ShortMarginRatioByVolume);
    }
    else
    {
        marginRateRsp->set_failedreason(reason);
    }

    std::string strRsp = rsp.SerializeAsString();
    std::string head = "strategy_trader.MarginRateRsp." + identity;
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

void StrategyEvent::CommissionRateReqHandle(MsgStruct& msg)
{
    strategy_trader::message reqMsg;
    reqMsg.ParseFromString(msg.pbMsg);
    utils::printProtoMsg(reqMsg);
    auto identify = reqMsg.commission_rate_req().process_random_id();
    auto& traderSer = TraderSevice::getInstance();
    if(!traderSer.ROLE(Trader).ROLE(CtpTraderApi).isLogIN)
    {
        ERROR_LOG("ctp not login!");
        pubCommissionRateRsp(identify, false, "ctp_logout");
        return;
    }

    traderSer.ROLE(Trader).ROLE(TmpStore).commissionRate.ProcessRandomId = identify;
    utils::InstrumtntID ins_exch;
    ins_exch.ins = reqMsg.commission_rate_req().instrument_info().instrument_id();
    ins_exch.exch = reqMsg.commission_rate_req().instrument_info().exchange_id();
    auto* traderApi = traderSer.ROLE(Trader).ROLE(CtpTraderApi).traderApi;

    if (traderApi->ReqQryInstrumentCommissionRate(ins_exch) != 0)
    {
        ERROR_LOG("req error!");
        pubCommissionRateRsp(identify, false, "req error");
        return;
    }

    std::string semName = "commission_rate";
    globalSem.waitSemBySemName(semName);
    INFO_LOG("waitSemBySemName [%s] ok",semName.c_str());
    globalSem.delOrderSem(semName);

    if (traderSer.ROLE(Trader).ROLE(TmpStore).commissionRate.rsp_is_null == true)
    {
        pubCommissionRateRsp(identify, false, "rsp is null");
        return;
    }

    pubCommissionRateRsp(identify, true);
}

void StrategyEvent::pubCommissionRateRsp(std::string identity, bool result, const std::string& reason)
{
    strategy_trader::message rsp;
    auto* commissionRateRsp = rsp.mutable_commission_rate_rsp();
    auto& traderSer = TraderSevice::getInstance();
    commissionRateRsp->set_result(result?strategy_trader::Result::success:strategy_trader::Result::failed);

    if (result == true)
    {
        auto& commissionRateInfo = traderSer.ROLE(Trader).ROLE(TmpStore).commissionRate;
        commissionRateRsp->set_openratiobymoney(commissionRateInfo.OpenRatioByMoney);
        commissionRateRsp->set_openratiobyvolume(commissionRateInfo.OpenRatioByVolume);
        commissionRateRsp->set_closeratiobymoney(commissionRateInfo.CloseRatioByMoney);
        commissionRateRsp->set_closeratiobyvolume(commissionRateInfo.CloseRatioByVolume);
        commissionRateRsp->set_closetodayratiobymoney(commissionRateInfo.CloseTodayRatioByMoney);
        commissionRateRsp->set_closetodayratiobyvolume(commissionRateInfo.CloseTodayRatioByVolume);
    }
    else
    {
        commissionRateRsp->set_failedreason(reason);
    }

    std::string strRsp = rsp.SerializeAsString();
    std::string head = "strategy_trader.CommissionRateRsp." + identity;
    auto& recerSender = RecerSender::getInstance();
    bool sendRes = recerSender.ROLE(Sender).ROLE(ProxySender).send(head.c_str(), strRsp.c_str());
    utils::printProtoMsg(rsp);
    if(!sendRes)
    {
        ERROR_LOG("send CommissionRateRsp error");
        return;
    }
    return;
}

void StrategyEvent::InstrumentReqHandle(MsgStruct& msg)
{
    strategy_trader::message reqMsg;
    reqMsg.ParseFromString(msg.pbMsg);
    utils::printProtoMsg(reqMsg);
    auto identify = reqMsg.instrument_req().process_random_id();
    auto& traderSer = TraderSevice::getInstance();
    if(!traderSer.ROLE(Trader).ROLE(CtpTraderApi).isLogIN)
    {
        ERROR_LOG("ctp not login!");
        pubInstrumentRsp(identify, false, "ctp_logout");
        return;
    }

    traderSer.ROLE(Trader).ROLE(TmpStore).instrumentInfo.ProcessRandomId = identify;
    utils::InstrumtntID ins_exch;
    ins_exch.ins = reqMsg.instrument_req().instrument_info().instrument_id();
    ins_exch.exch = reqMsg.instrument_req().instrument_info().exchange_id();
    auto* traderApi = traderSer.ROLE(Trader).ROLE(CtpTraderApi).traderApi;

    if (traderApi->ReqQryInstrument(ins_exch) != 0)
    {
        ERROR_LOG("req error!");
        pubInstrumentRsp(identify, false, "req error");
        return;
    }

    std::string semName = "trader_ReqQryInstrument_Single";
    globalSem.waitSemBySemName(semName);
    INFO_LOG("waitSemBySemName [%s] ok",semName.c_str());
    globalSem.delOrderSem(semName);

    if (traderSer.ROLE(Trader).ROLE(TmpStore).instrumentInfo.rsp_is_null == true)
    {
        pubInstrumentRsp(identify, false, "rsp is null");
        return;
    }

    pubInstrumentRsp(identify, true);
}

void StrategyEvent::pubInstrumentRsp(std::string identity, bool result, const std::string& reason)
{
    strategy_trader::message rsp;
    auto* instrumentRsp = rsp.mutable_instrument_rsp();
    auto& traderSer = TraderSevice::getInstance();
    instrumentRsp->set_result(result?strategy_trader::Result::success:strategy_trader::Result::failed);

    if (result == true)
    {
        auto& instrumentInfo = traderSer.ROLE(Trader).ROLE(TmpStore).instrumentInfo;
        instrumentRsp->set_is_trading(instrumentInfo.is_trading);
        instrumentRsp->set_max_limit_order_volume(instrumentInfo.max_limit_order_volume);
        instrumentRsp->set_max_market_order_volume(instrumentInfo.max_market_order_volume);
        instrumentRsp->set_min_limit_order_volume(instrumentInfo.min_limit_order_volume);
        instrumentRsp->set_min_market_order_volume(instrumentInfo.min_market_order_volume);
        instrumentRsp->set_price_tick(instrumentInfo.price_tick);
        instrumentRsp->set_volume_multiple(instrumentInfo.volume_multiple);
    }
    else
    {
        instrumentRsp->set_failedreason(reason);
    }

    std::string strRsp = rsp.SerializeAsString();
    std::string head = "strategy_trader.InstrumentRsp." + identity;
    auto& recerSender = RecerSender::getInstance();
    bool sendRes = recerSender.ROLE(Sender).ROLE(ProxySender).send(head.c_str(), strRsp.c_str());
    utils::printProtoMsg(rsp);
    if(!sendRes)
    {
        ERROR_LOG("send InstrumentRsp error");
        return;
    }
    return;
}
