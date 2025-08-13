#include "graph/graph.hpp"

int main() {
    Graph g1;

    Node* n1 = g1.newVertex(1);
    Node* n2 = g1.newVertex(2);
    Node* n3 = g1.newVertex(3);
    Node* n4 = g1.newVertex(4);

    Edge* e1 = g1.newEdge(0, 2);
    Edge* e2 = g1.newEdge(1, 3);
    Edge* e3 = g1.newEdge(2, 3);

    std::cout << "--- G1 ---" << std::endl;
    g1.fastPrint();

    Graph g2 = g1.makeSubGraph({0, 1, 2}, {0});
    std::cout << "\n--- G2 ---" << std::endl;
    g2.fastPrint();

    return 0;
}