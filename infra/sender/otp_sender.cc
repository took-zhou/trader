#include "trader/infra/sender/otp_sender.h"
#include <cstring>
#include <string>
#include <thread>
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/utils.h"
#include "oes_api/oes_api.h"
#include "trader/infra/recer/otp_recer.h"

std::map<int, OtpTraderInfo> OtpSender::otp_trader_info_map;

static int32 ApiAsyncConnect(OesAsyncApiChannelT *async_channel, void *callback);
static int32 ApiAsyncDisconnect(OesAsyncApiChannelT *async_channel, void *callback);
static int32 HandleOrderRsp(OesApiSessionInfoT *session, SMsgHeadT *head, void *item, void *callback);
static int32 HandleReportMsg(OesApiSessionInfoT *rpt_channel, SMsgHeadT *head, void *item, void *callback);
static int32 ApiQueryCashAsset(OesApiSessionInfoT *session, SMsgHeadT *head, void *item, OesQryCursorT *qry_cursor, void *callback);
static int32 ApiQueryCommissionRate(OesApiSessionInfoT *session, SMsgHeadT *head, void *item, OesQryCursorT *qry_cursor, void *callback);

OtpSender::OtpSender() { ; }

bool OtpSender::ReqUserLogin() {
  INFO_LOG("login time, is going to login.");
  bool ret = true;
  if (!Init()) {
    Release();
    ret = false;
  } else {
    if (!OesAsyncApi_Start(async_context_)) {
      ERROR_LOG("Failed to start the asynchronous API thread! error[%d - %s]", OesApi_GetLastError(),
                OesApi_GetErrorMsg(OesApi_GetLastError()));
      ret = FALSE;
    }

    INFO_LOG("login otp ok!");
  }

  return ret;
}

bool OtpSender::ReqUserLogout() {
  INFO_LOG("logout time, is going to logout.");

  Release();
  return true;
}

bool OtpSender::InsertOrder(utils::OrderContent &content) {
  bool ret = true;
  auto pos = otp_trader_info_map.find(content.session_id);
  if (pos != otp_trader_info_map.end()) {
    OesOrdReqT ord_req = {NULLOBJ_OES_ORD_REQ};
    ord_req.clSeqNo = stoi(content.order_ref);
    if (content.exchange_id == "SHSE") {
      ord_req.mktId = OES_MKT_SH_ASHARE;
    } else if (content.exchange_id == "SZSE") {
      ord_req.mktId = OES_MKT_SZ_ASHARE;
    }
    ord_req.ordType = OES_ORD_TYPE_LMT;
    ord_req.bsType = content.direction;
    ord_req.ordQty = content.total_volume;
    ord_req.ordPrice = content.limit_price * 0.0001;
    strcpy(ord_req.securityId, content.instrument_id.c_str());
    if (OesAsyncApi_SendOrderReq(pos->second.ord_channel, &ord_req) < 0) {
      ERROR_LOG("Send order error.");
    }
  } else {
    ret = false;
    ERROR_LOG("can not find session id: %ld", content.session_id);
  }
  return ret;
}

bool OtpSender::CancelOrder(utils::OrderContent &content) {
  bool ret = true;
  auto pos = otp_trader_info_map.find(content.session_id);
  if (pos != otp_trader_info_map.end()) {
    OesOrdCancelReqT cancel_req = {NULLOBJ_OES_ORD_CANCEL_REQ};
    cancel_req.clSeqNo = stoi(content.order_ref);
    if (content.exchange_id == "SHSE") {
      cancel_req.mktId = OES_MKT_SH_ASHARE;
    } else if (content.exchange_id == "SZSE") {
      cancel_req.mktId = OES_MKT_SZ_ASHARE;
    }
    cancel_req.origClSeqNo = stoi(content.order_ref);
    cancel_req.origClEnvId = pos->first;
    strcpy(cancel_req.securityId, content.instrument_id.c_str());
    if (OesAsyncApi_SendOrderCancelReq(pos->second.ord_channel, &cancel_req) < 0) {
      ERROR_LOG("Cancle order error.");
    }
  }
  return ret;
}

