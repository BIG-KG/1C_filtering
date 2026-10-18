#include <set>
#include <vector>

#include "node.hpp"
#include "passage.hpp"

namespace rooms {

struct price_pd {
  int nodeid;
  float price;
};

class Graph {
 public:
  Graph(float clear_cost) : nodes(0), cost_of_clearing_a_tunnel(clear_cost) {}
  Graph(int nodes_count, float clear_cost)
      : nodes(nodes_count), cost_of_clearing_a_tunnel(clear_cost) {}

  int add_node(float prize);
  int add_passage(int nodeid_0, int nodeid_1, float chance_of_tunnel_collapse);

  int number_of_nodes() const;

 private:
  std::vector<Node> nodes;
  std::vector<Passage> passages;
  std::set<price_pd> accessible_nodes;
  const float cost_of_clearing_a_tunnel;
};

}  // namespace rooms