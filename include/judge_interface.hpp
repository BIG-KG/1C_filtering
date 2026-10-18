#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace rooms {

struct PathInfo {
  int passage_id;
  int destination_id;
  bool is_open;
};

struct ExpeditionResult {
  double collected_prize;
  int days;
  double profit;
};

class JudgeInterface {
 public:
  std::vector<PathInfo> look(std::size_t passage_count);
  bool move(int passage_id);
  void clear(int passage_id);
  ExpeditionResult end();

 private:
  std::string read_reply();
};

}  // namespace rooms