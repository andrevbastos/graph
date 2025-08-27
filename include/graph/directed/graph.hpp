#pragma once

#include "graph/common/graph.hpp"

namespace directed {
    class Graph : public common::Graph
    {
    public:
        virtual common::Node* newVertex();
        virtual common::Edge* newEdge(int v1_ID, int v2_ID);
        virtual common::Edge* newEdge(common::Node* v1_ID, common::Node* v2_ID);

        virtual void removeVertex(int v_ID) override;
        virtual void removeVertex(common::Node* v) override;
        virtual void removeEdge(int e_ID) override;
        virtual void removeEdge(common::Edge* e) override;

        virtual Graph* makeSubGraph(const std::vector<int>& targetVertices, const std::vector<int>& targetEdges) const { return nullptr; };
    };
}