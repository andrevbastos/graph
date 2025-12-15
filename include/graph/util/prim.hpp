#pragma once

#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>

#include "graph/common/graph.hpp"
#include "graph/common/node.hpp"

namespace util::Prim {
    std::vector<common::Edge*> getMST(common::Graph* g);
}