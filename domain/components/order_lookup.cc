/*
 * order.cpp
 *
 *      Author: Administrator
 */

#include "trader/domain/components/order_lookup.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include "common/extern/json/json.h"
#include "common/extern/log/log.h"

#include "common/self/file_util.h"
#include "common/self/utils.h"
#include "trader/domain/components/fd_manage.h"
#include "trader/interface/trader_event.h"

OrderLookup::OrderLookup() {
  init_database_flag = false;
  InitDatabase();
  PrepareSqlSentence();
  RestoreFromSqlite3();
}

OrderLookup::~OrderLookup() {
  sqlite3_finalize(insert_lookup_);
  sqlite3_finalize(update_lookup_);
  sqlite3_finalize(delete_lookup_);
}

void OrderLookup::PrepareSqlSentence() {
  const char *sql = "insert into order_lookup(order_index, user_id, order_ref) VALUES(?, ?, ?);";
  if (sqlite3_prepare_v2(FdManage::GetInstance().trader_conn, sql, strlen(sql), &insert_lookup_, 0) != SQLITE_OK) {
    ERROR_LOG("prepare sql sentence error.");
    sqlite3_close(FdManage::GetInstance().trader_conn);
  }
  sql = "update order_lookup set user_id = ?, order_ref = ? where order_index = ?;";
  if (sqlite3_prepare_v2(FdManage::GetInstance().trader_conn, sql, strlen(sql), &update_lookup_, 0) != SQLITE_OK) {
    ERROR_LOG("prepare sql sentence error.");
    sqlite3_close(FdManage::GetInstance().trader_conn);
  }
  sql = "delete from order_lookup where order_index = ?;";
  if (sqlite3_prepare_v2(FdManage::GetInstance().trader_conn, sql, strlen(sql), &delete_lookup_, 0) != SQLITE_OK) {
    ERROR_LOG("prepare sql sentence error.");
    sqlite3_close(FdManage::GetInstance().trader_conn);
  }
}

void OrderLookup::RestoreFromSqlite3() {
  int nrow = 0, ncolumn = 0;
  char **result;
  char *error_msg = nullptr;

  const char *sql = "select * from order_lookup;";
  if (sqlite3_get_table(FdManage::GetInstance().trader_conn, sql, &result, &nrow, &ncolumn, &error_msg) != SQLITE_OK) {
    ERROR_LOG("Sql error %s.", error_msg);
    sqlite3_free(error_msg);
    sqlite3_close(FdManage::GetInstance().trader_conn);
  }
  for (int i = 1; i <= nrow; i++) {
    order_index_map_[result[i * ncolumn + 0]] = std::make_shared<OrderPara>(result[i * ncolumn + 1], result[i * ncolumn + 2]);
  }
  sqlite3_free_table(result);
}

void OrderLookup::InitDatabase() {
  if (init_database_flag == false) {
    char *error_msg = nullptr;
    const char *sql = "create table if not exists order_lookup(order_index TEXT PRIMARY KEY, user_id TEXT, order_ref TEXT);";
    if (sqlite3_exec(FdManage::GetInstance().trader_conn, sql, NULL, NULL, &error_msg) != SQLITE_OK) {
      ERROR_LOG("Sql error %s.", error_msg);
      sqlite3_free(error_msg);
      sqlite3_close(FdManage::GetInstance().trader_conn);
    }
    init_database_flag = true;
  }
}

OrderLookup::OrderPara *OrderLookup::GetOrderPara(const std::string &order_index) {
  auto iter = order_index_map_.find(order_index);
  if (iter != order_index_map_.end()) {
    return iter->second.get();
  } else {
    return nullptr;
  }
}

bool OrderLookup::DelOrderIndex(const std::string &order_index) {
  auto iter = order_index_map_.find(order_index);
  if (iter != order_index_map_.end()) {
    INFO_LOG("total order index map size [%d]", (int)order_index_map_.size());
    order_index_map_.erase(iter);
    INFO_LOG("del order index[%s] ok", order_index.c_str());
    INFO_LOG("total order index map size [%d]", (int)order_index_map_.size());
    sqlite3_reset(delete_lookup_);
    sqlite3_bind_text(delete_lookup_, 1, order_index.c_str(), order_index.size(), 0);
    if (sqlite3_step(delete_lookup_) != SQLITE_DONE) {
      ERROR_LOG("do sql sentence error.");
      sqlite3_close(FdManage::GetInstance().trader_conn);
    }
  } else {
    ERROR_LOG("not find order index: %s", order_index.c_str());
  }

  return true;
}

bool OrderLookup::BuildOrderIndex(const std::string &order_index, const std::shared_ptr<OrderPara> &para) {
  auto pos = order_index_map_.find(order_index);
  if (pos == order_index_map_.end()) {
    order_index_map_[order_index] = para;
    sqlite3_reset(insert_lookup_);
    sqlite3_bind_text(insert_lookup_, 1, order_index.c_str(), order_index.size(), 0);
    sqlite3_bind_text(insert_lookup_, 2, para->user_id.c_str(), para->user_id.size(), 0);
    sqlite3_bind_text(insert_lookup_, 3, para->order_ref.c_str(), para->order_ref.size(), 0);
    if (sqlite3_step(insert_lookup_) != SQLITE_DONE) {
      ERROR_LOG("do sql sentence error.");
      sqlite3_close(FdManage::GetInstance().trader_conn);
    }
  } else {
    pos->second->user_id = para->user_id;
    pos->second->order_ref = para->order_ref;
    sqlite3_reset(update_lookup_);
    sqlite3_bind_text(update_lookup_, 1, para->user_id.c_str(), para->user_id.size(), 0);
    sqlite3_bind_text(update_lookup_, 2, para->order_ref.c_str(), para->order_ref.size(), 0);
    sqlite3_bind_text(update_lookup_, 3, order_index.c_str(), order_index.size(), 0);
    if (sqlite3_step(update_lookup_) != SQLITE_DONE) {
      ERROR_LOG("do sql sentence error.");
      sqlite3_close(FdManage::GetInstance().trader_conn);
    }
  }

  return true;
}
