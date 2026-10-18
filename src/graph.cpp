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

void Graph::dfs(int nodeid, int parentid) {
  if (nodeid == parentid) {
    return;
  }

  if (!accessible_nodes.insert({nodeid, 0.0f}).second) {
    return;
  }

  for (const Passage& passage : nodes[nodeid].passages) {
    if (passage.expected_price != 0.0f) {
      continue;
    }

    int next_nodeid =
        passage.nodeid_0 == nodeid ? passage.nodeid_1 : passage.nodeid_0;

    dfs(next_nodeid, parentid);
  }
}

void Graph::update_price(int passageid, float price) {
  passages[passageid].expected_price = price;
}

}  // namespace rooms
