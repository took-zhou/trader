#include <memory>
#include "common/self/file_util.h"
#include "gtest/gtest.h"
#include "trader/domain/components/order_lookup.h"

namespace {
struct OrderLookupTest : public testing::Test {
 public:
  void SetUp() override {
    auto &json_cfg = utils::JsonConfig::GetInstance();
    json_cfg.SetFileName("/etc/marktrade/config.json");
    order_lookup_ = std::make_unique<OrderLookup>();
  }

  void TearDown() override {}
  std::unique_ptr<OrderLookup> &GetOrderLookup() { return order_lookup_; };

 private:
  std::unique_ptr<OrderLookup> order_lookup_;
};

TEST_F(OrderLookupTest, test1) {
  // GetOrderLookup()->UpdateOrderIndex("TA501", "0001", "name01", "129080", "00000001");
  auto &index_map = GetOrderLookup()->GetOrderIndexMap();
  auto pos = index_map.find("TA501.0001");
  if (pos != index_map.end()) {
    auto pos2 = pos->second.find("name01.129080");
    if (pos2 != pos->second.end()) {
      std::cout << "yesterday volume: " << pos2->second->GetYesterdayVolume() << std::endl;
      std::cout << "today volume: " << pos2->second->GetTodayVolume() << std::endl;
    }
  }
  GetOrderLookup()->UpdateOpenInterest("TA501", "0001", "name01", "129080", 0, 1);
  pos = index_map.find("TA501.0001");
  if (pos != index_map.end()) {
    auto pos2 = pos->second.find("name01.129080");
    if (pos2 != pos->second.end()) {
      std::cout << "yesterday volume: " << pos2->second->GetYesterdayVolume() << std::endl;
      std::cout << "today volume: " << pos2->second->GetTodayVolume() << std::endl;
    }
  }
}

}  // namespace