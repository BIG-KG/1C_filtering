#pragma once

namespace rooms {

struct Passage {
  int nodeid_0;
  int nodeid_1;
  float expected_price = 0;

  int passageid = -1;
  bool known = false;
};

}  // namespace rooms