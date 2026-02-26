#include "graph/util/postman.hpp"

namespace util::Postman {
    std::vector<common::Node*> findDeliveryRoute(common::Graph* graph)
    {
        std::vector<common::Node*> route;

        // Inicialização
        // Vpar <- {conjunto dos vértices de grau par};
        auto vertices = graph->getVertices();
        std::vector<bool> evenVertices;
        for (auto v : vertices) {
            if (v->getOrder() % 2 == 0) evenVertices.push_back(true);
            else evenVertices.push_back(false);
        }

        // Ge <- G;
        auto ge = graph->clone();

        // Calcula os caminhos mínimos
        // Execute FLOYD(G, W) para construir a matriz de distâncias Dn;
        auto [distances, routes] = util::Floyd::computeAllPairs(ge);

        // Remove da matriz as linhas e colunas dos vértices de grau par
        // Dimpar <- Dn - (linhas e colunas de V par );
        std::unordered_map<common::Node*, std::unordered_map<common::Node*, int>> d;
        for (size_t i = 0; i < vertices.size(); i++) {
            for (size_t j = 0; j < vertices.size(); j++) {
                if (evenVertices[i] || evenVertices[j]) continue;
                d[vertices[i]][vertices[j]] = distances[i][j];
            }
        }

        // Laço principal
        // enquanto Dimpar != ∅ faça
        while (!d.empty()) {
            // Determine em Dimpar o par de vértices vi e vj com menor custo Dimpar i,j;
            int smallest = std::numeric_limits<int>::max();
            std::pair<common::Node*, common::Node*> minPair = {nullptr, nullptr}; 
            for (const auto& [u, neighbors] : d) {
                for (const auto& [v, dist] : neighbors) {
                    if (u != v && dist < smallest) {
                        smallest = dist;
                        minPair = {u, v};
                    }
                }
            }

            // Construa um caminho artificial de vi para vj com custo Dimpar i,j no grafo Ge;
            ge->newEdge(minPair.first, minPair.second, smallest);

            // Remove da matriz as linhas e colunas de vi e vj
            // Dimpar <- D impar - (linhas e colunas de vi e vj);
            d.erase(minPair.first);
            d.erase(minPair.second);
            for (auto& [u, neighbors] : d) { 
                neighbors.erase(minPair.first); 
                neighbors.erase(minPair.second);
            }
        }

        // Encontre um ciclo euleriano do grafo Ge;
        route = util::Hierholzer::findEulerianCycle(ge);

        return route;
    };
};

