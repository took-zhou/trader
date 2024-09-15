#include "trader/domain/components/order_allocate.h"
#include <algorithm>
#include <memory>
#include <random>
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/utils.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"

OrderAllocate::OrderAllocate() {}

void OrderAllocate::UpdateOrderList(utils::OrderContent &content) {
  auto &trader_ser = TraderService::GetInstance();
  bool ret = true;

  order_list_.clear();
  ret = BuildOrderContent(content);

  for (auto &item : order_list_) {
    trader_ser.ROLE(OrderManage).BuildOrder(item->order_ref, item);
    trader_ser.ROLE(OrderLookup).UpdateOrderIndex(item->instrument_id, item->index, item->group_id, item->user_id, item->order_ref);
    INFO_LOG("%s %s %s %d %d", item->instrument_id.c_str(), item->index.c_str(), item->order_ref.c_str(), item->comboffset,
             item->once_volume);
  }

  if (!ret) {
    ERROR_LOG("ins: %s, index: %s, allocate order fail, volume: %d", content.instrument_id.c_str(), content.index.c_str(),
              content.once_volume);
  }
}

bool OrderAllocate::BuildOrderContent(utils::OrderContent &content) {
  bool ret = true;

  if (content.comboffset == strategy_trader::CombOffsetType::OPEN) {
    ret = OpenOrder(content);
  } else {
    ret = CloseOrder(content);
  }

  return ret;
}

bool OrderAllocate::OpenOrder(utils::OrderContent &content) {
  auto &trader_ser = TraderService::GetInstance();
  auto &json_cfg = utils::JsonConfig::GetInstance();
  auto assign_mode = json_cfg.GetConfig("trader", "AccountAssignMode").get<std::string>();
  auto &group_assign = trader_ser.ROLE(GroupAssign);
  auto &order_lookup = trader_ser.ROLE(OrderLookup);

  bool ret = true;
  std::string temp_key;
  temp_key += content.instrument_id;
  temp_key += ".";
  temp_key += content.index;

  uint32_t once_volume = content.once_volume;
  auto &order_index_map = order_lookup.GetOrderIndexMap();
  for (auto &item : group_assign.GetAccoutGroupMap()) {
    uint32_t exist_volume = 0;
    for (auto &account : item.second) {
      std::string temp_user;
      temp_user += item.first;
      temp_user += ".";
      temp_user += account;

      if (order_index_map.find(temp_key) != order_index_map.end() &&
          order_index_map[temp_key].find(temp_user) != order_index_map[temp_key].end()) {
        auto &order_index = order_index_map[temp_key][temp_user];
        exist_volume += order_index->GetTodayVolume();
        exist_volume += order_index->GetYesterdayVolume();
      }
    }

    if (content.hold_volume == exist_volume) {
      continue;
    }

    if (content.hold_volume - exist_volume < once_volume) {
      content.once_volume = content.hold_volume - exist_volume;
    } else {
      content.once_volume = once_volume;
    }
    once_volume -= content.once_volume;

    if (assign_mode == "cycle") {
      CycleOpenOrder(item.first, content);
    } else if (assign_mode == "share") {
      ShareOpenOrder(item.first, content);
    }

    if (once_volume <= 0) {
      break;
    }
  }

  return ret;
}

bool OrderAllocate::CycleOpenOrder(const std::string &group_id, utils::OrderContent &content) {
  auto &trader_ser = TraderService::GetInstance();
  auto &account_assign = trader_ser.ROLE(AccountAssign);
  auto &account_info_map = account_assign.GetAccountInfoMap();
  auto &group_assign = trader_ser.ROLE(GroupAssign);
  auto &group_info_map = group_assign.GetAccoutGroupMap();
  bool ret = true;

  std::string temp_key;
  temp_key += content.instrument_id;
  temp_key += ".";
  temp_key += content.index;

  std::vector<std::string> account_list(group_info_map[group_id].begin(), group_info_map[group_id].end());
  std::random_device rmd;
  std::mt19937 gen(rmd());
  std::shuffle(account_list.begin(), account_list.end(), gen);

  for (auto &item : account_list) {
    auto &account_info = account_info_map[item];
    if (account_info->GetAvailable() >= minimum_account_available_ && account_info->NotOnBlacklist(temp_key)) {
      content.session_id = account_info->GetSessionId();
      content.order_ref = to_string(account_info->IncOrderRef());
      content.user_id = item;
      content.group_id = group_id;
      order_list_.push_back(std::make_shared<utils::OrderContent>(content));
      break;
    }
  }

  return ret;
}

bool OrderAllocate::ShareOpenOrder(const std::string &group_id, utils::OrderContent &content) {
  auto &trader_ser = TraderService::GetInstance();
  auto &account_assign = trader_ser.ROLE(AccountAssign);
  auto &account_info_map = account_assign.GetAccountInfoMap();
  auto &group_assign = trader_ser.ROLE(GroupAssign);
  auto &group_info_map = group_assign.GetAccoutGroupMap();

  std::string temp_key;
  temp_key += content.instrument_id;
  temp_key += ".";
  temp_key += content.index;

  uint32_t once_volume = content.once_volume;
  double total_money = 0;
  for (auto &item : group_info_map[group_id]) {
    auto &account_info = account_info_map[item];
    if (account_info->NotOnBlacklist(temp_key)) {
      total_money += account_info->GetAvailable();
    }
  }
  double sub_volume = 0;
  for (auto &item : group_info_map[group_id]) {
    auto &account_info = account_info_map[item];
    if (account_info->NotOnBlacklist(temp_key)) {
      auto volume = static_cast<uint32_t>(account_info->GetAvailable() / total_money * once_volume + 0.5000001 + sub_volume);
      sub_volume += (account_info->GetAvailable() / total_money * once_volume - volume);
      if (volume > 0) {
        content.session_id = account_info->GetSessionId();
        content.order_ref = to_string(account_info->IncOrderRef());
        content.user_id = item;
        content.group_id = group_id;
        content.once_volume = volume;
        order_list_.push_back(std::make_shared<utils::OrderContent>(content));
      }
    }
  }

  return true;
}

