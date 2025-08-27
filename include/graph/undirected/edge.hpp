#pragma once

#include "graph/common/edge.hpp"

namespace undirected {
    class Edge : public common::Edge {
    public:
        using common::Edge::Edge;
      
        virtual void linkNodes() override;
    };
};