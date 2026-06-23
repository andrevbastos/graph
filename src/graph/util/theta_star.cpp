#include "graph/util/theta_star.hpp"
#include <cmath>

namespace util {
    double thetaEuclideanDistance(common::Node* a, common::Node* b) {
        auto [valid3D, coords3D] = getCoords3D(a);
        if (valid3D) {
            auto [valid3DB, coords3D_B] = getCoords3D(b);
            if (valid3DB) {
                auto [xA, yA, zA] = coords3D;
                auto [xB, yB, zB] = coords3D_B;
                return std::sqrt(std::pow(xA - xB, 2) + std::pow(yA - yB, 2) + std::pow(zA - zB, 2));
            }
        }
        auto [valid2D, coords2D] = getCoords2D(a);
        if (valid2D) {
            auto [valid2DB, coords2D_B] = getCoords2D(b);
            if (valid2DB) {
                auto [xA, yA] = coords2D;
                auto [xB, yB] = coords2D_B;
                return std::sqrt(std::pow(xA - xB, 2) + std::pow(yA - yB, 2));
            }
        }
        return 0.0;
    }

    std::vector<common::Node*> ThetaStar(common::Graph* graph, int startId, int endId, heuristics::HeuristicFunc heuristic, std::function<bool(common::Node*, common::Node*)> lineOfSight) 
    {
        if (!graph) return {};
        common::Node* startNode = graph->getVertex(startId);
        common::Node* endNode = graph->getVertex(endId);
        if (!startNode || !endNode) return {};

        if (startId == endId) return {startNode};

        auto edgeWeights = graph->getWeights();
        
        using Element = std::pair<double, int>;
        std::priority_queue<Element, std::vector<Element>, std::greater<Element>> openSet;
        std::unordered_map<int, double> gScore;
        std::unordered_map<int, int> cameFrom;

        gScore[startId] = 0.0;
        cameFrom[startId] = startId;
        double startF = heuristic(startNode, endNode);
        openSet.push({startF, startId});

        while (!openSet.empty()) {
            auto [currentF, currentId] = openSet.top();
            openSet.pop();

            if (currentId == endId) {
                std::vector<common::Node*> path;
                int curr = endId;
                while (curr != startId) {
                    path.push_back(graph->getVertex(curr));
                    curr = cameFrom[curr];
                }
                path.push_back(startNode);
                std::reverse(path.begin(), path.end());
                return path;
            }

            common::Node* current = graph->getVertex(currentId);
            if (currentF > gScore[currentId] + heuristic(current, endNode) + 1e-9) {
                continue;
            }

            for (common::Node* neighbor : current->adj()) {
                int neighborId = neighbor->getId();
                
                int parentId = cameFrom[currentId];
                common::Node* parentNode = graph->getVertex(parentId);
                
                if (lineOfSight && lineOfSight(parentNode, neighbor)) {
                    double dist = thetaEuclideanDistance(parentNode, neighbor);
                    double tentative_gScore = gScore[parentId] + dist;
                    
                    if (gScore.find(neighborId) == gScore.end() || tentative_gScore < gScore[neighborId]) {
                        cameFrom[neighborId] = parentId;
                        gScore[neighborId] = tentative_gScore;
                        double f = tentative_gScore + heuristic(neighbor, endNode);
                        openSet.push({f, neighborId});
                    }
                } else {
                    common::Edge* edge = current->getEdgeTo(neighbor);
                    double weight = 1.0;
                    if (edge && edgeWeights.find(edge) != edgeWeights.end()) {
                        weight = static_cast<double>(edgeWeights.at(edge));
                    }
                    double tentative_gScore = gScore[currentId] + weight;
                    
                    if (gScore.find(neighborId) == gScore.end() || tentative_gScore < gScore[neighborId]) {
                        cameFrom[neighborId] = currentId;
                        gScore[neighborId] = tentative_gScore;
                        double f = tentative_gScore + heuristic(neighbor, endNode);
                        openSet.push({f, neighborId});
                    }
                }
            }
        }

        return {};
    }

    bool lwGridLineOfSight(int startId, int endId, const common::lwGrid& grid) {
        int w = grid.getWidth();
        int h = grid.getHeight();
        if (w <= 0 || h <= 0) return false;

        int x0 = startId % w;
        int y0 = startId / w;
        int x1 = endId % w;
        int y1 = endId / w;

        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        int x = x0;
        int y = y0;

        while (true) {
            if (!grid.isPassable(x, y)) {
                return false;
            }
            if (x == x1 && y == y1) {
                break;
            }
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x += sx;
            }
            if (e2 < dx) {
                err += dx;
                y += sy;
            }
        }
        return true;
    }
}
