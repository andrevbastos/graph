#include <iostream>
#include <random>
#include <vector>
#include <ctime>

#include "graph/graph.hpp"

std::vector<common::Node*> objects = {};
int main() {
    auto* g = new undirected::lwGraph<int>(5);

    

    delete g;
    return 0;
}