#include "graph/graph.hpp"

Node* Graph::newVertex(int value) {
    vertices.push_back(std::make_unique<Node>(value));
    return vertices.back().get();
};

Node* Graph::getVertex(int i) {
    if (i < vertices.size()) {
        return vertices.at(i).get();
    }
    return nullptr;
};

std::vector<Node*> Graph::getVertices() {
    std::vector<Node*> verticesCopy;
    for (const auto& vertex : vertices) {
        verticesCopy.push_back(vertex.get());
    }
    return verticesCopy;
};

Edge* Graph::newEdge(int v1, int v2) {
    edges.push_back(std::make_unique<Edge>(getVertex(v1), getVertex(v2)));
    return edges.back().get();
};

Edge* Graph::getEdge(int i) {
    if (i < edges.size()) {
        return edges.at(i).get();
    }
    return nullptr;
};

std::vector<Edge*> Graph::getEdges() {
    std::vector<Edge*> edgesCopy;
    for (const auto& edge : edges) {
        edgesCopy.push_back(edge.get());
    }
    return edgesCopy;
};

int Graph::getOrder() const {
    return vertices.size();
};

int Graph::getSize() const {
    return edges.size();
};

float Graph::getDensity() const {
    float n = getOrder();
    float m = getSize();
    return (n > 0) ? (m*2)/(n*(n-1)) : 0.0f;
}

int Graph::getDegree(int v) const {
    if (v < vertices.size()) {
        return vertices.at(v)->getNeighbors().size();
    }
    return 0;
}

std::vector<int> Graph::getDegreeSequence() const {
    std::vector<int> degrees;
    for (const auto& v : vertices) {
        degrees.push_back(v->getNeighbors().size());
    }
    std::sort(degrees.begin(), degrees.end());
    return degrees;
}

Graph Graph::makeSubGraph(std::vector<int> targetVertices, std::vector<int> targetEdges) {
    Graph subgraph;

    if (targetVertices.size() <= vertices.size()) {
        for (int v : targetVertices) {
            subgraph.newVertex(vertices[v]->getValue());
        }
    }

    std::vector<Node*> subVertices = subgraph.getVertices();
    if (targetEdges.size() <= edges.size()) {
        for (int e : targetEdges) {
            Node* n1 = edges[e]->getNode1();
            Node* n2 = edges[e]->getNode2();

            int n1Index = -1, n2Index = -1;

            for (int i = 0; i < subVertices.size(); i++) {
                if (subVertices[i]->getValue() == n1->getValue()) {
                    n1Index = i;
                }
                if (subVertices[i]->getValue() == n2->getValue()) {
                    n2Index = i;
                }
            }

            if (n1Index != -1 && n2Index != -1) {
                subgraph.newEdge(n1Index, n2Index);
            }
        }
    }
    return subgraph;
}

void Graph::fastPrint() const {
    if (getOrder() > 0) {
        std::cout << "Vertices in graph:" << std::endl;
        for (auto& v : vertices) {
            std::cout << "\t" << v->getValue() << " -> ";
            std::cout << "Neighbors: ";
            for (auto& n : v->getNeighbors()) {
                std::cout << n->getValue() << " ";
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "Still not a graph!" << std::endl;
        return;
    }

    std::cout << "\nEdges in graph:" << std::endl;
    if (getSize() > 0) {
        for (auto& e : edges) {
            std::cout << "\t(" << e->getNode1()->getValue() << ", " << e->getNode2()->getValue() << ")" << std::endl;
        }
    } else {
        std::cout << "∅" << std::endl;
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nGraph statistics:" << std::endl;
    std::cout << "\tOrder: " << getOrder() << std::endl;
    std::cout << "\tSize: " << getSize() << std::endl;
    std::cout << "\tDensity: " << getDensity() << std::endl;
    std::cout << "\tDegree Sequence: ";
    std::vector<int> degreeSeq = getDegreeSequence();
    for (int i = 0; i < degreeSeq.size(); i++) {
        std::cout << degreeSeq[i];
        if (i < degreeSeq.size() - 1) std::cout << ", ";
        else std::cout << std::endl;;
    }
}