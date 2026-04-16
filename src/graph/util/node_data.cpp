#include "graph/util/node_data.hpp"

namespace util {
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
}