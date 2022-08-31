/*
 * trader.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#include <chrono>
#include <string>
#include "common/extern/log/log.h"
#include "common/self/fileUtil.h"
#include "common/self/utils.h"
#include "trader/domain/traderService.h"
#include "trader/infra/innerZmq.h"
#include "trader/infra/recerSender.h"
#include "trader/infra/zmqBase.h"
#include "trader/interface/traderEvent.h"

int main(int argc, char *agrv[]) {
  auto &jsonCfg = utils::JsonConfig::getInstance();

  //开启log
  std::string traderLogPath = jsonCfg.getConfig("trader", "LogPath").get<std::string>();
  utils::creatFolder(traderLogPath);
  LOG_INIT(traderLogPath.c_str(), "traderlog", 6);
  INFO_LOG("TRADE LOG PATH is %s", traderLogPath.c_str());

  // 打印版本信息
  std::string compile_time = utils::GetCompileTime();
  jsonCfg.writeConfig("trader", "version", compile_time);
  INFO_LOG("program last build at %s.", compile_time.c_str());

  INFO_LOG("begin init traderSer");
  auto &traderSer = TraderSevice::getInstance();
  INFO_LOG("traderSer init ok");

  INFO_LOG("begin init traderEvent");
  auto &traderEvent = TraderEvent::getInstance();
  INFO_LOG("traderEvent init ok");

  INFO_LOG("begin run");
  traderEvent.run();
  return 0;
}
