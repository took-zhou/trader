/*
 * ctpTradeApi.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#include "trader/domain/components/ctpTradeApi/ctpTradeApi.h"
#include "common/self/fileUtil.h"
#include "common/self/utils.h"
#include "common/self/basetype.h"
#include "common/self/timer.h"
#include "trader/infra/recer/ctpRecer.h"
#include "common/self/define.h"
#include <string>
#include <string.h>
#include <thread>
#include <array>
#include "trader/domain/components/settlementConfirm.h"
#include "trader/domain/components/order.h"
#include "common/self/semaphorePart.h"
extern GlobalSem globalSem;


int CtpTraderBaseApi::ReqQryInstrument(utils::InstrumtntID ins_exch)
{
    CThostFtdcQryInstrumentField requestMsg{0};
    std::strcpy(requestMsg.ExchangeID, ins_exch.exch.c_str());
    std::strcpy(requestMsg.InstrumentID, ins_exch.ins.c_str());

    std::string semName = "";
    if (ins_exch.exch == "" && ins_exch.ins == "")
    {
        semName = "trader_ReqQryInstrument_ALL";
    }
    else
    {
        semName = "trader_ReqQryInstrument_Single";
    }

    globalSem.addOrderSem(semName);
    int result = m_pApi->ReqQryInstrument(&requestMsg, requestIdBuildAlg());
    INFO_LOG("ReqQryInstrument send result is [%d]",result);
    return result;
}

int CtpTraderBaseApi::ReqQryTradingAccount()
{
    CThostFtdcQryTradingAccountField  requestMsg{0};
    auto& jsonCfg = utils::JsonConfig::getInstance();
    const std::string username = jsonCfg.getConfig("common","user").get<std::string>();
    const std::string brokerID = jsonCfg.getDeepConfig("users", username, "BrokerID").get<std::string>();
    const std::string investorID = jsonCfg.getDeepConfig("users", username, "InvestorID").get<std::string>();
    strcpy(requestMsg.InvestorID, investorID.c_str());
    strcpy(requestMsg.BrokerID, brokerID.c_str());
    strcpy(requestMsg.CurrencyID , "CNY");

    int result = m_pApi->ReqQryTradingAccount(&requestMsg, requestIdBuildAlg());
    INFO_LOG("ReqQryTradingAccount send result is [%d]",result);

    if (result == 0)
    {
        std::string semName = "trader_ReqQryTradingAccount";
        globalSem.addOrderSem(semName);
    }

    return result;
}

CThostFtdcTraderApi* CtpTraderBaseApi::CreateFtdcTraderApi(const char *pszFlowPath)
{
    m_pApi = CThostFtdcTraderApi::CreateFtdcTraderApi(pszFlowPath);
    return m_pApi;
}

const char* CtpTraderBaseApi::GetTradingDay()
{
    return m_pApi->GetTradingDay();
};

void CtpTraderBaseApi::RegisterFront(char* pszFrontAddress)
{
    m_pApi->RegisterFront(pszFrontAddress);
    INFO_LOG("RegisterFront [%s] ok",pszFrontAddress);
}

void CtpTraderBaseApi::SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType)
{
    m_pApi->SubscribePrivateTopic(nResumeType);
    INFO_LOG("SubscribePrivateTopic ok sub type is [%d]",(int)nResumeType);
}

void CtpTraderBaseApi::SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType)
{
    m_pApi->SubscribePublicTopic(nResumeType);
    INFO_LOG("SubscribePublicTopic ok sub type is [%d]",(int)nResumeType);
}

void CtpTraderBaseApi::RegisterSpi(CThostFtdcTraderSpi *pSpi)
{
    m_pApi->RegisterSpi(pSpi);
    INFO_LOG("RegisterSpi ok");
}

int CtpTraderBaseApi::ReqOrderAction(OrderContent& orderContent, char actionFlag)
{
    CThostFtdcInputOrderActionField orderActionReq{0};
    strcpy(orderActionReq.OrderRef, orderContent.orderRef.c_str());
    orderActionReq.FrontID=orderContent.frontId;
    orderActionReq.SessionID = orderContent.sessionId;
    strcpy(orderActionReq.InstrumentID, orderContent.instrumentID.c_str());
    strcpy(orderActionReq.InvestorID, orderContent.investorId.c_str());
    strcpy(orderActionReq.UserID, orderContent.userId.c_str());
    orderActionReq.ActionFlag = actionFlag;
    int result = m_pApi->ReqOrderAction(&orderActionReq, requestIdBuildAlg());
    INFO_LOG("ReqOrderAction send result is [%d]",result);
    return result;
}

int CtpTraderBaseApi::ReqSettlementInfoConfirm()
{
    CThostFtdcSettlementInfoConfirmField requestMsg{0};
    auto& jsonCfg = utils::JsonConfig::getInstance();
    const std::string username = jsonCfg.getConfig("common","user").get<std::string>();
    const std::string brokerID = jsonCfg.getDeepConfig("users", username, "BrokerID").get<std::string>();
    const std::string investorID = jsonCfg.getDeepConfig("users", username, "InvestorID").get<std::string>();
    strcpy(requestMsg.InvestorID, investorID.c_str());
    strcpy(requestMsg.BrokerID, brokerID.c_str());

    std::string semName = "trader_ReqSettlementInfoConfirm";
    globalSem.addOrderSem(semName);
    int result = m_pApi->ReqSettlementInfoConfirm(&requestMsg, requestIdBuildAlg());
    INFO_LOG("ReqSettlementInfoConfirm send result is [%d]",result);
    return result;
}

int CtpTraderBaseApi::ReqOrderInsert(CThostFtdcInputOrderField* pInputOrder)
{
    std::string semName = "trader_ReqOrderInsert"+std::string(pInputOrder->OrderRef);
    globalSem.addOrderSem(semName);
    int result = m_pApi->ReqOrderInsert(pInputOrder, requestIdBuildAlg());
    INFO_LOG("ReqOrderInsert send result is [%d], orderRef is[%s]",result,pInputOrder->OrderRef);
    return result;
}

int CtpTraderBaseApi::requestIdBuildAlg()
{
    if(requestID == S32_MAX)
    {
        requestID = 1;
        return requestID;
    }
    requestID++;
    return requestID;
}

int CtpTraderBaseApi::ReqAuthenticate()
{
    CThostFtdcReqAuthenticateField pReqAuthenticateField = {0};
    auto& jsonCfg = utils::JsonConfig::getInstance();
    const std::string username = jsonCfg.getConfig("common","user").get<std::string>();
    const std::string authCode = jsonCfg.getDeepConfig("users", username, "AuthCode").get<std::string>();
    const std::string appId = jsonCfg.getDeepConfig("users", username, "AppID").get<std::string>();
    const std::string userID = jsonCfg.getDeepConfig("users", username, "UserID").get<std::string>();
    const std::string brokerID = jsonCfg.getDeepConfig("users", username, "BrokerID").get<std::string>();

    strcpy(pReqAuthenticateField.BrokerID, brokerID.c_str());
    strcpy(pReqAuthenticateField.UserID, userID.c_str());
    //strcpy(a.UserProductInfo, "");
    strcpy(pReqAuthenticateField.AuthCode, authCode.c_str());
    strcpy(pReqAuthenticateField.AppID, appId.c_str());

    std::string semName = "trader_reqAuthenticate";
    globalSem.addOrderSem(semName);
    int result = m_pApi->ReqAuthenticate(&pReqAuthenticateField, requestIdBuildAlg());
    INFO_LOG("ReqAuthenticate send result is [%d]",result);
    return result;
}

int CtpTraderBaseApi::ReqUserLogin()
{
    CThostFtdcReqUserLoginField reqUserLogin{ 0 };
    auto& jsonCfg = utils::JsonConfig::getInstance();
    const std::string username = jsonCfg.getConfig("common","user").get<std::string>();
    const std::string userID = jsonCfg.getDeepConfig("users", username, "UserID").get<std::string>();
    const std::string brokerID = jsonCfg.getDeepConfig("users", username, "BrokerID").get<std::string>();
    const std::string passWord = jsonCfg.getDeepConfig("users", username, "Password").get<std::string>();
    strcpy(reqUserLogin.BrokerID, brokerID.c_str());
    strcpy(reqUserLogin.UserID, userID.c_str());
    strcpy(reqUserLogin.Password, passWord.c_str());

    std::string semName = "trader_logIn";
    globalSem.addOrderSem(semName);
    int result = m_pApi->ReqUserLogin(&reqUserLogin, requestIdBuildAlg());
    INFO_LOG("ReqUserLogin send result is [%d]",result);
    return result;
}

int CtpTraderBaseApi::ReqUserLogout()
{
    CThostFtdcUserLogoutField logOutField{0};
    auto& jsonCfg = utils::JsonConfig::getInstance();
    const std::string username = jsonCfg.getConfig("common","user").get<std::string>();
    const std::string userID = jsonCfg.getDeepConfig("users", username, "UserID").get<std::string>();
    const std::string brokerID = jsonCfg.getDeepConfig("users", username, "BrokerID").get<std::string>();
    strcpy(logOutField.BrokerID, brokerID.c_str());
    strcpy(logOutField.UserID, userID.c_str());

    std::string semName = "trader_logOut";
    globalSem.addOrderSem(semName);
    int result = m_pApi->ReqUserLogout(&logOutField, requestIdBuildAlg());
    INFO_LOG("ReqUserLogout send result is [%d]",result);
    return result;
}

int CtpTraderBaseApi::ReqQryInstrumentMarginRate(utils::InstrumtntID ins_exch)
{
    CThostFtdcQryInstrumentMarginRateField marginRateField{0};

    auto& jsonCfg = utils::JsonConfig::getInstance();
    const std::string username = jsonCfg.getConfig("common","user").get<std::string>();
    const std::string investorID = jsonCfg.getDeepConfig("users", username, "InvestorID").get<std::string>();
    const std::string brokerID = jsonCfg.getDeepConfig("users", username, "BrokerID").get<std::string>();

    strcpy(marginRateField.BrokerID, brokerID.c_str());
    strcpy(marginRateField.InvestorID, investorID.c_str());
    strcpy(marginRateField.ExchangeID, ins_exch.exch.c_str());
    strcpy(marginRateField.InstrumentID, ins_exch.ins.c_str());
    marginRateField.HedgeFlag = THOST_FTDC_HF_Speculation;

    int result = m_pApi->ReqQryInstrumentMarginRate(&marginRateField, requestIdBuildAlg());
    INFO_LOG("ReqQryInstrumentMarginRate send result is [%d]",result);

    if (result == 0)
    {
        std::string semName = "margin_rate";
        globalSem.addOrderSem(semName);
    }

    return result;
}

int CtpTraderBaseApi::ReqQryInstrumentCommissionRate(utils::InstrumtntID ins_exch)
{
    CThostFtdcQryInstrumentCommissionRateField commissonRateField{0};
    auto& jsonCfg = utils::JsonConfig::getInstance();
    const std::string username = jsonCfg.getConfig("common","user").get<std::string>();
    const std::string investorID = jsonCfg.getDeepConfig("users", username, "InvestorID").get<std::string>();
    const std::string brokerID = jsonCfg.getDeepConfig("users", username, "BrokerID").get<std::string>();
    strcpy(commissonRateField.BrokerID, brokerID.c_str());
    strcpy(commissonRateField.InvestorID, investorID.c_str());
    strcpy(commissonRateField.ExchangeID, ins_exch.exch.c_str());
    strcpy(commissonRateField.InstrumentID, ins_exch.ins.c_str());

    int result = m_pApi->ReqQryInstrumentCommissionRate(&commissonRateField, requestIdBuildAlg());
    INFO_LOG("ReqQryInstrumentCommissionRate send result is [%d]",result);

    if (result == 0)
    {
        std::string semName = "commission_rate";
        globalSem.addOrderSem(semName);
    }

    return result;
}

void CtpTraderBaseApi::Release()
{
    if (m_pApi)
    {
        m_pApi->Release();
        m_pApi = NULL;
    }
    return;
}

void CtpTraderBaseApi::Init()
{
    std::string semName = "trader_init";
    globalSem.addOrderSem(semName);
    m_pApi->Init();
    INFO_LOG("m_pApi->Init send ok!");
}

bool CtpTraderApi::init()
{
    INFO_LOG("begin CtpTraderApi init");

    traderApi = new CtpTraderBaseApi;     //新建tradeAPI
    auto& jsonCfg = utils::JsonConfig::getInstance();
    const std::string conPath = jsonCfg.getConfig("trader","ConPath").get<std::string>();
    utils::creatFolder(conPath);
    traderApi->CreateFtdcTraderApi(conPath.c_str()); //设置.con的保存位置
//        CSimpleHandler sh(pUserApi);     //初始化回调类
    traderSpi = TraderSpi();

    traderApi->RegisterSpi(&traderSpi);      //注册回调类
    traderApi->SubscribePrivateTopic(THOST_TERT_QUICK);   //订阅
    traderApi->SubscribePublicTopic(THOST_TERT_QUICK);    //订阅

    const std::string username = jsonCfg.getConfig("common","user").get<std::string>();
    const std::string frontaddr = jsonCfg.getDeepConfig("users", username, "FrontAddr").get<std::string>();

    traderApi->RegisterFront(const_cast<char *>(frontaddr.c_str()));  //注册前置地址

    return true;
}

namespace {
    using TimeDuration = std::array<U32, 2>;
    string getLogInSystemLocalTime()
    {
        //system time
        time_t now_time = time(NULL);
        //local time
        tm* local_time = localtime(&now_time);
        string sstr_localtime = asctime(local_time);
        sstr_localtime.pop_back();
        return sstr_localtime;
    }

    vector<string> splitString(const string &str, const string &pattern)
    {
        vector<string> res;
        if(str == "")
            return res;
        string strs = str + pattern;
        size_t pos = strs.find(pattern);
        while(pos != strs.npos)
        {
            string temp = strs.substr(0, pos);
            res.push_back(temp);
            strs = strs.substr(pos+1, strs.size());
            pos = strs.find(pattern);
        }
        return res;
    }

    std::vector<string> tradeDay = {
        "Mon","Monday","Tue","Tuesday","Wed","Wednesday","Thu","Thursday","Fri","Friday"
    };
    //vector<string> tradeDay2 = {
    //"Mon","Monday","Tue","Tuesday","Wed","Wednesday","Thu","Thursday","Fri","Friday","Sat","Saturday","Sun","Sunday"
    //};

    bool isTradeDuringWeek(const string& week)
    {
        for(auto day : tradeDay)
        {
            if(day == week)
            {
                return true;
            }
        }
        return false;
    }
    std::vector<TimeDuration> getTimeDurations()
    {
        std::vector<TimeDuration> tmpDurationList;
        auto& jsonCfg = utils::JsonConfig::getInstance();
        const std::string logInConfig = jsonCfg.getConfig("trader","LogInTimeList").get<std::string>();
        std::vector<std::string> timeDurationSplited = splitString(logInConfig, ";");
        for(auto duration : timeDurationSplited)
        {
            std::vector<std::string> vecBST = splitString(duration,"-");
            if(vecBST.size() != 2)
            {
                continue;
            }
            std::string beginT = vecBST[0];
            std::string endT = vecBST[1];
            std::vector<std::string> vecBThourMinute = splitString(beginT,":");
            std::vector<std::string> vecEThourMinute = splitString(endT,":");

            U32 beginTime_int = static_cast<U32>(atoi(vecBThourMinute[0].c_str()) * 3600 + atoi(vecBThourMinute[1].c_str()) * 60);
            U32 endTime_int = static_cast<U32>(atoi(vecEThourMinute[0].c_str()) * 3600 + atoi(vecEThourMinute[1].c_str()) * 60);
            TimeDuration tmpDuration;
            tmpDuration[0] = beginTime_int;
            tmpDuration[1] = endTime_int;
            tmpDurationList.push_back(tmpDuration);
        }
        return tmpDurationList;
    }

    bool isTradeDuringHourMuiteSecond(const string& hour,
                                      const string& minute,
                                      const string& sec)
    {
        int hour_int = atoi(hour.c_str());
        int minute_int = atoi(minute.c_str());
        int sec_int = atoi(sec.c_str());
        U32 curTime = (U32)(hour_int*3600 + minute_int*60 + sec_int);

        auto isDuringDurationFunc = [&]()->bool {
            auto timeDurationList = getTimeDurations();
            if(timeDurationList.size() < 1)
            {
                return false;
            }

            for(auto timeDuration : timeDurationList)
            {
                if(curTime >=timeDuration[0] && curTime <= timeDuration[1])
                {
                    return true;
                }
            }
            return false;
        };

        if(isDuringDurationFunc())
        {
            return true;
        }

        return false;
    }
    std::map<string,int> logMode = {
            {"7x24",  0},
            {"normal",1}
    };
    bool isDuringTradeTime()
    {
        auto& jsonCfg = utils::JsonConfig::getInstance();
        const std::string mode = jsonCfg.getConfig("trader","LoginMode",true).get<std::string>();
        if(logMode.at(string(mode.c_str())) == 0)
        {
            return true;
        }
        string local_time = getLogInSystemLocalTime();
        vector<string> splitedTimeStr = splitString(local_time, string(" "));
        string week ;
        string month;
        string day;
        string currentTime;
        string year;
        if(splitedTimeStr.size() == 5)
        {
            week = splitedTimeStr[0];
            month = splitedTimeStr[1];
            day = splitedTimeStr[2];
            currentTime = splitedTimeStr.at(3);
            year = splitedTimeStr[4];
        }
        else if(splitedTimeStr.size() == 6)
        {
            week = splitedTimeStr[0];
            month = splitedTimeStr[1];
            day = splitedTimeStr[3];
            currentTime = splitedTimeStr.at(4);
            year = splitedTimeStr[5];
        }else
        {
            ERROR_LOG("local_time parse error");
            return true;
        }

        vector<string> splitedCurrentTime = splitString(currentTime, string(":"));

        string hour = splitedCurrentTime[0];
        string minute = splitedCurrentTime[1];
        string second = splitedCurrentTime[2];
        if (isTradeDuringHourMuiteSecond(hour, minute, second))
        {
            return true;
        }
        return false;
    }

    void logOutReqTimeOutFunc()
    {
        INFO_LOG("begin loglout timeout func");
        std::string semName = "trader_logOut";
        globalSem.postSemBySemName(semName);
        INFO_LOG("force sem_pose sem_logout exec ok");
        auto& timerPool = TimeoutTimerPool::getInstance();
        timerPool.delTimerByName(FORCE_LOG_OUT_TIMER);
    }
}


void CtpTraderApi::release()
{
    traderApi->Release();
    delete traderApi;
    traderApi = nullptr;
}

bool CtpTraderApi::logIn()
{
    init();
    auto ctpLoginFunc = [this] {
        this->isLogInThreadRunning = true;
        traderApi->Init();    //connect to ctp
        std::string semName = "trader_init";
        globalSem.waitSemBySemName(semName);
        globalSem.delOrderSem(semName);
        if (this->isForceExitThreadRuning)
        {
            INFO_LOG("%s", "login thread force exit");
            this->isForceExitThreadRuning = false;
            INFO_LOG("%s", "isForceExitThreadRuning set to false");
            this->isLogInThreadRunning = false;
            INFO_LOG("%s", "isLogInThreadRunning force set to false");
            return;
        }
        INFO_LOG("%s", "ctp init ok");

        traderApi->ReqAuthenticate();
        semName = "trader_reqAuthenticate";
        globalSem.waitSemBySemName(semName);
        globalSem.delOrderSem(semName);
        if (this->isForceExitThreadRuning)
        {
            INFO_LOG("%s", "login thread force exit");
            this->isForceExitThreadRuning = false;
            INFO_LOG("%s", "isForceExitThreadRuning set to false");
            this->isLogInThreadRunning = false;
            INFO_LOG("%s", "isLogInThreadRunning force set to false");
            return;
        }
        INFO_LOG("ReqAuthenticate ok");

        traderApi->ReqUserLogin();
        semName = "trader_logIn";
        globalSem.waitSemBySemName(semName);
        globalSem.delOrderSem(semName);
        if (this->isForceExitThreadRuning)
        {
            INFO_LOG("%s", "login thread force exit");
            this->isForceExitThreadRuning = false;
            INFO_LOG("%s", "isForceExitThreadRuning set to false");
            this->isLogInThreadRunning = false;
            INFO_LOG("%s", "isLogInThreadRunning force set to false");
            return;
        }
        INFO_LOG("login ctp ok!");
        this->isLogIN = true;
        std::string tradingDay = traderApi->GetTradingDay();
        INFO_LOG("trading day:%s", tradingDay.c_str());
        while (true)
        {
            if (this->ROLE(SettlementConfirm).confirm(tradingDay))
            {
                INFO_LOG("SettlementConfirm confirm success!");
                break;
            }
            ERROR_LOG("SettlementConfirm confirm failed, try again!");
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        this->isLogInThreadRunning = false;
        INFO_LOG("%s", "isLogInThreadRunning set to false normally");
    };
    std::thread(ctpLoginFunc).detach();
    return true;
}

void CtpTraderApi::logOut()
{
    traderApi->ReqUserLogout();
    auto& timerPool = TimeoutTimerPool::getInstance();
    timerPool.addTimer(FORCE_LOG_OUT_TIMER, logOutReqTimeOutFunc,FORCE_LOGOUT_TIME_OUT);
    std::string semName = "trader_logOut";
    globalSem.waitSemBySemName(semName);
    globalSem.delOrderSem(semName);
    timerPool.killTimerByName(FORCE_LOG_OUT_TIMER);
    release();
    INFO_LOG("ctp log out ok!");
    isLogIN = false;
}

CtpTraderApi::CtpTraderApi()
{
    // static CtpTraderBaseApi ctpTraderBaseApi;
    // traderApi = &ctpTraderBaseApi;
}

CtpLogInState CtpTraderApi::getCtpLogInState()
{
    if (!isLogInThreadRunning && !isLogIN)
    {
        return CtpLogInState::Prepare_State;
    }
    else if (isLogInThreadRunning && !isLogIN)
    {
        return CtpLogInState::InitFailed_State;
    }
    else if (!isLogInThreadRunning && isLogIN)
    {
        return CtpLogInState::Connected_State;
    }
    else
    {
        return CtpLogInState::Invalid_State;
    }
}

void CtpTraderApi::runLogInAndLogOutAlg()
{
    INFO_LOG("start login and logout control thread");
    auto ctpLogInState = this->getCtpLogInState();
    if(isDuringTradeTime() && ((ctpLogInState == CtpLogInState::Connected_State)
                                || (ctpLogInState == CtpLogInState::Invalid_State)))
    {
        WARNING_LOG("during trade time, log in......");
        while(true)
        {
            if(!this->logIn())
            {
                ERROR_LOG("log in failed, try after 5s");
                std::this_thread::sleep_for(std::chrono::milliseconds(5*1000));
                continue;
            }
            break;
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5 * 1000));
    WARNING_LOG("begin while loop");
    while(true)
    {
        ctpLogInState = this->getCtpLogInState();
        if (ctpLogInState == CtpLogInState::Invalid_State)
        {
            ERROR_LOG("Error CtpLogInState [Invalid_State],code has bug or waiting SettlementConfirm response");
        }
        if(isDuringTradeTime() && ((ctpLogInState == CtpLogInState::Connected_State)
                                      || (ctpLogInState == CtpLogInState::InitFailed_State)))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            continue;
        }

        if(isDuringTradeTime() && (ctpLogInState == CtpLogInState::Prepare_State))
        {
            WARNING_LOG("during trade time, need to log in");
            while(true)
            {
                if(!this->logIn())
                {
                    ERROR_LOG("log in failed, try after 5s");
                    sleep(5);
                    continue;
                }
                break;
            }
        }
        if(!isDuringTradeTime() && (ctpLogInState == CtpLogInState::Connected_State))
        {
            INFO_LOG("trade time over, need to logout");
            this->logOut();
            continue;
        }
        if (!isDuringTradeTime() && (ctpLogInState == CtpLogInState::InitFailed_State))
        {
            INFO_LOG("%s", "init failed and trading time over, need to logout");
            this->isForceExitThreadRuning = true;
            if(globalSem.existSem("trader_init"))
            {
                globalSem.postSemBySemName("trader_init");
            }
            if(globalSem.existSem("trader_reqAuthenticate"))
            {
                globalSem.postSemBySemName("trader_reqAuthenticate");
            }
            if(globalSem.existSem("trader_logIn"))
            {
                globalSem.postSemBySemName("trader_logIn");
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            this->logOut();
            continue;
        }
        if( !isDuringTradeTime() && (ctpLogInState == CtpLogInState::Prepare_State))
        {
            WARNING_LOG("not during trade  time! and will relogin when time is right... please wait!");
            while(!isDuringTradeTime())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            WARNING_LOG("return to trade  time!");
            continue;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
