#include "trader/domain/components/order_allocate.h"
#include <memory>
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/utils.h"
#include "trader/domain/trader_service.h"

OrderAllocate::OrderAllocate() {}

void OrderAllocate::UpdateOrderList(utils::OrderContent &content) {
  auto &trader_ser = TraderService::GetInstance();
  auto &json_cfg = utils::JsonConfig::GetInstance();
  bool ret = true;

  order_list_.clear();

  auto assign_mode = json_cfg.GetConfig("trader", "AccountAssignMode").get<std::string>();
  if (assign_mode == "cycle") {
    ret = BuildCycleOrderContent(content);
  } else if (assign_mode == "share") {
    ret = BuildShareOrderContent(content);
  }

  for (auto &item : order_list_) {
    trader_ser.ROLE(OrderManage).BuildOrder(item->order_ref, item);
    trader_ser.ROLE(OrderLookup).UpdateOrderIndex(item->instrument_id, item->index, item->user_id, item->order_ref);
    INFO_LOG("%s %s %s %d %d", item->instrument_id.c_str(), item->index.c_str(), item->order_ref.c_str(), item->comboffset,
             item->total_volume);
  }

  if (!ret) {
    ERROR_LOG("ins: %s, index: %s, allocate order fail, volume: %d", content.instrument_id.c_str(), content.index.c_str(),
              content.total_volume);
  }
}

bool OrderAllocate::BuildCycleOrderContent(utils::OrderContent &content) {
  bool ret = true;

  if (content.comboffset == strategy_trader::CombOffsetType::OPEN) {
    ret = CycleOpenOrder(content);
  } else {
    ret = CloseOrder(content);
  }

  return ret;
}

bool OrderAllocate::BuildShareOrderContent(utils::OrderContent &content) {
  bool ret = true;
  if (content.comboffset == strategy_trader::CombOffsetType::OPEN) {
    ret = ShareOpenOrder(content);
  } else {
    ret = CloseOrder(content);
  }

  return ret;
}

bool OrderAllocate::CycleOpenOrder(utils::OrderContent &content) {
  auto &trader_ser = TraderService::GetInstance();
  auto &account_assign = trader_ser.ROLE(AccountAssign);
  std::string temp_key;
  temp_key += content.instrument_id;
  temp_key += ".";
  temp_key += content.index;

  bool ret = true;
  uint32_t find_count = 0;
  auto &account_info_map = account_assign.GetAccountInfoMap();
  for (static auto pos = account_info_map.begin();; pos++) {
    if (pos == account_info_map.end()) {
      pos = account_info_map.begin();
    }
    if (pos->second->GetAvailable() >= minimum_account_available_ && pos->second->NotOnBlacklist(temp_key)) {
      content.session_id = pos->second->GetSessionId();
      content.order_ref = to_string(pos->second->IncOrderRef());
      content.user_id = pos->first;
      pos++;
      order_list_.push_back(std::make_shared<utils::OrderContent>(content));
      break;
    }
    if (find_count++ > account_info_map.size()) {
      ret = false;
      break;
    }
  }

  return ret;
}

bool OrderAllocate::ShareOpenOrder(utils::OrderContent &content) {
  auto &trader_ser = TraderService::GetInstance();
  auto &account_assign = trader_ser.ROLE(AccountAssign);
  std::string temp_key;
  temp_key += content.instrument_id;
  temp_key += ".";
  temp_key += content.index;

  uint32_t total_volume = content.total_volume;
  double total_money = 0;
  auto &account_info_map = account_assign.GetAccountInfoMap();
  for (auto &item : account_info_map) {
    if (item.second->NotOnBlacklist(temp_key)) {
      total_money += item.second->GetAvailable();
    }
  }
  double sub_volume = 0;
  for (auto &item : account_info_map) {
    if (item.second->NotOnBlacklist(temp_key)) {
      auto volume = static_cast<uint32_t>(item.second->GetAvailable() / total_money * total_volume + 0.5000001 + sub_volume);
      sub_volume += (item.second->GetAvailable() / total_money * total_volume - volume);
      if (volume > 0) {
        content.session_id = item.second->GetSessionId();
        content.order_ref = to_string(item.second->IncOrderRef());
        content.user_id = item.first;
        content.total_volume = volume;
        order_list_.push_back(std::make_shared<utils::OrderContent>(content));
      }
    }
  }

  return true;
}

