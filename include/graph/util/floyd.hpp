#pragma once

#include "graph/common/graph.hpp"
#include "graph/common/edge.hpp"

namespace util::Floyd {
    std::pair<std::vector<std::vector<int>>, std::vector<std::vector<common::Node*>>> computeAllPairs(common::Graph* g);

    std::vector<common::Node*> getShortestPath(common::Graph* g, common::Node* source, common::Node* target, std::vector<std::vector<common::Node*>> r = {});

    void printPath(const std::vector<std::vector<common::Node*>>& r, int i, int j);
    void printAllPairsShortestPath(common::Graph* g);
};