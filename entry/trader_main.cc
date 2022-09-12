/*
 * trader.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#include <chrono>
#include <string>
#include <thread>
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/utils.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/base_zmq.h"
#include "trader/infra/inner_zmq.h"
#include "trader/infra/recer_sender.h"
#include "trader/interface/trader_event.h"

int main(int argc, char *agrv[]) {
  auto &jsonCfg = utils::JsonConfig::getInstance();

  //开启log
  std::string traderLogPath = jsonCfg.get_config("trader", "LogPath").get<std::string>();
  utils::CreatFolder(traderLogPath);
  LOG_INIT(traderLogPath.c_str(), "traderlog", 6);
  INFO_LOG("trade log path is %s", traderLogPath.c_str());

  // 打印版本信息
  std::string compile_time = utils::get_compile_time();
  jsonCfg.WriteConfig("trader", "CompileTime", compile_time);
  INFO_LOG("program last build at %s.", compile_time.c_str());

  INFO_LOG("begin init traderSer");
  auto &traderSer = TraderSevice::getInstance();
  INFO_LOG("traderSer init ok");

  std::this_thread::sleep_for(5s);

  INFO_LOG("begin init traderEvent");
  auto &traderEvent = TraderEvent::getInstance();
  INFO_LOG("traderEvent init ok");

  INFO_LOG("begin run");
  traderEvent.Run();
  return 0;
}
