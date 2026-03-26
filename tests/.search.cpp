#include <iostream>
#include <tuple>
#include <unordered_map>
#include <ostream>

#include "graph/graph.hpp"

std::ostream &operator<<(std::ostream &os, const std::tuple<int, int, int> &t)
{
    os << "(" << std::get<0>(t) << ", " << std::get<1>(t) << ", " << std::get<2>(t) << ")";
    return os;
}

int main()
{
    directed::Graph g1;
    directed::Graph g2;

    // BFS
    std::unordered_map<common::Node*, std::tuple<int, int, int>> states;

    states[g1.newVertex()] = {8, 0, 0};
    states[g1.newVertex()] = {3, 5, 0};
    states[g1.newVertex()] = {5, 0, 3};
    states[g1.newVertex()] = {0, 5, 3};
    states[g1.newVertex()] = {5, 3, 0};
    states[g1.newVertex()] = {3, 2, 3};
    states[g1.newVertex()] = {2, 3, 3};
    states[g1.newVertex()] = {6, 2, 0};
    states[g1.newVertex()] = {2, 5, 1};
    states[g1.newVertex()] = {6, 0, 2};
    states[g1.newVertex()] = {7, 0, 1};
    states[g1.newVertex()] = {1, 5, 2};
    states[g1.newVertex()] = {7, 1, 0};
    states[g1.newVertex()] = {4, 1, 3};
    states[g1.newVertex()] = {1, 4, 3};
    states[g1.newVertex()] = {4, 4, 0};

    g1.newEdge(0, 1);
    g1.newEdge(1, 0);
    g1.newEdge(0, 2);
    g1.newEdge(2, 0);
    g1.newEdge(1, 3);
    g1.newEdge(3, 1);
    g1.newEdge(2, 3);
    g1.newEdge(3, 2);
    g1.newEdge(2, 4);
    g1.newEdge(4, 2);
    g1.newEdge(1, 5);
    g1.newEdge(5, 1);
    g1.newEdge(4, 6);
    g1.newEdge(6, 4);
    g1.newEdge(5, 7);
    g1.newEdge(7, 5);
    g1.newEdge(6, 8);
    g1.newEdge(8, 6);
    g1.newEdge(7, 9);
    g1.newEdge(9, 7);
    g1.newEdge(8, 10);
    g1.newEdge(10, 8);
    g1.newEdge(9, 11);
    g1.newEdge(11, 9);
    g1.newEdge(10, 12);
    g1.newEdge(12, 10);
    g1.newEdge(11, 14);
    g1.newEdge(14, 11);
    g1.newEdge(12, 13);
    g1.newEdge(13, 12);
    g1.newEdge(14, 15);
    g1.newEdge(15, 14);
    g1.newEdge(13, 15);
    g1.newEdge(15, 13);

    g1.newEdge(4, 0);
    g1.newEdge(10, 0);
    g1.newEdge(9, 0);
    g1.newEdge(15, 0);
    g1.newEdge(7, 1);
    g1.newEdge(8, 1);
    g1.newEdge(11, 1);
    g1.newEdge(5, 2);
    g1.newEdge(9, 2);
    g1.newEdge(10, 2);
    g1.newEdge(13, 2);
    g1.newEdge(14, 2);
    g1.newEdge(5, 3);
    g1.newEdge(6, 3);
    g1.newEdge(8, 3);
    g1.newEdge(11, 3);

    std::cout << "=-=-= Teste BFS =-=-=" << std::endl;
    util::BFS::tableData(&g1, 0);
    std::vector<common::Node*> path = util::BFS::getPath(&g1, 0, 15);
    std::cout << "Path 0 -> 15:" << std::endl;
    for (const auto &node : path)
    {
        std::cout << "Vertex " << node->getId() << ": " << states.at(node) << std::endl;
    }

    // DFS
    int i = 6;
    while (i--) g2.newVertex();

    g2.newEdge(0, 1);
    g2.newEdge(0, 3);
    g2.newEdge(1, 4);
    g2.newEdge(2, 4);
    g2.newEdge(2, 5);
    g2.newEdge(3, 1);
    g2.newEdge(4, 3);
    g2.newEdge(5, 5);

    std::cout << "\n=-=-= Teste DFS =-=-=" << std::endl;
    util::DFS::tableData(&g2);

    return 0;
}