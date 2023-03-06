#ifndef TRADER_ORDER_INDEX_H
#define TRADER_ORDER_INDEX_H
#include <memory>
#include <string>
#include <unordered_map>
#include "common/extern/sqlite3/sqlite3.h"

struct OrderLookup {
 public:
  struct OrderPara {
    OrderPara(const std::string &user_id, const std::string &order_ref) : user_id(user_id), order_ref(order_ref) {}
    std::string user_id;
    std::string order_ref;
  };

  OrderLookup();
  ~OrderLookup();

  bool BuildOrderIndex(const std::string &order_index, const std::shared_ptr<OrderPara> &para);
  bool DelOrderIndex(const std::string &order_index);
  OrderPara *GetOrderPara(const std::string &order_index);
  bool init_database_flag = false;

 private:
  std::unordered_map<std::string, std::shared_ptr<OrderPara>> order_index_map_;
  void PrepareSqlSentence();
  void RestoreFromSqlite3();
  void InitDatabase();
  sqlite3_stmt *insert_lookup_ = nullptr;
  sqlite3_stmt *delete_lookup_ = nullptr;
  sqlite3_stmt *update_lookup_ = nullptr;
};

#endif