/*
 * trader.cpp
 *
 *  Created on: 2020��8��29��
 *      Author: Administrator
 */

#include "common/self/fileUtil.h"
#include <string>
#include <chrono>
#include "common/extern/log/log.h"
#include "common/self/utils.h"
#include "trader/infra/zmqBase.h"
#include "trader/interface/traderEvent.h"
#include "trader/infra/recerSender.h"
#include "trader/domain/traderService.h"

int main(int argc, char* agrv[])
{
    auto& jsonCfg = utils::JsonConfig::getInstance();

    //开启log
    INFO_LOG("hai************begin");
    std::string traderLogPath = jsonCfg.getConfig("trader","LogPath").get<std::string>();
    INFO_LOG("hai************end");
    utils::creatFolder(traderLogPath);
    LOG_INIT(traderLogPath.c_str(), "traderlog", 6);
    INFO_LOG("TRADE LOG PATH is %s",traderLogPath.c_str());

    //初始化zmq模块
    auto& zmq = ZmqBase::getInstance();
    while(!zmq.init())
    {
        ERROR_LOG("zmq init failed");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    INFO_LOG("begin init recerSender");
    //消息接收\发送器初始化
    auto& recerSender = RecerSender::getInstance();
    while(!recerSender.init())
    {
        ERROR_LOG("recerSender init failed");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    INFO_LOG("begin init traderSer");
    //初始化ctpTrader
    auto& traderSer = TraderSevice::getInstance();
    while(not traderSer.init())
    {
        ERROR_LOG("traderSer init failed");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    INFO_LOG("begin init traderEvent");
    // 初始化strategyEvent模块
    auto& traderEvent = TraderEvent::getInstance();
    while(! traderEvent.init())
    {
        ERROR_LOG("marketEvent init failed");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    INFO_LOG("begin run");
    //strategyEvent 循环等待消息触发
    traderEvent.run();
    return 0;
}
