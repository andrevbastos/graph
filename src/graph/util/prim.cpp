#include "graph/util/prim.hpp"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <vector>

namespace util::Prim {
    std::vector<common::Edge*> getMST(common::Graph* g) 
    {
        std::vector<common::Edge*> mstEdges;
        
        if (!g || g->getOrder() == 0) {
            return mstEdges;
        }
        
        auto weights = g->getWeights();
        auto vertices = g->getVertices();
        
        std::unordered_map<common::Node*, std::vector<common::Edge*>> adj;
        for (auto* edge : g->getEdges()) {
            adj[edge->getFirstNode()].push_back(edge);
            adj[edge->getSecondNode()].push_back(edge);
        }

        using PQElement = std::pair<int, common::Node*>;
        std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> pq;

        std::unordered_set<common::Node*> inMST;
        std::unordered_map<common::Node*, int> key;
        std::unordered_map<common::Node*, common::Edge*> parent;

        int INF = std::numeric_limits<int>::max();
        for (auto* v : vertices) {
            key[v] = INF;
            parent[v] = nullptr;
        }

        common::Node* startNode = vertices[0];
        key[startNode] = 0;
        pq.push({0, startNode});

        while (!pq.empty()) {
            int uWeight = pq.top().first;
            common::Node* u = pq.top().second;
            pq.pop();

            if (inMST.find(u) != inMST.end()) {
                continue;
            }

            inMST.insert(u);
            
            if (parent[u] != nullptr) {
                mstEdges.push_back(parent[u]);
            }

            for (auto* edge : adj[u]) {
                common::Node* v = (edge->getFirstNode() == u) ? edge->getSecondNode() : edge->getFirstNode();
                
                int weight = weights.count(edge) ? weights.at(edge) : 1;

                if (inMST.find(v) == inMST.end() && weight < key[v]) {
                    key[v] = weight;
                    pq.push({key[v], v});
                    parent[v] = edge;
                }
            }
        }

        return mstEdges;
    }
}