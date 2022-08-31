#include "trader/infra/sender/itpSender.h"
#include "common/self/fileUtil.h"
#include "trader/infra/sender/ctpSender.h"
#include "trader/infra/sender/xtpSender.h"

ItpSender::ItpSender() {
  auto &jsonCfg = utils::JsonConfig::getInstance();
  auto apiType = jsonCfg.getConfig("common", "ApiType");
  if (apiType == "ctp") {
    sendApi = new CtpSender();
  } else if (apiType == "xtp") {
    sendApi = new XtpSender();
  }
}

bool ItpSender::ReqUserLogin() { sendApi->ReqUserLogin(); }
bool ItpSender::ReqUserLogout() { sendApi->ReqUserLogout(); }
bool ItpSender::InsertOrder(utils::OrderContent &content) { sendApi->InsertOrder(content); }
bool ItpSender::CancelOrder(utils::OrderContent &content) { sendApi->CancelOrder(content); }
bool ItpSender::ReqAvailableFunds(const int requestId) { sendApi->ReqAvailableFunds(requestId); }
bool ItpSender::ReqInstrumentInfo(const utils::InstrumtntID &ins_exch, const int requestId) {
  sendApi->ReqInstrumentInfo(ins_exch, requestId);
}
bool ItpSender::ReqTransactionCost(const utils::InstrumtntID &ins_exch, const int requestId) {
  sendApi->ReqTransactionCost(ins_exch, requestId);
}
bool ItpSender::LossConnection() { sendApi->LossConnection(); }
