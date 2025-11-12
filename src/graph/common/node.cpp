#include "graph/common/node.hpp"

namespace common {
    Node::Node(int id, std::any data)
        : _id(id), _data(data) {};

    void Node::linkTo(common::Node* neighbor, common::Edge* edge) {
        _adj[neighbor] = edge;
    }

    void Node::unlinkFrom(common::Node* neighbor) {
        if (_adj.count(neighbor)) {
            _adj.erase(neighbor);
        }
    }

    int Node::getId() const { return _id; };
    Edge* Node::getEdgeTo(common::Node* neighbor) const { return (_adj.count(neighbor) > 0) ? _adj.at(neighbor) : nullptr; };
    int Node::getOrder() const { return _adj.size(); }
    std::any Node::getData() const { return _data; };
    bool Node::hasData() const { return _data.has_value(); };

    std::vector<common::Node*> Node::adj() const  {
        std::vector<common::Node*> result;
        result.reserve(_adj.size());
        for (const auto &pair : _adj)
            result.push_back(pair.first);

        return result;
    };
    std::vector<common::Edge*> Node::edges() const  {
        std::vector<common::Edge*> result;
        result.reserve(_adj.size());
        for (const auto &pair : _adj)
            result.push_back(pair.second);

        return result;
    };

    bool Node::adjTo(common::Node* neighbor) const {
        return _adj.count(neighbor) > 0;
    };
}