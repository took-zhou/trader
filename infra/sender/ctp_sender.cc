/*
 * ctpSender.cpp
 *
 *  Created on: 2020.08.29
 *      Author: Administrator
 */

#include "trader/infra/sender/ctp_sender.h"
#include <fstream>
#include <string>
#include <thread>
#include "common/extern/json/json.h"
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/global_sem.h"
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/utils.h"
#include "trader/infra/recer/ctp_recer.h"

std::map<std::string, CtpApiSpiInfo> CtpSender::ctp_api_spi_info_map;
std::map<uint64_t, CtpTraderInfo> CtpSender::ctp_trader_info_map;
CThostFtdcInputOrderField CtpSender::default_order_field;

CtpSender::CtpSender(void) {
  default_order_field.MinVolume = 1;
  default_order_field.ContingentCondition = THOST_FTDC_CC_Immediately;
  default_order_field.StopPrice = 0;
  default_order_field.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
  default_order_field.IsAutoSuspend = 0;
  default_order_field.UserForceClose = 0;
  default_order_field.IsSwapOrder = 0;
  default_order_field.CombHedgeFlag[0] = '1';

  std::string mac;
  if (!utils::GetLocalMac(mac)) {
    ERROR_LOG("get_local_mac error!");
  }
  strcpy(default_order_field.MacAddress, mac.c_str());

  std::string ip_str;
  std::string host_name;
  if (!utils::GetHostInfo(host_name, ip_str)) {
    ERROR_LOG("get_host_info error!");
  }
  strcpy(default_order_field.IPAddress, ip_str.c_str());
}

CtpSender::~CtpSender(void) { Release(); }

bool CtpSender::ReqUserLogin() {
  bool ret = true;
  if (!Init()) {
    Release();
    ret = false;
  } else {
    if (Authenticate()) {
      Login();
      Confirm();
    } else {
      ret = false;
    }
  }
  return ret;
}

bool CtpSender::ReqUserLogout() {
  INFO_LOG("logout time, is going to logout.");
  for (auto &item : ctp_api_spi_info_map) {
    CThostFtdcUserLogoutField log_out_field{{0}};
    auto &json_cfg = utils::JsonConfig::GetInstance();
    const auto user_id = json_cfg.GetDeepConfig("users", item.first, "UserID").get<std::string>();
    const auto broker_id = json_cfg.GetDeepConfig("users", item.first, "BrokerID").get<std::string>();
    strcpy(log_out_field.BrokerID, broker_id.c_str());
    strcpy(log_out_field.UserID, user_id.c_str());

    if (item.second.trader_api != nullptr) {
      int result = item.second.trader_api->ReqUserLogout(&log_out_field, request_id_++);
      INFO_LOG("ReqUserLogout send result is [%d]", result);
      auto &global_sem = GlobalSem::GetInstance();
      global_sem.WaitSemBySemName(SemName::kLoginLogout, 10);
    }
  }
  Release();
  INFO_LOG("ctp log out ok!");

  return true;
}

bool CtpSender::InsertOrder(utils::OrderContent &content) {
  bool ret = true;
  auto &json_cfg = utils::JsonConfig::GetInstance();
  auto pos = ctp_trader_info_map.find(content.session_id);

  if (pos != ctp_trader_info_map.end()) {
    BuildOrder(content, default_order_field);
    const auto broker_id = json_cfg.GetDeepConfig("users", pos->second.user_name, "BrokerID").get<std::string>();
    const auto investor_id = json_cfg.GetDeepConfig("users", pos->second.user_name, "InvestorID").get<std::string>();
    const auto user_id = json_cfg.GetDeepConfig("users", pos->second.user_name, "UserID").get<std::string>();

    strcpy(default_order_field.BrokerID, broker_id.c_str());
    strcpy(default_order_field.InvestorID, investor_id.c_str());
    strcpy(default_order_field.UserID, user_id.c_str());

    int result = pos->second.trader_api->ReqOrderInsert(&default_order_field, request_id_++);
    INFO_LOG("ReqOrderInsert send result is [%d], order_ref is[%s]", result, default_order_field.OrderRef);
  } else {
    ret = false;
    ERROR_LOG("can not find session id: %ld", content.session_id);
  }
  return ret;
}

