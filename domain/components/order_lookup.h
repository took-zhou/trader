#ifndef TRADER_ORDER_INDEX_H
#define TRADER_ORDER_INDEX_H
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "common/extern/sqlite3/sqlite3.h"

struct OrderPara {
 public:
  OrderPara(const std::string &order_ref, uint32_t volume1, uint32_t volume2)
      : order_ref_(order_ref), yesterday_volume_(volume1), today_volume_(volume2) {}
  void SetOrderRef(const std::string &ref) { order_ref_ = ref; }
  void AddYesterdayVolume(uint32_t volume) { yesterday_volume_ += volume; }
  void AddTodayVolume(uint32_t volume) { today_volume_ += volume; }
  uint32_t GetYesterdayVolume(void) { return yesterday_volume_; }
  uint32_t GetTodayVolume(void) { return today_volume_; }
  void MoveTodayVolumeToYesterdayVolume(void) {
    yesterday_volume_ += today_volume_;
    today_volume_ = 0;
  }
  const std::string &GetOrderRef(void) { return order_ref_; }

 private:
  std::string order_ref_;
  uint32_t yesterday_volume_ = 0;
  uint32_t today_volume_ = 0;
};

struct OrderLookup {
 public:
  OrderLookup();
  ~OrderLookup();

  bool UpdateOrderIndex(const std::string &ins, const std::string &index, const std::string &group_id, const std::string &user_id,
                        const std::string &order_ref);
  void UpdateOpenInterest(const std::string &ins, const std::string &index, const std::string &group_id, const std::string &user_id,
                          int32_t yesterday_volume, int32_t today_volume);
  bool DelOrderIndex(const std::string &ins, const std::string &index);
  void HandleTraderClose();
  std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<OrderPara>>> &GetOrderIndexMap();

 private:
  void PrepareSqlSentence();
  void RestoreFromSqlite3();
  void InitDatabase();
  std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<OrderPara>>> order_index_map_;
  sqlite3_stmt *insert_lookup_ = nullptr;
  sqlite3_stmt *delete_lookup_ = nullptr;
  sqlite3_stmt *update_order_ref_ = nullptr;
  sqlite3_stmt *update_position_ = nullptr;
};

#endif