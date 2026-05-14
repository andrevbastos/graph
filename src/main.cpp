#include <iostream>
#include <vector>
#include <iomanip>

#include "graph/undirected/lw_graph.hpp"
#include "graph/util/jps.hpp"

int main() {
    int width = 5;
    int height = 5;
    int numVertices = width * height;

    undirected::lwGraph<util::Vertex2D> graph(numVertices);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int id = y * width + x;
            graph.setVertex(id, util::Vertex2D(static_cast<double>(x), static_cast<double>(y)));
        }
    }

    std::vector<int> map = {
        0, 0, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 0, 0, 0
    };

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int currentId = y * width + x;
            if (map[currentId] == 1) continue;

            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) continue;
                    
                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        int neighborId = ny * width + nx;
                        if (map[neighborId] == 0) {
                            graph.addEdge(currentId, neighborId, (dx != 0 && dy != 0) ? 1.414 : 1.0);
                        }
                    }
                }
            }
        }
    }

    util::JumpPointSearchLw jps(graph, width, height);

    auto heuristic = [](const util::Vertex2D& a, const util::Vertex2D& b) {
        double dx = a.x - b.x;
        double dy = a.y - b.y;
        return std::sqrt(dx * dx + dy * dy);
    };

    int startId = 0;
    int endId = 24;

    std::cout << "Buscando caminho de " << startId << " ate " << endId << "..." << std::endl;
    std::vector<int> path = jps.find(startId, endId, heuristic);

    std::cout << "\nGrid (S=Inicio, E=Fim, #=Parede, *=Caminho):\n";
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int id = y * width + x;
            bool isPath = false;
            for (int pId : path) if (pId == id) isPath = true;

            if (id == startId) std::cout << "S ";
            else if (id == endId) std::cout << "E ";
            else if (map[id] == 1) std::cout << "# ";
            else if (isPath) std::cout << "* ";
            else std::cout << ". ";
        }
        std::cout << std::endl;
    }

    if (!path.empty()) {
        std::cout << "\nCaminho encontrado (IDs): ";
        for (int id : path) std::cout << id << " ";
        std::cout << std::endl;
    } else {
        std::cout << "\nNenhum caminho encontrado." << std::endl;
    }

    return 0;
}