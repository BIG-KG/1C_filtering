#include "state.hpp"

#include <set>
#include <stdexcept>
#include <utility>

namespace rooms {

State::State(Graph graph)
    : graph_(std::move(graph)), position_(graph_.start_node) {
  if (graph_.nodes.empty()) {
    throw std::invalid_argument("The graph is empty");
  }

  if (!graph_.is_valid_node(position_)) {
    throw std::invalid_argument("Invalid starting node");
  }

  if (!(graph_.cost_of_clearing_a_tunnel > 0)) {
    throw std::invalid_argument("Clearing cost must be positive");
  }

  collect_prize();
}

int State::position() const { return position_; }

float State::collected_prize() const { return collected_prize_; }

int State::days() const { return days_; }

float State::profit() const {
  return collected_prize_ - days_ * graph_.cost_of_clearing_a_tunnel;
}

void State::collect_prize() {
  collected_prize_ += graph_.nodes[position_].prize;
  graph_.nodes[position_].prize = 0;
}

const std::vector<Passage>& State::look() {
  if (!graph_.needs_look(position_)) {
    return graph_.look(position_);
  }

  auto reply = judge_.look(graph_.look(position_).size());

  std::vector<std::pair<int, bool>> updates;
  std::multiset<int> expected;

  for (const Passage& passage : graph_.look(position_)) {
    expected.insert(passage.passageid);
  }

  for (const PathInfo& path : reply) {
    int id = path.passage_id;
    auto entry = expected.find(id);

    if (entry == expected.end()) {
      throw std::runtime_error("LOOK contains missing or repeated passages");
    }

    expected.erase(entry);

    if (graph_.other_node(id, position_) != path.destination_id) {
      throw std::runtime_error("LOOK response does not match the graph");
    }

    const Passage& passage = graph_.passages.at(id);

    if (passage.known && (passage.expected_price == 0) != path.is_open) {
      throw std::runtime_error("LOOK contradicts a known passage state");
    }

    updates.emplace_back(id, path.is_open);
  }

  for (const auto& [id, is_open] : updates) {
    graph_.update_passage(id, is_open);
  }

  return graph_.look(position_);
}

bool State::move(int passageid) {
  int destination = graph_.other_node(passageid, position_);

  bool success = judge_.move(passageid);
  graph_.update_passage(passageid, success);

  if (success) {
    position_ = destination;
    collect_prize();
  }

  return success;
}

void State::clear(int passageid) {
  const Passage& passage = graph_.passages.at(passageid);

  if (!passage.known || passage.expected_price == 0) {
    throw std::logic_error("CLEAR requires a known broken passage");
  }

  judge_.clear(passageid);
  graph_.update_passage(passageid, true);

  ++days_;
  position_ = graph_.start_node;
}

ExpeditionResult State::end() { return judge_.end(); }

}  // namespace rooms
