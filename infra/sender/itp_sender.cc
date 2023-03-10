#include "trader/infra/sender/itp_sender.h"
#include "common/self/file_util.h"
#include "trader/infra/sender/btp_sender.h"
#include "trader/infra/sender/ctp_sender.h"
#include "trader/infra/sender/xtp_sender.h"

ItpSender::ItpSender() {
  auto &json_cfg = utils::JsonConfig::GetInstance();
  auto api_type = json_cfg.GetConfig("common", "ApiType");
  if (api_type == "ctp") {
    send_api_ = new CtpSender();
  } else if (api_type == "xtp") {
    send_api_ = new XtpSender();
  } else if (api_type == "btp") {
    send_api_ = new BtpSender();
  }
}

bool ItpSender::ReqUserLogin() { return send_api_->ReqUserLogin(); }
bool ItpSender::ReqUserLogout() { return send_api_->ReqUserLogout(); }
bool ItpSender::InsertOrder(utils::OrderContent &content) { return send_api_->InsertOrder(content); }
bool ItpSender::CancelOrder(utils::OrderContent &content) { return send_api_->CancelOrder(content); }
bool ItpSender::ReqAvailableFunds() { return send_api_->ReqAvailableFunds(); }
bool ItpSender::ReqInstrumentInfo(const utils::InstrumtntID &ins_exch) { return send_api_->ReqInstrumentInfo(ins_exch); }
bool ItpSender::ReqTransactionCost(const utils::InstrumtntID &ins_exch) { return send_api_->ReqTransactionCost(ins_exch); }
bool ItpSender::LossConnection() { return send_api_->LossConnection(); }
