#include "graph/common/graph.hpp"
using namespace abstract;

Node* Graph::newVertex(Vertex glVertex)
{
    vertices.push_back(std::make_unique<Node>(vertices.size(), glVertex));
    Node* newVertex = vertices.back().get();
    if (mesh)
        vertexToRenderQueue(newVertex);
    return newVertex;
};

Edge* Graph::newEdge(int v1, int v2)
{
    return newEdge(getVertex(v1), getVertex(v2));
};

Edge* Graph::newEdge(Node* v1, Node* v2)
{
    auto it1 = std::find_if(vertices.begin(), vertices.end(), 
        [&v1](const std::unique_ptr<Node>& ptr) {
            return ptr.get() == v1;
        });

    auto it2 = std::find_if(vertices.begin(), vertices.end(), 
        [&v2](const std::unique_ptr<Node>& ptr) {
            return ptr.get() == v2;
        });

    if (it1 == vertices.end() || it2 == vertices.end())
        return nullptr;

    edges.push_back(std::make_unique<Edge>(v1, v2));
    Edge* newEdge = edges.back().get();
    if (mesh)
        edgeToRenderQueue(newEdge);
    return newEdge;
};

void Graph::removeVertex(Node* n)
{
    edges.erase(
        std::remove_if(edges.begin(), edges.end(),
                       [&](const std::unique_ptr<Edge>& edge) {
                           return edge->getNode1() == n || edge->getNode2() == n;
                       }),
        edges.end());

    vertices.erase(
        std::remove_if(vertices.begin(), vertices.end(),
                       [&](const std::unique_ptr<Node>& vertex) {
                           return vertex.get() == n;
                       }),
        vertices.end());
}

void Graph::removeVertex(int n)
{
    if (n < 0 || n >= vertices.size())
        return;

    removeVertex(getVertex(n));
}

std::vector<Node*> Graph::getVertices() const
{
    std::vector<Node*> verticesCopy;
    for (const auto &vertex : vertices)
    {
        verticesCopy.push_back(vertex.get());
    }
    return verticesCopy;
};

std::vector<Edge*> Graph::getEdges() const
{
    std::vector<Edge*> edgesCopy;
    for (const auto &edge : edges)
    {
        edgesCopy.push_back(edge.get());
    }
    return edgesCopy;
};

Node* Graph::getVertex(int i)
{
    if (i < vertices.size())
        return vertices.at(i).get();

    return nullptr;
};

Edge* Graph::getEdge(int i)
{
    if (i < edges.size())
        return edges.at(i).get();

    return nullptr;
};

int Graph::getOrder() const
{
    return vertices.size();
};

int Graph::getSize() const
{
    return edges.size();
};

float Graph::getDensity() const
{
    float n = getOrder();
    float m = getSize();
    return (n > 0) ? (m * 2) / (n * (n - 1)) : 0.0f;
};

Graph* Graph::makeSubGraph(const std::vector<int>& targetVertices, const std::vector<int>& targetEdges) const
{
    Graph* subgraph;

    if (targetVertices.size() <= vertices.size())
    {
        for (int v_index : targetVertices)
        {
            if (v_index < 0 || v_index >= vertices.size())
                continue;

            Node* originalNode = vertices[v_index].get();

            Vertex originalGLVertex = originalNode->getGLVertex();

            subgraph->newVertex(originalGLVertex);
        }
    }

    std::vector<Node* > subVertices = subgraph->getVertices();
    if (targetEdges.size() <= edges.size())
    {
        for (int e_index : targetEdges)
        {
            if (e_index < 0 || e_index >= edges.size())
                continue;

            Node* n1 = edges[e_index]->getNode1();
            Node* n2 = edges[e_index]->getNode2();

            int n1Index = -1, n2Index = -1;

            for (int i = 0; i < subVertices.size(); i++)
            {
                if (subVertices[i]->getValue() == n1->getValue())
                {
                    n1Index = i;
                }
                if (subVertices[i]->getValue() == n2->getValue())
                {
                    n2Index = i;
                }
            }

            if (n1Index != -1 && n2Index != -1)
            {
                subgraph->newEdge(n1Index, n2Index);
            }
        }
    }
    return subgraph;
};

