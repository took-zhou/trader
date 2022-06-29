/*
 * order.cpp
 *
 *      Author: Administrator
 */

#include "trader/domain/components/order.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include "common/extern/json/json.h"
#include "common/extern/log/log.h"
#include "common/self/basetype.h"
#include "common/self/fileUtil.h"
#include "common/self/utils.h"
#include "trader/infra/define.h"
#include "trader/interface/traderEvent.h"

using json = nlohmann::json;

CThostFtdcInputOrderField OrderManage::defaultOrderField;

OrderManage::OrderManage() {
  auto &jsonCfg = utils::JsonConfig::getInstance();
  const std::string username = jsonCfg.getConfig("common", "user").get<std::string>();
  const std::string brokerId = jsonCfg.getDeepConfig("users", username, "BrokerID").get<std::string>();

  strcpy(defaultOrderField.BrokerID, brokerId.c_str());

  std::string investorId = jsonCfg.getDeepConfig("users", username, "InvestorID").get<std::string>();
  strcpy(defaultOrderField.InvestorID, investorId.c_str());

  std::string userId = jsonCfg.getDeepConfig("users", username, "UserID").get<std::string>();
  strcpy(defaultOrderField.UserID, userId.c_str());

  defaultOrderField.MinVolume = 1;
  defaultOrderField.ContingentCondition = THOST_FTDC_CC_Immediately;
  defaultOrderField.StopPrice = 0;
  defaultOrderField.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
  defaultOrderField.IsAutoSuspend = 0;
  defaultOrderField.UserForceClose = 0;
  defaultOrderField.IsSwapOrder = 0;
  defaultOrderField.CombHedgeFlag[0] = '1';

  string mac;
  if (!utils::get_local_mac(mac)) {
    ERROR_LOG("get_local_mac error!");
  }
  strcpy(defaultOrderField.MacAddress, mac.c_str());

  string Ip;
  string hostName;
  if (!utils::GetHostInfo(hostName, Ip)) {
    ERROR_LOG("GetHostInfo error!");
  }
  strcpy(defaultOrderField.IPAddress, Ip.c_str());
}

bool OrderManage::addOrder(const std::string &orderKey) {
  if (orderMaps.find(orderKey) != orderMaps.end()) {
    return false;
  }
  OrderContent *tmpOrder = new OrderContent();

  orderMaps.insert(std::pair<std::string, OrderContent *>(orderKey, tmpOrder));
  INFO_LOG("add new order[%s] ok", orderKey.c_str());
  return true;
}

void OrderManage::delOrder(const std::string &orderKey) {
  auto it = orderMaps.find(orderKey);
  if (it != orderMaps.end()) {
    INFO_LOG("total ordermap size [%d]", (int)orderMaps.size());
    delete static_cast<OrderContent *>(it->second);
    it->second = nullptr;
    orderMaps.erase(it);
    INFO_LOG("del order[%s] ok", orderKey.c_str());
    INFO_LOG("total ordermap size [%d]", (int)orderMaps.size());

  } else {
    ERROR_LOG("not find order in local ref is[%s]", orderKey.c_str());
  }
}

int OrderManage::requestIdBuildAlg() {
  if (requestID == S32_MAX) {
    requestID = 1;
    return requestID;
  }
  requestID++;
  return requestID;
}

bool OrderManage::buildOrder(const std::string &orderKey, const strategy_trader::OrderInsertReq &orderInsertReq,
                             CThostFtdcInputOrderField &order) {
  auto orderIndication = orderInsertReq.order();
  auto &jsonCfg = utils::JsonConfig::getInstance();

  memcpy(&order, &defaultOrderField, sizeof(CThostFtdcInputOrderField));

  strcpy(order.OrderRef, orderKey.c_str());
  std::string instrumentID = orderIndication.instrument();
  strcpy(order.InstrumentID, instrumentID.c_str());
  std::string exchangeId = orderIndication.exchangeid();
  strcpy(order.ExchangeID, exchangeId.c_str());
  auto direction = orderIndication.direction();
  order.Direction = direction == strategy_trader::Direction::BUY ? BUY_ : SELL_;
  auto &limitPriceStr = orderIndication.limitprice();
  auto limitprice = atof(limitPriceStr.c_str());
  order.LimitPrice = limitprice;
  order.VolumeTotalOriginal = orderIndication.volume_total_original();

  auto &openClose = orderIndication.comb_offset_flag();
  if (openClose == "open") {
    order.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
  } else if (openClose == "close") {
    order.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
  } else if (openClose == "close_yesterday") {
    order.CombOffsetFlag[0] = THOST_FTDC_OF_CloseYesterday;
  } else if (openClose == "close_today") {
    order.CombOffsetFlag[0] = THOST_FTDC_OFEN_CloseToday;
  } else {
    ERROR_LOG("error comb_offset_flag [%s]", openClose.c_str());
    return false;
  }

  auto orderType = orderIndication.order_type();
  if (orderType == strategy_trader::OrderType::limit_LIMIT) {
    order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    order.TimeCondition = THOST_FTDC_TC_GFD;
    order.VolumeCondition = THOST_FTDC_VC_AV;
  } else if (orderType == strategy_trader::OrderType::Limit_FAK) {
    order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    order.TimeCondition = THOST_FTDC_TC_IOC;
    order.VolumeCondition = THOST_FTDC_VC_AV;
  } else if (orderType == strategy_trader::OrderType::limit_FOK) {
    order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    order.TimeCondition = THOST_FTDC_TC_IOC;
    order.VolumeCondition = THOST_FTDC_VC_CV;
  } else if (orderType == strategy_trader::OrderType::AnyPrice_Fok) {
    order.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
    order.TimeCondition = THOST_FTDC_TC_GFD;
    order.VolumeCondition = THOST_FTDC_VC_AV;
  } else if (orderType == strategy_trader::OrderType::AnyPrice_Fak) {
    order.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
    order.TimeCondition = THOST_FTDC_TC_IOC;
    order.VolumeCondition = THOST_FTDC_VC_AV;
  }

  order.RequestID = requestIdBuildAlg();
  return true;
}
