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
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/semaphore.h"
#include "common/self/utils.h"
#include "trader/infra/recer/ctp_recer.h"

std::map<std::string, CtpTraderInfo> CtpSender::kCtpTraderInfoMap;
CThostFtdcInputOrderField CtpSender::kDefaultOrderField;

CtpSender::CtpSender(void) {
  kDefaultOrderField.MinVolume = 1;
  kDefaultOrderField.ContingentCondition = THOST_FTDC_CC_Immediately;
  kDefaultOrderField.StopPrice = 0;
  kDefaultOrderField.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
  kDefaultOrderField.IsAutoSuspend = 0;
  kDefaultOrderField.UserForceClose = 0;
  kDefaultOrderField.IsSwapOrder = 0;
  kDefaultOrderField.CombHedgeFlag[0] = '1';

  std::string mac;
  if (!utils::get_local_mac(mac)) {
    ERROR_LOG("get_local_mac error!");
  }
  strcpy(kDefaultOrderField.MacAddress, mac.c_str());

  std::string Ip;
  std::string hostName;
  if (!utils::get_host_info(hostName, Ip)) {
    ERROR_LOG("get_host_info error!");
  }
  strcpy(kDefaultOrderField.IPAddress, Ip.c_str());
}

bool CtpSender::ReqUserLogin() {
  bool ret = true;
  if (Init() == false) {
    Release();
    ret = false;
  } else {
    Authenticate();

    for (auto &item : kCtpTraderInfoMap) {
      CThostFtdcReqUserLoginField reqUserLogin{0};
      auto &jsonCfg = utils::JsonConfig::getInstance();
      const std::string userID = jsonCfg.get_deep_config("users", item.first, "UserID").get<std::string>();
      const std::string brokerID = jsonCfg.get_deep_config("users", item.first, "BrokerID").get<std::string>();
      const std::string passWord = jsonCfg.get_deep_config("users", item.first, "Password").get<std::string>();
      strcpy(reqUserLogin.BrokerID, brokerID.c_str());
      strcpy(reqUserLogin.UserID, userID.c_str());
      strcpy(reqUserLogin.Password, passWord.c_str());

      auto &globalSem = GlobalSem::getInstance();
      int result = item.second.trader_api->ReqUserLogin(&reqUserLogin, request_id++);
      INFO_LOG("ReqUserLogin send result is [%d]", result);
      globalSem.WaitSemBySemName(GlobalSem::kLoginLogout);
      INFO_LOG("login ctp ok!");
    }
    Confirm();
    return true;
  }
}
bool CtpSender::ReqUserLogout() {
  for (auto &item : kCtpTraderInfoMap) {
    CThostFtdcUserLogoutField logOutField{0};
    auto &jsonCfg = utils::JsonConfig::getInstance();
    const std::string userID = jsonCfg.get_deep_config("users", item.first, "UserID").get<std::string>();
    const std::string brokerID = jsonCfg.get_deep_config("users", item.first, "BrokerID").get<std::string>();
    strcpy(logOutField.BrokerID, brokerID.c_str());
    strcpy(logOutField.UserID, userID.c_str());

    auto &globalSem = GlobalSem::getInstance();
    int result = item.second.trader_api->ReqUserLogout(&logOutField, request_id++);
    INFO_LOG("ReqUserLogout send result is [%d]", result);

    globalSem.WaitSemBySemName(GlobalSem::kLoginLogout, 10);
  }
  Release();
  INFO_LOG("ctp log out ok!");
}

bool CtpSender::InsertOrder(utils::OrderContent &content) {
  bool ret = true;
  auto &jsonCfg = utils::JsonConfig::getInstance();
  auto pos = kCtpTraderInfoMap.find(content.userId);
  if (pos != kCtpTraderInfoMap.end()) {
    BuildOrder(content, kDefaultOrderField);
    const std::string brokerId = jsonCfg.get_deep_config("users", pos->first, "BrokerID").get<std::string>();
    const std::string investorId = jsonCfg.get_deep_config("users", pos->first, "InvestorID").get<std::string>();
    const std::string userId = jsonCfg.get_deep_config("users", pos->first, "UserID").get<std::string>();

    strcpy(kDefaultOrderField.BrokerID, brokerId.c_str());
    strcpy(kDefaultOrderField.InvestorID, investorId.c_str());
    strcpy(kDefaultOrderField.UserID, userId.c_str());

    int result = pos->second.trader_api->ReqOrderInsert(&kDefaultOrderField, request_id++);
    INFO_LOG("ReqOrderInsert send result is [%d], orderRef is[%s]", result, kDefaultOrderField.OrderRef);
  }

  return ret;
}

