/*
 * Interactor.cpp
 *
 *  Created on: 2020Äê3ÔÂ8ÈÕ
 *      Author: Administrator
 */

#include "traderInteractor.h"
#include "log.h"
#include <stdio.h>
#include <unistd.h>
#include "getconfig.h"


namespace{
    enum struct CommandType
    {
        showTraderLoginState,
        showTraderConfig
    };
    std::map<std::string, CommandType> cmdToCommandType =
    {
            {"showTraderLoginState",CommandType::showTraderLoginState},
            {"showTraderConfig",CommandType::showTraderConfig}
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
    rspMsg["ctp_state"] = ROLE(LogInPart).isLogIN;
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

bool TraderInteractor::buildResponse()
{
    JsonPrint(reqMsg);
    rspMsg.clear();
    std::string cmd = reqMsg["cmd"].get<std::string>();
    auto it = cmdToCommandType.find(cmd);
    if(it == cmdToCommandType.end())
    {
        ERROR_LOG("can't find the cmd [%s]",cmd.c_str());
        return false;
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
        default:
        {
            WARNING_LOG("%s","can't match, return false");
            return false;
        }

    }

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
        ERROR_LOG("close fifo file error!");
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
    if(res < 0)
    {
        ERROR_LOG("receive msg error!");
        return false;
    }
    char buff[msgHead.length];

    res = read(pipe_fd,buff,msgHead.length);
    if(res < 0)
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
        ERROR_LOG("close fifo file error!");
        return;
    }
    close(pipe_fd);
    return;
}

