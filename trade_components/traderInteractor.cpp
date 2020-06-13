/*
 * Interactor.cpp
 *
 *  Created on: 2020��3��8��
 *      Author: Administrator
 */

#include "traderInteractor.h"
#include "log.h"
#include <stdio.h>
#include <unistd.h>
#include "getconfig.h"
#include "orderstates.h"
#include "semaphorePart.h"

extern GlobalSem globalSem;
extern json AllInstruments;
namespace{
    enum struct CommandType
    {
        showTraderLoginState,
        showTraderConfig,
        showAllConfig,
        showOrderState,
        queryInstrument
    };
    std::map<std::string, CommandType> cmdToCommandType =
    {
            {"showTraderLoginState",    CommandType::showTraderLoginState},
            {"showTraderConfig",        CommandType::showTraderConfig},
            {"showAllConfig",           CommandType::showAllConfig},
            {"showOrderState",          CommandType::showOrderState},
            {"queryInstrument",         CommandType::queryInstrument}
    };
}


void TraderInteractor::start()
{
    INFO_LOG("%s","TraderInteractor start");
    while(true)
    {
        DEBUG_LOG("%s","new loop");
        if(!creatBidirectionalFifos())
        {
            ERROR_LOG("%s","create fifos error!");
            closeFifos();
            continue;
        }
        INFO_LOG("%s","create fifos ok!");
        if(!receiveMsg())
        {
            DEBUG_LOG("%s","receiveMsg failed");
            closeFifos();
            continue;
        }

        if(!buildResponse())
        {
            DEBUG_LOG("%s","buildResponse failed");
            closeFifos();
            continue;
        }

        if(!sendMsg())
        {
            DEBUG_LOG("%s","sendMsg failed");
            closeFifos();
            continue;
        }
        DEBUG_LOG("%s","sendMsg ok");
        closeFifos();
    }
}

bool TraderInteractor::buildShowTraderLoginStateRsp()
{
    rspMsg["title"] = "login state";
    rspMsg["ctp_state.isLogIN"] = ROLE(LogInPart).isLogIN;
    rspMsg["ctp_state.isLogInThreadRunning"] = ROLE(LogInPart).isLogInThreadRunning;
    rspMsg["ctp_state.isForceExitThreadRuning"] = ROLE(LogInPart).isForceExitThreadRuning;
    rspMsg["route_state"] = ROLE(SocketClient).isRouterConnected;
    return true;
}

bool TraderInteractor::buildShowTraderConfigRsp()
{
    rspMsg["title"] = "trader config";
//    std::vector<std::string> allTitles = getAllTitles();
    json data;
    auto tmpPair = getAllKeyValueOfTitle("trade");
    for(auto& iter : tmpPair)
    {
        data[iter.first.c_str()] = iter.second.c_str();
    }
    rspMsg["data"] = data;
    return true;
}
bool TraderInteractor::buildShowAllConfigRsp()
{
    json data;
    std::vector<std::string> allTitles = getAllTitles();
    for(auto& iter : allTitles)
    {
        json tmpPart;
        auto tmpPairs = getAllKeyValueOfTitle(iter);
        for(auto& kv : tmpPairs)
        {
            tmpPart[kv.first.c_str()] = kv.second.c_str();
        }
        data[iter.c_str()] = tmpPart;
    }
    rspMsg["data"] = data;
    rspMsg["title"] = "all config";
    return true;
}

bool TraderInteractor::buildShowOrderStateRsp()
{
    rspMsg["title"] = "order state";
    auto& orderState = OrderStates::getInstance();
    auto& orderStateRecords = orderState.orderKey2StateMap;
    if(orderStateRecords.size() == 0)
    {
        rspMsg["data"] = "null";
        return true;
    }
    json data;
    for(const auto& orderRecord : orderStateRecords)
    {
        std::string states{""};
        if(orderRecord.second->size() == 0)
        {
            continue;
        }
        for(const auto& state : *(orderRecord.second))
        {
            states.push_back(state);
            states += string("->");
        }
        states.pop_back();
        states.pop_back();
        data[orderRecord.first.c_str()] = states.c_str();
    }
    rspMsg["data"] = data;
}

bool TraderInteractor::buildQueryInstrumentRsp()
{
    rspMsg["title"] = "QueryInstrumentRsp";
    auto parmNum = reqMsg["paramNum"].get<int>();
    if(parmNum<2)
    {
        rspMsg["result"] = "paramNum error!";
        return true;
    }

    std::string exchangeId = reqMsg["params"]["param1"].get<std::string>();
    std::string instrumentId = reqMsg["params"]["param2"].get<std::string>();
    AllInstruments.clear();
    CThostFtdcQryInstrumentField qryBody{0};
    std::strcpy(qryBody.ExchangeID, exchangeId.c_str());
    std::strcpy(qryBody.InstrumentID, instrumentId.c_str());
    ROLE(CtpClient).sh.ReqQryInstrument_hai(qryBody);
    sem_wait(&globalSem.sem_query);

    AllInstruments["result"] = "0";
    rspMsg["data"] = AllInstruments;
    return true;
}

