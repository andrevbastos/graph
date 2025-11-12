#pragma once

#include "graph/common/graph.hpp"
#include "graph/util/floyd.hpp"

namespace util::Postman {
    std::vector<common::Node*> findDeliveryRoute(common::Graph* graph, common::Node* start);
};