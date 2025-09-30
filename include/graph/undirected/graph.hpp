#pragma once

#include "graph/common/graph.hpp"

namespace undirected {
    class Graph : public common::Graph
    {
    public:
        common::Node* newVertex(std::string label = "") override;
        common::Edge* newEdge(int v1_ID, int v2_ID);
        common::Edge* newEdge(common::Node* v1, common::Node* v2);

        void removeVertex(common::Node* v) override;
        void removeEdge(common::Edge* e) override;

        Graph* clone() const override;
        Graph* makeSubGraph(const std::vector<int>& targetVertices, const std::vector<int>& targetEdges) const override { return nullptr; };
    };
}