#include "graph/util/postman.hpp"

namespace util::Postman {
    std::vector<common::Node*> findDeliveryRoute(common::Graph* graph, common::Node* start)
    {
        std::vector<common::Node*> route;

        // Inicialização
        auto vertices = graph->getVertices();
        std::vector<common::Node*> evenVertices;
        for (auto v : vertices) {
            if (v->getOrder() % 2 == 0) evenVertices.push_back(v);
        }
        auto g = graph->clone();

        // Calcula os caminhos mínimos
        auto [distances, routes] = util::Floyd::computeAllPairs(g);
        std::unordered_map<common::Node*, std::unordered_map<common::Node*, int>> d;
        for (size_t i = 0; i < vertices.size(); i++) {
            for (size_t j = 0; j < vertices.size(); j++) {
                d[vertices[i]][vertices[j]] = distances[i][j];
            }
        }

        // Remove da matriz as linhas e colunas dos vértices de grau par
        for (auto v : evenVertices) {
            d.erase(v);
            for (auto& [key, val] : d) {
                val.erase(v);
            }
        }

        // Laço principal
        while (!d.empty()) {
            // Determine em D impar o par de vértices vi e vj com menor custo d(i,j)
            int smallest = std::numeric_limits<int>::max();
            common::Node* vi = nullptr;
            common::Node* vj = nullptr;
            for (const auto& [u, neighbors] : d) {
                for (const auto& [v, dist] : neighbors) {
                    if (u != v && dist < smallest) {
                        smallest = dist;
                        vi = u;
                        vj = v;
                    }
                }
            }

            // Construa um caminho artificial de vi para vj com custo d(i,j) no grafo G 
            if (vi && vj && vi != vj) {
                std::vector<common::Node*> artificialPath = util::Floyd::getShortestPath(g, vi, vj, routes);
                for (size_t i = 1; i < artificialPath.size(); i++) {
                    auto u = artificialPath[i - 1];
                    auto v = artificialPath[i];
                    int cost = distances[u->getId()][v->getId()];
                    g->newEdge(u, v, cost);
                }
            }

            // Remove da matriz as linhas e colunas de vi e vj
            d.erase(vi);
            d.erase(vj);
            for (auto& [key, val] : d) { 
                val.erase(vi); 
                val.erase(vj);
            }
        }

        return route;
    };
};