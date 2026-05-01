#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include "graph/common/graph.hpp"
#include "graph/common/lw_graph.hpp"

namespace util {
    struct Vertex3D {
        int x, y, z;

        bool operator==(const Vertex3D& other) const {
            return x == other.x && y == other.y && z == other.z;
        }
    };

    typedef std::pair<double, Vertex3D> PQElement;
    typedef std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> PQ;

    Vertex3D jump(const common::Graph* graph, const Vertex3D& source, const Vertex3D& target);
    std::vector<Vertex3D> getSuccessors(const common::Graph* graph, const Vertex3D& source, const Vertex3D& parent,const Vertex3D& target);
    std::vector<int> jumpPointSearch(common::Graph* graph, int start, int end);

    Vertex3D jumpLw(const common::lwGraph<Vertex3D>& graph, const Vertex3D& source, const Vertex3D& target) {

    };

    std::vector<Vertex3D> getSuccessorsLw(const common::lwGraph<Vertex3D>& graph, const Vertex3D& source, const Vertex3D& parent,const Vertex3D& target) {

    };
    
    std::vector<int> jumpPointSearchLw(const common::lwGraph<Vertex3D>& graph, int start, int end) {

    };
}