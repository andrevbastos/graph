#include "graph/undirected/graph.hpp"
#include "graph/undirected/node.hpp"
#include "graph/undirected/edge.hpp"

namespace undirected {
    common::Node* Graph::newVertex()
    {
        int newId = nextNodeId++;
        auto newNode = std::make_unique<undirected::Node>(newId);
        common::Node* ptr = newNode.get();
        vertices.emplace(newId, std::move(newNode));

        return ptr;
    };

    common::Node* Graph::newVertex(std::any data)
    {
        int newId = nextNodeId++;
        auto newNode = std::make_unique<undirected::Node>(newId, data);
        common::Node* ptr = newNode.get();
        vertices.emplace(newId, std::move(newNode));

        return ptr;
    };

    common::Edge* Graph::newEdge(int v1_ID, int v2_ID, int weight) {
        common::Node* v1 = getVertex(v1_ID);
        common::Node* v2 = getVertex(v2_ID);
        return newEdge(v1, v2, weight);
    };

    common::Edge* Graph::newEdge(common::Node* v1, common::Node* v2, int weight)
    {
        if (!v1 || !v2) return nullptr;

        undirected::Node* dv1 = dynamic_cast<undirected::Node*>(v1);
        undirected::Node* dv2 = dynamic_cast<undirected::Node*>(v2);

        if (!dv1 || !dv2) return nullptr;

        int newId = nextEdgeId++;
        auto newEdge = std::make_unique<undirected::Edge>(newId, dv1, dv2);
        common::Edge* ptr = newEdge.get();
        edges.emplace(newId, std::move(newEdge));
        weights[ptr] = weight;
        
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
            v2->unlinkFrom(v1);
        }

        edges.erase(e->getId());
    };

    std::vector<std::vector<int>> Graph::getWeightMatrix() const {
        std::vector<common::Node*> nodes = getVertices();
        int n = nodes.size();
        
        std::unordered_map<int, int> tempIds;
        for (int i = 0; i < n; ++i) {
            tempIds[nodes[i]->getId()] = i;
        }

        int inf = std::numeric_limits<int>::max();
        std::vector<std::vector<int>> matrix(n, std::vector<int>(n, inf));

        for (int i = 0; i < n; ++i) {
            matrix[i][i] = 0;
        }

        for (const auto& pair : edges) {
            common::Edge* e = pair.second.get();
            int uId = tempIds.at(e->getFirstNode()->getId());
            int vId = tempIds.at(e->getSecondNode()->getId());

            matrix[uId][vId] = weights.at(e);
            matrix[vId][uId] = weights.at(e);
        }

        return matrix;
    };

    Graph* Graph::clone() const
    {
        Graph* newGraph = new undirected::Graph();

        std::unordered_map<int, common::Node*> oldIdToNewNode;

        for (common::Node* oldNode : this->getVertices()) {
            if (oldNode) {
                common::Node* newNode = newGraph->newVertex(oldNode->getData());
                oldIdToNewNode[oldNode->getId()] = newNode;
            }
        }

        for (common::Edge* oldEdge : this->getEdges()) {
            if (oldEdge) {
                auto nodes = this->getNodesFromEdge(oldEdge);
                
                common::Node* newV1 = oldIdToNewNode.at(nodes[0]->getId());
                common::Node* newV2 = oldIdToNewNode.at(nodes[1]->getId());
                
                newGraph->newEdge(newV1, newV2);
            }
        }

        return newGraph;
    };
};