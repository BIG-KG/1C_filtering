#pragma once

#include "graph.hpp"
#include "judge_interface.hpp"

namespace rooms {

class State {
 public:
  explicit State(Graph graph);

  int position() const;
  float collected_prize() const;
  int days() const;
  float profit() const;

  const std::vector<Passage>& look();
  bool move(int passageid);
  void clear(int passageid);
  ExpeditionResult end();

 private:
  friend class Traversal;

  void collect_prize();

  Graph graph_;
  JudgeInterface judge_;
  int position_ = 0;
  float collected_prize_ = 0;
  int days_ = 0;
};

}  // namespace rooms