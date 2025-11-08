#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

#include "graph/graph.hpp"

const int WIDTH = 30;
const int HEIGHT = 30;

bool isObstacle(int x, int y) {
    if (x == 15 && y < 28) return true;

    if (y == 10 && x > 5 && x < 15) return true;
    if (y == 20 && x > 15 && x < 25) return true;

    return false;
}

int getId(int x, int y) {
    return y * WIDTH + x;
}

int main() {
    std::cout << "Gerando mapa " << WIDTH << "x" << HEIGHT << "..." << std::endl;
    
    auto* g = new directed::Graph();

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            g->newVertex(std::make_pair(static_cast<double>(x), static_cast<double>(y)));
        }
    }

    int edgesCreated = 0;
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (isObstacle(x, y)) continue;

            int u = getId(x, y);

            if (x + 1 < WIDTH && !isObstacle(x + 1, y)) {
                int v = getId(x + 1, y);
                g->newEdge(u, v, 1);
                g->newEdge(v, u, 1);
                edgesCreated += 2;
            }

            if (y + 1 < HEIGHT && !isObstacle(x, y + 1)) {
                int v = getId(x, y + 1);
                g->newEdge(u, v, 1);
                g->newEdge(v, u, 1);
                edgesCreated += 2;
            }
        }
    }

    std::cout << "Grafo gerado com " << g->getOrder() << " nos e " << edgesCreated << " arestas.\n";

    int startNodeId = getId(0, 0);
    int endNodeId = getId(29, 0);

    std::cout << "Iniciando A* de (" << 0 << "," << 0 << ") [ID " << startNodeId << "] "
              << "ate (" << WIDTH-1 << "," << 0 << ") [ID " << endNodeId << "]...\n";

    auto path = util::AStar::getPath(g, startNodeId, endNodeId, util::AStar::euclideanHeuristic2D);

    if (path.empty()) {
        std::cout << "Nenhum caminho encontrado!" << std::endl;
    } else {
        std::cout << "Caminho encontrado com " << path.size() << " passos!" << std::endl;
        std::cout << "Caminho (IDs): ";
        for (const auto& node : path) {
            std::cout << node->getId() << " ";
        }
        std::cout << std::endl;
    }

    delete g;
    return 0;
}