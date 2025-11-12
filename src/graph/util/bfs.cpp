#include "graph/util/bfs.hpp"

using namespace util;

std::vector<common::Node*> BFS::getPath(common::Graph* graph, int start_ID, int target_ID) 
{
    common::Node* start = graph->getVertex(start_ID);
    common::Node* target = graph->getVertex(target_ID);
    return (start && target) ? getPath(graph, start, target) : std::vector<common::Node*>();
};

std::vector<common::Node*> BFS::getPath(common::Graph* graph, common::Node* start, common::Node* target) 
{    
    if (!graph || !start || !target) return {};
    if (start == target) return {start};

    std::unordered_map<common::Node*, common::Node*> routing = getRoutingVector(graph, start);

    std::unordered_map<common::Node*, common::Node*> parent_map;
    for (const auto& pair : routing) {
        parent_map[pair.first] = pair.second;
    }

    std::vector<common::Node*> path;
    common::Node* current = target;

    while (current != nullptr) {
        path.push_back(current);
        if (current == start) {
            break; 
        }
        current = parent_map[current];
    }

    if (path.empty() || path.back() != start) {
        return {};
    }
    
    std::reverse(path.begin(), path.end());

    return path;
};

std::unordered_map<common::Node*, common::Node*> BFS::getRoutingVector(common::Graph* graph, int start_ID) 
{
    common::Node* start = graph->getVertex(start_ID);
    return start ? getRoutingVector(graph, start) : std::unordered_map<common::Node*, common::Node*>();
};

std::unordered_map<common::Node*, common::Node*> BFS::getRoutingVector(common::Graph* graph, common::Node* start) 
{
    if (!graph || !start) return std::unordered_map<common::Node*, common::Node*>();

    std::unordered_map<common::Node*, Data> data;

    for (const auto& node : graph->getVertices())
        data[node] = Data();

    data[start].distance = 0;
    data[start].color = GRAY;

    std::queue<common::Node*> queue;
    queue.push(start);

    while (!queue.empty()) {
        common::Node* current = queue.front();
        queue.pop();

        for (const auto& v : current->adj()) {
            if (data[v].color == WHITE) {
                queue.push(v);
                data[v].color = GRAY;
                data[v].parent = current;
                data[v].distance = data[current].distance + 1;
            }
        }

        data[current].color = BLACK;
    }

    std::unordered_map<common::Node*, common::Node*> routing;
    routing.reserve(data.size());
    for (const auto& d : data)
        routing[d.first] = d.second.parent;

    return routing;
};

void BFS::printData(common::Graph* graph, int start_ID) 
{
    common::Node* start = graph->getVertex(start_ID);
    printData(graph, start);
};

void BFS::printData(common::Graph* graph, common::Node* start) 
{
    if (!graph || !start) return;

    std::unordered_map<common::Node*, common::Node*> routing = getRoutingVector(graph, start);

    for (const auto& pair : routing) {
        common::Node* v = pair.first;
        int parent_ID = (pair.second) ? pair.second->getId() : -1;
        std::cout << "Vertex " << v->getId() << ": (p = " << parent_ID << ")" << std::endl;      
    }
};

void BFS::tableData(common::Graph* graph, int start_ID) 
{
    common::Node* start = graph->getVertex(start_ID);
    tableData(graph, start);
};

void BFS::tableData(common::Graph* graph, common::Node* start) 
{
    if (!graph || !start) return;

    std::unordered_map<common::Node*, common::Node*> routing = getRoutingVector(graph, start);

    std::cout << "| Vertex\t | Parent \t|" << std::endl;
    std::cout << "| \t-\t | \t-\t|" << std::endl;
    for (const auto& pair : routing) {
        common::Node* v = pair.first;
        int parent_ID = (pair.second) ? pair.second->getId() : -1;
        std::cout << "| \t" << v->getId() << "\t|\t" << parent_ID << "\t|" << std::endl;
    }
};

std::unordered_map<common::Node*, common::Node*> BFS::getConnectedComponents(common::Graph* graph)
{
    if (!graph) return {};

    std::unordered_map<common::Node*, common::Node*> components;
    std::unordered_map<common::Node*, Data> nodeData;

    for (common::Node* node : graph->getVertices()) 
        nodeData[node] = Data();

    for (common::Node* startNode : graph->getVertices()) {
        if (nodeData.at(startNode).color == WHITE) {
            common::Node* root = startNode;
            std::queue<common::Node*> q;

            q.push(root);
            nodeData.at(root).color = GRAY;
            components[root] = root;

            while (!q.empty()) {
                common::Node* u = q.front();
                q.pop();

                for (common::Node* neighbor : u->adj()) {
                    if (nodeData.at(neighbor).color == WHITE) {
                        nodeData.at(neighbor).color = GRAY;
                        components[neighbor] = root;
                        q.push(neighbor);
                    }
                }
                nodeData.at(u).color = BLACK;
            }
        }
    }
    return components;
};

void BFS::printComponents(common::Graph* graph)
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
};