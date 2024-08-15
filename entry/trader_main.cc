/*
 * trader.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#include "trader/entry/trader_main.h"
#include <signal.h>
#include <chrono>
#include <string>
#include <thread>
#include "common/extern/log/log.h"
#include "common/extern/pybind11/include/pybind11/embed.h"
#include "common/self/file_util.h"
#include "common/self/profiler.h"
#include "common/self/utils.h"
#include "trader/domain/components/fd_manage.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"
#include "trader/interface/trader_event.h"

void SignalHandler(int signal) {
  auto &trader_main = TraderMain::GetInstance();
  trader_main.Exit();
}

void TraderMain::Entry(int argc, char *argv[]) {
  pybind11::scoped_interpreter python;
  pybind11::gil_scoped_release release;

  signal(SIGINT, SignalHandler);

  auto &json_cfg = utils::JsonConfig::GetInstance();
  json_cfg.SetFileName("/etc/marktrade/config.json");

  auto trader_log_path = json_cfg.GetConfig("trader", "LogPath").get<std::string>();
  utils::CreatFolder(trader_log_path);
  LOG_INIT(trader_log_path.c_str(), "traderlog", 6);
  INFO_LOG("trade log path is %s", trader_log_path.c_str());

  auto trader_data_path = json_cfg.GetConfig("trader", "ControlParaFilePath").get<std::string>();
  utils::CreatFolder(trader_data_path);
  profiler::FlameGraphWriter::Instance().SetFilePath(trader_data_path);

  FdManage::GetInstance();
  RecerSender::GetInstance();

  auto &trader_ser = TraderService::GetInstance();
  INFO_LOG("trader server init ok");
  trader_ser.Run();

  std::this_thread::sleep_for(std::chrono::seconds(5));

  auto &trader_event = TraderEvent::GetInstance();
  INFO_LOG("trader event init ok");
  trader_event.Run();

  HoldOn();
}

void TraderMain::HoldOn(void) {
  while (is_hold_on_) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

void TraderMain::Exit(void) {
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  is_hold_on_ = false;
}

const std::string &TraderMain::GetTraderName() { return trader_name_; }

int main(int argc, char *argv[]) {
  auto &trader_main = TraderMain::GetInstance();
  trader_main.Entry(argc, argv);
  return 0;
}
