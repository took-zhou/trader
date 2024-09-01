#ifndef TRADER_GROUP_ASSIGN_H
#define TRADER_GROUP_ASSIGN_H
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include "common/extern/sqlite3/sqlite3.h"

struct GroupAssign {
 public:
  GroupAssign();
  ~GroupAssign();

  void UpdateGroupInfo(const std::string &value, const std::set<std::string> &account_list);
  std::unordered_map<std::string, std::set<std::string>> &GetAccoutGroupMap(void) { return account_group_map_; };

 private:
  void PrepareSqlSentence();
  void RestoreFromSqlite3();
  void InitDatabase();
  std::unordered_map<std::string, std::set<std::string>> account_group_map_;
  sqlite3_stmt *insert_account_ = nullptr;
  sqlite3_stmt *delete_account_ = nullptr;
};

#endif