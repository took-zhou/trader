#include "trader/domain/components/order_lookup.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include "common/extern/log/log.h"
#include "trader/domain/components/fd_manage.h"

OrderLookup::OrderLookup() {
  InitDatabase();
  PrepareSqlSentence();
  RestoreFromSqlite3();
}

OrderLookup::~OrderLookup() {
  sqlite3_finalize(insert_lookup_);
  sqlite3_finalize(update_order_ref_);
  sqlite3_finalize(update_position_);
  sqlite3_finalize(delete_lookup_);
}

void OrderLookup::PrepareSqlSentence() {
  const char *sql = "insert into order_lookup(order_index, user_id, order_ref, yesterday_volume, today_volume) VALUES(?, ?, ?, 0,0);";
  if (sqlite3_prepare_v2(FdManage::GetInstance().GetTraderConn(), sql, strlen(sql), &insert_lookup_, 0) != SQLITE_OK) {
    ERROR_LOG("prepare sql sentence error.");
    sqlite3_close(FdManage::GetInstance().GetTraderConn());
  }
  sql = "update order_lookup set order_ref = ? where order_index = ? and user_id = ?;";
  if (sqlite3_prepare_v2(FdManage::GetInstance().GetTraderConn(), sql, strlen(sql), &update_order_ref_, 0) != SQLITE_OK) {
    ERROR_LOG("prepare sql sentence error.");
    sqlite3_close(FdManage::GetInstance().GetTraderConn());
  }
  sql = "update order_lookup set yesterday_volume = ?, today_volume = ? where order_index = ? and user_id = ?;";
  if (sqlite3_prepare_v2(FdManage::GetInstance().GetTraderConn(), sql, strlen(sql), &update_position_, 0) != SQLITE_OK) {
    ERROR_LOG("prepare sql sentence error.");
    sqlite3_close(FdManage::GetInstance().GetTraderConn());
  }
  sql = "delete from order_lookup where order_index = ?;";
  if (sqlite3_prepare_v2(FdManage::GetInstance().GetTraderConn(), sql, strlen(sql), &delete_lookup_, 0) != SQLITE_OK) {
    ERROR_LOG("prepare sql sentence error.");
    sqlite3_close(FdManage::GetInstance().GetTraderConn());
  }
}

void OrderLookup::RestoreFromSqlite3() {
  int nrow = 0, ncolumn = 0;
  char **result;
  char *error_msg = nullptr;

  const char *sql = "select * from order_lookup;";
  if (sqlite3_get_table(FdManage::GetInstance().GetTraderConn(), sql, &result, &nrow, &ncolumn, &error_msg) != SQLITE_OK) {
    ERROR_LOG("sql error%s.", error_msg);
    sqlite3_free(error_msg);
    sqlite3_close(FdManage::GetInstance().GetTraderConn());
  }
  for (int i = 1; i <= nrow; i++) {
    OrderPara order_para = {result[i * ncolumn + 2], static_cast<uint32_t>(stoi(result[i * ncolumn + 3])),
                            static_cast<uint32_t>(stoi(result[i * ncolumn + 4]))};
    order_index_map_[result[i * ncolumn + 0]][result[i * ncolumn + 1]] = std::make_unique<OrderPara>(order_para);
  }
  sqlite3_free_table(result);
}

void OrderLookup::InitDatabase() {
  char *error_msg = nullptr;
  const char *sql =
      "create table if not exists order_lookup(order_index TEXT, user_id TEXT, order_ref TEXT, yesterday_volume INT, "
      "today_volume INT);";
  if (sqlite3_exec(FdManage::GetInstance().GetTraderConn(), sql, NULL, NULL, &error_msg) != SQLITE_OK) {
    ERROR_LOG("sql error%s.", error_msg);
    sqlite3_free(error_msg);
    sqlite3_close(FdManage::GetInstance().GetTraderConn());
  }
}

bool OrderLookup::DelOrderIndex(const std::string &ins, const std::string &index) {
  std::string temp_key;
  temp_key += ins;
  temp_key += ".";
  temp_key += index;

  auto iter = order_index_map_.find(temp_key);
  if (iter != order_index_map_.end()) {
    INFO_LOG("total order index map size [%d]", (int)order_index_map_.size());
    order_index_map_.erase(iter);
    INFO_LOG("del order index[%s] ok", temp_key.c_str());
    INFO_LOG("total order index map size [%d]", (int)order_index_map_.size());
    sqlite3_reset(delete_lookup_);
    sqlite3_bind_text(delete_lookup_, 1, temp_key.c_str(), temp_key.size(), 0);
    if (sqlite3_step(delete_lookup_) != SQLITE_DONE) {
      ERROR_LOG("do sql sentence error.");
      sqlite3_close(FdManage::GetInstance().GetTraderConn());
    }
  } else {
    ERROR_LOG("not find order index: %s", temp_key.c_str());
  }

  return true;
}

