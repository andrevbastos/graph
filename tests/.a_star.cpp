#include <iostream>
#include <random>
#include <vector>
#include <ctime>

#include "graph/graph.hpp"

std::vector<common::Node*> objects = {};
int main() {
    srand(static_cast<unsigned>(time(NULL)));

    auto* g = new undirected::Graph();

    int w, h;
    std::cout << "Altura: ";
    std::cin >> w;
    std::cout << "Largura: ";
    std::cin >> h;
    std::cout << std::endl;

    const int totalNodes = w * h;

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            auto newNode = g->newVertex(std::make_pair(i, j));
        }
    }

    std::vector<bool> obstacleGrid(totalNodes, false);
    for (int i = 0; i < totalNodes; ++i) {
        int roll = rand() % 100;
        if (roll < 20) {
            obstacleGrid[i] = true;
            objects.push_back(g->getVertex(i));
        }
    }

    obstacleGrid[0] = false;
    obstacleGrid[totalNodes - 1] = false;

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int currentId = i * w + j;
            if (obstacleGrid[currentId]) continue; 

            common::Node* currentNode = g->getVertex(currentId);

            if (j + 1 < w) {
                int rightId = i * w + (j + 1);
                if (!obstacleGrid[rightId]) {
                    g->newEdge(currentNode, g->getVertex(rightId));
                }
            }

            if (i + 1 < h) {
                int bottomId = (i + 1) * w + j;
                if (!obstacleGrid[bottomId]) {
                    g->newEdge(currentNode, g->getVertex(bottomId));
                }
            }
        }
    }

    std::cout << "Buscando caminho de (0,0) para (99,99)..." << std::endl;
    auto path = util::AStar::getPath(g, 0, totalNodes - 1, util::AStar::euclideanHeuristic2D);

    std::cout << "Caminho de (0,0) para (" << h - 1 << "," << w - 1 << "):" << std::endl;
    for (const auto& node : path) {
        auto [row, col] = std::any_cast<std::pair<int, int>>(node->getData());
        std::cout << "(" << row << "," << col << ") ";
    }
    std::cout << std::endl;

    std::vector<bool> isPath(totalNodes, false);
    for (const auto& node : path) {
        isPath[node->getId()] = true;
    }

    std::cout << "\nGrid:\n";
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int currentId = i * w + j;
            if (obstacleGrid[currentId]) {
                std::cout << "# ";
            } else if (isPath[currentId]) {
                std::cout << "O ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << "\n";
    }
    
    delete g;
    return 0;
}