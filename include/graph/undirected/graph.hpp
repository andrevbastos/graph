#pragma once

#include "graph/common/graph.hpp"

namespace undirected {
    class Graph : public common::Graph
    {
    public:
        common::Node* newVertex() override;
        common::Node* newVertex(std::any data) override;
        common::Edge* newEdge(int v1_ID, int v2_ID, int weight = 1) override;
        common::Edge* newEdge(common::Node* v1, common::Node* v2, int weight = 1) override;

        void removeVertex(common::Node* v) override;
        void removeEdge(common::Edge* e) override;

        std::vector<std::vector<int>> getWeightMatrix() const override;

        Graph* clone() const override;
        Graph* makeSubGraph(const std::vector<int>& targetVertices, const std::vector<int>& targetEdges) const override { return nullptr; };
    };
}