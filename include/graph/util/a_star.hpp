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
                std::vector<int> rawPath;
                int curr = endId;
                while (curr != -1) {
                    rawPath.push_back(curr);
                    curr = cameFrom[curr];
                }
                std::reverse(rawPath.begin(), rawPath.end());

                if (rawPath.size() <= 2) return rawPath;

                std::vector<int> smoothedPath;
                smoothedPath.push_back(rawPath[0]);

                for (size_t i = 1; i < rawPath.size() - 1; ++i) {
                    const T& p1 = graph.getVertexData(smoothedPath.back());
                    const T& p2 = graph.getVertexData(rawPath[i]);
                    const T& p3 = graph.getVertexData(rawPath[i+1]);

                    float crossProduct = (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);
                    bool isCollinear = std::abs(crossProduct) < 0.001f;

                    if (!isCollinear) {
                        smoothedPath.push_back(rawPath[i]);
                    }
                }
                
                smoothedPath.push_back(rawPath.back());
                return smoothedPath;
            }

            if (currentF > gScore[currentId] + heuristic(graph.getVertexData(currentId), endData) + 1e-9) {
                continue; 
            }

            const T& currentData = graph.getVertexData(currentId);

            for (const auto& edge : graph.adj(currentId)) {
                int neighborId = edge.target;
                const T& neighborData = graph.getVertexData(neighborId);

                double tentative_gScore = gScore[currentId] + edge.weight;

                if (tentative_gScore < gScore[neighborId]) {
                    cameFrom[neighborId] = currentId;
                    gScore[neighborId] = tentative_gScore;
                    
                    bool isDiagonal = (currentData.x != neighborData.x) && (currentData.y != neighborData.y);
                    double M = isDiagonal ? std::sqrt(2.0) : 1.0; 
                    
                    double f = tentative_gScore + heuristic(neighborData, endData) * M;
                    
                    openSet.push({f, neighborId});
                }
            }
        }
        
        return {};
    }
}