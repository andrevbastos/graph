#include <iostream>

#include "graph/graph.hpp"

bool isObstacle(int x, int y) {
    if ((x == 4 && y >= 2 && y <= 7) || (y == 6 && x >= 2 && x <= 7))
        return true;

    return false;
}

int main() {
    auto* g = new undirected::Graph();

    const int w = 10;
    const int h = 10;

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            auto newNode = g->newVertex(std::make_pair(i, j));
        }
    }

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (isObstacle(i, j)) continue;

            int currentId = i * w + j;
            common::Node* currentNode = g->getVertex(currentId);

            if (j + 1 < w && !isObstacle(i, j + 1)) {
                int rightId = i * w + (j + 1);
                common::Node* rightNode = g->getVertex(rightId);
                g->newEdge(currentNode, rightNode);
            }

            if (i + 1 < h && !isObstacle(i + 1, j)) {
                int bottomId = (i + 1) * w + j;
                common::Node* bottomNode = g->getVertex(bottomId);
                g->newEdge(currentNode, bottomNode);
            }
        }
    }

    auto path = util::AStar::getPath(g, g->getVertex(0)->getId(), g->getVertex(97)->getId(), util::AStar::euclideanHeuristic2D);
    std::cout << "Path from (0,0) to (9,7):" << std::endl;
    for (const auto& node : path) {
        auto [x, y] = std::any_cast<std::pair<int, int>>(node->getData());
        std::cout << "(" << x << "," << y << ") ";
    }
    std::cout << std::endl;

    std::vector<bool> isPath(w * h, false);
    for (const auto& node : path) {
        isPath[node->getId()] = true;
    }

    std::cout << "\nGrid Visualization:\n";
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (isObstacle(i, j)) {
                std::cout << "# ";
            } else if (isPath[i * w + j]) {
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