/*
 * TraderTimeState.cpp
 *
 *  Created on: 2020.11.13
 *      Author: Administrator
 */
#include "trader/domain/components/ctpTradeApi/traderTimeState.h"
#include "common/self/protobuf/ctpview-trader.pb.h"
#include "common/self/fileUtil.h"
#include "common/self/utils.h"
#include "common/extern/log/log.h"
#include <string>
#include <string.h>
#include <unistd.h>

U32 TraderTimeState::isDuringDayLogoutTime(void)
{
    U32 out;
    out = 0U;

    if ((strcmp(input.loginTime, "night") != 0) && (strcmp(input.loginMode, "normal") ==
        0) && ((input.day_logout_mins < input.now_mins) && (input.now_mins <
          input.night_login_mins)))
    {
        out = 1U;
    }

    return out;
}

U32 TraderTimeState::isDuringNightLogoutTime(void)
{
    U32 out;
    out = 0U;

    if ((strcmp(input.loginTime, "day") != 0) && (strcmp(input.loginMode, "normal") ==
        0) && ((input.night_logout_mins < input.now_mins) && (input.now_mins <
          input.day_login_mins)))
    {
        out = 1U;
    }

    return out;
}

U32 TraderTimeState::isDuringDayLoginTime(void)
{
    U32 out;
    out = 0U;

    if ((strcmp(input.loginTime, "night") != 0) && (strcmp(input.loginMode, "normal") ==
        0) && ((input.day_login_mins <= input.now_mins) && (input.now_mins <=
          input.day_logout_mins)))
    {
        out = 1U;
    }

    return out;
}

U32 TraderTimeState::isDuringNightLoginTime(void)
{
    U32 out;
    out = 0U;
    if ((strcmp(input.loginTime, "day") != 0) && (strcmp(input.loginMode, "normal") ==
          0) && ((input.night_login_mins <= input.now_mins) || (input.now_mins <=
          input.night_logout_mins)))
    {
        out = 1U;
    }

    return out;
}

void TraderTimeState::determineLoginMode(void)
{
    if (strcmp(input.loginMode, "normal") == 0)
    {
        output.status = LOGOUT_TIME;
    }
    else
    {
        output.status = LOGIN_TIME;
    }
}

// Model step function
void TraderTimeState::step()
{
    if (rtDW.is_active_TraderTimeState == 0U) 
    {
        rtDW.is_active_TraderTimeState = 1U;
        rtDW.is_TraderTimeState = IN_init_sts;
        determineLoginMode();
    }
    else
    {
        switch (rtDW.is_TraderTimeState)
        {
            case IN_day_login:
                if (input.now_mins == input.day_logout_mins || time_state == LOGOUT_TIME)
                {
                    rtDW.is_TraderTimeState = IN_day_logout;
                    output.status = LOGOUT_TIME;
                }
                break;

            case IN_day_logout:
                if ((strcmp(input.loginTime, "day") != 0) && (input.now_mins == input.night_login_mins || time_state == LOGIN_TIME))
                {
                    rtDW.is_TraderTimeState = IN_night_login;
                    output.status = LOGIN_TIME;
                }
                else
                {
                    if ((strcmp(input.loginTime, "day") == 0) && (input.now_mins == input.day_login_mins || time_state == LOGIN_TIME))
                    {
                        rtDW.is_TraderTimeState = IN_day_login;
                        output.status = LOGIN_TIME;
                    }
                }
                break;

            case IN_init_sts:
                if (isDuringDayLogoutTime() != 0U)
                {
                    rtDW.is_TraderTimeState = IN_day_logout;
                    output.status = LOGOUT_TIME;
                } 
                else if (isDuringNightLogoutTime() != 0U)
                {
                    rtDW.is_TraderTimeState = IN_night_logout;
                    output.status = LOGOUT_TIME;
                }
                else if (isDuringDayLoginTime() != 0U)
                {
                    rtDW.is_TraderTimeState = IN_day_login;
                    output.status = LOGIN_TIME;
                }
                else
                {
                    if (isDuringNightLoginTime() != 0U)
                    {
                        rtDW.is_TraderTimeState = IN_night_login;
                        output.status = LOGIN_TIME;
                    }
                }
                break;

            case IN_night_login:
                if (input.now_mins == input.night_logout_mins || time_state == LOGOUT_TIME)
                {
                    rtDW.is_TraderTimeState = IN_night_logout;
                    output.status = LOGOUT_TIME;
                }
                break;

            default:
                if ((strcmp(input.loginTime, "night") != 0) && (input.now_mins == input.day_login_mins || time_state == LOGIN_TIME))
                {
                    rtDW.is_TraderTimeState = IN_day_login;
                    output.status = LOGIN_TIME;
                }
                else
                {
                    if ((strcmp(input.loginTime, "night") == 0) && (input.now_mins == input.night_login_mins || time_state == LOGIN_TIME))
                    {
                        rtDW.is_TraderTimeState = IN_night_login;
                        output.status = LOGIN_TIME;
                    }
                }
                break;
        }
    }
}

