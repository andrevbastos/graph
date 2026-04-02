#include "graph/util/a_star.hpp"

namespace util::AStar {
    std::vector<common::Node*> getPath(common::Graph* graph, int startId, int endId, HeuristicFunc heuristic) 
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

    std::pair<bool, std::pair<double, double>> getCoords2D(common::Node* n) 
    {
        if (!n->hasData()) return {false, {0.0, 0.0}};
        const std::any& data = n->getData();
        if (data.type() == typeid(std::pair<double, double>)) {
            return {true, std::any_cast<std::pair<double, double>>(data)};
        } else if (data.type() == typeid(std::pair<int, int>)) {
            auto p = std::any_cast<std::pair<int, int>>(data);
            return {true, {static_cast<double>(p.first), static_cast<double>(p.second)}};
        } else if (data.type() == typeid(std::pair<float, float>)) {
            auto p = std::any_cast<std::pair<float, float>>(data);
            return {true, {static_cast<double>(p.first), static_cast<double>(p.second)}};
        }
        return {false, {0.0, 0.0}};
    };

    std::pair<bool, std::tuple<double, double, double>> getCoords3D(common::Node* n) 
    {
        if (!n->hasData()) return {false, {0.0, 0.0, 0.0}};
        const std::any& data = n->getData();
        if (data.type() == typeid(std::tuple<double, double, double>)) {
            return {true, std::any_cast<std::tuple<double, double, double>>(data)};
        } else if (data.type() == typeid(std::tuple<int, int, int>)) {
            auto t = std::any_cast<std::tuple<int, int, int>>(data);
            return {true, {static_cast<double>(std::get<0>(t)), static_cast<double>(std::get<1>(t)), static_cast<double>(std::get<2>(t))}};
        } else if (data.type() == typeid(std::tuple<float, float, float>)) {
            auto t = std::any_cast<std::tuple<float, float, float>>(data);
            return {true, {static_cast<double>(std::get<0>(t)), static_cast<double>(std::get<1>(t)), static_cast<double>(std::get<2>(t))}};
        }
        return {false, {0.0, 0.0, 0.0}};
    };

    double zeroHeuristic(common::Node*, common::Node*) { return 0.0; };

    double euclideanHeuristic2D(common::Node* a, common::Node* b) 
    {
        auto [validA, coordsA] = getCoords2D(a);
        auto [validB, coordsB] = getCoords2D(b);

        if (!validA || !validB) return 0.0;

        auto [xA, yA] = coordsA;
        auto [xB, yB] = coordsB;

        return std::sqrt(std::pow(xA - xB, 2) + std::pow(yA - yB, 2));
    };

    double euclideanHeuristic3D(common::Node* a, common::Node* b) 
    {
        auto [validA, coordsA] = getCoords3D(a);
        auto [validB, coordsB] = getCoords3D(b);

        if (!validA || !validB) return 0.0;

        auto [xA, yA, zA] = coordsA;
        auto [xB, yB, zB] = coordsB;

        return std::sqrt(std::pow(xA - xB, 2) + std::pow(yA - yB, 2) + std::pow(zA - zB, 2));
    };

    double manhattanHeuristic2D(common::Node* a, common::Node* b) 
    {
        auto [validA, coordsA] = getCoords2D(a);
        auto [validB, coordsB] = getCoords2D(b);

        if (!validA || !validB) return 0.0;

        auto [xA, yA] = coordsA;
        auto [xB, yB] = coordsB;

        return std::abs(xA - xB) + std::abs(yA - yB);
    };

    double manhattanHeuristic3D(common::Node* a, common::Node* b) 
    {
        auto [validA, coordsA] = getCoords3D(a);
        auto [validB, coordsB] = getCoords3D(b);

        if (!validA || !validB) return 0.0;

        auto [xA, yA, zA] = coordsA;
        auto [xB, yB, zB] = coordsB;

        return std::abs(xA - xB) + std::abs(yA - yB) + std::abs(zA - zB);
    };

    double chebyshevHeuristic2D(common::Node* a, common::Node* b) 
    {
            auto [validA, coordsA] = getCoords2D(a);
            auto [validB, coordsB] = getCoords2D(b);

            if (!validA || !validB) return 0.0;

            auto [xA, yA] = coordsA;
            auto [xB, yB] = coordsB;

            return std::max(std::abs(xA - xB), std::abs(yA - yB));
    };

    double chebyshevHeuristic3D(common::Node* a, common::Node* b) 
    {
        auto [validA, coordsA] = getCoords3D(a);
        auto [validB, coordsB] = getCoords3D(b);

        if (!validA || !validB) return 0.0;

        auto [xA, yA, zA] = coordsA;
        auto [xB, yB, zB] = coordsB;
        
        return std::max({std::abs(xA - xB), std::abs(yA - yB), std::abs(zA - zB)});
    };
};