/**
 * @file graph.hpp
 * @author André V Bastos (andrevbastos)
 * @brief Definition of the Graph class for graph representation.
 */

#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <iomanip>
#include <unordered_map>
#include <string>
#include <nlohmann/json.hpp>

#include "graph/common/node.hpp"
#include "graph/common/edge.hpp"

namespace common {
    class Graph
    {
    public:
        virtual ~Graph() = default;

        virtual common::Node* newVertex() =  0;
        virtual common::Node* newVertex(std::any data) = 0;
        virtual common::Edge* newEdge(int v1_ID, int v2_ID, int weight = 1) = 0;
        virtual common::Edge* newEdge(common::Node* v1, common::Node* v2, int weight = 1) = 0;

        virtual void removeVertex(common::Node* v) = 0;
        virtual void removeEdge(common::Edge* e) = 0;

        common::Node* getVertex(int v_ID) const;
        common::Edge* getEdge(int e_ID) const;
        common::Node* getOpposite(common::Node* v, common::Edge* e) const;
        std::vector<common::Node*> getVertices() const;
        std::vector<common::Edge*> getEdges() const;
        std::unordered_map<common::Edge*, int> getWeights() const;
        virtual std::vector<std::vector<int>> getWeightMatrix() const = 0;
        std::vector<common::Node*> getNodesFromEdge(common::Edge* e) const;

        virtual Graph* clone() const = 0; 
        virtual Graph* makeSubGraph(const std::vector<int>& targetVertices, const std::vector<int>& targetEdges) const = 0;
        
        int getOrder() const;
        int getSize() const;
        float getDensity() const;
        void fastPrint() const;
        void print() const;
    
    protected:
        std::unordered_map<int, std::unique_ptr<common::Node>> vertices;
        std::unordered_map<int, std::unique_ptr<common::Edge>> edges;
        std::unordered_map<common::Edge*, int> weights;

        int nextNodeId = 0;
        int nextEdgeId = 0;
    };
}