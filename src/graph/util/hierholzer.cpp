#include "graph/util/hierholzer.hpp"

#include <unordered_map>

namespace util::Hierholzer {

    bool isEulerian(common::Graph* g) {
        for (auto v : g->getVertices()) {
            if (v->getOrder() % 2 != 0) return false;
        }
        return true;
    }

    std::vector<common::Node*> findEulerianCycle(common::Graph* graph) {
        std::vector<common::Node*> circuit;
        
        if (!graph || graph->getVertices().empty()) return circuit;

        common::Graph* g = graph->clone();

        if (!isEulerian(g)) {
            delete g;
            return circuit; 
        }

        std::stack<common::Node*> currentPath;
        std::vector<common::Node*> finalCircuit;

        common::Node* startNode = g->getVertices()[0];
        for(auto v : g->getVertices()){
            if(v->getOrder() > 0) {
                startNode = v;
                break;
            }
        }

        currentPath.push(startNode);

        while (!currentPath.empty()) {
            common::Node* u = currentPath.top();

            auto edges = u->edges();
            
            if (!edges.empty()) {
                common::Edge* e = edges.front();
                common::Node* v = g->getOpposite(u, e);

                currentPath.push(v);

                g->removeEdge(e);
            } else {
                finalCircuit.push_back(u);
                currentPath.pop();
            }
        }

        delete g;

        return finalCircuit;
    }
}