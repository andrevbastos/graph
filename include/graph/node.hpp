#include <vector>

#include "ifcg/common/vertex.hpp"
#include "ifcg/graphics3D/geometry/mesh.hpp"

class Node
{
public:
    Node(int v, int x = 0.0f, int y = 0.0f, int z = 0.0f) : v(v), glVertex(Vertex(x, y, z)) {};
    Node(int v, Vertex vertex) : v(v), glVertex(vertex) {};

    void linkTo(Node* n) { adj.push_back(n); };
    int getValue() const { return v; };
    std::vector<Node*> getNeighbors() const { return adj; };

    Vertex getGLVertex() const { return glVertex; };
    void setGLVertex(const Vertex &v) { glVertex = v; };
    void setMesh(Mesh3D* m) { mesh = m; };
    void translate(float x, float y, float z)
    {
        glVertex.x += x;
        glVertex.y += y;
        glVertex.z += z;
        if (mesh)
            mesh->translate(x, y, z);
    }

private:
    int v;
    std::vector<Node*> adj;
    
    Vertex glVertex;
    Mesh3D* mesh = nullptr;
};