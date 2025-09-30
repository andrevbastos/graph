#include <iostream>
#include "graph/undirected/graph.hpp"
#include "graph/directed/graph.hpp"

void printGraphDetails(const common::Graph* g) {
    std::cout << "Order (number of vertices): " << g->getOrder() << std::endl;
    std::cout << "Size (number of edges): " << g->getSize() << std::endl;
    g->fastPrint();
}

int main() {
    undirected::Graph undir;

    auto* u_v1 = undir.newVertex("A");
    auto* u_v2 = undir.newVertex("B");
    auto* u_v3 = undir.newVertex("C");
    auto* u_v4 = undir.newVertex("D");

    undir.newEdge(u_v1, u_v2);
    undir.newEdge(u_v2, u_v3);
    undir.newEdge(u_v3, u_v1);
    undir.newEdge(u_v3, u_v4);

    std::cout << "Undirected Graph" << std::endl;
    printGraphDetails(&undir);

    directed::Graph dir;

    auto* d_v1 = dir.newVertex("W");
    auto* d_v2 = dir.newVertex("X");
    auto* d_v3 = dir.newVertex("Y");
    auto* d_v4 = dir.newVertex("Z");

    dir.newEdge(d_v1, d_v2);
    dir.newEdge(d_v1, d_v3);
    dir.newEdge(d_v3, d_v4);
    dir.newEdge(d_v4, d_v1);

    std::cout << "\nDirected Graph" << std::endl;
    printGraphDetails(&dir);

    return 0;
}