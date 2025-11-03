#include <iostream>

#include "graph/graph.hpp"

int main() {
    directed::Graph* dijkstra = new directed::Graph();

    auto v1 = dijkstra->newVertex("v1");
    auto v2 = dijkstra->newVertex("v2");
    auto v3 = dijkstra->newVertex("v3");
    auto v4 = dijkstra->newVertex("v4");
    auto v5 = dijkstra->newVertex("v5");

    dijkstra->newEdge(v1, v2, 2);
    dijkstra->newEdge(v1, v5, 10);
    dijkstra->newEdge(v2, v3, 3);
    dijkstra->newEdge(v2, v5, 7);
    dijkstra->newEdge(v3, v4, 4);
    dijkstra->newEdge(v5, v3, 8);
    dijkstra->newEdge(v5, v4, 5);

    util::Dijkstra::printPathTo(dijkstra, v1, v4);
    std::cout << std::endl;
    
    directed::Graph* floyd = new directed::Graph();

    auto u1 = floyd->newVertex("u1");
    auto u2 = floyd->newVertex("u2");
    auto u3 = floyd->newVertex("u3");
    auto u4 = floyd->newVertex("u4");
    auto u5 = floyd->newVertex("u5");

    floyd->newEdge(u1, u2, 3);
    floyd->newEdge(u1, u3, 8);
    floyd->newEdge(u1, u5, -4);
    floyd->newEdge(u2, u4, 1);
    floyd->newEdge(u2, u5, 7);
    floyd->newEdge(u3, u2, 4);
    floyd->newEdge(u4, u1, 2);
    floyd->newEdge(u4, u3, -5);
    floyd->newEdge(u5, u4, 6);

    util::Floyd::printAllPairsShortestPath(floyd);

    return 0;
}