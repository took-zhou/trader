/*
 * Interactor.h
 *
 *  Created on: 2020��3��8��
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
#include "trade_components/ctpClient.h"
#include "define.h"

using json = nlohmann::json;

constexpr char FIFO_INTERACT_TRADE[]        = "/tmp/interact_to_trade";
constexpr char FIFO_TRADE_INTERACT[]        = "/tmp/trade_to_interact";

// constexpr char FIFO_INTERACT_TRADE[]        = "/tmp/interact_to_trade_test";
// constexpr char FIFO_TRADE_INTERACT[]        = "/tmp/trade_to_interact_test";

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
    bool buildShowAllConfigRsp();
    bool buildShowOrderStateRsp();
    bool buildQueryInstrumentRsp();

private:
    json reqMsg;
    json rspMsg;

public:
    IMPL_ROLE(TraderWriteFifo);
    IMPL_ROLE(TraderReadFifo);

public:
    USE_ROLE(LogInPart);
    USE_ROLE(SocketClient);
    USE_ROLE(CtpClient);

};


#endif /* WORKSPACE_TRADE_TRADE_COMPONENTS_TRADERINTERACTOR_H_ */
