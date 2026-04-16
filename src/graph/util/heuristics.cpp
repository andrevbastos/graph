#include "graph/util/heuristics.hpp"

namespace util::heuristics {
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
}