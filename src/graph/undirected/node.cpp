#include "graph/undirected/node.hpp"
#include "graph/undirected/edge.hpp"

namespace undirected {
void Node::linkTo(common::Node* neighbor, common::Edge* edge) {
        undirected::Node* undirectedNeighbor = dynamic_cast<undirected::Node*>(neighbor);
        undirected::Edge* undirectedEdge = dynamic_cast<undirected::Edge*>(edge);
        
        if (undirectedNeighbor && undirectedEdge) 
            common::Node::linkTo(undirectedNeighbor, undirectedEdge);
    };

    void Node::unlinkFrom(common::Node* neighbor) {
        undirected::Node* undirectedNeighbor = dynamic_cast<undirected::Node*>(neighbor);
        
        if (undirectedNeighbor)
            common::Node::unlinkFrom(neighbor);
    };
};