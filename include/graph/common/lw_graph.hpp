#pragma once

#include <stdexcept>
#include <vector>
#include <unordered_map>
#include <memory>
#include <any>

namespace common {
    struct lwEdge {
        int target;
        double weight;

        lwEdge(int v, double w) 
            : target(v), weight(w) {}
    };

    template <typename T>
    class lwGraph {
    public:
        lwGraph(int num_vertices) {
            vertices.resize(num_vertices);
            adjList.resize(num_vertices);
        };

        void setVertex(int index, const T& data) {
            if (index >= 0 && index < vertices.size()) {
                vertices[index] = data;
            } else {
                throw std::out_of_range("Vertex index out of range.");
            }
        };
        virtual void addEdge(int v1, int v2, double weight = 1.0) = 0;

        const std::vector<lwEdge>& adj(int v) const {
            return adjList[v];
        }

        const T& getVertexData(int v) const {
            return vertices[v];
        }

        int getOrder() const {
            return vertices.size();
        }
        
    protected:
        std::vector<std::vector<lwEdge>> adjList; 
        std::vector<T> vertices;
    };
};