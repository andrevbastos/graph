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
#include "graph/common/node.hpp"
#include "graph/common/edge.hpp"

namespace util {
    struct Vertex2D {
        double x, y;

        Vertex2D(double x, double y) 
            : x(x), y(y) {}
            
        Vertex2D()
            : x(0), y(0) {}

        Vertex2D operator*(double scalar) const {
            return {x * scalar, y * scalar};
        }

        Vertex2D operator*(const Vertex2D& other) noexcept {
            return {x * other.x, y * other.y};
        }
    
        Vertex2D operator+(const Vertex2D& other) const {
            return {x + other.x, y + other.y};
        }

        Vertex2D operator-(const Vertex2D& other) const {
            return {x - other.x, y - other.y};
        }

        Vertex2D operator-() const {
            return {-x, -y};
        }

        bool operator==(const Vertex2D& other) const {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Vertex2D& other) const {
            return !(*this == other);
        }

        std::ostream& operator<<(std::ostream& os) const {
            return os << "<Vertex2D {" << x << ", " << y << "}>";
        }

        Vertex2D direction() const {
            return Vertex2D{
                x > 0 ? 1.0 : (x < 0 ? -1.0 : 0.0),
                y > 0 ? 1.0 : (y < 0 ? -1.0 : 0.0)
            };
        }
    };

    struct Vertex3D {
        double x, y, z;

        Vertex3D(double x, double y, double z) 
            : x(x), y(y), z(z) {}

        Vertex3D()
            : x(0), y(0), z(0) {}

        Vertex3D operator*(double scalar) const {
            return {x * scalar, y * scalar, z * scalar};
        }

        Vertex3D operator*(const Vertex3D& other) noexcept {
            return {x * other.x, y * other.y, z * other.z};
        }

        Vertex3D operator+(const Vertex3D& other) const {
            return {x + other.x, y + other.y, z + other.z};
        }

        Vertex3D operator-(const Vertex3D& other) const {
            return {x - other.x, y - other.y, z - other.z};
        }

        Vertex3D operator-() const {
            return {-x, -y, -z};
        }

        bool operator==(const Vertex3D& other) const {
            return x == other.x && y == other.y && z == other.z;
        }

        bool operator!=(const Vertex3D& other) const {
            return !(*this == other);
        }

        std::ostream& operator<<(std::ostream& os) const {
            return os << "<Vertex3D {" << x << ", " << y << ", " << z << "}>";
        }

        Vertex3D direction() const {
            return Vertex3D{
                x > 0 ? 1.0 : (x < 0 ? -1.0 : 0.0),
                y > 0 ? 1.0 : (y < 0 ? -1.0 : 0.0),
                z > 0 ? 1.0 : (z < 0 ? -1.0 : 0.0)
            };
        }
    };

    std::pair<bool, std::pair<double, double>> getCoords2D(common::Node* n);
    std::pair<bool, std::tuple<double, double, double>> getCoords3D(common::Node* n);
}