#pragma once

#include "graph/common/node.hpp"

namespace directed {
    class Node : public common::Node
    {
    public:
        using common::Node::Node;

        void linkTo(common::Node* neighbor, common::Edge* edge) override;
        void unlinkFrom(common::Node* neighbor) override;

        std::vector<common::Node*> entry() const;
        std::vector<common::Edge*> entryEdges() const;

        void newEntry(common::Node* neighbor, common::Edge* edge);
        void removeEntry(common::Node* neighbor);

    private:
        std::unordered_map<common::Node*, common::Edge*> _entry;
    };
};