bool CtpSender::CancelOrder(utils::OrderContent &content) {
  bool ret = true;
  auto &jsonCfg = utils::JsonConfig::getInstance();
  auto pos = kCtpTraderInfoMap.find(content.userId);
  if (pos != kCtpTraderInfoMap.end()) {
    CThostFtdcInputOrderActionField orderActionReq{0};
    strcpy(orderActionReq.OrderRef, content.orderRef.c_str());
    orderActionReq.FrontID = pos->second.front_id;
    orderActionReq.SessionID = pos->second.session_id;
    strcpy(orderActionReq.InstrumentID, content.instrumentID.c_str());
    const std::string investorId = jsonCfg.get_deep_config("users", pos->first, "InvestorID").get<std::string>();
    const std::string userId = jsonCfg.get_deep_config("users", pos->first, "UserID").get<std::string>();
    strcpy(orderActionReq.InvestorID, investorId.c_str());
    strcpy(orderActionReq.UserID, userId.c_str());
    orderActionReq.ActionFlag = THOST_FTDC_AF_Delete;
    int result = pos->second.trader_api->ReqOrderAction(&orderActionReq, request_id++);
    INFO_LOG("ReqOrderAction send result is [%d]", result);
  }

  return ret;
}

bool CtpSender::ReqAvailableFunds(const int requestId) {
  auto &jsonCfg = utils::JsonConfig::getInstance();

  for (auto &item : kCtpTraderInfoMap) {
    CThostFtdcQryTradingAccountField requestMsg{0};
    const std::string brokerID = jsonCfg.get_deep_config("users", item.first, "BrokerID").get<std::string>();
    const std::string investorID = jsonCfg.get_deep_config("users", item.first, "InvestorID").get<std::string>();
    strcpy(requestMsg.InvestorID, investorID.c_str());
    strcpy(requestMsg.BrokerID, brokerID.c_str());
    strcpy(requestMsg.CurrencyID, "CNY");

    int result = item.second.trader_api->ReqQryTradingAccount(&requestMsg, requestId);
    if (requestId != 0) {
      INFO_LOG("ReqQryTradingAccount send result is [%d]", result);
    }
  }
}

bool CtpSender::ReqInstrumentInfo(const utils::InstrumtntID &ins_exch, const int requestId) {
  for (auto &item : kCtpTraderInfoMap) {
    CThostFtdcQryInstrumentField requestMsg{0};
    std::strcpy(requestMsg.ExchangeID, ins_exch.exch.c_str());
    std::strcpy(requestMsg.InstrumentID, ins_exch.ins.c_str());

    int result = item.second.trader_api->ReqQryInstrument(&requestMsg, requestId);
    INFO_LOG("ReqQryInstrument send result is [%d]", result);
  }
}

