#pragma once

#include "graph/common/node.hpp"

namespace undirected {
    class Node : public common::Node
    {
    public:
        using common::Node::Node;
        
        void linkTo(common::Node* neighbor, common::Edge* edge) override;
        void unlinkFrom(common::Node* neighbor) override;
    };
};