bool CtpSender::CancelOrder(utils::OrderContent &content) {
  bool ret = true;
  auto &json_cfg = utils::JsonConfig::GetInstance();
  auto pos = ctp_trader_info_map.find(content.session_id);
  if (pos != ctp_trader_info_map.end()) {
    CThostFtdcInputOrderActionField order_action_req{{0}};
    strcpy(order_action_req.OrderRef, content.order_ref.c_str());
    order_action_req.FrontID = pos->second.front_id;
    order_action_req.SessionID = pos->first;
    strcpy(order_action_req.InstrumentID, content.instrument_id.c_str());
    const auto investor_id = json_cfg.GetDeepConfig("users", pos->second.user_name, "InvestorID").get<std::string>();
    const auto user_id = json_cfg.GetDeepConfig("users", pos->second.user_name, "UserID").get<std::string>();
    strcpy(order_action_req.InvestorID, investor_id.c_str());
    strcpy(order_action_req.UserID, user_id.c_str());
    order_action_req.ActionFlag = THOST_FTDC_AF_Delete;
    int result = pos->second.trader_api->ReqOrderAction(&order_action_req, request_id_++);
    INFO_LOG("ReqOrderAction send result is [%d]", result);
  }

  return ret;
}

bool CtpSender::ReqAvailableFunds() {
  auto &json_cfg = utils::JsonConfig::GetInstance();

  for (auto &item : ctp_api_spi_info_map) {
    if (item.second.trader_api != nullptr) {
      CThostFtdcQryTradingAccountField request_msg{{0}};
      const auto broker_id = json_cfg.GetDeepConfig("users", item.first, "BrokerID").get<std::string>();
      const auto investor_id = json_cfg.GetDeepConfig("users", item.first, "InvestorID").get<std::string>();
      strcpy(request_msg.InvestorID, investor_id.c_str());
      strcpy(request_msg.BrokerID, broker_id.c_str());
      strcpy(request_msg.CurrencyID, "CNY");

      int result = item.second.trader_api->ReqQryTradingAccount(&request_msg, 0);
      if (result != 0) {
        INFO_LOG("ReqQryTradingAccount send result is [%d]", result);
      }
    }
  }
  return true;
}

bool CtpSender::ReqInstrumentInfo(const utils::InstrumtntID &ins_exch) {
  for (auto &item : ctp_api_spi_info_map) {
    CThostFtdcQryInstrumentField request_msg{{0}};
    std::strcpy(request_msg.ExchangeID, ins_exch.exch.c_str());
    std::strcpy(request_msg.InstrumentID, ins_exch.ins.c_str());

    int result = item.second.trader_api->ReqQryInstrument(&request_msg, 0);
    INFO_LOG("ReqQryInstrument send result is [%d]", result);
    break;
  }

  return true;
}

