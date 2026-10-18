#include "graph.hpp"

#include <cmath>
#include <cstddef>
#include <iostream>
#include <stdexcept>
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

  passages.back().expected_price =
      chance_of_tunnel_collapse * cost_of_clearing_a_tunnel;

  passages.back().passageid = static_cast<int>(passages.size()) - 1;

  nodes[nodeid_0].passages.push_back(passages.back());
  nodes[nodeid_1].passages.push_back(passages.back());

  return passages.size() - 1;
}

int Graph::number_of_nodes() const { return nodes.size(); }

bool Graph::is_valid_node(int node_id) const {
  return node_id >= 0 && static_cast<std::size_t>(node_id) < nodes.size();
}

void Graph::update_price(int passageid, float price) {
  Passage& passage = passages.at(passageid);
  passage.expected_price = price;

  for (int nodeid : {passage.nodeid_0, passage.nodeid_1}) {
    for (Passage& copy : nodes[nodeid].passages) {
      if (copy.passageid == passageid) {
        copy = passage;
      }
    }
  }
}

const std::vector<Passage>& Graph::look(int nodeid) const {
  if (!is_valid_node(nodeid)) {
    throw std::out_of_range("Invalid node id");
  }

  return nodes[nodeid].passages;
}

bool Graph::needs_look(int nodeid) const {
  for (const Passage& passage : look(nodeid)) {
    if (!passage.known) {
      return true;
    }
  }

  return false;
}

int Graph::other_node(int passageid, int nodeid) const {
  const Passage& passage = passages.at(passageid);

  if (passage.nodeid_0 == nodeid) {
    return passage.nodeid_1;
  }

  if (passage.nodeid_1 == nodeid) {
    return passage.nodeid_0;
  }

  throw std::invalid_argument("Passage is not adjacent to this node");
}

void Graph::update_passage(int passageid, bool is_open) {
  Passage& passage = passages.at(passageid);
  passage.known = true;

  update_price(passageid, is_open ? 0.0f : cost_of_clearing_a_tunnel);
}

Graph read_graph() {
  int nodes_count;
  int passages_count;
  float clear_cost;

  std::cin >> nodes_count >> passages_count >> clear_cost;
  rooms::Graph graph(clear_cost);

  for (int i = 0; i < nodes_count; ++i) {
    float prize;

    if (!(std::cin >> prize) || prize < 0) {
      throw std::runtime_error("Invalid room prize");
    }

    graph.add_node(prize);
  }

  for (int i = 0; i < passages_count; ++i) {
    int from;
    int to;
    float probability_open;

    if (!(std::cin >> from >> to >> probability_open) ||
        !graph.is_valid_node(from) || !graph.is_valid_node(to) ||
        !std::isfinite(probability_open) || probability_open < 0 ||
        probability_open > 1) {
      throw std::runtime_error("Invalid passage");
    }

    graph.add_passage(from, to, 1.0f - probability_open);
  }

  return graph;
}

}  // namespace rooms
