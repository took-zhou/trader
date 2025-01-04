#include "trader/infra/recer/otp_recer.h"
#include "common/extern/log/log.h"
#include "common/self/global_sem.h"
#include "common/self/protobuf/ipc.pb.h"
#include "common/self/utils.h"
#include "trader/infra/recer_sender.h"

int32_t OtpTraderSpi::OnConnected(eOesApiChannelTypeT channel_type, OesApiSessionInfoT *session_info,
                                  OesApiSubscribeInfoT *subscribe_info) {
  return 0;
};

int32_t OtpTraderSpi::OnDisconnected(eOesApiChannelTypeT channel_type, OesApiSessionInfoT *session_info) { return 0; }

void OtpTraderSpi::OnBusinessReject(const OesOrdRejectT *order_reject) {
  if (order_reject != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(order_reject));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "otp_trader";
    msg.msg_name = "OnBusinessReject";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("order reject is nullptr");
  }
}

void OtpTraderSpi::OnOrderReport(const OesOrdCnfmT *order_report) {
  if (order_report != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(order_report));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "otp_trader";
    msg.msg_name = "OnOrderReport";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("order reject is nullptr");
  }
}

void OtpTraderSpi::OnTradeReport(const OesTrdCnfmT *trade_report) {
  if (trade_report != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(trade_report));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "otp_trader";
    msg.msg_name = "OnTradeReport";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("order reject is nullptr");
  }
}

void OtpTraderSpi::OnQueryCashAsset(const OesApiSessionInfoT *session, const OesCashAssetItemT *cash_asset) {
  if (cash_asset != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(cash_asset));
    send_msg->set_session_id(session->__clEnvId);
    send_msg->set_user_id(cash_asset->custId);
    INFO_LOG("cust id: %s, cash acct id: %s", cash_asset->custId, cash_asset->cashAcctId);
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "otp_trader";
    msg.msg_name = "OnQueryCashAsset";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("order reject is nullptr");
  }
}

void OtpTraderSpi::OnQueryCommissionRate(const OesApiSessionInfoT *session, const OesCommissionRateItemT *commission_rate) {
  if (commission_rate != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(commission_rate));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "otp_trader";
    msg.msg_name = "OnQueryCommissionRate";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("order reject is nullptr");
  }
}
