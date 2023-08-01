#include "trader/domain/components/order_allocate.h"
#include <memory>
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/utils.h"
#include "trader/domain/trader_service.h"

OrderAllocate::OrderAllocate() {
  auto &json_cfg = utils::JsonConfig::GetInstance();
  auto assign_mode = json_cfg.GetConfig("trader", "AccountAssignMode");
  if (assign_mode == "cycle") {
    assign_mode_ = kCycle;
  } else if (assign_mode == "share") {
    assign_mode_ = kShare;
  }
}

void OrderAllocate::UpdateOrderList(utils::OrderContent &content) {
  auto &trader_ser = TraderService::GetInstance();
  bool ret = true;

  order_list.clear();

  if (assign_mode_ == kCycle) {
    ret = BuildCycleOrderContent(content);
  } else if (assign_mode_ == kShare) {
    ret = BuildShareOrderContent(content);
  }

  for (auto &item : order_list) {
    trader_ser.ROLE(OrderManage).BuildOrder(item->order_ref, item);
    trader_ser.ROLE(OrderLookup).UpdateOrderIndex(item->instrument_id, item->index, item->user_id, item->order_ref);
    INFO_LOG("%s %s %s %d %d", item->instrument_id.c_str(), item->index.c_str(), item->order_ref.c_str(), item->comboffset,
             item->total_volume);
  }

  if (ret == false) {
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
  for (static auto pos = account_assign.account_info_map.begin();; pos++) {
    if (pos == account_assign.account_info_map.end()) {
      pos = account_assign.account_info_map.begin();
    }
    if (pos->second->available >= minimum_account_available_ &&
        pos->second->open_blacklist.find(temp_key) == pos->second->open_blacklist.end()) {
      content.session_id = pos->second->session_id;
      content.order_ref = to_string(pos->second->order_ref++);
      content.user_id = pos->first;
      order_list.push_back(std::make_shared<utils::OrderContent>(content));
      break;
    }
    if (find_count++ > account_assign.account_info_map.size()) {
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
  for (auto &item : account_assign.account_info_map) {
    if (item.second->open_blacklist.find(temp_key) == item.second->open_blacklist.end()) {
      total_money += item.second->available;
    }
  }
  double sub_volume = 0;
  for (auto &item : account_assign.account_info_map) {
    if (item.second->open_blacklist.find(temp_key) == item.second->open_blacklist.end()) {
      uint32_t volume = uint32_t(item.second->available / total_money * total_volume + 0.5000001 + sub_volume);
      sub_volume += (item.second->available / total_money * total_volume - volume);
      if (volume > 0) {
        content.session_id = item.second->session_id;
        content.order_ref = to_string(item.second->order_ref++);
        content.user_id = item.first;
        content.total_volume = volume;
        order_list.push_back(std::make_shared<utils::OrderContent>(content));
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
  uint32_t total_volume = content.total_volume;
  uint32_t send_volume = 0;

  auto pos = order_lookup.order_index_map.find(temp_key);
  if (pos != order_lookup.order_index_map.end()) {
    for (auto &sub_item : pos->second) {
      if (sub_item.second->yesterday_volume > 0 &&
          account_assign.account_info_map.find(sub_item.first) != account_assign.account_info_map.end()) {
        content.session_id = account_assign.account_info_map[sub_item.first]->session_id;
        content.order_ref = to_string(account_assign.account_info_map[sub_item.first]->order_ref++);
        content.user_id = sub_item.first;
        content.comboffset = strategy_trader::CombOffsetType::CLOSE_YESTERDAY;
        if (sub_item.second->yesterday_volume + send_volume <= total_volume) {
          content.total_volume = sub_item.second->yesterday_volume;
          send_volume += sub_item.second->yesterday_volume;
          order_list.push_back(std::make_shared<utils::OrderContent>(content));
          continue;
        } else {
          content.total_volume = total_volume - send_volume;
          send_volume = total_volume;
          order_list.push_back(std::make_shared<utils::OrderContent>(content));
          break;
        }
      }

      if (sub_item.second->today_volume > 0 &&
          account_assign.account_info_map.find(sub_item.first) != account_assign.account_info_map.end()) {
        content.session_id = account_assign.account_info_map[sub_item.first]->session_id;
        content.order_ref = to_string(account_assign.account_info_map[sub_item.first]->order_ref++);
        content.user_id = sub_item.first;
        content.comboffset = strategy_trader::CombOffsetType::CLOSE_TODAY;
        if (sub_item.second->today_volume + send_volume <= total_volume) {
          content.total_volume = sub_item.second->today_volume;
          send_volume += sub_item.second->today_volume;
          order_list.push_back(std::make_shared<utils::OrderContent>(content));
          continue;
        } else {
          content.total_volume = total_volume - send_volume;
          send_volume = total_volume;
          order_list.push_back(std::make_shared<utils::OrderContent>(content));
          break;
        }
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
