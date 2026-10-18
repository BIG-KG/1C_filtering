#pragma once

#include <set>
#include <utility>
#include <vector>

#include "node.hpp"
#include "passage.hpp"

namespace rooms {

using price_pd = std::pair<int, float>;

class Graph {
 public:
  Graph(float clear_cost) : nodes(0), cost_of_clearing_a_tunnel(clear_cost) {}

  Graph(int nodes_count, float clear_cost)
      : nodes(nodes_count), cost_of_clearing_a_tunnel(clear_cost) {}

  int add_node(float prize);
  int add_passage(int nodeid_0, int nodeid_1, float chance_of_tunnel_collapse);

  int number_of_nodes() const;
  bool is_valid_node(int node_id) const;

  const std::vector<Passage>& look(int nodeid) const;
  bool needs_look(int nodeid) const;
  int other_node(int passageid, int nodeid) const;

  void update_price(int passageid, float price);
  void update_passage(int passageid, bool is_open);

 private:
  friend class State;
  friend class Traversal;

  std::vector<Node> nodes;
  std::vector<Passage> passages;
  std::set<int> accessible_nodes;
  const float cost_of_clearing_a_tunnel;
};

Graph read_graph();

}  // namespace rooms
