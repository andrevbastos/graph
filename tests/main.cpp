#include <iostream>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <ostream>

#include "graph/directed/graph.hpp"
#include "graph/directed/node.hpp"
#include "graph/bfs.hpp"

std::ostream& operator<<(std::ostream& os, const std::tuple<int, int, int>& t) {
    os << "(" << std::get<0>(t) << ", " << std::get<1>(t) << ", " << std::get<2>(t) << ")";
    return os;
}

int main() {
    directed::Graph g;
    
    std::unordered_map<Node*, std::tuple<int, int, int>> states;

    states[g.newVertex()] = {8, 0, 0};
    states[g.newVertex()] = {3, 5, 0};
    states[g.newVertex()] = {5, 0, 3};
    states[g.newVertex()] = {0, 5, 3};
    states[g.newVertex()] = {5, 3, 0};
    states[g.newVertex()] = {3, 2, 3};
    states[g.newVertex()] = {2, 3, 3};
    states[g.newVertex()] = {6, 2, 0};
    states[g.newVertex()] = {2, 5, 1};
    states[g.newVertex()] = {6, 0, 2};
    states[g.newVertex()] = {7, 0, 1};
    states[g.newVertex()] = {1, 5, 2};
    states[g.newVertex()] = {7, 1, 0};
    states[g.newVertex()] = {4, 1, 3};
    states[g.newVertex()] = {1, 4, 3};
    states[g.newVertex()] = {4, 4, 0};

    g.newEdge(0, 1);
    g.newEdge(1, 0);
    g.newEdge(0, 2);
    g.newEdge(2, 0);
    g.newEdge(1, 3);
    g.newEdge(3, 1);
    g.newEdge(2, 3);
    g.newEdge(3, 2);
    g.newEdge(2, 4);
    g.newEdge(4, 2);
    g.newEdge(1, 5);
    g.newEdge(5, 1);
    g.newEdge(4, 6);
    g.newEdge(6, 4);
    g.newEdge(5, 7);
    g.newEdge(7, 5);
    g.newEdge(6, 8);
    g.newEdge(8, 6);
    g.newEdge(7, 9);
    g.newEdge(9, 7);
    g.newEdge(8, 10);
    g.newEdge(10, 8);
    g.newEdge(9, 11);
    g.newEdge(11, 9);
    g.newEdge(10, 12);
    g.newEdge(12, 10);
    g.newEdge(11, 14);
    g.newEdge(14, 11);
    g.newEdge(12, 13);
    g.newEdge(13, 12);
    g.newEdge(14, 15);
    g.newEdge(15, 14);
    g.newEdge(13, 15);
    g.newEdge(15, 13);

    g.newEdge(4, 0);
    g.newEdge(10, 0);
    g.newEdge(9, 0);
    g.newEdge(15, 0);
    g.newEdge(7, 1);
    g.newEdge(8, 1);
    g.newEdge(11, 1);
    g.newEdge(5, 2);
    g.newEdge(9, 2);
    g.newEdge(10, 2);
    g.newEdge(13, 2);
    g.newEdge(14, 2);
    g.newEdge(5, 3);
    g.newEdge(6, 3);
    g.newEdge(8, 3);
    g.newEdge(11, 3);
    
    std::vector<Node*> path = BFS::getPath(&g, 0, 15);
    std::cout << "Caminho encontrado:" << std::endl;
    for (const auto& node : path) {
        std::cout << "Vertice " << node->getId() << ": " << states.at(node) << std::endl;
    }

    return 0;
}