bool OtpSender::Init(void) {
  bool out = true;

  if (!is_init_) {
    INFO_LOG("begin OtpTraderApi init");
    if (!__OesApi_CheckApiVersion()) {
      ERROR_LOG("Api version error, version[%s], libversion[%s]", OES_APPL_VER_ID, OesApi_GetApiVersion());
    } else {
      INFO_LOG("API version: %s", OesApi_GetApiVersion());
    }
    if (!OesApi_InitLoggerDirect("file", "FATAL", "./api.log", 0, 0)) {
      ERROR_LOG("init oes log fail!");
    }

    OesAsyncApiContextParamsT context_params = {DEFAULT_OESAPI_ASYNC_CONTEXT_PARAMS};
    context_params.asyncQueueSize = 100000;
    context_params.isHugepageAble = TRUE;
    context_params.isAsyncCallbackAble = FALSE;
    context_params.isBusyPollAble = TRUE;
    context_params.isPreconnectAble = TRUE;
    context_params.isBuiltinQueryable = TRUE;
    async_context_ = OesAsyncApi_CreateContextSimple2(NULL, NULL, &context_params);
    if (!async_context_) {
      ERROR_LOG("failed to create an asynchronous API runtime environment.");
      out = false;
    }
    auto &json_cfg = utils::JsonConfig::GetInstance();
    auto users = json_cfg.GetConfig("trader", "User");
    uint8_t envid = 1;
    for (auto &user : users) {
      OtpTraderInfo trader_info;
      trader_info.trader_spi = new OtpTraderSpi();
      OesApiRemoteCfgT remote_cfg = {NULLOBJ_OESAPI_REMOTE_CFG};
      auto ord_server = json_cfg.GetDeepConfig("users", static_cast<std::string>(user), "FrontOrdServer").get<std::string>();
      auto rpt_server = json_cfg.GetDeepConfig("users", static_cast<std::string>(user), "FrontRptServer").get<std::string>();
      auto qry_server = json_cfg.GetDeepConfig("users", static_cast<std::string>(user), "FrontQryServer").get<std::string>();
      auto user_id = json_cfg.GetDeepConfig("users", user, "UserID").get<std::string>();
      auto password = json_cfg.GetDeepConfig("users", user, "Password").get<std::string>();
      strcpy(remote_cfg.username, user_id.c_str());
      strcpy(remote_cfg.password, password.c_str());
      remote_cfg.heartBtInt = 30;
      remote_cfg.clEnvId = envid++;
      remote_cfg.addrCnt = OesApi_ParseAddrListString(ord_server.c_str(), remote_cfg.addrList, GENERAL_CLI_MAX_REMOTE_CNT);
      trader_info.user_id = user_id;
      trader_info.user_name = user;
      trader_info.ord_channel = OesAsyncApi_AddChannel(async_context_, OESAPI_CHANNEL_TYPE_ORDER, OESAPI_CFG_DEFAULT_KEY_ORD_ADDR,
                                                       &remote_cfg, nullptr, HandleOrderRsp, trader_info.trader_spi, ApiAsyncConnect,
                                                       trader_info.trader_spi, ApiAsyncDisconnect, trader_info.trader_spi);
      if ((!trader_info.ord_channel)) {
        ERROR_LOG("failed to add a delegate channel! channelTag[%s]", OESAPI_CFG_DEFAULT_KEY_ORD_ADDR);
        out = false;
      }
      remote_cfg.addrCnt = OesApi_ParseAddrListString(rpt_server.c_str(), remote_cfg.addrList, GENERAL_CLI_MAX_REMOTE_CNT);
      trader_info.rpt_channel = OesAsyncApi_AddChannel(async_context_, OESAPI_CHANNEL_TYPE_REPORT, OESAPI_CFG_DEFAULT_KEY_RPT_ADDR,
                                                       &remote_cfg, nullptr, HandleReportMsg, trader_info.trader_spi, ApiAsyncConnect,
                                                       trader_info.trader_spi, ApiAsyncDisconnect, trader_info.trader_spi);
      if ((!trader_info.rpt_channel)) {
        ERROR_LOG("failed to add a return channel! channelTag[%s]", OESAPI_CFG_DEFAULT_KEY_RPT_ADDR);
        out = false;
      }
      otp_trader_info_map[remote_cfg.clEnvId] = trader_info;
    }
    OesAsyncApi_SetBuiltinQueryable(async_context_, TRUE);
    OesAsyncApi_SetPreconnectAble(async_context_, TRUE);
    OesApi_SetCustomizedDriverId("WD-WCC4M0EP1YDD");
    INFO_LOG("traderApi init ok.");
    is_init_ = true;
  }

  return out;
}

bool OtpSender::Release() {
  INFO_LOG("Is going to release traderApi.");

  if (async_context_ != nullptr) {
    OesAsyncApi_Stop(async_context_);
    OesAsyncApi_ReleaseContext(async_context_);
    async_context_ = nullptr;
  }

  for (auto &item : otp_trader_info_map) {
    if (item.second.trader_spi != nullptr) {
      delete item.second.trader_spi;
      item.second.trader_spi = NULL;
    }
  }

  otp_trader_info_map.erase(otp_trader_info_map.begin(), otp_trader_info_map.end());
  is_init_ = false;

  return true;
}

