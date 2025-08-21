#include <thread>
#include <chrono>

#include "graph/graph.hpp"

int main()
{
    IFCG::init();
    IFCG::createWindow(800, 600);

    IFCG::setup3D();

    Graph g1;

    Node* n1 = g1.newVertex(0, Vertex(1.0f, 1.0f));
    Node* n2 = g1.newVertex(1, Vertex(-1.0f, 1.0f));
    Node* n3 = g1.newVertex(2, Vertex(1.0f, -1.0f));
    Node* n4 = g1.newVertex(3, Vertex(-1.0f, -1.0f));

    Edge* e1 = g1.newEdge(0, 1);
    Edge* e2 = g1.newEdge(0, 2);
    Edge* e3 = g1.newEdge(0, 3);
    Edge* e4 = g1.newEdge(1, 2);
    Edge* e5 = g1.newEdge(1, 3);
    Edge* e6 = g1.newEdge(2, 3);

    Graph g2 = g1.makeSubGraph({0, 1, 2, 3}, {0, 1, 2});

    g1.addToRenderQueue();
    g2.addToRenderQueue();

    g1.getMesh()->translate(0.0f, 0.0f, -3.0f);
    g1.getMesh()->rotate(1.0f, 1.0f, 0.0f, 0.0f);
    g2.getMesh()->translate(0.0f, 3.0f, -3.0f);

    IFCG::setFramesPerSecond(60);
    IFCG::loop([&] {
        if (glfwGetKey(IFCG::window, GLFW_KEY_C) == GLFW_PRESS)
        {
            if (glfwGetKey(IFCG::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                g2.newVertex(4, Vertex(-2.0f, 1.0f));
            else
                g1.newVertex(4, Vertex(2.0f, 2.0f));
        }
        if (glfwGetKey(IFCG::window, GLFW_KEY_X) == GLFW_PRESS)
        {
            if (glfwGetKey(IFCG::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                g2.newEdge(0, 4);
            else
                g1.newEdge(0, 4);
        }
    });

    IFCG::terminate();
    return 0;
}