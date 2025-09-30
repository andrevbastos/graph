#include "graph/common/graph.hpp"

using namespace common;

common::Node* Graph::getVertex(int v_ID) const
{
    auto it = vertices.find(v_ID);
    return (it != vertices.end()) ? it->second.get() : nullptr;    
};

common::Edge* Graph::getEdge(int e_ID) const
{
    auto it = edges.find(e_ID);
    return (it != edges.end()) ? it->second.get() : nullptr;
};

common::Node* Graph::getOpposite(common::Node* v, common::Edge* e) const {
    if (!v || !e) return nullptr;

    if (e->getFirstNode() == v) {
        return e->getSecondNode();
    } else if (e->getSecondNode() == v) {
        return e->getFirstNode();
    }

    return nullptr;
}

std::vector<common::Node*> Graph::getVertices() const
{
    std::vector<common::Node*> result;
    result.reserve(vertices.size());
    for (const auto &pair : vertices)
        result.push_back(pair.second.get());
    
    return result;
};

std::vector<common::Edge*> Graph::getEdges() const
{
    std::vector<common::Edge*> result;
    result.reserve(edges.size());
    for (const auto &pair : edges)
        result.push_back(pair.second.get());
    
    return result;
};

std::vector<common::Node*> Graph::getNodesFromEdge(common::Edge* e) const
{
    if (!e)
        return {};
    return {e->getFirstNode(), e->getSecondNode()};
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
            std::cout << "\t" << v.second.get()->getLabel() << "[" << v.second.get()->getId() << "]" << " -> ";
            std::cout << "Adjacent: ";
            for (auto &n : v.second.get()->adj())
            {
                std::cout << "[" << n->getId() << "]" << " ";
            }
            std::cout << std::endl;
        }
    }
    else
    {
        std::cout << "Still not a graph!" << std::endl;
        return;
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