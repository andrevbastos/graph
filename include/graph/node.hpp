#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <unordered_map>

#include "ifcg/common/vertex.hpp"
#include "ifcg/graphics3D/geometry/mesh.hpp"

class Edge; 

class Node
{
public:
    Node(int id, Vertex vertex)
        : _id(id), _glVertex(vertex), _mesh(nullptr) {};
    Node(int id, float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : _id(id), _glVertex(Vertex(x, y, z)), _mesh(nullptr) {};;

    void linkTo(Node* neighbor, Edge* edge) { _adj[neighbor] = edge; };
    void unlinkFrom(Node* neighbor) { _adj.erase(neighbor); };

    int getId() const { return _id; };
    Vertex getGLVertex() const { return _glVertex; };
    std::vector<Node*> getNeighbors() const {
        std::vector<Node*> result;
        result.reserve(_adj.size());
        for (const auto &pair : _adj)
            result.push_back(pair.first);

        return result;
    };
    std::vector<Edge*> getAdjacentEdges() const{
        std::vector<Edge*> result;
        result.reserve(_adj.size());
        for (const auto &pair : _adj)
            result.push_back(pair.second);

        return result;
    };;
    Edge* getEdgeTo(Node* neighbor) const { return (_adj.count(neighbor) > 0) ? _adj.at(neighbor) : nullptr; };

    void setGLVertex(const Vertex &v) { _glVertex = v; };
    void setMesh(Mesh3D* m) { _mesh = m; };
    void translate(float x, float y, float z) {
        _glVertex.x += x;
        _glVertex.y += y;
        _glVertex.z += z;
        _mesh->translate(x, y, z);
    };

private:
    int _id;
    std::unordered_map<Node*, Edge*> _adj;
    
    Vertex _glVertex;
    Mesh3D* _mesh = nullptr;
};

#endif // NODE_HPP
