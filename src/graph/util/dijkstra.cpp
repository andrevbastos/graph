#include "graph/util/dijkstra.hpp"

namespace util::Dijkstra {
    std::unordered_map<common::Node*, Data> getPaths(common::Graph* graph, common::Node* start)
    {
        std::unordered_map<common::Node*, Data> data;
        for (common::Node* v : graph->getVertices()) {
            data[v] = Data(); 
        }
        data[start].distance = 0;

        std::unordered_map<common::Edge*, int> w = graph->getWeights();
        MinHeap Q;

        Q.push({0, start});

        std::unordered_set<common::Node*> S;

        while (!Q.empty()) {
            int currentDist = Q.top().first;
            common::Node* u = Q.top().second;
            Q.pop();

            if (currentDist > data[u].distance) {
                continue;
            }

            S.insert(u);

            for (common::Node* v : u->adj()) {
                if (S.count(v)) {
                    continue; 
                }

                common::Edge* e = u->getEdgeTo(v);
                
                int weight = 1;
                if (e && w.find(e) != w.end()) {
                    weight = w.at(e);
                }

                int newDist = data[u].distance + weight;
                if (newDist < data[v].distance) {
                    data[v].distance = newDist;
                    data[v].parent = u;
                    
                    Q.push({newDist, v});
                }
            }
        }

        return data;
    };

    std::vector<common::Node*> getPathTo(common::Graph* graph, common::Node* start, common::Node* end)
    {
        std::unordered_map<common::Node*, Data> data = getPaths(graph, start);
        std::vector<common::Node*> path;

        common::Node* current = end;
        while (current != nullptr) {
            path.push_back(current);
            current = data[current].parent;
        }
        std::reverse(path.begin(), path.end());

        if (path.front() != start) {
            return {};
        }

        return path;
    };

    void printPathTo(common::Graph* graph, common::Node* start, common::Node* end)
    {
        std::vector<common::Node*> path = getPathTo(graph, start, end);
        if (path.empty()) {
            std::cout << "No path from " << start->getId() << " to " << end->getId() << std::endl;
            return;
        }

        for (size_t i = 0; i < path.size(); i++) {
            std::cout << path[i]->getId();
            if (i != path.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
    };
};