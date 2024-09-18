#include "trader/domain/components/group_assign.h"
#include <unistd.h>
#include <cstring>
#include <memory>
#include <thread>
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/protobuf/ipc.pb.h"
#include "trader/domain/components/fd_manage.h"

GroupAssign::GroupAssign() {
  InitDatabase();
  PrepareSqlSentence();
  RestoreFromSqlite3();
}

GroupAssign::~GroupAssign() {
  sqlite3_finalize(insert_account_);
  sqlite3_finalize(delete_account_);
}

void GroupAssign::InitDatabase() {
  char *error_msg = nullptr;
  const char *sql = "create table if not exists group_info(group_id TEXT, account TEXT);";
  if (sqlite3_exec(FdManage::GetInstance().GetTraderConn(), sql, NULL, NULL, &error_msg) != SQLITE_OK) {
    ERROR_LOG("sql error%s.", error_msg);
    sqlite3_free(error_msg);
    sqlite3_close(FdManage::GetInstance().GetTraderConn());
  }
}

void GroupAssign::PrepareSqlSentence() {
  const char *sql = "delete from group_info where group_id = ?;";
  if (sqlite3_prepare_v2(FdManage::GetInstance().GetTraderConn(), sql, strlen(sql), &delete_account_, 0) != SQLITE_OK) {
    ERROR_LOG("prepare sql sentence error.");
    sqlite3_close(FdManage::GetInstance().GetTraderConn());
  }
  sql = "insert into group_info(group_id, account) select ?, ?;";
  if (sqlite3_prepare_v2(FdManage::GetInstance().GetTraderConn(), sql, strlen(sql), &insert_account_, 0) != SQLITE_OK) {
    ERROR_LOG("prepare sql sentence error.");
    sqlite3_close(FdManage::GetInstance().GetTraderConn());
  }
}

void GroupAssign::RestoreFromSqlite3() {
  int nrow = 0, ncolumn = 0;
  char **result;
  char *error_msg = nullptr;

  const char *sql = "select * from group_info;";
  if (sqlite3_get_table(FdManage::GetInstance().GetTraderConn(), sql, &result, &nrow, &ncolumn, &error_msg) != SQLITE_OK) {
    ERROR_LOG("sql error%s.", error_msg);
    sqlite3_free(error_msg);
    sqlite3_close(FdManage::GetInstance().GetTraderConn());
  }
  for (int i = 1; i <= nrow; i++) {
    account_group_map_[result[i * ncolumn]].insert(result[i * ncolumn + 1]);
  }
  sqlite3_free_table(result);
}

void GroupAssign::UpdateGroupInfo(const std::string &value, const std::set<std::string> &account_list) {
  if (account_list.size() == 0) {
    auto pos = account_group_map_.find(value);
    if (pos != account_group_map_.end()) {
      account_group_map_.erase(pos);
    }
  } else {
    account_group_map_[value] = account_list;
  }

  sqlite3_reset(delete_account_);
  sqlite3_bind_text(delete_account_, 1, value.c_str(), value.size(), 0);
  if (sqlite3_step(delete_account_) != SQLITE_DONE) {
    ERROR_LOG("do sql sentence error.");
    sqlite3_close(FdManage::GetInstance().GetTraderConn());
  }

  for (auto &item : account_list) {
    sqlite3_reset(insert_account_);
    sqlite3_bind_text(insert_account_, 1, value.c_str(), value.size(), 0);
    sqlite3_bind_text(insert_account_, 2, item.c_str(), item.size(), 0);
    if (sqlite3_step(insert_account_) != SQLITE_DONE) {
      ERROR_LOG("do sql sentence error.");
      sqlite3_close(FdManage::GetInstance().GetTraderConn());
    }
  }
}

void GroupAssign::HandleTraderOpen() {
  auto &json_cfg = utils::JsonConfig::GetInstance();
  auto users = json_cfg.GetConfig("trader", "User");
  std::set<std::string> user_id_set;
  for (auto &user : users) {
    const auto user_id = json_cfg.GetDeepConfig("users", static_cast<std::string>(user), "UserID").get<std::string>();
    user_id_set.insert(user_id);
  }

  if (account_group_map_.size() == 0) {
    UpdateGroupInfo("name01", user_id_set);
  }
}