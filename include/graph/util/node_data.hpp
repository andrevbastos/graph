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
#include "graph/common/node.hpp"
#include "graph/common/edge.hpp"

namespace util {
    std::pair<bool, std::pair<double, double>> getCoords2D(common::Node* n);
    std::pair<bool, std::tuple<double, double, double>> getCoords3D(common::Node* n);
}