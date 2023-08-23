#ifndef TRADER_ORDER_INDEX_H
#define TRADER_ORDER_INDEX_H
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "common/extern/sqlite3/sqlite3.h"

struct OrderLookup {
 public:
  struct OrderPara {
    OrderPara(const std::string &order_ref, double price, uint32_t volume1, uint32_t volume2)
        : order_ref(order_ref), price(price), yesterday_volume(volume1), today_volume(volume2) {}
    std::string order_ref;
    double price = 0;
    uint32_t yesterday_volume = 0;
    uint32_t today_volume = 0;
  };

  OrderLookup();
  ~OrderLookup();

  bool UpdateOrderIndex(const std::string &ins, const std::string &index, const std::string &user_id, const std::string &order_ref);
  void UpdateOpenInterest(const std::string &ins, const std::string &index, const std::string &user_id, double price,
                          int32_t yesterday_volume, int32_t today_volume);
  bool DelOrderIndex(const std::string &ins, const std::string index);
  void HandleTraderClose();

  std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<OrderPara>>> order_index_map;
  bool init_database_flag = false;

 private:
  void PrepareSqlSentence();
  void RestoreFromSqlite3();
  void InitDatabase();
  sqlite3_stmt *insert_lookup_ = nullptr;
  sqlite3_stmt *delete_lookup_ = nullptr;
  sqlite3_stmt *update_order_ref_ = nullptr;
  sqlite3_stmt *update_position_ = nullptr;
};

#endif