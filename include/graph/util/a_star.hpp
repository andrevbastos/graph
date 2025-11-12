#pragma once

#include <vector>
#include <unordered_map>
#include <queue>
#include <functional>
#include <limits>
#include <algorithm>
#include <cmath>

#include "graph/common/graph.hpp"
#include "graph/common/node.hpp"
#include "graph/common/edge.hpp"

namespace util::AStar {
    using HeuristicFunc = std::function<double(common::Node*, common::Node*)>;

    std::vector<common::Node*> getPath(common::Graph* graph, int startId, int endId, HeuristicFunc heuristic);

    std::pair<bool, std::pair<double, double>> getCoords2D(common::Node* n);
    std::pair<bool, std::tuple<double, double, double>> getCoords3D(common::Node* n);

    double zeroHeuristic(common::Node*, common::Node*);

    double euclideanHeuristic2D(common::Node* a, common::Node* b);
    double euclideanHeuristic3D(common::Node* a, common::Node* b);

    double manhattanHeuristic2D(common::Node* a, common::Node* b);
    double manhattanHeuristic3D(common::Node* a, common::Node* b);
}