#pragma once

#include <vector>
#include <unordered_map>

#include "graph/common/graph.hpp"
#include "graph/common/node.hpp"
#include "graph/util/union_find.hpp"

namespace util::Kruskal {
    std::vector<common::Edge*> getMST(common::Graph* g);
}