#pragma once

#include <queue>

#include "graph/common/graph.hpp"

namespace util {
    class BFS {
    public:
        enum Color { WHITE, GRAY, BLACK };
        struct Data {
            Data()
            : parent(nullptr), color(WHITE), distance(-1) {};

            common::Node* parent;
            Color color;
            int distance;
        };

        static std::vector<common::Node*> getPath(common::Graph* graph, int start_ID, int target_ID);
        static std::vector<common::Node*> getPath(common::Graph* graph, common::Node* start, common::Node* target);

        static std::unordered_map<common::Node*, common::Node*> getRoutingVector(common::Graph* graph, int start_ID);
        static std::unordered_map<common::Node*, common::Node*> getRoutingVector(common::Graph* graph, common::Node* start);

        static void printData(common::Graph* graph, int start_ID);
        static void printData(common::Graph* graph, common::Node* start);
        static void tableData(common::Graph* graph, int start_ID);
        static void tableData(common::Graph* graph, common::Node* start);

        static std::unordered_map<common::Node*, common::Node*> getConnectedComponents(common::Graph* graph);
        static void printComponents(common::Graph* graph);
    };
}