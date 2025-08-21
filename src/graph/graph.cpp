#include "graph/graph.hpp"

Mesh3D *makeLine(glm::vec3 origin, glm::vec3 end)
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

Node* Graph::newVertex(int value, Vertex glVertex)
{
    vertices.push_back(std::make_unique<Node>(value, glVertex));
    Node* newVertex = vertices.back().get();
    if (mesh)
        vertexToRenderQueue(newVertex);
    return newVertex;
};

Node* Graph::getVertex(int i)
{
    if (i < vertices.size())
    {
        return vertices.at(i).get();
    }
    return nullptr;
};

std::vector<Node*> Graph::getVertices()
{
    std::vector<Node*> verticesCopy;
    for (const auto &vertex : vertices)
    {
        verticesCopy.push_back(vertex.get());
    }
    return verticesCopy;
};

Edge* Graph::newEdge(int v1, int v2)
{
    edges.push_back(std::make_unique<Edge>(getVertex(v1), getVertex(v2)));
    Edge* newEdge = edges.back().get();
    if (mesh)
        edgeToRenderQueue(newEdge);
    return newEdge;
};

Edge* Graph::getEdge(int i)
{
    if (i < edges.size())
    {
        return edges.at(i).get();
    }
    return nullptr;
};

std::vector<Edge*> Graph::getEdges()
{
    std::vector<Edge*> edgesCopy;
    for (const auto &edge : edges)
    {
        edgesCopy.push_back(edge.get());
    }
    return edgesCopy;
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

int Graph::getDegree(int v) const
{
    if (v < vertices.size())
    {
        return vertices.at(v)->getNeighbors().size();
    }
    return 0;
};

std::vector<int> Graph::getDegreeSequence() const
{
    std::vector<int> degrees;
    for (const auto &v : vertices)
    {
        degrees.push_back(v->getNeighbors().size());
    }
    std::sort(degrees.begin(), degrees.end());
    return degrees;
};

Graph Graph::makeSubGraph(std::vector<int> targetVertices, std::vector<int> targetEdges)
{
    Graph subgraph;

    if (targetVertices.size() <= vertices.size())
    {
        for (int v_index : targetVertices)
        {
            Node* originalNode = vertices[v_index].get();

            int originalValue = originalNode->getValue();
            Vertex originalGLVertex = originalNode->getGLVertex();

            subgraph.newVertex(originalValue, originalGLVertex);
        }
    }

    std::vector<Node* > subVertices = subgraph.getVertices();
    if (targetEdges.size() <= edges.size())
    {
        for (int e : targetEdges)
        {
            Node* n1 = edges[e]->getNode1();
            Node* n2 = edges[e]->getNode2();

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
                subgraph.newEdge(n1Index, n2Index);
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