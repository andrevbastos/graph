#include <thread>
#include <chrono>

#include "graph/directed_graph.hpp"

int main()
{
    IFCG::init();
    IFCG::createWindow(800, 600);

    IFCG::setup3D();

    directed::Graph g1;

    g1.newVertex(Vertex(1.0f, 1.0f));
    g1.newVertex(Vertex(-1.0f, 1.0f));
    g1.newVertex(Vertex(1.0f, -1.0f));
    g1.newVertex(Vertex(-1.0f, -1.0f));

    g1.newEdge(0, 1);
    g1.newEdge(0, 2);
    g1.newEdge(0, 3);
    g1.newEdge(1, 2);
    g1.newEdge(1, 3);
    g1.newEdge(2, 3);

    g1.addToRenderQueue();

    g1.getMesh()->translate(0.0f, 0.0f, -3.0f);
    g1.getMesh()->rotate(1.0f, 1.0f, 0.0f, 0.0f);

    IFCG::setFramesPerSecond(60);
    IFCG::loop([&] {});

    IFCG::terminate();
    return 0;
}