bool OrderAllocate::CloseOrder(utils::OrderContent &content) {
  auto &trader_ser = TraderService::GetInstance();
  auto &order_lookup = trader_ser.ROLE(OrderLookup);
  auto &order_index_map = order_lookup.GetOrderIndexMap();
  auto &group_assign = trader_ser.ROLE(GroupAssign);
  bool ret = true;

  std::string temp_key;
  temp_key += content.instrument_id;
  temp_key += ".";
  temp_key += content.index;

  uint32_t once_volume = content.once_volume;
  for (auto &item : group_assign.GetAccoutGroupMap()) {
    uint32_t exist_volume = 0;
    for (auto &account : item.second) {
      std::string temp_user;
      temp_user += item.first;
      temp_user += ".";
      temp_user += account;

      if (order_index_map.find(temp_key) != order_index_map.end() &&
          order_index_map[temp_key].find(temp_user) != order_index_map[temp_key].end()) {
        auto &order_index = order_index_map[temp_key][temp_user];
        exist_volume += order_index->GetTodayVolume();
        exist_volume += order_index->GetYesterdayVolume();
      }
    }

    if (exist_volume == 0) {
      continue;
    }

    if (once_volume > exist_volume) {
      content.once_volume = exist_volume;
    } else {
      content.once_volume = once_volume;
    }
    once_volume -= content.once_volume;

    SequenceCloseOrder(item.first, content);

    if (once_volume <= 0) {
      return ret;
    }
  }

  if (once_volume > 0 && order_list_.size() > 0) {
    strategy_trader::message message;
    auto *insert_rsp = message.mutable_order_insert_rsp();
    insert_rsp->set_instrument(content.instrument_id);
    insert_rsp->set_index(content.index);
    insert_rsp->set_result(strategy_trader::Result::failed);
    insert_rsp->set_reason(strategy_trader::FailedReason::No_Opened_Order);
    auto *rsp_info = insert_rsp->mutable_info();
    rsp_info->set_orderprice(content.limit_price);
    rsp_info->set_ordervolume(once_volume);

    utils::ItpMsg msg;
    message.SerializeToString(&msg.pb_msg);
    msg.session_name = "strategy_trader";
    msg.msg_name = "OrderInsertRsp";
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
  }

  return ret;
}

bool OrderAllocate::SequenceCloseOrder(const std::string &group_id, utils::OrderContent &content) {
  auto &trader_ser = TraderService::GetInstance();
  auto &order_lookup = trader_ser.ROLE(OrderLookup);
  auto &order_index_map = order_lookup.GetOrderIndexMap();
  auto &account_assign = trader_ser.ROLE(AccountAssign);
  auto &account_info_map = account_assign.GetAccountInfoMap();
  auto &group_assign = trader_ser.ROLE(GroupAssign);
  auto &group_info_map = group_assign.GetAccoutGroupMap();

  std::string temp_key;
  temp_key += content.instrument_id;
  temp_key += ".";
  temp_key += content.index;

  uint32_t once_volume = content.once_volume;
  uint32_t send_volume = 0;
  for (auto &account : group_info_map[group_id]) {
    std::string temp_user;
    temp_user += group_id;
    temp_user += ".";
    temp_user += account;

    if (order_index_map[temp_key].find(temp_user) == order_index_map[temp_key].end()) {
      continue;
    }

    auto &order_index = order_index_map[temp_key][temp_user];
    if (order_index->GetYesterdayVolume() > 0 && account_info_map.find(account) != account_info_map.end()) {
      content.session_id = account_info_map[account]->GetSessionId();
      content.order_ref = to_string(account_info_map[account]->IncOrderRef());
      content.user_id = account;
      content.group_id = group_id;
      content.comboffset = strategy_trader::CombOffsetType::CLOSE_YESTERDAY;
      if (order_index->GetYesterdayVolume() + send_volume < once_volume) {
        content.once_volume = order_index->GetYesterdayVolume();
        send_volume += order_index->GetYesterdayVolume();
        order_list_.push_back(std::make_shared<utils::OrderContent>(content));
        continue;
      } else {
        content.once_volume = once_volume - send_volume;
        send_volume = once_volume;
        order_list_.push_back(std::make_shared<utils::OrderContent>(content));
        break;
      }
    }

    if (order_index->GetTodayVolume() > 0 && account_info_map.find(account) != account_info_map.end()) {
      content.session_id = account_info_map[account]->GetSessionId();
      content.order_ref = to_string(account_info_map[account]->IncOrderRef());
      content.user_id = account;
      content.group_id = group_id;
      content.comboffset = strategy_trader::CombOffsetType::CLOSE_TODAY;
      if (order_index->GetTodayVolume() + send_volume < once_volume) {
        content.once_volume = order_index->GetTodayVolume();
        send_volume += order_index->GetTodayVolume();
        order_list_.push_back(std::make_shared<utils::OrderContent>(content));
        continue;
      } else {
        content.once_volume = once_volume - send_volume;
        send_volume = once_volume;
        order_list_.push_back(std::make_shared<utils::OrderContent>(content));
        break;
      }
    }
  }

  return true;
}

std::vector<std::shared_ptr<utils::OrderContent>> &OrderAllocate::GetOderList() { return order_list_; }
