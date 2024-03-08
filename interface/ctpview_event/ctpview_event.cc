/*
 * manageEvent.cpp
 *
 *  Created on: 2020.11.13
 *      Author: Administrator
 */
#include "trader/interface/ctpview_event/ctpview_event.h"
#include <thread>
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/profiler.h"
#include "common/self/protobuf/ctpview-trader.pb.h"
#include "trader/domain/trader_service.h"

CtpviewEvent::CtpviewEvent() { RegMsgFun(); }

void CtpviewEvent::RegMsgFun() {
  int cnt = 0;
  msg_func_map_.clear();
  msg_func_map_["LoginControl"] = [this](utils::ItpMsg &msg) { LoginControlHandle(msg); };
  msg_func_map_["BugInjection"] = [this](utils::ItpMsg &msg) { BugInjectionHandle(msg); };
  msg_func_map_["ProfilerControl"] = [this](utils::ItpMsg &msg) { ProfilerControlHandle(msg); };
  msg_func_map_["UpdatePara"] = [this](utils::ItpMsg &msg) { UpdateParaHandle(msg); };
  msg_func_map_["ClearDiagnosticEvent"] = [this](utils::ItpMsg &msg) { ClearDiagnosticEventHandle(msg); };

  for (auto &iter : msg_func_map_) {
    INFO_LOG("msg_func_map_[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
}

void CtpviewEvent::Handle(utils::ItpMsg &msg) {
  auto iter = msg_func_map_.find(msg.msg_name);
  if (iter != msg_func_map_.end()) {
    iter->second(msg);
    return;
  }
  ERROR_LOG("can not find func for msg_name [%s]!", msg.msg_name.c_str());
  return;
}

void CtpviewEvent::LoginControlHandle(utils::ItpMsg &msg) {
  ctpview_trader::message login_control;
  login_control.ParseFromString(msg.pb_msg);
  auto indication = login_control.login_control();

  int command = indication.command();
  auto &trader_ser = TraderService::GetInstance();

  INFO_LOG("force set time state: %d", command);
  trader_ser.ROLE(TraderTimeState).SetTimeState(command);
}

void CtpviewEvent::BugInjectionHandle(utils::ItpMsg &msg) {
  ctpview_trader::message bug_injection;
  bug_injection.ParseFromString(msg.pb_msg);

  auto injection = bug_injection.bug_injection();

  ctpview_trader::BugInjection_InjectionType type = injection.type();
  INFO_LOG("set bug injection type: %d", type);

  if (type == ctpview_trader::BugInjection_InjectionType_double_free) {
    int *temp_a = new int;
    std::shared_ptr<int> ptr(temp_a);
    delete temp_a;
  }
}

void CtpviewEvent::ProfilerControlHandle(utils::ItpMsg &msg) {
  ctpview_trader::message message;
  message.ParseFromString(msg.pb_msg);
  auto control = message.profiler_control();

  auto action = control.profiler_action();
  if (action == ctpview_trader::ProfilerControl::start_write) {
    profiler::FlameGraphWriter::Instance().StartAddData();
    INFO_LOG("start write tracepoint");
  } else if (action == ctpview_trader::ProfilerControl::stop_write) {
    profiler::FlameGraphWriter::Instance().StopAddData();
    INFO_LOG("stop write tracepoint");
  }
}

void CtpviewEvent::UpdateParaHandle(utils::ItpMsg &msg) {
  ctpview_trader::message message;
  message.ParseFromString(msg.pb_msg);
  auto update = message.update_para();

  auto action = update.update_action();
  if (action == ctpview_trader::UpdatePara::update) {
    utils::JsonConfig::GetInstance().GetConfig();
    INFO_LOG("reload config file");
  }
}

void CtpviewEvent::ClearDiagnosticEventHandle(utils::ItpMsg &msg) {
  auto &trader_ser = TraderService::GetInstance();
  ctpview_trader::message message;
  message.ParseFromString(msg.pb_msg);
  auto clear_diagnostic_event = message.clear_diagnostic_event();
  auto event_id = clear_diagnostic_event.diagnostic_event_id();

  trader_ser.ROLE(Diagnostic).ClearStatus(static_cast<DiagnosticEventId>(event_id));
}
