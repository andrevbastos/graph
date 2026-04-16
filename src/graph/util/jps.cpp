#include "graph/util/jps.hpp"

namespace util {
    std::pair<int, int> getDirection(common::Graph* graph, int fromId, int toId) {
        common::Node* from = graph->getVertex(fromId);
        common::Node* to = graph->getVertex(toId);

        if (!from || !to) return {0, 0};

        auto [valid1, coord1] = getCoords2D(to);
        auto [valid2, coord2] = getCoords2D(from);

        if (!valid1 || !valid2) return {0, 0};

        auto [x1, y1] = coord1;
        auto [x2, y2] = coord2;

        int dx = static_cast<int>(x1 - x2);
        int dy = static_cast<int>(y1 - y2);

        if (dx != 0) dx /= std::abs(dx);
        if (dy != 0) dy /= std::abs(dy);

        return {dx, dy};
    }

    int jump(common::Graph* graph, int currentId, int dirX, int dirY, int endId) {
        int nextId = currentId;

        auto hasNeighborAt = [&](common::Node* node, double targetX, double targetY) {
            for (const auto& n : node->adj()) {
                auto [v, c] = getCoords2D(n);
                if (v && c.first == targetX && c.second == targetY) return true;
            }
            return false;
        };

        while (true) {
            auto [valid, coords] = getCoords2D(graph->getVertex(nextId));
            if (!valid) return -1;

            auto [x, y] = coords;
            int targetX = x + dirX;
            int targetY = y + dirY;

            int nextNodeId = -1;
            for (const auto& neighbor : graph->getVertex(nextId)->adj()) {
                auto [nValid, nCoords] = getCoords2D(neighbor);
                if (!nValid) continue;

                auto [nx, ny] = nCoords;
                if (nx == targetX && ny == targetY) {
                    nextNodeId = neighbor->getId();
                    break;
                }
            }

            if (nextNodeId == -1) return -1;
            
            if (nextNodeId == endId) return nextNodeId;

            common::Node* nextNode = graph->getVertex(nextNodeId);
            double nx = targetX;
            double ny = targetY;

            if (dirX != 0 && dirY == 0) {
                if ((!hasNeighborAt(nextNode, nx, ny + 1) && hasNeighborAt(nextNode, nx + dirX, ny + 1)) ||
                    (!hasNeighborAt(nextNode, nx, ny - 1) && hasNeighborAt(nextNode, nx + dirX, ny - 1))) {
                    return nextNodeId;
                }
            } else if (dirX == 0 && dirY != 0) {
                if ((!hasNeighborAt(nextNode, nx + 1, ny) && hasNeighborAt(nextNode, nx + 1, ny + dirY)) ||
                    (!hasNeighborAt(nextNode, nx - 1, ny) && hasNeighborAt(nextNode, nx - 1, ny + dirY))) {
                    return nextNodeId;
                }
            } else {
                if ((!hasNeighborAt(nextNode, nx - dirX, ny) && hasNeighborAt(nextNode, nx - dirX, ny + dirY)) ||
                    (!hasNeighborAt(nextNode, nx, ny - dirY) && hasNeighborAt(nextNode, nx + dirX, ny - dirY))) {
                    return nextNodeId;
                }
                
                if (jump(graph, nextNodeId, dirX, 0, endId) != -1 || 
                    jump(graph, nextNodeId, 0, dirY, endId) != -1) {
                    return nextNodeId;
                }
            }

            nextId = nextNodeId;
        }
    }

    std::vector<common::Node*> JumpPointSearch(common::Graph *graph, int startId, int endId, util::heuristics::HeuristicFunc heuristic)
    {
        if (!graph) return {};
        if (startId == endId) return {graph->getVertex(startId)};

        std::priority_queue<JPSNode, std::vector<JPSNode>, std::greater<JPSNode>> openSet;
        std::unordered_map<int, double> gScore;
        std::unordered_map<int, int> cameFrom;

        openSet.push({0.0, startId});
        gScore[startId] = 0.0;

        while (!openSet.empty()) {
            int currentId = openSet.top().id;
            openSet.pop();

            if (currentId == endId) {
                std::vector<common::Node*> path;
                int currId = endId;
                while (currId != startId) {
                    path.push_back(graph->getVertex(currId));
                    currId = cameFrom[currId];
                }
                path.push_back(graph->getVertex(startId));
                std::reverse(path.begin(), path.end());
                return path;
            }

            common::Node* current = graph->getVertex(currentId);
            int parentId = cameFrom.count(currentId) ? cameFrom[currentId] : -1;

            for (const auto& neighbor : current->adj()) {
                int neighborId = neighbor->getId();
                if (neighborId == parentId) continue;

                auto [ndx, ndy] = getDirection(graph, currentId, neighborId);
                if (ndx == 0 && ndy == 0) continue;

                int jumpId = jump(graph, currentId, ndx, ndy, endId);
                
                if (jumpId == -1) continue;

                double tentative_gScore = gScore[currentId] + heuristic(current, graph->getVertex(jumpId));
                
                if (!gScore.count(jumpId) || tentative_gScore < gScore[jumpId]) {
                    cameFrom[jumpId] = currentId;
                    gScore[jumpId] = tentative_gScore;
                    
                    double fScore = tentative_gScore + heuristic(graph->getVertex(jumpId), graph->getVertex(endId));
                    openSet.push({fScore, jumpId});
                }
            }
        }

        return {};
    }
}