#include "graph/util/path_smoothing.hpp"
#include <cmath>

namespace util {
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
}
