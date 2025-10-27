#pragma once

#include <cassert>
#include <utility>

namespace directed { class Node; };

namespace common {
    class Node;

    class Edge {
    public:
        Edge(int id, common::Node* n1, common::Node* n2)
            : _id(id), _nodes(n1, n2) {};

        int getId() const { return _id; };
        Node* getFirstNode() const { return _nodes.first;};
        Node* getSecondNode() const { return _nodes.second;};

        virtual void linkNodes() = 0;
        
    protected:
        std::pair<common::Node*, common::Node*> _nodes;

    private:
        int _id;
    };
};