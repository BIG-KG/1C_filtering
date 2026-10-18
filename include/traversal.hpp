#pragma once

#include <cstddef>
#include <limits>
#include <optional>
#include <set>
#include <vector>

#include "state.hpp"

namespace rooms {

class Traversal {
 public:
  explicit Traversal(State& state);

  ExpeditionResult run();

 private:
  struct PriceCompare {
    bool operator()(const price_pd& left, const price_pd& right) const {
      if (left.second != right.second) {
        return left.second < right.second;
      }

      return left.first < right.first;
    }
  };

  struct DijkstraData {
    explicit DijkstraData(std::size_t count)
        : prices(count, std::numeric_limits<float>::infinity()),
          first_passages(count, -1) {}

    std::vector<float> prices;
    std::vector<int> first_passages;
    std::set<price_pd, PriceCompare> queue;

    int best_nodeid = -1;
    float best_profit = 0;
  };

  void dfs(int nodeid, int parentid = -1);
  void dfs_impl(int nodeid, int parentid);

  std::optional<int> dijkstra() const;
  void navigate_to(int nodeid);

  DijkstraData prepare_dijkstra() const;

  void relax(DijkstraData& data, int nodeid, float price,
             int first_passage) const;

  State& state;
};

}  // namespace rooms
