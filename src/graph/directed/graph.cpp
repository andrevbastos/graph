#include "graph/directed/graph.hpp"
#include "graph/directed/node.hpp"
#include "graph/directed/edge.hpp"

namespace directed {
    common::Node* Graph::newVertex()
    {
        int newId = nextNodeId++;
        auto newNode = std::make_unique<directed::Node>(newId);
        common::Node* ptr = newNode.get();
        vertices.emplace(newId, std::move(newNode));

        return ptr;
    };

    common::Edge* Graph::newEdge(int v1_ID, int v2_ID) {
        return newEdge(getVertex(v1_ID), getVertex(v2_ID));
    };

    common::Edge* Graph::newEdge(common::Node* v1, common::Node* v2)
    {
        if (!v1 || !v2) return nullptr;

        directed::Node* dv1 = dynamic_cast<directed::Node*>(v1);
        directed::Node* dv2 = dynamic_cast<directed::Node*>(v2);

        if (!dv1 || !dv2) return nullptr;

        int newId = nextEdgeId++;
        auto newEdge = std::make_unique<directed::Edge>(newId, v1, v2);
        common::Edge* ptr = newEdge.get();
        edges.emplace(newId, std::move(newEdge));
        
        ptr->linkNodes();

        return ptr;
    };

    void Graph::removeVertex(int v_ID) {
        removeVertex(getVertex(v_ID));
    }
    
    void Graph::removeVertex(common::Node* v) {
        if (!v || vertices.find(v->getId()) == vertices.end()) {
            return;
        }
        
        std::vector<common::Edge*> edges_to_remove;
        for (const auto& pair : edges) {
            if (pair.second->getFirstNode() == v || pair.second->getSecondNode() == v) {
                edges_to_remove.push_back(pair.second.get());
            }
        }
        
        for (common::Edge* edge : edges_to_remove) {
            removeEdge(edge);
        }
        
        vertices.erase(v->getId());
    }

    void Graph::removeEdge(int e_ID) {
        removeEdge(getEdge(e_ID));
    }

    void Graph::removeEdge(common::Edge* e) {
        if (!e || edges.find(e->getId()) == edges.end()) {
            return;
        }

        common::Node* v1 = e->getFirstNode();
        common::Node* v2 = e->getSecondNode();

        if (v1 && v2) {
            v1->unlinkFrom(v2);
        }

        edges.erase(e->getId());
    }
};