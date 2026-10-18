#pragma once

#include <vector>

#include "passage.hpp"

namespace rooms {

struct Node {
  std::vector<Passage> passages;
  float prize = 0;
};

}  // namespace rooms
