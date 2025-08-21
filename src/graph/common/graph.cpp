#include "graph/common/graph.hpp"
using namespace abstract;

Node* Graph::newVertex(Vertex glVertex)
{
    int newID = nextNodeId++;
    auto newNode = std::make_unique<Node>(newID, glVertex);
    Node* nodePtr = newNode.get();
    vertices.emplace(newID, std::move(newNode));
    std::cout << "New vertex: " << newID << std::endl;
    
    if (mesh)
        vertexToRenderQueue(nodePtr);
    return nodePtr;
};

Edge* Graph::newEdge(int v1_ID, int v2_ID)
{
    return newEdge(getVertex(v1_ID), getVertex(v2_ID));
};

Edge* Graph::newEdge(Node* v1, Node* v2)
{
    if (!v1 || !v2) return nullptr;

    if (vertices.find(v1->getId()) == vertices.end() || vertices.find(v2->getId()) == vertices.end())
        return nullptr;

    int newId = nextEdgeId++;
    auto newEdgeUniquePtr = std::make_unique<Edge>(newId, v1, v2);
    Edge* newEdgePtr = newEdgeUniquePtr.get();
    
    edges.emplace(newId, std::move(newEdgeUniquePtr));

    v1->linkTo(v2, newEdgePtr);
    v2->linkTo(v1, newEdgePtr);

    if (mesh)
        edgeToRenderQueue(newEdgePtr);
        
    return newEdgePtr;
};

void Graph::removeVertex(int n)
{
    removeVertex(getVertex(n));
}

void Graph::removeVertex(Node* n)
{
    if (!n || vertices.find(n->getId()) == vertices.end())
        return;

    std::vector<int> edgesToRemove;
    for (const auto& pair : edges) {
        if (pair.second->getNode1() == n || pair.second->getNode2() == n)
            edgesToRemove.push_back(pair.first);
    }

    for (int edgeID : edgesToRemove) {
        edges.erase(edgeID);
    }

    vertices.erase(n->getId());
}

void Graph::removeEdge(int eId)
{
    edges.erase(eId);
}

void Graph::removeEdge(Edge* e)
{
    if (e) {
        removeEdge(e->getId());
    }
}

Node* Graph::getVertex(int v_ID) const
{
    auto it = vertices.find(v_ID);
    return (it != vertices.end()) ? it->second.get() : nullptr;    
};

Edge* Graph::getEdge(int e_ID) const
{
    auto it = edges.find(e_ID);
    return (it != edges.end()) ? it->second.get() : nullptr;
};

Node* Graph::getOpposite(int v_ID, int e_ID) const
{
    return getOpposite(getVertex(v_ID), getEdge(e_ID));
};

Node* Graph::getOpposite(Node* v, Edge* e) const
{
    if (v != e->getNode1()) {
        if (v != e->getNode2()) return nullptr;
        else return e->getNode2();
    } else return e->getNode1();
};

std::vector<Node*> Graph::getVertices() const
{
    std::vector<Node*> result;
    result.reserve(vertices.size());
    for (const auto &pair : vertices)
        result.push_back(pair.second.get());
    
    return result;
};

std::vector<Edge*> Graph::getEdges() const
{
    std::vector<Edge*> result;
    result.reserve(edges.size());
    for (const auto &pair : edges)
        result.push_back(pair.second.get());
    
    return result;
};

std::vector<Node*> Graph::getNodesFromEdge(int e_ID) const
{
    return getNodesFromEdge(getEdge(e_ID));
};

std::vector<Node*> Graph::getNodesFromEdge(Edge* e) const
{
    if (!e)
        return {};
    return {e->getNode1(), e->getNode2()};
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

void Graph::fastPrint() const
{
    if (getOrder() > 0)
    {
        std::cout << "Vertices in graph:" << std::endl;
        for (auto &v : vertices)
        {
            std::cout << "\t" << v.first << " -> ";
            std::cout << "Neighbors: ";
            for (auto &n : v.second.get()->getNeighbors())
            {
                std::cout << n->getId() << " ";
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
            std::cout << "\t(" << e.second->getNode1()->getId() << ", " << e.second->getNode2()->getId() << ")" << std::endl;
        }
    }
    else
    {
        std::cout << "∅" << std::endl;
    }
};

void Graph::print() const
{
    fastPrint();
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nGraph statistics:" << std::endl;
    std::cout << "\tOrder: " << getOrder() << std::endl;
    std::cout << "\tSize: " << getSize() << std::endl;
    std::cout << "\tDensity: " << getDensity() << std::endl;
};

void Graph::addToRenderQueue()
{
    sphere->scale(0.1f, 0.1f, 0.1f);

    mesh = new MeshTree3D({});
    mesh->translate(0.0f, 0.0f, -3.0f);
    for (const auto &v : vertices)
    {
        vertexToRenderQueue(v.second.get());
    }
    for (const auto &e : edges)
    {
        edgeToRenderQueue(e.second.get());
    }

    IFCG::addMesh(mesh);
};

MeshTree3D* Graph::getMesh()
{
    return mesh;
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