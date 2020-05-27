#include "login_part.h"
#include "log.h"
#include "getconfig.h"
#include "ThostFtdcUserApiStruct.h"
#include<cstring>
#include "semaphorePart.h"
#include "trader_commom.h"
#include <unistd.h>
#include <time.h>
#include <vector>
#include <stdlib.h>
#include <unordered_map>

extern GlobalSem globalSem;

namespace {

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
        //鍦ㄥ瓧绗︿覆鏈熬涔熷姞鍏ュ垎闅旂锛屾柟渚挎埅鍙栨渶鍚庝竴娈�1锟�7锟�1锟�7
        string strs = str + pattern;
        size_t pos = strs.find(pattern);
        while(pos != strs.npos)
        {
            string temp = strs.substr(0, pos);
            res.push_back(temp);
            //鍘绘帀宸插垎鍓茬殑瀛楃涓�1锟�7锟�1锟�7鍦ㄥ墿涓嬬殑瀛楃涓蹭腑杩涜鍒嗗壊
            strs = strs.substr(pos+1, strs.size());
            pos = strs.find(pattern);
        }
        return res;
    }

    vector<string> tradeDay = {
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

    bool isTradeDuringHourMuiteSecond(const string& hour,
                                      const string& minute,
                                      const string sec)
    {
        int hour_int = atoi(hour.c_str());
        int minute_int = atoi(minute.c_str());
        float curTime = static_cast<float>(hour_int) + static_cast<float>(minute_int / 60.0);
        std::string dayLogIn = getConfig("trade", "DayLogInTime");
        std::string dayLogOut = getConfig("trade", "DayLogOutTime");
        auto isDuringDurationFunc = [&](std::string beginTime, std::string endTime)->bool {
            auto vecBT = splitString(beginTime, ":");
            if (vecBT.size() != 2)
            {
                ERROR_LOG("logIn time config is error,please check");
                return false;
            }
            auto vecET = splitString(endTime, ":");
            if (vecET.size() != 2)
            {
                ERROR_LOG("logOut time config is error,please check");
                return false;
            }
            int btHour = atoi(vecBT[0].c_str());
            int btMinute = atoi(vecBT[1].c_str());
            float tLogIn = static_cast<float>(btHour) + static_cast<float>(btMinute / 60.0);

            int etHour = atoi(vecET[0].c_str());
            int etMinute = atoi(vecET[1].c_str());
            float tLogOut = static_cast<float>(etHour) + static_cast<float>(etMinute / 60.0);

            if (curTime > tLogIn && curTime < tLogOut)
            {
                return true;
            }
            return false;
        };
        if(isDuringDurationFunc(dayLogIn, dayLogOut))
        {
            return true;
        }

        std::string nightLogIn = getConfig("trade", "NightLogInTime");
        std::string nightLogOut = getConfig("trade", "NightLogOutTime");
        if (isDuringDurationFunc(nightLogIn, nightLogOut))
        {
            return true;
        }

        return false;
    }
    unordered_map<string,int> logMode = {
            {"7x24",  0},
            {"normal",1}
    };
    bool isDuringTradeTime()
    {
        std::string mode = getConfig("trade", "LoginMode");
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
        if(isTradeDuringWeek(week) && isTradeDuringHourMuiteSecond(hour,minute,second))
        {
            return true;
        }
        return false;
    }
}
bool LogInPart::logIn()
{
    ROLE(CtpClient).init();
    auto ctpLoginFunc = [this] {
        this->isLogInThreadRunning = true;
        auto& traderHandle = this->ROLE(CtpClient).sh;
        auto* pUserApi = this->ROLE(CtpClient).pUserApi;
        pUserApi->Init();    //connect to ctp
        sem_wait(&globalSem.sem_login);
        INFO_LOG("%s", "ctp init ok");

        traderHandle.ReqAuthenticate();
        sem_wait(&globalSem.sem_login);
        WARNING_LOG("ReqAuthenticate ok");

        CThostFtdcReqUserLoginField reqUserLogin{ 0 };
        strcpy(reqUserLogin.BrokerID, getConfig("trade", "BrokerID").c_str());
        strcpy(reqUserLogin.UserID, getConfig("trade", "UserID").c_str());
        strcpy(reqUserLogin.Password, getConfig("trade", "Password").c_str());
        auto& RequestID = this->ROLE(Trader_Info).RequestID;
        traderHandle.ReqUserLogin(reqUserLogin, RequestID);
        RequestID++;
        sem_wait(&globalSem.sem_login);
        if (this->isForceExitThreadRuning)
        {
            this->isForceExitThreadRuning = false;
        }
        else
        {
            this->isLogIN = true;
            INFO_LOG("login ctp ok!");
            std::string tradingDay = pUserApi->GetTradingDay();
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
        }
        this->isLogInThreadRunning = false;
    };
    std::thread(ctpLoginFunc).detach();
    return true;
}
CtpLogInState LogInPart::getCtpLogInState()
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
bool LogInPart::logOut()
{
    auto& traderHandle= ROLE(CtpClient).sh;
    CThostFtdcUserLogoutField logOutField{0};
    strcpy(logOutField.BrokerID, getConfig("trade", "BrokerID").c_str());
    strcpy(logOutField.UserID, getConfig("trade", "UserID").c_str());
    auto& RequestID = ROLE(Trader_Info).RequestID;
    traderHandle.ReqUserLogout(logOutField, RequestID);
    RequestID++;
    sem_wait(&globalSem.sem_logout);
    ROLE(CtpClient).release();
    INFO_LOG("ctp log out ok!");
    isLogIN = false;
    return true;
}

void LogInPart::startLoginOutControl()
{
    INFO_LOG("begin to creat logInOutThread");
    logInOutThread = std::thread(loginAndLogoutControl, this);
    logInOutThread.detach();
}

void LogInPart::loginAndLogoutControl(LogInPart* _this)
{
    INFO_LOG("start login and logout control thread");
    auto ctpLogInState = _this->getCtpLogInState();
    if(isDuringTradeTime() && ((ctpLogInState == CtpLogInState::Connected_State)
                                || (ctpLogInState == CtpLogInState::Invalid_State)))
    {
        WARNING_LOG("during trade time, log in......");
        while(true)
        {
            if(!_this->logIn())
            {
                ERROR_LOG("log in failed, try after 5s");
                std::this_thread::sleep_for(std::chrono::milliseconds(5*1000));
                continue;
            }
            break;
        }
        INFO_LOG("log in success!!!");
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5 * 1000));
    WARNING_LOG("begin while loop");
    while(true)
    {
        ctpLogInState = _this->getCtpLogInState();
        if (ctpLogInState == CtpLogInState::Invalid_State)
        {
            ERROR_LOG("Error CtpLogInState [Invalid_State],code has bug");
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
                if(!_this->logIn())
                {
                    ERROR_LOG("log in failed, try after 5s");
                    sleep(5);
                    continue;
                }
                break;
            }
            INFO_LOG("log in success during trade time!");
        }
        if(!isDuringTradeTime() && (ctpLogInState == CtpLogInState::Connected_State))
        {
            INFO_LOG("trade time over, need to logout");
            _this->logOut();
            continue;
        }
        if (!isDuringTradeTime() && (ctpLogInState == CtpLogInState::InitFailed_State))
        {
            INFO_LOG("%s", "init failed and trading time over, need to logout");
            _this->isForceExitThreadRuning = true;
            sem_post(&globalSem.sem_login);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            _this->logOut();
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
