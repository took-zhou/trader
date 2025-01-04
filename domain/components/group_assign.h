#ifndef TRADER_GROUP_ASSIGN_H
#define TRADER_GROUP_ASSIGN_H
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include "common/extern/sqlite3/sqlite3.h"

struct GroupInfo {
 public:
  void ClearAccountList(void) { account_list_.clear(); }
  void AddAccount(const std::string &account) { account_list_.push_back(account); }
  std::vector<std::string> &GetAccountList(void) { return account_list_; }
  uint32_t GetNextAccountIndex(void) { return (account_index_++ % account_list_.size()); }

 private:
  std::vector<std::string> account_list_;
  uint32_t account_index_ = 0;
};

struct GroupAssign {
 public:
  GroupAssign();
  ~GroupAssign();

  void UpdateGroupInfo(const std::string &value, const std::set<std::string> &account_list);
  void HandleTraderOpen();
  std::unordered_map<std::string, GroupInfo> &GetAccoutGroupMap(void) { return account_group_map_; };

 private:
  void PrepareSqlSentence();
  void RestoreFromSqlite3();
  void InitDatabase();
  std::unordered_map<std::string, GroupInfo> account_group_map_;
  sqlite3_stmt *insert_account_ = nullptr;
  sqlite3_stmt *delete_account_ = nullptr;
};

#endif