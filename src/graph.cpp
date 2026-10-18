#include "graph.hpp"

#include <vector>

#include "node.hpp"
#include "passage.hpp"

namespace rooms {

int Graph::add_node(float prize) {
  nodes.emplace_back();
  nodes.back().prize = prize;
  return nodes.size() - 1;
}

int Graph::add_passage(int nodeid_0, int nodeid_1,
                       float chance_of_tunnel_collapse) {
  passages.emplace_back();
  passages.back().nodeid_0 = nodeid_0;
  passages.back().nodeid_1 = nodeid_1;
  passages.back().expected_price = chance_of_tunnel_collapse;

  nodes[nodeid_0].passages.push_back(passages.back());
  nodes[nodeid_1].passages.push_back(passages.back());

  return passages.size() - 1;
}

int Graph::number_of_nodes() const { return nodes.size(); }

}  // namespace rooms
