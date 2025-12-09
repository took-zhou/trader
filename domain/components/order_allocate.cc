#include "trader/domain/components/order_allocate.h"
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/utils.h"
#include "trader/domain/trader_service.h"

OrderAllocate::OrderAllocate() {}

void OrderAllocate::UpdateOrderList(utils::OrderContent &content) {
  auto &trader_ser = TraderService::GetInstance();
  bool ret = true;

  order_list_.clear();
  ret = BuildOrderContent(content);

  for (auto &item : order_list_) {
    auto &order_manage = trader_ser.ROLE(OrderManage);
    auto &order_lookup = trader_ser.ROLE(OrderLookup);
    order_manage.BuildOrder(item->order_ref, item);
    order_lookup.UpdateOrderIndex(item->instrument_id, item->index, item->group_id, item->user_id, item->comboffset, item->order_ref);
    INFO_LOG("%s %s %d %s %d", item->instrument_id.c_str(), item->index.c_str(), item->comboffset, item->order_ref.c_str(),
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

  uint32_t left_volume = content.once_volume;
  auto &order_index_map = order_lookup.GetOrderIndexMap();
  for (auto &item : group_assign.GetAccoutGroupMap()) {
    uint32_t exist_volume = 0;
    for (auto &account : item.second.GetAccountList()) {
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

    if (content.hold_volume - exist_volume < left_volume) {
      content.once_volume = content.hold_volume - exist_volume;
    } else {
      content.once_volume = left_volume;
    }

    if (assign_mode == "first") {
      left_volume -= FirstOpenOrder(item.first, content);
    } else if (assign_mode == "cycle") {
      left_volume -= CycleOpenOrder(item.first, content);
    } else if (assign_mode == "share") {
      left_volume -= ShareOpenOrder(item.first, content);
    }

    if (left_volume <= 0) {
      break;
    }
  }
  content.once_volume = left_volume;

  return ret;
}

uint32_t OrderAllocate::FirstOpenOrder(const std::string &group_id, utils::OrderContent &content) {
  auto &trader_ser = TraderService::GetInstance();
  auto &account_assign = trader_ser.ROLE(AccountAssign);
  auto &account_info_map = account_assign.GetAccountInfoMap();
  auto &group_assign = trader_ser.ROLE(GroupAssign);
  auto &group_info_map = group_assign.GetAccoutGroupMap();

  std::string temp_key;
  temp_key += content.instrument_id;
  temp_key += ".";
  temp_key += content.index;

  uint32_t openned_volume = 0;
  auto &account_list = group_info_map[group_id].GetAccountList();
  auto &account = account_list[0];

  auto &account_info = account_info_map[account];
  if (account_info->GetAvailable() >= minimum_account_available_ && account_info->NotOnBlacklist(temp_key)) {
    content.session_id = account_info->GetSessionId();
    content.order_ref = to_string(account_info->IncOrderRef());
    content.user_id = account;
    content.group_id = group_id;
    openned_volume += content.once_volume;
    order_list_.push_back(std::make_shared<utils::OrderContent>(content));
  }

  return openned_volume;
}

uint32_t OrderAllocate::CycleOpenOrder(const std::string &group_id, utils::OrderContent &content) {
  auto &trader_ser = TraderService::GetInstance();
  auto &account_assign = trader_ser.ROLE(AccountAssign);
  auto &account_info_map = account_assign.GetAccountInfoMap();
  auto &group_assign = trader_ser.ROLE(GroupAssign);
  auto &group_info_map = group_assign.GetAccoutGroupMap();

  std::string temp_key;
  temp_key += content.instrument_id;
  temp_key += ".";
  temp_key += content.index;

  uint32_t openned_volume = 0;
  auto &account_list = group_info_map[group_id].GetAccountList();
  auto account_index = group_info_map[group_id].GetNextAccountIndex();
  auto &account = account_list[account_index];

  auto &account_info = account_info_map[account];
  if (account_info->GetAvailable() >= minimum_account_available_ && account_info->NotOnBlacklist(temp_key)) {
    content.session_id = account_info->GetSessionId();
    content.order_ref = to_string(account_info->IncOrderRef());
    content.user_id = account;
    content.group_id = group_id;
    openned_volume += content.once_volume;
    order_list_.push_back(std::make_shared<utils::OrderContent>(content));
  }

  return openned_volume;
}

uint32_t OrderAllocate::ShareOpenOrder(const std::string &group_id, utils::OrderContent &content) {
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
  uint32_t openned_volume = 0;
  double total_money = 0;
  for (auto &item : group_info_map[group_id].GetAccountList()) {
    auto &account_info = account_info_map[item];
    if (account_info->GetAvailable() >= minimum_account_available_ && account_info->NotOnBlacklist(temp_key)) {
      total_money += account_info->GetAvailable();
    }
  }
  double sub_volume = 0;
  for (auto &item : group_info_map[group_id].GetAccountList()) {
    auto &account_info = account_info_map[item];
    if (account_info->GetAvailable() >= minimum_account_available_ && account_info->NotOnBlacklist(temp_key)) {
      auto volume = static_cast<uint32_t>(account_info->GetAvailable() / total_money * once_volume + 0.5000001 + sub_volume);
      sub_volume += (account_info->GetAvailable() / total_money * once_volume - volume);
      if (volume > 0) {
        content.session_id = account_info->GetSessionId();
        content.order_ref = to_string(account_info->IncOrderRef());
        content.user_id = item;
        content.group_id = group_id;
        content.once_volume = volume;
        openned_volume += volume;
        order_list_.push_back(std::make_shared<utils::OrderContent>(content));
      }
    }
  }

  return openned_volume;
}

bool OrderAllocate::CloseOrder(utils::OrderContent &content) {
  auto &trader_ser = TraderService::GetInstance();
  auto &order_lookup = trader_ser.ROLE(OrderLookup);
  auto &order_index_map = order_lookup.GetOrderIndexMap();
  auto &group_info_map = trader_ser.ROLE(GroupAssign).GetAccoutGroupMap();
  bool ret = true;

  std::string temp_key;
  temp_key += content.instrument_id;
  temp_key += ".";
  temp_key += content.index;

  uint32_t left_volume = content.once_volume;
  for (auto &item : group_info_map) {
    uint32_t exist_volume = 0;
    for (auto &account : item.second.GetAccountList()) {
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

    if (left_volume > exist_volume) {
      content.once_volume = exist_volume;
    } else {
      content.once_volume = left_volume;
    }

    left_volume -= content.once_volume;
    SequenceCloseOrder(item.first, content);

    if (left_volume <= 0) {
      break;
    }
  }
  content.once_volume = left_volume;

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
  for (auto &account : group_info_map[group_id].GetAccountList()) {
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
