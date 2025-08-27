#include "graph/undirected/edge.hpp"
#include "graph/undirected/node.hpp"

namespace undirected {
    void Edge::linkNodes() {
        undirected::Node* firstNode = dynamic_cast<undirected::Node*>(_nodes.first);
        undirected::Node* secondNode = dynamic_cast<undirected::Node*>(_nodes.second);
        if (firstNode && secondNode) {
            firstNode->linkTo(secondNode, this);
            secondNode->linkTo(firstNode, this);
        }
    };
}