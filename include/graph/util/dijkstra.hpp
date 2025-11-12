#pragma once

#include <queue>
#include <unordered_set>

#include "graph/common/graph.hpp"

namespace util::Dijkstra {
    using MinHeapPair = std::pair<int, common::Node*>;
    using MinHeap = std::priority_queue<MinHeapPair, std::vector<MinHeapPair>, std::greater<MinHeapPair>>;

    struct Data {
        Data()
        : parent(nullptr), distance(std::numeric_limits<int>::max()) {};

        common::Node* parent;
        int distance;
    };

    std::unordered_map<common::Node*, Data> getPaths(common::Graph* graph, common::Node* start);

    std::vector<common::Node*> getPathTo(common::Graph* graph, common::Node* start, common::Node* end);
    void printPathTo(common::Graph* graph, common::Node* start, common::Node* end);
};