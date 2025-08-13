#include <vector>

#include "ifcg/common/vertex.hpp"

class Node {
public:
    Node(int v, int x = 0.0f, int y = 0.0f, int z = 0.0f) : v(v), glVertex(Vertex(x, y, z)) {};
    
    void linkTo(Node* n) { adj.push_back(n); };
    int getValue() const { return v; };
    std::vector<Node*> getNeighbors() const { return adj; };
    
    Vertex getGLVertex() const { return glVertex; };
    void setGLVertex(const Vertex& v) { glVertex = v; };

private:
    int v;
    std::vector<Node*> adj;
    Vertex glVertex;
};