bool CtpSender::ReqTransactionCost(const utils::InstrumtntID &ins_exch, const int requestId) {
  auto &jsonCfg = utils::JsonConfig::getInstance();
  for (auto &item : kCtpTraderInfoMap) {
    CThostFtdcQryInstrumentMarginRateField marginRateField{0};
    const std::string investorID = jsonCfg.get_deep_config("users", item.first, "InvestorID").get<std::string>();
    const std::string brokerID = jsonCfg.get_deep_config("users", item.first, "BrokerID").get<std::string>();
    strcpy(marginRateField.BrokerID, brokerID.c_str());
    strcpy(marginRateField.InvestorID, investorID.c_str());
    strcpy(marginRateField.ExchangeID, ins_exch.exch.c_str());
    strcpy(marginRateField.InstrumentID, ins_exch.ins.c_str());
    marginRateField.HedgeFlag = THOST_FTDC_HF_Speculation;
    int result = item.second.trader_api->ReqQryInstrumentMarginRate(&marginRateField, requestId);
    INFO_LOG("ReqQryInstrumentMarginRate send result is [%d]", result);

    std::this_thread::sleep_for(1s);
    CThostFtdcQryInstrumentCommissionRateField commissonRateField{0};
    strcpy(commissonRateField.BrokerID, brokerID.c_str());
    strcpy(commissonRateField.InvestorID, investorID.c_str());
    strcpy(commissonRateField.ExchangeID, ins_exch.exch.c_str());
    strcpy(commissonRateField.InstrumentID, ins_exch.ins.c_str());
    result = item.second.trader_api->ReqQryInstrumentCommissionRate(&commissonRateField, requestId);
    INFO_LOG("ReqQryInstrumentCommissionRate send result is [%d]", result);
  }
}

bool CtpSender::Init() {
  bool out = true;
  if (is_init == false) {
    INFO_LOG("begin Ctptrader_api init");

    auto &jsonCfg = utils::JsonConfig::getInstance();
    auto users = jsonCfg.get_config("market", "User");
    for (auto &user : users) {
      std::string conPath = jsonCfg.get_config("trader", "ConPath").get<std::string>() + "/" + (std::string)user + "/";
      utils::CreatFolder(conPath);

      std::string confirmPath = jsonCfg.get_config("trader", "ConfirmRecordPath").get<std::string>() + "/" + (std::string)user + "/";
      utils::CreatFolder(confirmPath);

      CtpTraderInfo traderInfo;

      traderInfo.trader_api = CThostFtdcTraderApi::CreateFtdcTraderApi(conPath.c_str());
      traderInfo.trader_spi = new CtpTraderSpi();
      traderInfo.trader_api->RegisterSpi(traderInfo.trader_spi);
      traderInfo.trader_api->SubscribePrivateTopic(THOST_TERT_QUICK);
      traderInfo.trader_api->SubscribePublicTopic(THOST_TERT_QUICK);
      INFO_LOG("ctp version: %s.", traderInfo.trader_api->GetApiVersion());

      const std::string frontaddr = jsonCfg.get_deep_config("users", (std::string)user, "FrontAddr").get<std::string>();
      traderInfo.trader_api->RegisterFront(const_cast<char *>(frontaddr.c_str()));
      traderInfo.trader_api->Init();

      auto &globalSem = GlobalSem::getInstance();
      if (globalSem.WaitSemBySemName(GlobalSem::kLoginLogout, 60)) {
        out = false;
        ERROR_LOG("%s", "trader init fail.");
      } else {
        INFO_LOG("%s", "trader init ok.");
      }

      traderInfo.session_id = traderInfo.trader_spi->session_id;
      traderInfo.front_id = traderInfo.trader_spi->front_id;
      kCtpTraderInfoMap[(std::string)user] = traderInfo;
      std::this_thread::sleep_for(1000ms);
    }
  }
  return out;
}

bool CtpSender::Authenticate(void) {
  bool ret = true;
  CThostFtdcReqAuthenticateField pReqAuthenticateField = {0};
  auto &jsonCfg = utils::JsonConfig::getInstance();
  for (auto &item : kCtpTraderInfoMap) {
    const std::string authCode = jsonCfg.get_deep_config("users", item.first, "AuthCode").get<std::string>();
    const std::string appId = jsonCfg.get_deep_config("users", item.first, "AppID").get<std::string>();
    const std::string userID = jsonCfg.get_deep_config("users", item.first, "UserID").get<std::string>();
    const std::string brokerID = jsonCfg.get_deep_config("users", item.first, "BrokerID").get<std::string>();

    strcpy(pReqAuthenticateField.BrokerID, brokerID.c_str());
    strcpy(pReqAuthenticateField.UserID, userID.c_str());
    strcpy(pReqAuthenticateField.AuthCode, authCode.c_str());
    strcpy(pReqAuthenticateField.AppID, appId.c_str());

    auto &globalSem = GlobalSem::getInstance();
    int result = item.second.trader_api->ReqAuthenticate(&pReqAuthenticateField, request_id++);
    INFO_LOG("ReqAuthenticate send result is [%d]", result);

    globalSem.WaitSemBySemName(GlobalSem::kLoginLogout);
  }

  INFO_LOG("ReqAuthenticate ok");
  return ret;
}