bool OtpSender::ReqAvailableFunds() {
  for (auto &item : otp_trader_info_map) {
    if (OesAsyncApi_QueryCashAsset(item.second.ord_channel, nullptr, ApiQueryCashAsset, static_cast<void *>(item.second.trader_spi)) < 0) {
      ERROR_LOG("query fund error.");
    }
  }

  return true;
}

bool OtpSender::ReqInstrumentInfo(const utils::InstrumtntID &ins_exch) {
  INFO_LOG("ReqInstrumentInfo not support.");
  return true;
}

bool OtpSender::ReqTransactionCost(const utils::InstrumtntID &ins_exch) {
  for (auto &item : otp_trader_info_map) {
    if (OesAsyncApi_QueryCommissionRate(item.second.ord_channel, nullptr, ApiQueryCommissionRate,
                                        static_cast<void *>(item.second.trader_spi)) < 0) {
      ERROR_LOG("query transation cost error.");
    }
    break;
  }
  return true;
}

bool OtpSender::LossConnection() { return false; }

static int32 ApiAsyncConnect(OesAsyncApiChannelT *async_channel, void *callback) {
  auto *subscribe_info = static_cast<OesApiSubscribeInfoT *>(NULL);
  auto *trader_spi = static_cast<OtpTraderSpi *>(callback);
  int32 ret = 0;

  if (async_channel->pChannelCfg->channelType == OESAPI_CHANNEL_TYPE_REPORT) {
    subscribe_info = OesAsyncApi_GetChannelSubscribeCfg(async_channel);
    if ((!subscribe_info)) {
      ERROR_LOG("Illegal extended subscribe info! pAsyncChannel[%p], channelTag[%s]", async_channel,
                async_channel->pChannelCfg->channelTag);
    }
  }
  ret = trader_spi->OnConnected(static_cast<eOesApiChannelTypeT>(async_channel->pChannelCfg->channelType), async_channel->pSessionInfo,
                                subscribe_info);
  if ((ret < 0)) {
    ERROR_LOG("Call SPI.OnConnected failure! channelType[%d], ret[%d]", async_channel->pChannelCfg->channelType, ret);
  }
  return OesAsyncApi_DefaultOnConnect(async_channel, NULL);
}

static int32 ApiAsyncDisconnect(OesAsyncApiChannelT *async_channel, void *callback) {
  auto *trader_spi = static_cast<OtpTraderSpi *>(callback);
  int32 ret = 0;

  ret = trader_spi->OnDisconnected(static_cast<eOesApiChannelTypeT>(async_channel->pChannelCfg->channelType), async_channel->pSessionInfo);
  if ((ret < 0)) {
    ERROR_LOG("Call SPI.OnDisconnected failure! channelType[%d], ret[%d]", async_channel->pChannelCfg->channelType, ret);
  }

  return 0;
}

static int32 HandleOrderRsp(OesApiSessionInfoT *session, SMsgHeadT *head, void *item, void *callback) { return 0; }

static int32 HandleReportMsg(OesApiSessionInfoT *rpt_channel, SMsgHeadT *head, void *item, void *callback) {
  auto *trader_spi = static_cast<OtpTraderSpi *>(callback);
  auto *rsp_msg = static_cast<OesRspMsgBodyT *>(item);
  OesRptMsgT *rpt_msg = &rsp_msg->rptMsg;

  switch (head->msgId) {
    case OESMSG_RPT_BUSINESS_REJECT:
      trader_spi->OnBusinessReject(&rpt_msg->rptBody.ordRejectRsp);
      break;

    case OESMSG_RPT_ORDER_REPORT:
      trader_spi->OnOrderReport(&rpt_msg->rptBody.ordCnfm);
      break;

    case OESMSG_RPT_TRADE_REPORT:
      trader_spi->OnTradeReport(&rpt_msg->rptBody.trdCnfm);
      break;

    default:
      break;
  }

  return 0;
}

static int32 ApiQueryCashAsset(OesApiSessionInfoT *session, SMsgHeadT *head, void *item, OesQryCursorT *qry_cursor, void *callback) {
  auto trader_spi = static_cast<OtpTraderSpi *>(callback);
  trader_spi->OnQueryCashAsset(session, static_cast<OesCashAssetItemT *>(item));
  return 0;
}

static int32 ApiQueryCommissionRate(OesApiSessionInfoT *session, SMsgHeadT *head, void *item, OesQryCursorT *qry_cursor, void *callback) {
  auto trader_spi = static_cast<OtpTraderSpi *>(callback);
  trader_spi->OnQueryCommissionRate(session, static_cast<OesCommissionRateItemT *>(item));
  return 0;
}