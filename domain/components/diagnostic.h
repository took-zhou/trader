#ifndef TRADER_DIAGNOSTIC_H
#define TRADER_DIAGNOSTIC_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/extern/sqlite3/sqlite3.h"
#include "common/self/dem.h"

enum DiagnosticEventId { kApiCallFailed = 0, kPositionMisMatched = 1, kLoginFailed = 2, kNoEnoughMoney = 3, kEventMax = 4 };

struct Diagnostic : Dem {
 public:
  Diagnostic();

  void MonitorStatus();
  void RestoreFromSqlite3();

 private:
  void RecordStatus(DiagnosticEventId event_id);
  void PrepareSqlSentence();
  void InitDatabase();

  const uint32_t sql_length_ = 255;
  sqlite3_stmt* update_diagnostic_info_ = nullptr;
};

#endif
