#pragma once

#include "graph/common/lw_graph.hpp"

namespace undirected {
    template <typename T>
    class lwGraph : public common::lwGraph<T> {
    public:
        lwGraph(int num_vertices) 
            : common::lwGraph<T>(num_vertices) {}

        void addEdge(int v1, int v2, double weight = 1.0) override {
            if (v1 < 0 || v1 >= this->vertices.size() || 
                v2 < 0 || v2 >= this->vertices.size()) {
                throw std::out_of_range("Ambos os vértices devem estar dentro dos limites do grafo.");
            }
            
            this->adjList[v1].emplace_back(v2, weight);
            this->adjList[v2].emplace_back(v1, weight);
        }
    };
}