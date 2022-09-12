#include "trader/infra/sender/itp_sender.h"
#include "common/self/file_util.h"
#include "trader/infra/sender/ctp_sender.h"
#include "trader/infra/sender/xtp_sender.h"

ItpSender::ItpSender() {
  auto &jsonCfg = utils::JsonConfig::getInstance();
  auto apiType = jsonCfg.get_config("common", "ApiType");
  if (apiType == "ctp") {
    send_api = new CtpSender();
  } else if (apiType == "xtp") {
    send_api = new XtpSender();
  }
}

bool ItpSender::ReqUserLogin() { send_api->ReqUserLogin(); }
bool ItpSender::ReqUserLogout() { send_api->ReqUserLogout(); }
bool ItpSender::InsertOrder(utils::OrderContent &content) { send_api->InsertOrder(content); }
bool ItpSender::CancelOrder(utils::OrderContent &content) { send_api->CancelOrder(content); }
bool ItpSender::ReqAvailableFunds(const int requestId) { send_api->ReqAvailableFunds(requestId); }
bool ItpSender::ReqInstrumentInfo(const utils::InstrumtntID &ins_exch, const int requestId) {
  send_api->ReqInstrumentInfo(ins_exch, requestId);
}
bool ItpSender::ReqTransactionCost(const utils::InstrumtntID &ins_exch, const int requestId) {
  send_api->ReqTransactionCost(ins_exch, requestId);
}
bool ItpSender::LossConnection() { send_api->LossConnection(); }
