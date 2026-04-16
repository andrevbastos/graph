#include "graph/util/a_star.hpp"

namespace util {
    std::vector<common::Node*> AStar(common::Graph* graph, int startId, int endId, heuristics::HeuristicFunc heuristic) 
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
        double startF = heuristic(startNode, endNode);
        openSet.push({startF, startId});

        while (!openSet.empty()) {
            auto [currentF, currentId] = openSet.top();
            openSet.pop();

            if (currentF > gScore[currentId] + heuristic(graph->getVertex(currentId), endNode) + 1e-9) {
                continue;
            }   

            if (currentId == endId) {
                std::vector<common::Node*> path;
                int curr = endId;
                while (curr != startId && cameFrom.find(curr) != cameFrom.end()) {
                    path.push_back(graph->getVertex(curr));
                    curr = cameFrom[curr];
                }
                path.push_back(startNode);
                std::reverse(path.begin(), path.end());
                return path;
            }

            common::Node* current = graph->getVertex(currentId);

            for (common::Node* neighbor : current->adj()) {
                int neighborId = neighbor->getId();
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

        return {};
    };

    std::vector<common::Node*> AStarMod(common::Graph* graph, int startId, int endId, util::heuristics::HeuristicFunc heuristic)
    {
        auto* SV = graph->getVertex(startId);
        auto* EV = graph->getVertex(endId);
        auto W = graph->getWeights();

        std::unordered_map<common::Node*, common::Node*> VM;
        std::unordered_map<common::Node*, double> CM;
        
        std::priority_queue<std::pair<double, common::Node*>, std::vector<std::pair<double, common::Node*>>, std::greater<>> VPQ;

        VPQ.push({0.0, SV});
        VM[SV] = nullptr;
        CM[SV] = 0.0;

        while (!VPQ.empty()) {
            auto AV = VPQ.top().second;
            VPQ.pop();

            if (AV == EV) {
                std::vector<common::Node*> VL;
                for (auto* node = EV; node != nullptr; node = VM[node]) {
                    VL.push_back(node);
                }
                std::reverse(VL.begin(), VL.end());

                return VL; 
            }

            auto NV = AV->adj();
            for (auto* neighbor : NV) {
                auto VC = CM[AV] + W[AV->getEdgeTo(neighbor)];
                
                if (CM.find(neighbor) == CM.end() || VC < CM[neighbor]) {
                    CM[neighbor] = VC;
                    
                    auto [validAV, coordsAV] = util::getCoords3D(AV);
                    auto [validNeighbor, coordsNeighbor] = util::getCoords3D(neighbor);

                    if (!validAV || !validNeighbor) continue;

                    auto [ax, ay, az] = coordsAV;
                    auto [nx, ny, nz] = coordsNeighbor;

                    bool isDiagonal = (ax != nx) && (ay != ny);
                    
                    double M = isDiagonal ? std::sqrt(2.0) : 1.0; 
                    auto NVP = VC + heuristic(neighbor, EV) * M;
                    
                    VPQ.push({NVP, neighbor});
                    VM[neighbor] = AV;
                }
            }
        }
        
        return {};
    }

    std::vector<common::Node*> pathSmoothing(const std::vector<common::Node*>& rawPath)
    {
        if (rawPath.size() <= 2) return rawPath;

        std::vector<common::Node*> smoothedPath;
        smoothedPath.push_back(rawPath[0]);

        for (size_t i = 1; i < rawPath.size() - 1; ++i) {
            common::Node* P1 = smoothedPath.back();
            common::Node* P2 = rawPath[i];
            common::Node* P3 = rawPath[i+1];

            auto [valid1, coords1] = util::getCoords3D(P1);
            auto [valid2, coords2] = util::getCoords3D(P2);
            auto [valid3, coords3] = util::getCoords3D(P3);

            if (!valid1 || !valid2 || !valid3) return {};

            auto [x1, y1, z1] = coords1;
            auto [x2, y2, z2] = coords2;
            auto [x3, y3, z3] = coords3;

            float crossProduct = (x2 - x1) * (y3 - y2) - (y2 - y1) * (x3 - x2);
            bool isCollinear = std::abs(crossProduct) < 0.001f;

            if (!isCollinear) {
                smoothedPath.push_back(P2);
            }
        }
        
        smoothedPath.push_back(rawPath.back());
        return smoothedPath;
    }
};