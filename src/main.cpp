#include <exception>
#include <iostream>

#include "graph.hpp"
#include "state.hpp"
#include "traversal.hpp"

int main() {
  rooms::State state(rooms::read_graph(std::cin));
  rooms::Traversal traversal(state);
  traversal.run();

  return 0;
}