void Graph::print() const
{
    fastPrint();
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nGraph statistics:" << std::endl;
    std::cout << "\tOrder: " << getOrder() << std::endl;
    std::cout << "\tSize: " << getSize() << std::endl;
    std::cout << "\tDensity: " << getDensity() << std::endl;
    std::cout << "\tDegree Sequence: ";
    std::vector<int> degreeSeq = getDegreeSequence();
    for (int i = 0; i < degreeSeq.size(); i++)
    {
        std::cout << degreeSeq[i];
        if (i < degreeSeq.size() - 1)
            std::cout << ", ";
        else
            std::cout << std::endl;
        ;
    }
};

void Graph::fastPrint() const
{
    if (getOrder() > 0)
    {
        std::cout << "Vertices in graph:" << std::endl;
        for (auto &v : vertices)
        {
            std::cout << "\t" << v->getValue() << " -> ";
            std::cout << "Neighbors: ";
            for (auto &n : v->getNeighbors())
            {
                std::cout << n->getValue() << " ";
            }
            std::cout << std::endl;
        }
    }
    else
    {
        std::cout << "Still not a graph!" << std::endl;
        return;
    }

    std::cout << "\nEdges in graph:" << std::endl;
    if (getSize() > 0)
    {
        for (auto &e : edges)
        {
            std::cout << "\t(" << e->getNode1()->getValue() << ", " << e->getNode2()->getValue() << ")" << std::endl;
        }
    }
    else
    {
        std::cout << "∅" << std::endl;
    }
};

void Graph::addToRenderQueue()
{
    sphere->scale(0.1f, 0.1f, 0.1f);

    mesh = new MeshTree3D({});
    mesh->translate(0.0f, 0.0f, -3.0f);
    for (const auto &v : vertices)
    {
        vertexToRenderQueue(v.get());
    }
    for (const auto &e : edges)
    {
        edgeToRenderQueue(e.get());
    }

    IFCG::addMesh(mesh);
};

Mesh3D *Graph::vertexToRenderQueue(Node* v)
{
    Vertex glVertex = v->getGLVertex();
    Mesh3D *vertMesh = sphere->duplicate();
    vertMesh->translate(glVertex.x * 10.f, glVertex.y * 10.f, glVertex.z * 10.f);
    mesh->addSubMesh(vertMesh);
    v->setMesh(vertMesh);
    return vertMesh;
};

Mesh3D *Graph::edgeToRenderQueue(Edge* e)
{
    Vertex v1 = e->getNode1()->getGLVertex();
    Vertex v2 = e->getNode2()->getGLVertex();
    Mesh3D *lineMesh = makeLine(glm::vec3(v1.x, v1.y, v1.z), glm::vec3(v2.x, v2.y, v2.z));
    mesh->addSubMesh(lineMesh);
    return lineMesh;
};

Mesh3D* Graph::makeLine(glm::vec3 origin, glm::vec3 end)
{
    float d = 0.01f;
    return new Mesh3D({Vertex(origin.x + d, origin.y + d, origin.z, 1.0f, 0.0f, 0.0f, 1.0f),
                       Vertex(origin.x + d, origin.y - d, origin.z, 1.0f, 0.0f, 0.0f, 1.0f),
                       Vertex(origin.x - d, origin.y + d, origin.z, 1.0f, 0.0f, 0.0f, 1.0f),
                       Vertex(origin.x - d, origin.y - d, origin.z, 1.0f, 0.0f, 0.0f, 1.0f),
                       Vertex(end.x + d, end.y + d, end.z, 1.0f, 0.0f, 0.0f, 1.0f),
                       Vertex(end.x + d, end.y - d, end.z, 1.0f, 0.0f, 0.0f, 1.0f),
                       Vertex(end.x - d, end.y + d, end.z, 1.0f, 0.0f, 0.0f, 1.0f),
                       Vertex(end.x - d, end.y - d, end.z, 1.0f, 0.0f, 0.0f, 1.0f)},
                      {0, 1, 2,
                       2, 1, 3,
                       4, 6, 5,
                       6, 7, 5,
                       0, 2, 4,
                       4, 2, 6,
                       1, 3, 5,
                       5, 3, 7,
                       0, 4, 1,
                       1, 4, 5,
                       2, 3, 6,
                       6, 3, 7},
                      IFCG::shader.id);
};