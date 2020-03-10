/*
 * Interactor.h
 *
 *  Created on: 2020Äê3ÔÂ8ÈÕ
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADE_TRADE_COMPONENTS_TRADERINTERACTOR_H_
#define WORKSPACE_TRADE_TRADE_COMPONENTS_TRADERINTERACTOR_H_

#include <string>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

#include "json.h"
#include "dci/Role.h"

#include "login_part.h"
#include "socket/socketClient.h"


typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;
typedef unsigned long  U64;

typedef signed char    S8;
typedef signed short   S16;
typedef signed int     S32;
typedef signed long    S64;

using json = nlohmann::json;

constexpr char FIFO_INTERACT_TRADE[]        = "/tmp/interact_to_trade";
constexpr char FIFO_TRADE_INTERACT[]        = "/tmp/trade_to_interact";

constexpr char INTERACTOR_RECEIVE[]         = "interactor";

struct MsgHeader
{
    char target[50];
    U32 length;
};

struct TraderWriteFifo
{
    bool OpenFifo();
    bool CreatFifo();
    bool Write(json& msg, const char destination[]);

    void Close();
private:
    std::string fifoName{FIFO_TRADE_INTERACT};
    int pipe_fd{-1};
//    std::string writeFifoPath{FIFO_TRADE_INTERACT};
};

struct TraderReadFifo
{
    bool OpenFifo();
    bool CreatFifo();
    bool Read(json& msg);

    void Close();
private:
    std::string fifoName{FIFO_INTERACT_TRADE};
    int pipe_fd{-1};
};

struct TraderInteractor: TraderWriteFifo
                        ,TraderReadFifo
{

public:
    bool receiveMsg();
    bool sendMsg();
    bool buildResponse();
    bool creatBidirectionalFifos();
    void closeFifos();
    void start();

public:
    bool buildShowTraderLoginStateRsp();
    bool buildShowTraderConfigRsp();

private:
    json reqMsg;
    json rspMsg;

public:
    IMPL_ROLE(TraderWriteFifo);
    IMPL_ROLE(TraderReadFifo);

public:
    USE_ROLE(LogInPart);
    USE_ROLE(SocketClient);

};


#endif /* WORKSPACE_TRADE_TRADE_COMPONENTS_TRADERINTERACTOR_H_ */