bool CtpSender::ReqTransactionCost(const utils::InstrumtntID &ins_exch) {
  auto &json_cfg = utils::JsonConfig::GetInstance();
  for (auto &item : ctp_api_spi_info_map) {
    item.second.trader_spi->SetTransactionCostExchange(ins_exch.exch);
    item.second.trader_spi->SetTransactionCostInstrument(ins_exch.ins);
    const auto investor_id = json_cfg.GetDeepConfig("users", item.first, "InvestorID").get<std::string>();
    const auto broker_id = json_cfg.GetDeepConfig("users", item.first, "BrokerID").get<std::string>();
    if (ins_exch.ins.size() <= 6) {
      CThostFtdcQryInstrumentMarginRateField margin_rate_field{{0}};
      strcpy(margin_rate_field.BrokerID, broker_id.c_str());
      strcpy(margin_rate_field.InvestorID, investor_id.c_str());
      strcpy(margin_rate_field.ExchangeID, ins_exch.exch.c_str());
      strcpy(margin_rate_field.InstrumentID, ins_exch.ins.c_str());
      margin_rate_field.HedgeFlag = THOST_FTDC_HF_Speculation;
      int result = item.second.trader_api->ReqQryInstrumentMarginRate(&margin_rate_field, 0);
      INFO_LOG("ReqQryInstrumentMarginRate send result is [%d]", result);

      std::this_thread::sleep_for(std::chrono::seconds(1));
      CThostFtdcQryInstrumentCommissionRateField commisson_rate_field{{0}};
      strcpy(commisson_rate_field.BrokerID, broker_id.c_str());
      strcpy(commisson_rate_field.InvestorID, investor_id.c_str());
      strcpy(commisson_rate_field.ExchangeID, ins_exch.exch.c_str());
      strcpy(commisson_rate_field.InstrumentID, ins_exch.ins.c_str());
      result = item.second.trader_api->ReqQryInstrumentCommissionRate(&commisson_rate_field, 0);
      INFO_LOG("ReqQryInstrumentCommissionRate send result is [%d]", result);
    } else {
      CThostFtdcQryOptionInstrCommRateField commisson_rate_field{{0}};
      strcpy(commisson_rate_field.BrokerID, broker_id.c_str());
      strcpy(commisson_rate_field.InvestorID, investor_id.c_str());
      strcpy(commisson_rate_field.ExchangeID, ins_exch.exch.c_str());
      strcpy(commisson_rate_field.InstrumentID, ins_exch.ins.c_str());
      int result = item.second.trader_api->ReqQryOptionInstrCommRate(&commisson_rate_field, 0);
      INFO_LOG("ReqQryOptionInstrCommRate send result is [%d]", result);
    }
    break;
  }

  return true;
}

