#pragma once


#include "graph/common/graph.hpp"
#include "graph/common/edge.hpp"

namespace util::Floyd {
    static std::pair<std::vector<std::vector<int>>, std::vector<std::vector<common::Node*>>> computeAllPairs(common::Graph* g) 
    {
        using namespace common;

        std::vector<std::vector<int>> dist = g->getWeightMatrix();
        std::unordered_map<Edge*, int> w = g->getWeights();
        int n = g->getOrder();

        const int INF = std::numeric_limits<int>::max();
        std::vector<std::vector<int>> d(n, std::vector<int>(n, -1));
        std::vector<std::vector<Node*>> r(n, std::vector<Node*>(n, nullptr));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                d[i][j] = dist[i][j];
                if (i != j && dist[i][j] < INF) r[i][j] = g->getVertex(i);
            }
        }

        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (d[i][k] < INF/2 && d[k][j] < INF/2) {
                        if (d[i][k] + d[k][j] < d[i][j]) {
                            d[i][j] = d[i][k] + d[k][j];
                            r[i][j] = r[k][j];
                        }
                    }
                }
            }
        }

        return std::make_pair(d, r);
    };

    void printPath(const std::vector<std::vector<common::Node*>>& r, int i, int j)
    {
        if (i == j) {
            std::cout << i;
        } else if (r[i][j] == nullptr) {
            std::cout << "No path from " << i << " to " << j;
        } else {
            printPath(r, i, r[i][j]->getId());
            std::cout << " -> " << j;
        }
    };

    static void printAllPairsShortestPath(common::Graph* g)
    {
        const auto [dist, r] = computeAllPairs(g);
        int n = g->getOrder();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                std::cout << "Path [" << i << " -> " << j << "]: ";
                printPath(r, i, j);
                std::cout << std::endl;
            }
        }
    };
};