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
        //在字符串末尾也加入分隔符，方便截取最后一殄1�7�1�7
        string strs = str + pattern;
        size_t pos = strs.find(pattern);
        while(pos != strs.npos)
        {
            string temp = strs.substr(0, pos);
            res.push_back(temp);
            //去掉已分割的字符丄1�7�1�7在剩下的字符串中进行分割
            strs = strs.substr(pos+1, strs.size());
            pos = strs.find(pattern);
        }
        return res;
    }

    vector<string> tradeDay = {
        "Mon","Monday","Tue","Tuesday","Wed","Wednesday","Thu","Thursday","Fri","Friday",
        "Sat","Saturday","Sun","Sunday"
    };

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
//        int sec_int = atoi(sec.c_str());
        if(hour_int > 20
           || (hour_int == 20 && minute_int >= 50)
           ||  hour_int < 15
           || (hour_int == 15 && minute_int < 50))
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
        string mode = getConfig("trade", "LoginMode");
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
    auto& traderHandle= ROLE(CtpClient).sh;
    auto* pUserApi =  ROLE(CtpClient).pUserApi;
    pUserApi->Init();    //连接交易扄1�7
    sem_wait(&globalSem.sem_login);       //等待连接成功 // @suppress("Function cannot be resolved")
    WARNING_LOG("hai********init ok");

    traderHandle.ReqAuthenticate();
    sem_wait(&globalSem.sem_login); // @suppress("Function cannot be resolved")
    WARNING_LOG("hai********ReqAuthenticate ok");

    CThostFtdcReqUserLoginField reqUserLogin{ 0 };
    strcpy(reqUserLogin.BrokerID, getConfig("trade", "BrokerID").c_str());
    strcpy(reqUserLogin.UserID, getConfig("trade", "UserID").c_str());
    strcpy(reqUserLogin.Password, getConfig("trade", "Password").c_str());
    auto& RequestID = ROLE(Trader_Info).RequestID;
    traderHandle.ReqUserLogin(reqUserLogin, RequestID);
    RequestID++;
    sem_wait(&globalSem.sem_login); // @suppress("Function cannot be resolved")
    WARNING_LOG("hai********ReqUserLogin ok");

    INFO_LOG("接受到了sem_login信号, login ctp ok!"); // @suppress("Invalid arguments")
    INFO_LOG("trading day:%s",pUserApi->GetTradingDay()); // @suppress("Invalid arguments")
    isLogIN = true;
    while(true)
    {
        if(ROLE(SettlementConfirm).confirm())
        {
            INFO_LOG("SettlementConfirm confirm success!");
            break;
        }
        ERROR_LOG("SettlementConfirm confirm failed, try again!");
    }
    return true;
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
    INFO_LOG("接受到了sem_logout信号, log out ok!");
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

    if(isDuringTradeTime() && !_this->isLogIN)
    {
        WARNING_LOG("during trade time, log in......");
        while(true)
        {
            if(!_this->logIn())
            {
                ERROR_LOG("log in failed, try after 5s");
                continue;
            }
            break;
        }
        INFO_LOG("log in success!!!");
    }
    WARNING_LOG("begin while loop");
    while(true)
    {

        if(isDuringTradeTime() && _this->isLogIN)
        {
            sleep(2);
            continue;
        }

        if(isDuringTradeTime() && !_this->isLogIN)
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

        if(!isDuringTradeTime() && _this->isLogIN)
        {
            INFO_LOG("trade time over, need to logout");
            _this->logOut();
            continue;
        }

        if( !isDuringTradeTime() && !_this->isLogIN)
        {
            WARNING_LOG("not during trade  time! and will relogin when time is right... please wait!");
            while(!isDuringTradeTime()){}
            WARNING_LOG("return to trade  time!");
            continue;
        }
    }
}

void LogInPart::goLoginAndLogoutControl()
{
//    INFO_LOG("start login and logout control thread");

    if(isDuringTradeTime() && ! isLogIN)
    {
        WARNING_LOG("during trade time, log in");
        while(true)
        {
            if(!this->logIn())
            {
                ERROR_LOG("log in failed, try after 5s");
                continue;
            }
            break;
        }
        INFO_LOG("log in success");
    }
    WARNING_LOG("begin while loop");
    while(true)
    {
        WARNING_LOG("begin while loop");
        if(isDuringTradeTime() && isLogIN)
        {
            sleep(2);
            continue;
        }

        if(isDuringTradeTime() && !isLogIN)
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
            INFO_LOG("log in success during trade time!");
        }

        if(!isDuringTradeTime() && isLogIN)
        {
            INFO_LOG("trade time over, need to logout");
            this->logOut();
            continue;
        }

        if( !isDuringTradeTime() && !isLogIN)
        {
            WARNING_LOG("not during trade  time!");
            while(!isDuringTradeTime()){}
            WARNING_LOG("return to trade  time!");
            continue;
        }
    }
}