bool CtpSender::Init() {
  bool out = true;
  if (!is_init_) {
    INFO_LOG("begin trader_api init");

    auto &json_cfg = utils::JsonConfig::GetInstance();
    auto users = json_cfg.GetConfig("trader", "User");
    for (auto &user : users) {
      std::string con_path = json_cfg.GetConfig("trader", "ConPath").get<std::string>() + "/" + static_cast<std::string>(user) + "/";
      utils::CreatFolder(con_path);

      std::string confirm_path =
          json_cfg.GetConfig("trader", "ConfirmRecordPath").get<std::string>() + "/" + static_cast<std::string>(user) + "/";
      utils::CreatFolder(confirm_path);

      CtpApiSpiInfo api_spi_info;

      api_spi_info.trader_api = CThostFtdcTraderApi::CreateFtdcTraderApi(con_path.c_str());
      api_spi_info.trader_spi = new CtpTraderSpi();
      api_spi_info.trader_api->RegisterSpi(api_spi_info.trader_spi);
      api_spi_info.trader_api->SubscribePrivateTopic(THOST_TERT_QUICK);
      api_spi_info.trader_api->SubscribePublicTopic(THOST_TERT_QUICK);
      INFO_LOG("ctp version: %s.", api_spi_info.trader_api->GetApiVersion());

      const auto frontaddr = json_cfg.GetDeepConfig("users", static_cast<std::string>(user), "FrontAddr").get<std::string>();
      api_spi_info.trader_api->RegisterFront(const_cast<char *>(frontaddr.c_str()));
      api_spi_info.trader_api->Init();

      auto &global_sem = GlobalSem::GetInstance();
      if (global_sem.WaitSemBySemName(SemName::kLoginLogout, 60)) {
        out = false;
        ERROR_LOG("%s", "trader init fail.");
      } else {
        INFO_LOG("%s", "trader init ok.");
      }

      ctp_api_spi_info_map[static_cast<std::string>(user)] = api_spi_info;
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    is_init_ = true;
  } else {
    ctp_trader_info_map.erase(ctp_trader_info_map.begin(), ctp_trader_info_map.end());
  }

  return out;
}

bool CtpSender::Authenticate(void) {
  bool ret = true;
  CThostFtdcReqAuthenticateField req_authenticate_field{{0}};
  auto &json_cfg = utils::JsonConfig::GetInstance();
  for (auto &item : ctp_api_spi_info_map) {
    const auto auth_code = json_cfg.GetDeepConfig("users", item.first, "AuthCode").get<std::string>();
    const auto app_id = json_cfg.GetDeepConfig("users", item.first, "AppID").get<std::string>();
    const auto user_id = json_cfg.GetDeepConfig("users", item.first, "UserID").get<std::string>();
    const auto broker_id = json_cfg.GetDeepConfig("users", item.first, "BrokerID").get<std::string>();

    strcpy(req_authenticate_field.BrokerID, broker_id.c_str());
    strcpy(req_authenticate_field.UserID, user_id.c_str());
    strcpy(req_authenticate_field.AuthCode, auth_code.c_str());
    strcpy(req_authenticate_field.AppID, app_id.c_str());

    int result = item.second.trader_api->ReqAuthenticate(&req_authenticate_field, request_id_++);
    if (result != 0) {
      INFO_LOG("ReqAuthenticate send result is [%d]", result);
      ret = false;
    } else {
      auto &global_sem = GlobalSem::GetInstance();
      global_sem.WaitSemBySemName(SemName::kLoginLogout);
      INFO_LOG("ReqAuthenticate ok");
    }
  }

  return ret;
}

bool CtpSender::Confirm() {
  bool ret = true;

  for (auto &item : ctp_api_spi_info_map) {
    CThostFtdcQrySettlementInfoConfirmField request_msg{{0}};
    auto &json_cfg = utils::JsonConfig::GetInstance();
    const auto broker_id = json_cfg.GetDeepConfig("users", item.first, "BrokerID").get<std::string>();
    const auto investor_id = json_cfg.GetDeepConfig("users", item.first, "InvestorID").get<std::string>();
    strcpy(request_msg.InvestorID, investor_id.c_str());
    strcpy(request_msg.BrokerID, broker_id.c_str());
    int result = item.second.trader_api->ReqQrySettlementInfoConfirm(&request_msg, request_id_++);
    if (result != 0) {
      INFO_LOG("ReqQrySettlementInfoConfirm send result is [%d]", result);
      ret = false;
    } else {
      auto &global_sem = GlobalSem::GetInstance();
      global_sem.WaitSemBySemName(SemName::kLoginLogout, 10);
    }

    INFO_LOG("trade/confirmed date: %s/%s", item.second.trader_api->GetTradingDay(), item.second.trader_spi->GetConfirmedDate().c_str());
    if (strcmp(item.second.trader_api->GetTradingDay(), item.second.trader_spi->GetConfirmedDate().c_str()) != 0) {
      CThostFtdcSettlementInfoConfirmField request_msg{{0}};
      strcpy(request_msg.InvestorID, investor_id.c_str());
      strcpy(request_msg.BrokerID, broker_id.c_str());
      int result = item.second.trader_api->ReqSettlementInfoConfirm(&request_msg, request_id_++);

      if (result != 0) {
        INFO_LOG("ReqSettlementInfoConfirm send result is [%d]", result);
        ret = false;
      } else {
        auto &global_sem = GlobalSem::GetInstance();
        global_sem.WaitSemBySemName(SemName::kLoginLogout);
      }
    }
  }

  return ret;
}

bool CtpSender::Login(void) {
  bool ret = true;
  for (auto &item : ctp_api_spi_info_map) {
    CThostFtdcReqUserLoginField req_user_login{{0}};
    auto &json_cfg = utils::JsonConfig::GetInstance();
    const auto user_id = json_cfg.GetDeepConfig("users", item.first, "UserID").get<std::string>();
    const auto broker_id = json_cfg.GetDeepConfig("users", item.first, "BrokerID").get<std::string>();
    const auto pass_word = json_cfg.GetDeepConfig("users", item.first, "Password").get<std::string>();
    strcpy(req_user_login.BrokerID, broker_id.c_str());
    strcpy(req_user_login.UserID, user_id.c_str());
    strcpy(req_user_login.Password, pass_word.c_str());

    int result = item.second.trader_api->ReqUserLogin(&req_user_login, request_id_++);

    if (result != 0) {
      INFO_LOG("ReqUserLogin send result is [%d]", result);
      ret = false;
    } else {
      auto &global_sem = GlobalSem::GetInstance();
      global_sem.WaitSemBySemName(SemName::kLoginLogout);

      CtpTraderInfo trader_info;
      trader_info.front_id = item.second.trader_spi->GetFrontid();
      trader_info.user_id = user_id;
      trader_info.user_name = item.first;
      trader_info.trader_api = item.second.trader_api;
      ctp_trader_info_map[item.second.trader_spi->GetSessionId()] = trader_info;
      INFO_LOG("login ctp ok!");
    }
  }
  return ret;
}

bool CtpSender::Release() {
  INFO_LOG("Is going to release trader_api.");
  for (auto &item : ctp_api_spi_info_map) {
    if (item.second.trader_api != nullptr) {
      item.second.trader_api->Release();
      item.second.trader_api = nullptr;
    }

    if (item.second.trader_spi != nullptr) {
      delete item.second.trader_spi;
      item.second.trader_spi = nullptr;
    }
  }

  ctp_api_spi_info_map.erase(ctp_api_spi_info_map.begin(), ctp_api_spi_info_map.end());
  ctp_trader_info_map.erase(ctp_trader_info_map.begin(), ctp_trader_info_map.end());
  is_init_ = false;

  return true;
}

bool CtpSender::BuildOrder(utils::OrderContent &content, CThostFtdcInputOrderField &order) {
  strcpy(order.OrderRef, content.order_ref.c_str());
  strcpy(order.InstrumentID, content.instrument_id.c_str());
  strcpy(order.ExchangeID, content.exchange_id.c_str());
  order.LimitPrice = content.limit_price;
  order.VolumeTotalOriginal = content.once_volume;

  if (content.comboffset == strategy_trader::OPEN) {
    order.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
  } else if (content.comboffset == strategy_trader::CLOSE) {
    order.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
  } else if (content.comboffset == strategy_trader::CLOSE_YESTERDAY) {
    order.CombOffsetFlag[0] = THOST_FTDC_OF_CloseYesterday;
  } else if (content.comboffset == strategy_trader::CLOSE_TODAY) {
    order.CombOffsetFlag[0] = THOST_FTDC_OFEN_CloseToday;
  }

  if (content.direction == strategy_trader::BUY) {
    order.Direction = '0';
  } else if (content.direction == strategy_trader::SELL) {
    order.Direction = '1';
  }

  if (content.order_type == strategy_trader::limit_LIMIT) {
    order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    order.TimeCondition = THOST_FTDC_TC_GFD;
    order.VolumeCondition = THOST_FTDC_VC_AV;
  } else if (content.order_type == strategy_trader::Limit_FAK) {
    order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    order.TimeCondition = THOST_FTDC_TC_IOC;
    order.VolumeCondition = THOST_FTDC_VC_AV;
  } else if (content.order_type == strategy_trader::limit_FOK) {
    order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    order.TimeCondition = THOST_FTDC_TC_IOC;
    order.VolumeCondition = THOST_FTDC_VC_CV;
  } else if (content.order_type == strategy_trader::AnyPrice_Fok) {
    order.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
    order.TimeCondition = THOST_FTDC_TC_GFD;
    order.VolumeCondition = THOST_FTDC_VC_AV;
  } else if (content.order_type == strategy_trader::AnyPrice_Fak) {
    order.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
    order.TimeCondition = THOST_FTDC_TC_IOC;
    order.VolumeCondition = THOST_FTDC_VC_AV;
  }
  return true;
}

bool CtpSender::LossConnection() {
  bool ret = false;
  for (auto &item : ctp_api_spi_info_map) {
    if (item.second.trader_spi != nullptr && item.second.trader_spi->GetFrontDisconnected()) {
      ret = true;
    }
    break;
  }
  return ret;
}
