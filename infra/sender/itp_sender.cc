#include "trader/infra/sender/itp_sender.h"
#include <memory>
#include "common/self/file_util.h"
#include "trader/infra/sender/btp_sender.h"
#include "trader/infra/sender/ctp_sender.h"
#include "trader/infra/sender/ftp_sender.h"
#include "trader/infra/sender/gtp_sender.h"
#include "trader/infra/sender/mtp_sender.h"
#include "trader/infra/sender/otp_sender.h"
#include "trader/infra/sender/xtp_sender.h"

ItpSender::ItpSender() {
  auto &json_cfg = utils::JsonConfig::GetInstance();
  auto api_type = json_cfg.GetConfig("common", "ApiType");
  if (api_type == "ctp") {
    send_api_ = std::make_unique<CtpSender>();
  } else if (api_type == "xtp") {
    send_api_ = std::make_unique<XtpSender>();
  } else if (api_type == "btp") {
    send_api_ = std::make_unique<BtpSender>();
  } else if (api_type == "otp") {
    send_api_ = std::make_unique<OtpSender>();
  } else if (api_type == "ftp") {
    send_api_ = std::make_unique<FtpSender>();
  } else if (api_type == "gtp") {
    send_api_ = std::make_unique<GtpSender>();
  } else if (api_type == "mtp") {
    send_api_ = std::make_unique<MtpSender>();
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
