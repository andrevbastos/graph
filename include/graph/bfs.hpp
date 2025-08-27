#pragma once

#include <queue>
#include "graph/common/graph.hpp"

using namespace common;

enum Color { WHITE, GRAY, BLACK };
struct Data
{
    Data()
     : parent(nullptr), color(WHITE), distance(-1) {};
    
    Node* parent;
    Color color;
    int distance;
};

class BFS {
public:
    static std::vector<Node*> getPath(Graph* graph, int start_ID, int target_ID);
    static std::vector<Node*> getPath(Graph* graph, Node* start, Node* target);

    static std::vector<std::pair<Node*, Node*>> getRoutingVector(Graph* graph, int start_ID);
    static std::vector<std::pair<Node*, Node*>> getRoutingVector(Graph* graph, Node* start);
};