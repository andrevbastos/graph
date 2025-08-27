#pragma once

#include "graph/common/node.hpp"

namespace undirected {
    class Node : public common::Node
    {
    public:
        using common::Node::Node;
        
        virtual void linkTo(common::Node* neighbor, common::Edge* edge) override;
        virtual void unlinkFrom(common::Node* neighbor) override;
    };
};