bool TraderInteractor::buildResponse()
{
    INFO_LOG("interactor request:");
    JsonPrint(reqMsg);
    rspMsg.clear();
    std::string cmd = reqMsg["cmd"].get<std::string>();
    auto it = cmdToCommandType.find(cmd);
    if(it == cmdToCommandType.end())
    {
        ERROR_LOG("can't find the cmd [%s]",cmd.c_str());
        std::string value = "can't find command [" + cmd + "]";
        rspMsg["error"] = value.c_str() ;
        return true;
    }
    switch(it->second)
    {
        case CommandType::showTraderLoginState:
        {
            buildShowTraderLoginStateRsp();
            break;
        }
        case CommandType::showTraderConfig:
        {
            buildShowTraderConfigRsp();
            break;
        }
        case CommandType::showAllConfig:
        {
            buildShowAllConfigRsp();
            break;
        }
        case CommandType::showOrderState:
        {
            buildShowOrderStateRsp();
            break;
        }
        case CommandType::queryInstrument:
        {
            buildQueryInstrumentRsp();
            break;
        }
        default:
        {
            WARNING_LOG("%s","can't match, return false");
            std::string value = "can't find command [" + cmd + "]";
            rspMsg["error"] = value.c_str() ;
            break;
        }

    }
    INFO_LOG("interactor response:");
    JsonPrint(rspMsg);
    return true;
}

bool TraderInteractor::creatBidirectionalFifos()
{
    if(!ROLE(TraderWriteFifo).CreatFifo())
    {
       return false;
    }

    if(!ROLE(TraderReadFifo).CreatFifo())
    {
       return false;
    }
    return true;
}
void TraderInteractor::closeFifos()
{
    ROLE(TraderWriteFifo).Close();
    ROLE(TraderReadFifo).Close();
}

bool TraderInteractor::receiveMsg()
{
    DEBUG_LOG("begin to open file");
    if(!ROLE(TraderReadFifo).OpenFifo())
    {
        ERROR_LOG("%s","open read file error");
        return false;
    }
    reqMsg.clear();
    if(ROLE(TraderReadFifo).Read(reqMsg))
    {
        return true;
    }
    return false;
}

bool TraderInteractor::sendMsg()
{
    if(!ROLE(TraderWriteFifo).OpenFifo())
    {
        return false;
    }
    if(ROLE(TraderWriteFifo).Write(rspMsg,INTERACTOR_RECEIVE))
    {
        return true;
    }
    return false;
}

bool TraderWriteFifo::OpenFifo()
{
    const char* fifo_name = fifoName.c_str();
    pipe_fd = open(fifo_name, O_WRONLY);
    if(pipe_fd < 0)
    {
        ERROR_LOG("open fifo file error");
        return false;
    }
//    INFO_LOG("fifo open succ");
    return true;
}

bool TraderWriteFifo::CreatFifo()
{
    const char* fifo_name = fifoName.c_str();
    if(access(fifo_name, F_OK) == -1)
    {
        INFO_LOG("begin mkfifo......");
        int res = mkfifo(fifo_name,0777);
        if(res!=0)
        {
            ERROR_LOG("can't create write fifo");
            return false;
        }
    }
    INFO_LOG("create write fifo ok");
    return true;
}

bool TraderWriteFifo::Write(json& msg, const char destination[])
{
    std::string jsonMsgStr = msg.dump() + '\0';

    U32 lengthOfmsgBody = jsonMsgStr.length();
    MsgHeader msgHead;
    strcpy(msgHead.target, destination);
    msgHead.length = lengthOfmsgBody;

    int res = write(pipe_fd, &msgHead, sizeof(msgHead));
    if(res < 0)
    {
        ERROR_LOG("write msg header error!");
        return false;
    }

    const char* buff = jsonMsgStr.c_str();
    res = write(pipe_fd, buff, lengthOfmsgBody);
    if(res < 0)
    {
        ERROR_LOG("write msg body error!");
        return false;
    }

//    INFO_LOG("send msg to %s succ",destination);
    return true;
}

void TraderWriteFifo::Close()
{
    if(pipe_fd < 0)
    {
        ERROR_LOG("close write fifo file error!");
        return;
    }
    close(pipe_fd);
    return;
}

bool TraderReadFifo::OpenFifo()
{
    const char* fifo_name = fifoName.c_str();
    DEBUG_LOG("read fifo name: %s",fifo_name);
    pipe_fd = open(fifo_name, O_RDONLY);
    DEBUG_LOG("pipe_fd = %d",pipe_fd);
    if(pipe_fd < 0)
    {
        ERROR_LOG("open fifo file error");
        return false;
    }

    return true;
}

bool TraderReadFifo::CreatFifo()
{
    INFO_LOG("read fifo name:%s",fifoName.c_str());
    const char* fifo_name = fifoName.c_str();

    if(access(fifo_name, F_OK) == -1)
    {
        INFO_LOG("begin mkfifo......");
        int res = mkfifo(fifo_name,0777);
        if(res!=0)
        {
            ERROR_LOG("can't create read fifo");
            return false;
        }
    }
    INFO_LOG("create read fifo ok!");
    return true;
}

bool TraderReadFifo::Read(json& msg)
{
    MsgHeader msgHead;
    int res = read(pipe_fd,&msgHead,sizeof(msgHead));
    INFO_LOG("read res from interactor = %d",res);
    if(res <= 0)
    {
        ERROR_LOG("receive msg error!");
        return false;
    }
    char buff[msgHead.length];
    res = read(pipe_fd,buff,msgHead.length);
    INFO_LOG("read res from interactor = %d",res);
    if(res <=0)
    {
        ERROR_LOG("receive msg error!");
        return false;
    }
    msg = json::parse(buff);

//    INFO_LOG("receive msg succ");
    return true;
}

void TraderReadFifo::Close()
{
    if(pipe_fd < 0)
    {
        ERROR_LOG("close read fifo file error!");
        return;
    }
    close(pipe_fd);
    return;
}

