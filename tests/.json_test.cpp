#include <iostream>

#include "graph/graph.hpp"

int main() {
    std::cout << "Cups Test\n==================" << std::endl;
    auto* cups = loadGraphFromFile("../resources/json/cups.json");
    std::vector<common::Node*> path = util::BFS::getPath(cups, 0, 15);
    std::cout << "Path from 0 to 15:";
    for (const auto& node : path) {
        std::cout << "\n" << node->getLabel() << "[" << node->getId() << "]";
    }
    std::cout << std::endl;

    std::cout << "\nBFS Test\n==================" << std::endl;
    auto* bfs = loadGraphFromFile("../resources/json/undir.json");
    util::BFS::tableData(bfs, 0);

    std::cout << "\nDFS Test\n==================" << std::endl;
    auto* dfs = loadGraphFromFile("../resources/json/dir.json");
    util::DFS::tableData(dfs);

    std::cout << "\nGoodman Test\n==================" << std::endl;
    auto* goodman = loadGraphFromFile("../resources/json/undir_disjoint.json");
    std::cout << "Components w/ Goodman: " << util::Goodman::goodmanAlgorithm(goodman) << std::endl;

    std::cout << "\nSet Test\n==================" << std::endl;
    auto* set = loadGraphFromFile("../resources/json/undir_disjoint.json");
    std::cout << "Components w/ Set: " << util::Set::componentsCount(set) << std::endl;

    return 0;
}