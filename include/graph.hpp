#pragma once

#include <set>
#include <string>
#include <unordered_map>
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

  const std::vector<Passage>& look(int nodeid) const;
  bool needs_look(int nodeid) const;
  int other_node(int passageid, int nodeid) const;

  void update_price(int passageid, float price);
  void update_passage(int passageid, bool is_open);

  void set_node_name(int nodeid, const std::string& name);
  void set_passage_name(int passageid, const std::string& name);

  int node_id(const std::string& name) const;
  int passage_id(const std::string& name) const;
  const std::string& passage_name(int passageid) const;

 private:
  friend class State;
  friend class Traversal;

  std::vector<Node> nodes;
  std::vector<Passage> passages;
  std::set<int> accessible_nodes;

  std::vector<std::string> node_names;
  std::vector<std::string> passage_names;

  std::unordered_map<std::string, int> node_ids;
  std::unordered_map<std::string, int> passage_ids;

  const float cost_of_clearing_a_tunnel;
};

}  // namespace rooms