#include "traversal.hpp"

#include <algorithm>
#include <deque>
#include <stdexcept>
#include <vector>

namespace rooms {

Traversal::Traversal(State& state) : state(state) {}

void Traversal::dfs(int nodeid, int parentid) {
  if (nodeid != state.position_) {
    throw std::logic_error("DFS must start at the current position");
  }

  dfs_impl(nodeid, parentid);
}

void Traversal::dfs_impl(int nodeid, int parentid) {
  Graph& graph = state.graph_;

  if (nodeid == parentid || !graph.accessible_nodes.insert(nodeid).second) {
    return;
  }

  auto paths = state.look();

  for (const Passage& passage : paths) {
    int next = graph.other_node(passage.passageid, nodeid);

    if (!passage.known || passage.expected_price != 0 || next == parentid ||
        graph.accessible_nodes.contains(next)) {
      continue;
    }

    if (!state.move(passage.passageid)) {
      continue;
    }

    dfs_impl(next, parentid);

    if (!state.move(passage.passageid)) {
      throw std::runtime_error("Failed to return through an open passage");
    }
  }
}

Traversal::DijkstraData Traversal::prepare_dijkstra() const {
  const Graph& graph = state.graph_;
  DijkstraData data(graph.nodes.size());

  for (int nodeid : graph.accessible_nodes) {
    data.prices[nodeid] = 0;
  }

  for (int nodeid : graph.accessible_nodes) {
    for (const Passage& passage : graph.look(nodeid)) {
      if (passage.known && passage.expected_price == 0) {
        continue;
      }

      relax(data, graph.other_node(passage.passageid, nodeid),
            passage.expected_price, passage.passageid);
    }
  }

  return data;
}

void Traversal::relax(DijkstraData& data, int nodeid, float price,
                      int first_passage) const {
  if (price >= data.prices[nodeid]) {
    return;
  }

  data.queue.erase({nodeid, data.prices[nodeid]});

  data.prices[nodeid] = price;
  data.first_passages[nodeid] = first_passage;

  data.queue.insert({nodeid, price});

  float profit = state.graph_.nodes[nodeid].prize - price;

  if (profit > data.best_profit) {
    data.best_profit = profit;
    data.best_nodeid = nodeid;
  }
}

std::optional<int> Traversal::dijkstra() const {
  const Graph& graph = state.graph_;
  auto data = prepare_dijkstra();

  while (!data.queue.empty()) {
    auto [nodeid, price] = *data.queue.begin();
    data.queue.erase(data.queue.begin());

    for (const Passage& passage : graph.look(nodeid)) {
      relax(data, graph.other_node(passage.passageid, nodeid),
            price + passage.expected_price, data.first_passages[nodeid]);
    }
  }

  if (data.best_nodeid == -1) {
    return std::nullopt;
  }

  return data.first_passages[data.best_nodeid];
}

void Traversal::navigate_to(int nodeid) {
  const Graph& graph = state.graph_;
  int start = state.position_;

  graph.nodes.at(nodeid);

  std::vector<int> parents(graph.nodes.size(), -1);
  std::vector<bool> visited(graph.nodes.size(), false);
  std::deque<int> queue{start};

  visited[start] = true;

  while (!queue.empty() && !visited[nodeid]) {
    int current = queue.front();
    queue.pop_front();

    for (const Passage& passage : graph.look(current)) {
      int next = graph.other_node(passage.passageid, current);

      if (!passage.known || passage.expected_price != 0 || visited[next]) {
        continue;
      }

      parents[next] = passage.passageid;
      visited[next] = true;
      queue.push_back(next);
    }
  }

  if (!visited[nodeid]) {
    throw std::runtime_error("No known open route to the target");
  }

  std::vector<int> route;

  for (int current = nodeid; current != start;) {
    int id = parents[current];
    route.push_back(id);
    current = graph.other_node(id, current);
  }

  std::reverse(route.begin(), route.end());

  for (int id : route) {
    if (!state.move(id)) {
      throw std::runtime_error("Failed to follow an open route");
    }
  }
}

ExpeditionResult Traversal::run() {
  dfs(state.position_);

  while (auto passageid = dijkstra()) {
    const Graph& graph = state.graph_;
    const Passage& passage = graph.passages[*passageid];

    int parentid = graph.accessible_nodes.contains(passage.nodeid_0)
                       ? passage.nodeid_0
                       : passage.nodeid_1;

    int next = graph.other_node(*passageid, parentid);

    state.clear(*passageid);
    navigate_to(next);
    dfs(next, parentid);
  }

  return state.end();
}

}  // namespace rooms