bool CtpSender::Confirm() {
  bool ret = true;
  char buffer[16];
  auto &jsonCfg = utils::JsonConfig::getInstance();

  for (auto &item : kCtpTraderInfoMap) {
    std::string confirmDir = jsonCfg.get_config("trader", "ConfirmRecordPath").get<std::string>();
    std::string confirmFile = confirmDir + "/" + item.first + "/confirmRecord.txt";
    {
      ifstream in(confirmFile);
      if (in.is_open()) {
        in.getline(buffer, 16);
      }
    }

    if (strcmp(buffer, item.second.trader_api->GetTradingDay()) == 0) {
      INFO_LOG("settlement today has been confirmed before!");
    } else {
      CThostFtdcSettlementInfoConfirmField requestMsg{0};
      auto &jsonCfg = utils::JsonConfig::getInstance();
      const std::string brokerID = jsonCfg.get_deep_config("users", item.first, "BrokerID").get<std::string>();
      const std::string investorID = jsonCfg.get_deep_config("users", item.first, "InvestorID").get<std::string>();
      strcpy(requestMsg.InvestorID, investorID.c_str());
      strcpy(requestMsg.BrokerID, brokerID.c_str());

      auto &globalSem = GlobalSem::getInstance();
      int result = item.second.trader_api->ReqSettlementInfoConfirm(&requestMsg, request_id++);
      INFO_LOG("ReqSettlementInfoConfirm send result is [%d]", result);
      globalSem.WaitSemBySemName(GlobalSem::kLoginLogout);

      ofstream out(confirmFile);
      if (out.is_open()) {
        out << item.second.trader_api->GetTradingDay();
      }
    }
  }
  return ret;
}

bool CtpSender::Release() {
  INFO_LOG("Is going to release trader_api.");
  for (auto &item : kCtpTraderInfoMap) {
    item.second.trader_api->Release();

    if (item.second.trader_spi) {
      delete item.second.trader_spi;
      item.second.trader_spi = NULL;
    }
  }
}

bool CtpSender::BuildOrder(utils::OrderContent &content, CThostFtdcInputOrderField &order) {
  strcpy(order.OrderRef, content.orderRef.c_str());
  strcpy(order.InstrumentID, content.instrumentID.c_str());
  strcpy(order.ExchangeID, content.exchangeId.c_str());
  order.LimitPrice = content.limit_price;
  order.VolumeTotalOriginal = content.total_volume;

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

  if (content.orderType == strategy_trader::limit_LIMIT) {
    order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    order.TimeCondition = THOST_FTDC_TC_GFD;
    order.VolumeCondition = THOST_FTDC_VC_AV;
  } else if (content.orderType == strategy_trader::Limit_FAK) {
    order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    order.TimeCondition = THOST_FTDC_TC_IOC;
    order.VolumeCondition = THOST_FTDC_VC_AV;
  } else if (content.orderType == strategy_trader::limit_FOK) {
    order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    order.TimeCondition = THOST_FTDC_TC_IOC;
    order.VolumeCondition = THOST_FTDC_VC_CV;
  } else if (content.orderType == strategy_trader::AnyPrice_Fok) {
    order.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
    order.TimeCondition = THOST_FTDC_TC_GFD;
    order.VolumeCondition = THOST_FTDC_VC_AV;
  } else if (content.orderType == strategy_trader::AnyPrice_Fak) {
    order.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
    order.TimeCondition = THOST_FTDC_TC_IOC;
    order.VolumeCondition = THOST_FTDC_VC_AV;
  }
}

bool CtpSender::LossConnection() {
  bool ret = false;
  for (auto &item : kCtpTraderInfoMap) {
    if (item.second.trader_spi != nullptr && item.second.trader_spi->front_disconnected == true) {
      ret = true;
    }
    break;
  }
  return ret;
}
