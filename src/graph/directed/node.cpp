#include "graph/directed/node.hpp"
#include "graph/directed/edge.hpp"

namespace directed {
    void Node::linkTo(common::Node* neighbor, common::Edge* edge) {
        directed::Node* directedNeighbor = dynamic_cast<directed::Node*>(neighbor);
        directed::Edge* directedEdge = dynamic_cast<directed::Edge*>(edge);
        if (directedNeighbor && directedEdge) {
            common::Node::linkTo(directedNeighbor, directedEdge);
            directedNeighbor->newEntry(this, directedEdge);
        }
    };

    void Node::unlinkFrom(common::Node* neighbor) {
        directed::Node* directedNeighbor = dynamic_cast<directed::Node*>(neighbor);
        if (directedNeighbor) {
            common::Node::unlinkFrom(neighbor);
            directedNeighbor->removeEntry(this);
        }
    };

    void Node::newEntry(common::Node* neighbor, common::Edge* edge) {
        _entry[neighbor] = edge;
    };

    void Node::removeEntry(common::Node* neighbor) {
        _entry.erase(neighbor);
    };

    std::vector<common::Node*> Node::entry() const {
        std::vector<common::Node*> result;
        result.reserve(_entry.size());
        for (const auto &pair : _entry)
            result.push_back(pair.first);

        return result;
    };

    std::vector<common::Edge*> Node::entryEdges() const {
        std::vector<common::Edge*> result;
        result.reserve(_entry.size());
        for (const auto &pair : _entry)
            result.push_back(pair.second);

        return result;
    };
}