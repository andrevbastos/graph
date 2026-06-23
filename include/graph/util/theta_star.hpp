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
#include "graph/common/lw_grid.hpp"
#include "graph/util/node_data.hpp"
#include "graph/util/heuristics.hpp"

namespace util {
    std::vector<common::Node*> ThetaStar(common::Graph* graph, int startId, int endId, heuristics::HeuristicFunc heuristic, std::function<bool(common::Node*, common::Node*)> lineOfSight = nullptr);

    bool lwGridLineOfSight(int startId, int endId, const common::lwGrid& grid);

    template <typename T, typename = void>
    struct theta_has_z : std::false_type {};

    template <typename T>
    struct theta_has_z<T, std::void_t<decltype(std::declval<T>().z)>> : std::true_type {};

    template <typename T, typename = void>
    struct theta_has_xy : std::false_type {};

    template <typename T>
    struct theta_has_xy<T, std::void_t<decltype(std::declval<T>().x), decltype(std::declval<T>().y)>> : std::true_type {};

    template <typename T>
    double thetaLwDistance(const T& a, const T& b) {
        if constexpr (theta_has_xy<T>::value) {
            if constexpr (theta_has_z<T>::value) {
                return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2) + std::pow(a.z - b.z, 2));
            } else {
                return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
            }
        } else {
            return 0.0;
        }
    }

    template <typename T, typename Heuristic, typename LineOfSight = std::nullptr_t>
    std::vector<int> lwThetaStar(const common::lwGraph<T>& graph, int startId, int endId, Heuristic heuristic, LineOfSight lineOfSight = nullptr) 
    {
        int numVertices = graph.getOrder(); 
        
        if (startId < 0 || startId >= numVertices || endId < 0 || endId >= numVertices) return {};
        if (startId == endId) return {startId};

        std::vector<double> gScore(numVertices, std::numeric_limits<double>::infinity());
        std::vector<int> cameFrom(numVertices, -1);

        using Element = std::pair<double, int>;
        std::priority_queue<Element, std::vector<Element>, std::greater<Element>> openSet;

        gScore[startId] = 0.0;
        cameFrom[startId] = startId;
        
        const T& startData = graph.getVertexData(startId);
        const T& endData = graph.getVertexData(endId);
        
        openSet.push({heuristic(startData, endData), startId});

        while (!openSet.empty()) {
            auto [currentF, currentId] = openSet.top();
            openSet.pop();

            if (currentId == endId) {
                std::vector<int> path;
                int curr = endId;
                while (curr != startId && curr != -1) {
                    path.push_back(curr);
                    curr = cameFrom[curr];
                }
                path.push_back(startId);
                std::reverse(path.begin(), path.end());
                return path;
            }

            if (currentF > gScore[currentId] + heuristic(graph.getVertexData(currentId), endData) + 1e-9) {
                continue; 
            }

            int parentId = cameFrom[currentId];
            const T& parentData = graph.getVertexData(parentId);

            for (const auto& edge : graph.adj(currentId)) {
                int neighborId = edge.target;
                const T& neighborData = graph.getVertexData(neighborId);

                bool hasLos = false;
                if constexpr (!std::is_same_v<LineOfSight, std::nullptr_t>) {
                    if (lineOfSight) {
                        hasLos = lineOfSight(parentId, neighborId);
                    }
                }

                if (hasLos) {
                    double dist = thetaLwDistance(parentData, neighborData);
                    double tentative_gScore = gScore[parentId] + dist;

                    if (tentative_gScore < gScore[neighborId]) {
                        cameFrom[neighborId] = parentId;
                        gScore[neighborId] = tentative_gScore;
                        double f = tentative_gScore + heuristic(neighborData, endData);
                        openSet.push({f, neighborId});
                    }
                } else {
                    double tentative_gScore = gScore[currentId] + edge.weight;

                    if (tentative_gScore < gScore[neighborId]) {
                        cameFrom[neighborId] = currentId;
                        gScore[neighborId] = tentative_gScore;
                        double f = tentative_gScore + heuristic(neighborData, endData);
                        openSet.push({f, neighborId});
                    }
                }
            }
        }

        return {};
    }
}
