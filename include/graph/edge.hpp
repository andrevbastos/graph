#ifndef EDGE_HPP
#define EDGE_HPP

#include "ifcg/graphics3D/geometry/mesh.hpp"

class Node;

class Edge {
public:
    Edge(int id, Node* n1, Node* n2)
        : _id(id), _nodes(n1, n2), _mesh(nullptr) {};

    int getId() const { return _id; };
    Node* getNode1() const { return _nodes.first;};
    Node* getNode2() const { return _nodes.second;};

    void setMesh(Mesh3D* m) { _mesh = m;};

private:
    int _id;
    std::pair<Node*, Node*> _nodes;
    Mesh3D* _mesh;
};

#endif // EDGE_HPP
