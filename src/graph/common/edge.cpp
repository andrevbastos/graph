#include "graph/common/edge.hpp"

namespace common {
    Edge::Edge(int id, common::Node* n1, common::Node* n2)
        : _id(id), _nodes(n1, n2) {};

    int Edge::getId() const { return _id; };
    Node* Edge::getFirstNode() const { return _nodes.first;};
    Node* Edge::getSecondNode() const { return _nodes.second;};
};