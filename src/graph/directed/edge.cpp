#include "graph/directed/edge.hpp"
#include "graph/directed/node.hpp"

namespace directed {
    void Edge::linkNodes() {
        directed::Node* exitNode = dynamic_cast<directed::Node*>(_nodes.first);
        directed::Node* entryNode = dynamic_cast<directed::Node*>(_nodes.second);
        if (exitNode && entryNode)
            exitNode->linkTo(entryNode, this);
    };
}