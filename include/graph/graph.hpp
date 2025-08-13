#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <iomanip>

#include "graph/edge.hpp"

class Graph {
public:
    Node* newVertex(int value);
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

    void fastPrint() const;

private:
    std::vector<std::unique_ptr<Node>> vertices;
    std::vector<std::unique_ptr<Edge>> edges;

};