#pragma once

#include <vector>
#include <unordered_map>
#include <queue>
#include <functional>
#include <limits>
#include <algorithm>
#include <cmath>
#include <any>
#include <tuple>

#include "graph/common/graph.hpp"
#include "graph/common/lw_graph.hpp"
#include "graph/common/node.hpp"
#include "graph/common/edge.hpp"
#include "graph/util/node_data.hpp"

namespace util::heuristics {
    using HeuristicFunc = std::function<double(common::Node*, common::Node*)>;

    double zeroHeuristic(common::Node*, common::Node*);

    double euclideanHeuristic2D(common::Node* a, common::Node* b);
    double euclideanHeuristic3D(common::Node* a, common::Node* b);

    double manhattanHeuristic2D(common::Node* a, common::Node* b);
    double manhattanHeuristic3D(common::Node* a, common::Node* b);

    double chebyshevHeuristic2D(common::Node* a, common::Node* b);
    double chebyshevHeuristic3D(common::Node* a, common::Node* b);
}