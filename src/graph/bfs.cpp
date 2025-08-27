#include "graph/bfs.hpp"

std::vector<Node*> BFS::getPath(Graph* graph, int start_ID, int target_ID) {
    Node* start = graph->getVertex(start_ID);
    Node* target = graph->getVertex(target_ID);
    return (start && target) ? getPath(graph, start, target) : std::vector<Node*>();
};

std::vector<Node*> BFS::getPath(Graph* graph, Node* start, Node* target) {
    if (!graph || !start || !target) return {};
    if (start == target) return {start};

    std::vector<std::pair<Node*, Node*>> routing = getRoutingVector(graph, start);
    
    std::unordered_map<Node*, Node*> parent_map;
    for (const auto& pair : routing) {
        parent_map[pair.first] = pair.second;
    }

    std::vector<Node*> path;
    Node* current = target;

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

std::vector<std::pair<Node*, Node*>> BFS::getRoutingVector(Graph* graph, int start_ID) {
    Node* start = graph->getVertex(start_ID);
    return start ? getRoutingVector(graph, start) : std::vector<std::pair<Node*, Node*>>();
};

std::vector<std::pair<Node*, Node*>> BFS::getRoutingVector(Graph* graph, Node* start) {
    if (!graph || !start) return std::vector<std::pair<Node*, Node*>>();

    std::unordered_map<Node*, Data> data;
    
    for (const auto& node : graph->getVertices()) 
        data[node] = Data();

    data[start].distance = 0;
    data[start].color = GRAY;

    std::queue<Node*> queue;
    queue.push(start);

    while (!queue.empty()) {
        Node* current = queue.front();
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
    
    std::vector<std::pair<Node*, Node*>> routing;
    routing.reserve(data.size());
    for (const auto& d : data)
        routing.push_back(std::make_pair(d.first, d.second.parent));
    
    return routing;
};