bool OrderLookup::UpdateOrderIndex(const std::string &ins, const std::string &index, const std::string &user_id,
                                   const std::string &order_ref) {
  std::string temp_key;
  temp_key += ins;
  temp_key += ".";
  temp_key += index;

  if (order_index_map_.find(temp_key) != order_index_map_.end() &&
      order_index_map_[temp_key].find(user_id) != order_index_map_[temp_key].end()) {
    sqlite3_reset(update_order_ref_);
    sqlite3_bind_text(update_order_ref_, 1, order_ref.c_str(), order_ref.size(), 0);
    sqlite3_bind_text(update_order_ref_, 2, temp_key.c_str(), temp_key.size(), 0);
    sqlite3_bind_text(update_order_ref_, 3, user_id.c_str(), user_id.size(), 0);
    if (sqlite3_step(update_order_ref_) != SQLITE_DONE) {
      ERROR_LOG("do sql sentence error.");
      sqlite3_close(FdManage::GetInstance().GetTraderConn());
    }
    order_index_map_[temp_key][user_id]->SetOrderRef(order_ref);
  } else {
    sqlite3_reset(insert_lookup_);
    sqlite3_bind_text(insert_lookup_, 1, temp_key.c_str(), temp_key.size(), 0);
    sqlite3_bind_text(insert_lookup_, 2, user_id.c_str(), user_id.size(), 0);
    sqlite3_bind_text(insert_lookup_, 3, order_ref.c_str(), order_ref.size(), 0);
    if (sqlite3_step(insert_lookup_) != SQLITE_DONE) {
      ERROR_LOG("do sql sentence error.");
      sqlite3_close(FdManage::GetInstance().GetTraderConn());
    }
    OrderPara order_para = {order_ref, 0, 0};
    order_index_map_[temp_key][user_id] = std::make_unique<OrderPara>(order_para);
  }

  return true;
}

void OrderLookup::UpdateOpenInterest(const std::string &ins, const std::string &index, const std::string &user_id, int32_t yesterday_volume,
                                     int32_t today_volume) {
  std::string temp_key;
  temp_key += ins;
  temp_key += ".";
  temp_key += index;

  if (order_index_map_.find(temp_key) != order_index_map_.end() &&
      order_index_map_[temp_key].find(user_id) != order_index_map_[temp_key].end()) {
    order_index_map_[temp_key][user_id]->AddYesterdayVolume(yesterday_volume);
    order_index_map_[temp_key][user_id]->AddTodayVolume(today_volume);
    sqlite3_reset(update_position_);
    sqlite3_bind_int(update_position_, 1, order_index_map_[temp_key][user_id]->GetYesterdayVolume());
    sqlite3_bind_int(update_position_, 2, order_index_map_[temp_key][user_id]->GetTodayVolume());
    sqlite3_bind_text(update_position_, 3, temp_key.c_str(), temp_key.size(), 0);
    sqlite3_bind_text(update_position_, 4, user_id.c_str(), user_id.size(), 0);
    if (sqlite3_step(update_position_) != SQLITE_DONE) {
      ERROR_LOG("do sql sentence error.");
      sqlite3_close(FdManage::GetInstance().GetTraderConn());
    }
  }
}

void OrderLookup::HandleTraderClose() {
  for (auto &item : order_index_map_) {
    for (auto &sub_item : item.second) {
      sub_item.second->MoveTodayVolumeToYesterdayVolume();
      sqlite3_reset(update_position_);
      sqlite3_bind_int(update_position_, 1, sub_item.second->GetYesterdayVolume());
      sqlite3_bind_int(update_position_, 2, sub_item.second->GetTodayVolume());
      sqlite3_bind_text(update_position_, 3, item.first.c_str(), item.first.size(), 0);
      sqlite3_bind_text(update_position_, 4, sub_item.first.c_str(), sub_item.first.size(), 0);
      if (sqlite3_step(update_position_) != SQLITE_DONE) {
        ERROR_LOG("do sql sentence error.");
        sqlite3_close(FdManage::GetInstance().GetTraderConn());
      }
    }
  }
}

std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<OrderPara>>> &OrderLookup::GetOrderIndexMap() {
  return order_index_map_;
}
