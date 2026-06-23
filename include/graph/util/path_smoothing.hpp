#pragma once

#include <vector>
#include <cmath>
#include "graph/common/node.hpp"
#include "graph/common/lw_graph.hpp"
#include "graph/util/node_data.hpp"

namespace util {
    std::vector<common::Node*> pathSmoothing(const std::vector<common::Node*>& rawPath);

    template <typename T>
    std::vector<int> lwPathSmoothing(const std::vector<int>& rawPath, const common::lwGraph<T>& graph) 
    {
        if (rawPath.size() <= 2) return rawPath;

        std::vector<int> smoothedPath;
        smoothedPath.push_back(rawPath[0]);

        for (size_t i = 1; i < rawPath.size() - 1; ++i) {
            const T& p1 = graph.getVertexData(smoothedPath.back());
            const T& p2 = graph.getVertexData(rawPath[i]);
            const T& p3 = graph.getVertexData(rawPath[i+1]);

            // Acesso direto às propriedades da sua struct Vertex3D/Point
            float crossProduct = (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);
            bool isCollinear = std::abs(crossProduct) < 0.001f;

            if (!isCollinear) {
                smoothedPath.push_back(rawPath[i]);
            }
        }
        
        smoothedPath.push_back(rawPath.back());
        return smoothedPath;
    }
}
