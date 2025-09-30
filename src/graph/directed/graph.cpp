#include "graph/directed/graph.hpp"
#include "graph/directed/node.hpp"
#include "graph/directed/edge.hpp"

namespace directed {
    common::Node* Graph::newVertex(std::string label)
    {
        int newId = nextNodeId++;
        auto newNode = std::make_unique<directed::Node>(newId, label);
        common::Node* ptr = newNode.get();
        vertices.emplace(newId, std::move(newNode));

        return ptr;
    };

    common::Edge* Graph::newEdge(int v1_ID, int v2_ID) {
        common::Node* v1 = getVertex(v1_ID);
        common::Node* v2 = getVertex(v2_ID);
        return newEdge(v1, v2);
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
    };

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
    };

    Graph* Graph::clone() const
    {
        Graph* newGraph = new directed::Graph();

        std::unordered_map<int, common::Node*> oldIdToNewNode;

        for (common::Node* oldNode : this->getVertices()) {
            if (oldNode) {
                common::Node* newNode = newGraph->newVertex(oldNode->getLabel());
                
                oldIdToNewNode[oldNode->getId()] = newNode;
            }
        }

        for (common::Edge* oldEdge : this->getEdges()) {
            if (oldEdge) {
                directed::Edge* directedOldEdge = static_cast<directed::Edge*>(oldEdge);
                common::Node* sourceNode = directedOldEdge->getFirstNode();
                common::Node* destinationNode = directedOldEdge->getSecondNode();

                common::Node* newSource = oldIdToNewNode.at(sourceNode->getId());
                common::Node* newDestination = oldIdToNewNode.at(destinationNode->getId());
                
                newGraph->newEdge(newSource, newDestination);
            }
        }

        return newGraph;
    };
};