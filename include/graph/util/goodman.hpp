#pragma once

#include <vector>
#include "graph/undirected/graph.hpp"
#include "graph/undirected/node.hpp"

namespace util::Goodman {
    static int goodmanAlgorithm(common::Graph* g)
    {
        if (!g) return 0;
        auto* graph = dynamic_cast<undirected::Graph*>(g);
        if (graph != g) return 0;

        auto* graphClone = graph->clone();

        int components = 0;
        while (graphClone->getOrder() > 0) {
            common::Node* startNode = graphClone->getVertices().front();

            while (startNode->adj().size() > 0) {
                common::Node* neighbor = startNode->adj().front();
                common::Edge* edge = startNode->getEdgeTo(neighbor);

                graphClone->removeEdge(edge);

                for (common::Node* n : neighbor->adj()) {
                    if (n != startNode) {
                        graphClone->newEdge(startNode, n);
                        graphClone->removeEdge(neighbor->getEdgeTo(n));
                    }
                }

                graphClone->removeVertex(neighbor);
            }

            graphClone->removeVertex(startNode);
            components++;
        }

        delete graphClone;
        return components;
    }

    static void printComponents(common::Graph* g)
    {
        if (!g) {
            std::cout << "The graph is empty." << std::endl;
            return;
        }

        int components = goodmanAlgorithm(g);
        if (components > 1) {
            std::cout << "The graph is not connected." << std::endl;
            std::cout << "It has " << components << " connected components." << std::endl;
        } else if (components == 1) {
            std::cout << "The graph is connected." << std::endl;
        } else {
            std::cout << "The graph is empty." << std::endl;
        }
    };
}