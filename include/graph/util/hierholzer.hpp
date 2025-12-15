#include <stack>
#include <algorithm>
#include <unordered_map>

#include "graph/common/graph.hpp"

namespace util::Hierholzer {
    bool isEulerian(common::Graph* g);
    std::vector<common::Node*> findEulerianCycle(common::Graph* graph);
}