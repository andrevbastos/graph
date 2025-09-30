#include "graph/util/dfs.hpp"

using namespace util;

std::unordered_map<common::Node*, DFS::Data> DFS::getGraphData(common::Graph* graph) {
    std::unordered_map<common::Node*, Data> result;
    int time = 1;

    std::vector<common::Node*> vertices = graph->getVertices();
    std::reverse(vertices.begin(), vertices.end());
    
    for (auto* v : vertices) {
        result[v] = Data();
    }

    for (auto* v : vertices) {
        if (result[v].color == WHITE) {
            visit(v, result, time);
        }
    }

    return result;
};

void DFS::printData(common::Graph* graph) {
    std::unordered_map<common::Node*, DFS::Data> data = getGraphData(graph);
    for (const auto& pair : data) {
        common::Node* v = pair.first;
        DFS::Data d = pair.second;
        int parent_ID = (d.parent) ? d.parent->getId() : -1;
        std::cout << "Vertex " << v->getId() << ": (p = " << parent_ID << ", o = " << d.opening << ", c = " << d.closing << ")" << std::endl;
    }
};

void DFS::tableData(common::Graph* graph) {
    std::unordered_map<common::Node*, DFS::Data> data = getGraphData(graph);
    std::cout << "| Vertex\t| Parent\t| Opening\t| Closing\t|" << std::endl;
    std::cout << "|\t-\t|\t-\t|\t-\t|\t-\t|" << std::endl;
    for (const auto& pair : data) {
        common::Node* v = pair.first;
        DFS::Data d = pair.second;
        int parent_ID = (d.parent) ? d.parent->getId() : -1;
        std::cout << "|\t" << v->getId() << "\t|\t" << parent_ID << "\t|\t" << d.opening << "\t|\t" << d.closing << "\t|" <<std::endl;
    }
};

std::unordered_map<common::Node*, common::Node*> DFS::getConnectedComponents(common::Graph* graph)
{
    if (!graph) return {};
    
    std::unordered_map<common::Node*, common::Node*> components;
    std::unordered_map<common::Node*, Data> nodeData;

    for (common::Node* node : graph->getVertices()) {
        nodeData[node] = Data();
    }

    for (common::Node* startNode : graph->getVertices()) {
        if (nodeData.at(startNode).color == WHITE) {
            visitComponent(startNode, startNode, components, nodeData);
        }
    }
    return components;
}

void DFS::printComponents(common::Graph* graph)
{
    if (!graph) return;

    auto componentsMap = getConnectedComponents(graph);
    std::unordered_map<common::Node*, std::vector<common::Node*>> data;

    for (const auto& pair : componentsMap) {
        common::Node* node = pair.first;
        common::Node* root = pair.second;
        data[root].push_back(node);
    }

    if (data.size() > 1) {
        std::cout << "The graph is not connected." << std::endl;
        std::cout << "It has " << data.size() << " connected components:" << std::endl;

        int setId = 1;
        for (const auto& pair : data) {
            std::cout << "\tSet " << setId++ << ": ";
            for (common::Node* node : pair.second) {
                std::cout << node->getId() << " ";
            }
            std::cout << std::endl;
        }
    } else if (data.size() == 1) {
        std::cout << "The graph is connected." << std::endl;
    } else {
        std::cout << "The graph is empty." << std::endl;
    }
}

void DFS::visit(common::Node* v, std::unordered_map<common::Node*, Data> &data, int &time) {
    data[v].color = GRAY;
    data[v].opening = time++;

    for (auto* neighbor : v->adj()) {
        if (data[neighbor].color == WHITE) {
            data[neighbor].parent = v;
            visit(neighbor, data, time);
        }
    }

    data[v].color = BLACK;
    data[v].closing = time++;
};

void DFS::visitComponent(common::Node* u, common::Node* root, std::unordered_map<common::Node*, common::Node*>& components, std::unordered_map<common::Node*, Data>& nodeData)
{
    nodeData.at(u).color = GRAY;
    components[u] = root;
    for (common::Node* neighbor : u->adj()) {
        if (nodeData.at(neighbor).color == WHITE) {
            visitComponent(neighbor, root, components, nodeData);
        }
    }
    nodeData.at(u).color = BLACK;
}