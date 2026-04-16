#pragma once

#include <vector>
#include <unordered_map>
#include <queue>
#include <functional>
#include <limits>
#include <algorithm>
#include <cmath>
#include <any>
#include <tuple>

#include "graph/common/graph.hpp"
#include "graph/common/lw_graph.hpp"
#include "graph/common/node.hpp"
#include "graph/common/edge.hpp"
#include "graph/util/node_data.hpp"
#include "graph/util/heuristics.hpp"

namespace util {
    std::vector<common::Node*> AStar(common::Graph* graph, int startId, int endId, heuristics::HeuristicFunc heuristic);
    std::vector<common::Node*> AStarMod(common::Graph* graph, int startId, int endId, heuristics::HeuristicFunc heuristic);

    template <typename T>
    std::vector<int> lwAStar(const common::lwGraph<T>& graph, int startId, int endId, std::function<double(const T&, const T&)> heuristic) 
    {
        int numVertices = graph.getOrder(); 
        
        if (startId < 0 || startId >= numVertices || endId < 0 || endId >= numVertices) return {};
        if (startId == endId) return {startId};

        std::vector<double> gScore(numVertices, std::numeric_limits<double>::infinity());
        std::vector<int> cameFrom(numVertices, -1);

        using Element = std::pair<double, int>;
        std::priority_queue<Element, std::vector<Element>, std::greater<Element>> openSet;

        gScore[startId] = 0.0;
        
        const T& startData = graph.getVertexData(startId);
        const T& endData = graph.getVertexData(endId);
        
        openSet.push({heuristic(startData, endData), startId});

        while (!openSet.empty()) {
            auto [currentF, currentId] = openSet.top();
            openSet.pop();

            if (currentId == endId) {
                std::vector<int> path;
                int curr = endId;
                while (curr != -1) {
                    path.push_back(curr);
                    curr = cameFrom[curr];
                }
                std::reverse(path.begin(), path.end());
                return path;
            }

            if (currentF > gScore[currentId] + heuristic(graph.getVertexData(currentId), endData) + 1e-9) {
                continue; 
            }

            for (const auto& edge : graph.adj(currentId)) {
                int neighborId = edge.target;
                double tentative_gScore = gScore[currentId] + edge.weight;

                if (tentative_gScore < gScore[neighborId]) {
                    cameFrom[neighborId] = currentId;
                    gScore[neighborId] = tentative_gScore;
                    
                    double f = tentative_gScore + heuristic(graph.getVertexData(neighborId), endData);
                    openSet.push({f, neighborId});
                }
            }
        }

        return {};
    }

   template <typename T>
    std::vector<int> lwAStarMod(const common::lwGraph<T>& graph, int startId, int endId, std::function<double(const T&, const T&)> heuristic) 
    {
        int numVertices = graph.getOrder();

        if (startId < 0 || startId >= numVertices || endId < 0 || endId >= numVertices) return {};
        if (startId == endId) return {startId};

        std::vector<int> VM(numVertices, -1);
        std::vector<double> CM(numVertices, std::numeric_limits<double>::infinity());
        
        std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<>> VPQ;

        VPQ.push({0.0, startId});
        CM[startId] = 0.0;
        
        const T& endData = graph.getVertexData(endId);

        while (!VPQ.empty()) {
            auto AV = VPQ.top().second;
            VPQ.pop();

            if (AV == endId) {
                std::vector<int> VL;
                for (int node = endId; node != -1; node = VM[node]) {
                    VL.push_back(node);
                }
                std::reverse(VL.begin(), VL.end());

                if (VL.size() <= 2) return VL;

                std::vector<int> PL;
                PL.push_back(VL[0]);

                for (size_t i = 1; i < VL.size() - 1; ++i) {
                    int P1 = PL.back();
                    int P2 = VL[i];
                    int P3 = VL[i+1];

                    const T& data1 = graph.getVertexData(P1);
                    const T& data2 = graph.getVertexData(P2);
                    const T& data3 = graph.getVertexData(P3);

                    // Acesso direto às propriedades x e y do seu tipo T
                    float crossProduct = (data2.x - data1.x) * (data3.y - data2.y) - (data2.y - data1.y) * (data3.x - data2.x);
                    bool isCollinear = std::abs(crossProduct) < 0.001f;

                    if (!isCollinear) {
                        PL.push_back(P2);
                    }
                }
                
                PL.push_back(VL.back());
                return PL; 
            }

            const auto& NV = graph.adj(AV);
            const T& avData = graph.getVertexData(AV);

            for (const auto& adj : NV) {
                int neighbor = adj.target;
                double VC = CM[AV] + adj.weight;
                
                if (VC < CM[neighbor]) {
                    CM[neighbor] = VC;
                    
                    const T& neighborData = graph.getVertexData(neighbor);

                    bool isDiagonal = (avData.x != neighborData.x) && (avData.y != neighborData.y);
                    
                    double M = isDiagonal ? std::sqrt(2.0) : 1.0; 
                    
                    double NVP = VC + heuristic(neighborData, endData) * M;
                    
                    VPQ.push({NVP, neighbor});
                    VM[neighbor] = AV;
                }
            }
        }
        
        return {};
    }
}