#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <iomanip>
#include <unordered_map>

#include "graph/common/node.hpp"
#include "graph/common/edge.hpp"

namespace common {
    class Graph
    {
    public:
        virtual ~Graph() = default;
        
        virtual common::Node* newVertex() = 0;
        virtual common::Edge* newEdge(int v1_ID, int v2_ID) = 0;
        virtual common::Edge* newEdge(common::Node* v1_ID, common::Node* v2_ID) = 0;
        
        virtual void removeVertex(int v_ID) = 0;
        virtual void removeVertex(common::Node* v_ID) = 0;
        virtual void removeEdge(int e_ID) = 0;
        virtual void removeEdge(common::Edge* e_ID) = 0;
        
        common::Node* getVertex(int v_ID) const;
        common::Edge* getEdge(int e_ID) const;
        common::Node* getOpposite(int v_ID, int e_ID) const;
        common::Node* getOpposite(common::Node* v, common::Edge* e) const;
        std::vector<common::Node*> getVertices() const;
        std::vector<common::Edge*> getEdges() const;
        std::vector<common::Node*> getNodesFromEdge(int e_ID) const;
        std::vector<common::Node*> getNodesFromEdge(common::Edge* e) const;

        virtual Graph* makeSubGraph(const std::vector<int>& targetVertices, const std::vector<int>& targetEdges) const = 0;
        
        int getOrder() const;
        int getSize() const;
        float getDensity() const;
        void fastPrint() const;
        void print() const;
    
    protected:
        std::unordered_map<int, std::unique_ptr<common::Node>> vertices;
        std::unordered_map<int, std::unique_ptr<common::Edge>> edges;

        int nextNodeId = 0;
        int nextEdgeId = 0;
    };
}