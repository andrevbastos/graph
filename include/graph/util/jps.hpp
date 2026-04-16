#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <cmath>
#include <graph/undirected/graph.hpp>
#include <graph/util/heuristics.hpp>
#include <graph/util/node_data.hpp>

namespace util {
    struct JPSNode {
        double fScore;
        int id;
        bool operator>(const JPSNode& other) const { return fScore > other.fScore; }
    };

    std::pair<int, int> getDirection(common::Graph* graph, int fromId, int toId);
    int jump(common::Graph* graph, int currentId, int dirX, int dirY, int endId);
    std::vector<common::Node*> JumpPointSearch(common::Graph* graph, int startId, int endId, heuristics::HeuristicFunc heuristic);

    template <typename T>
    std::vector<int> lwJumpPointSearch(const common::lwGraph<T>& graph, int startId, int endId, std::function<double(const T&, const T&)> heuristic) 
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

        auto hasNeighborAt = [&](int nodeId, double targetX, double targetY) -> bool {
            for (const auto& edge : graph.adj(nodeId)) {
                const T& nData = graph.getVertexData(edge.target);
                if (std::abs(nData.x - targetX) < 1e-4 && std::abs(nData.y - targetY) < 1e-4) {
                    return true;
                }
            }
            return false;
        };

        auto jump = [&](int currentId, int dirX, int dirY, auto& jumpRef) -> int {
            int nextId = currentId;

            while (true) {
                const T& currentData = graph.getVertexData(nextId);
                double targetX = currentData.x + dirX;
                double targetY = currentData.y + dirY;

                int nextNodeId = -1;

                for (const auto& edge : graph.adj(nextId)) {
                    const T& nData = graph.getVertexData(edge.target);
                    if (std::abs(nData.x - targetX) < 1e-4 && std::abs(nData.y - targetY) < 1e-4) {
                        nextNodeId = edge.target;
                        break;
                    }
                }

                if (nextNodeId == -1) return -1;
                if (nextNodeId == endId) return nextNodeId;

                const T& nextData = graph.getVertexData(nextNodeId);
                double nx = nextData.x;
                double ny = nextData.y;

                if (dirX != 0 && dirY == 0) {
                    if ((!hasNeighborAt(nextNodeId, nx, ny + 1) && hasNeighborAt(nextNodeId, nx + dirX, ny + 1)) ||
                        (!hasNeighborAt(nextNodeId, nx, ny - 1) && hasNeighborAt(nextNodeId, nx + dirX, ny - 1))) {
                        return nextNodeId;
                    }
                } else if (dirX == 0 && dirY != 0) {
                    if ((!hasNeighborAt(nextNodeId, nx + 1, ny) && hasNeighborAt(nextNodeId, nx + 1, ny + dirY)) ||
                        (!hasNeighborAt(nextNodeId, nx - 1, ny) && hasNeighborAt(nextNodeId, nx - 1, ny + dirY))) {
                        return nextNodeId;
                    }
                } else {
                    if ((!hasNeighborAt(nextNodeId, nx - dirX, ny) && hasNeighborAt(nextNodeId, nx - dirX, ny + dirY)) ||
                        (!hasNeighborAt(nextNodeId, nx, ny - dirY) && hasNeighborAt(nextNodeId, nx + dirX, ny - dirY))) {
                        return nextNodeId;
                    }
                    if (jumpRef(nextNodeId, dirX, 0, jumpRef) != -1 || jumpRef(nextNodeId, 0, dirY, jumpRef) != -1) {
                        return nextNodeId;
                    }
                }

                nextId = nextNodeId;
            }
        };

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

            int parentId = cameFrom[currentId];
            const T& currentData = graph.getVertexData(currentId);

            for (const auto& edge : graph.adj(currentId)) {
                int neighborId = edge.target;
                if (neighborId == parentId) continue;

                const T& neighborData = graph.getVertexData(neighborId);

                int ndx = 0, ndy = 0;
                double dx = neighborData.x - currentData.x;
                double dy = neighborData.y - currentData.y;
                
                if (std::abs(dx) > 1e-4) ndx = (dx > 0) ? 1 : -1;
                if (std::abs(dy) > 1e-4) ndy = (dy > 0) ? 1 : -1;

                if (ndx == 0 && ndy == 0) continue;

                int jumpId = jump(currentId, ndx, ndy, jump);
                if (jumpId == -1) continue;

                const T& jumpData = graph.getVertexData(jumpId);
                double jumpDistance = std::sqrt(std::pow(jumpData.x - currentData.x, 2) + std::pow(jumpData.y - currentData.y, 2));
                
                double tentative_gScore = gScore[currentId] + jumpDistance;

                if (tentative_gScore < gScore[jumpId]) {
                    cameFrom[jumpId] = currentId;
                    gScore[jumpId] = tentative_gScore;
                    
                    double fScore = tentative_gScore + heuristic(jumpData, endData);
                    openSet.push({fScore, jumpId});
                }
            }
        }

        return {};
    }
}