bool OrderAllocate::CloseOrder(utils::OrderContent &content) {
  auto &trader_ser = TraderService::GetInstance();
  auto &account_assign = trader_ser.ROLE(AccountAssign);
  auto &order_lookup = trader_ser.ROLE(OrderLookup);
  std::string temp_key;
  temp_key += content.instrument_id;
  temp_key += ".";
  temp_key += content.index;

  bool ret = true;
  auto pos = order_lookup.GetOrderIndexMap().find(temp_key);
  if (pos == order_lookup.GetOrderIndexMap().end()) {
    ret = false;
    return ret;
  }

  uint32_t total_volume = content.total_volume;
  uint32_t send_volume = 0;
  auto &account_info_map = account_assign.GetAccountInfoMap();
  for (auto &sub_item : pos->second) {
    if (sub_item.second->GetYesterdayVolume() > 0 && account_info_map.find(sub_item.first) != account_info_map.end()) {
      content.session_id = account_info_map[sub_item.first]->GetSessionId();
      content.order_ref = to_string(account_info_map[sub_item.first]->IncOrderRef());
      content.user_id = sub_item.first;
      content.comboffset = strategy_trader::CombOffsetType::CLOSE_YESTERDAY;
      if (sub_item.second->GetYesterdayVolume() + send_volume < total_volume) {
        content.total_volume = sub_item.second->GetYesterdayVolume();
        send_volume += sub_item.second->GetYesterdayVolume();
        order_list_.push_back(std::make_shared<utils::OrderContent>(content));
        continue;
      } else {
        content.total_volume = total_volume - send_volume;
        send_volume = total_volume;
        order_list_.push_back(std::make_shared<utils::OrderContent>(content));
        break;
      }
    }

    if (sub_item.second->GetTodayVolume() > 0 && account_info_map.find(sub_item.first) != account_info_map.end()) {
      content.session_id = account_info_map[sub_item.first]->GetSessionId();
      content.order_ref = to_string(account_info_map[sub_item.first]->IncOrderRef());
      content.user_id = sub_item.first;
      content.comboffset = strategy_trader::CombOffsetType::CLOSE_TODAY;
      if (sub_item.second->GetTodayVolume() + send_volume < total_volume) {
        content.total_volume = sub_item.second->GetTodayVolume();
        send_volume += sub_item.second->GetTodayVolume();
        order_list_.push_back(std::make_shared<utils::OrderContent>(content));
        continue;
      } else {
        content.total_volume = total_volume - send_volume;
        send_volume = total_volume;
        order_list_.push_back(std::make_shared<utils::OrderContent>(content));
        break;
      }
    }
  }

  if (send_volume < total_volume && send_volume > 0) {
    strategy_trader::message message;
    auto *insert_rsp = message.mutable_order_insert_rsp();
    insert_rsp->set_instrument(content.instrument_id);
    insert_rsp->set_index(content.index);
    insert_rsp->set_result(strategy_trader::Result::failed);
    insert_rsp->set_reason(strategy_trader::FailedReason::No_Opened_Order);
    auto *rsp_info = insert_rsp->mutable_info();
    rsp_info->set_orderprice(content.limit_price);
    rsp_info->set_ordervolume(total_volume - send_volume);

    utils::ItpMsg msg;
    message.SerializeToString(&msg.pb_msg);
    msg.session_name = "strategy_trader";
    msg.msg_name = "OrderInsertRsp";
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
  }

  return ret;
}

std::vector<std::shared_ptr<utils::OrderContent>> &OrderAllocate::GetOderList() { return order_list_; }
