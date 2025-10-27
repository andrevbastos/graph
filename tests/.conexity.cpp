#include <iostream>

#include "graph/graph.hpp"

int main()
{
    auto *g = loadGraphFromFile("../resources/json/undir_disjoint.json");

    std::cout << "-- BFS --" << std::endl;
    util::BFS::printComponents(g);

    std::cout << "\n-- DFS --" << std::endl;
    util::DFS::printComponents(g);

    std::cout << "\n-- Goodman Algorithm --" << std::endl;
    util::Goodman::printComponents(g);

    std::cout << "\n-- Set Algorithm --" << std::endl;
    util::Set::printComponents(g);

    return 0;
}