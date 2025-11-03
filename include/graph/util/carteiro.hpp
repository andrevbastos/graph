#pragma once

#include "graph/common/graph.hpp"

namespace util::Carteiro {
    std::vector<common::Node*> findDeliveryRoute(common::Graph* graph, common::Node* start)
    {
        std::vector<common::Node*> route;

        auto vertices = graph->getVertices();
        std::vector<common::Node*> vPar, vImpar;
        for (auto v : vertices) {
            if (v->getOrder() % 2 == 0)
                vPar.push_back(v);
            else
                vImpar.push_back(v);
        }

        

        return route;
    };
}