void TraderTimeState::update(void)
{
    time_t now = {0};
    struct tm *timenow = NULL;

    while(1)
    {
        time(&now);
        timenow = localtime(&now);//获取当前时间
        input.now_mins= timenow->tm_hour*60 + timenow->tm_min;

        step();

        sleep(1);
    }
}

void TraderTimeState::set_time_state(int command)
{
    if (command == ctpview_trader::LoginControl_Command_login)
    {
        time_state = LOGIN_TIME;
    }
    else if (command == ctpview_trader::LoginControl_Command_logout)
    {
        time_state = LOGOUT_TIME;
    }
    else if (command == ctpview_trader::LoginControl_Command_reserve)
    {
        time_state = RESERVE;
    }
}

void TraderTimeState::initialize()
{
    auto& jsonCfg = utils::JsonConfig::getInstance();

    string modeStr = jsonCfg.getConfig("trader","LoginMode").get<std::string>();
    strcpy(input.loginMode, modeStr.c_str());

    string timeStr = jsonCfg.getConfig("trader","LogInTimeList").get<std::string>();
    vector<string> timeDurationSplited = utils::splitString(timeStr, ";");
    vector<string> dayTimeStrSplit = utils::splitString(timeDurationSplited[0], "-");
    vector<string> nightTimeStrSplit = utils::splitString(timeDurationSplited[1], "-");
    vector<string> dayStartStrSplit = utils::splitString(dayTimeStrSplit[0], ":");
    vector<string> dayEndStrSplit = utils::splitString(dayTimeStrSplit[1], ":");
    vector<string> nightStartStrSplit = utils::splitString(nightTimeStrSplit[0], ":");
    vector<string> nightEndStrSplit = utils::splitString(nightTimeStrSplit[1], ":");

    input.day_login_mins = atoi(dayStartStrSplit[0].c_str())*60 + atoi(dayStartStrSplit[1].c_str());
    input.day_logout_mins = atoi(dayEndStrSplit[0].c_str())*60 + atoi(dayEndStrSplit[1].c_str());
    input.night_login_mins = atoi(nightStartStrSplit[0].c_str())*60 + atoi(nightStartStrSplit[1].c_str());
    input.night_logout_mins = atoi(nightEndStrSplit[0].c_str())*60 + atoi(nightEndStrSplit[1].c_str());

    INFO_LOG("%d %d %d %d", input.day_login_mins, input.day_logout_mins, input.night_login_mins, input.night_logout_mins);
    timeStr = jsonCfg.getConfig("trader","LoginTime").get<std::string>();
    strcpy(input.loginTime, timeStr.c_str());
}

TraderTimeState::TraderTimeState()
{
    initialize();
}

TraderTimeState::~TraderTimeState()
{
    ;
}

ERROR_STATUS * TraderTimeState::getRTM()
{
    return (&rtM);
}
