#include "graph/node.hpp"

class Edge {
public:
    Edge(Node* n1, Node* n2) : node1(n1), node2(n2) {
        n1->linkTo(n2);
        n2->linkTo(n1);
    };

    Node* getNode1() const { return node1; };
    Node* getNode2() const { return node2; };

    void setMesh(Mesh3D* m) { mesh = m; };

private:
    Node* node1;
    Node* node2;
    Mesh3D* mesh = nullptr;
};