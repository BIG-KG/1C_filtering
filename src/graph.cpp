#include "graph.hpp"

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
  return nodes.at(nodeid).passages;
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

}  // namespace rooms