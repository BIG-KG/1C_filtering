#pragma once

#include <iosfwd>
#include <set>
#include <utility>
#include <vector>

#include "node.hpp"
#include "passage.hpp"

namespace rooms {

using price_pd = std::pair<int, float>;

class Graph {
 public:
  Graph(float clear_cost) : Graph(0, clear_cost) {}

  Graph(int nodes_count, float clear_cost, int start_node = 0)
      : nodes(nodes_count),
        cost_of_clearing_a_tunnel(clear_cost),
        start_node(start_node) {}

  int add_node(float prize);
  int add_passage(int nodeid_0, int nodeid_1, float chance_of_tunnel_collapse);

  int number_of_nodes() const;
  bool is_valid_node(int node_id) const;

  const std::vector<Passage>& look(int nodeid) const;
  bool needs_look(int nodeid) const;
  int other_node(int passageid, int nodeid) const;

  void update_passage(int passageid, bool is_open);

 private:
  friend class State;
  friend class Traversal;

  void update_price(int passageid, float price);

  std::vector<Node> nodes;
  std::vector<Passage> passages;
  std::set<int> accessible_nodes;
  const float cost_of_clearing_a_tunnel;
  const int start_node;
};

Graph read_graph(std::istream& input);

}  // namespace rooms
