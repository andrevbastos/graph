#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <iomanip>

#include "graph/edge.hpp"
#include "ifcg/ifcg.hpp"
#include "ifcg/graphics3D/geometry/mesh.hpp"
#include "ifcg/graphics3D/geometry/meshTree.hpp"
#include "ifcg/graphics3D/geometry/sphere.hpp"

class Graph {
public:
    Node* newVertex(int value);
    Node* newVertex(int value, const Vertex& glVertex);
    Node* getVertex(int i);
    std::vector<Node*> getVertices();

    Edge* newEdge(int v1, int v2);
    Edge* getEdge(int i);
    std::vector<Edge*> getEdges();

    int getOrder() const;
    int getSize() const;
    float getDensity() const;
    int getDegree(int v) const;
    std::vector<int> getDegreeSequence() const;

    Graph makeSubGraph(std::vector<int> targetVertices, std::vector<int> targetEdges);

    void print() const;
    void fastPrint() const;
    void addToRenderQueue();

    MeshTree3D* getMesh() const { return mesh; };

private:
    std::vector<std::unique_ptr<Node>> vertices;
    std::vector<std::unique_ptr<Edge>> edges;
    MeshTree3D* mesh = nullptr;
};