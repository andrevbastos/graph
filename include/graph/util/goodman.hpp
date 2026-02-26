#pragma once

#include <vector>

#include "graph/undirected/graph.hpp"
#include "graph/undirected/node.hpp"

namespace util::Goodman {
    int goodmanAlgorithm(common::Graph* g);
    void printComponents(common::Graph* g);
}