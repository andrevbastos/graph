#pragma once

#include <queue>
#include "graph/common/graph.hpp"


namespace util {
    class Dijkstra {
    public:
        using MinHeapPair = std::pair<int, common::Node*>;
        using MinHeap = std::priority_queue<MinHeapPair, std::vector<MinHeapPair>, std::greater<MinHeapPair>>;

        struct Data {
            Data()
            : parent(nullptr), distance(std::numeric_limits<int>::max()) {};

            common::Node* parent;
            int distance;
        };

        static std::unordered_map<common::Node*, Data> getPaths(common::Graph* graph, common::Node* start)
        {
            std::unordered_map<common::Node*, Data> data;
            for (common::Node* v : graph->getVertices()) {
                data[v] = Data();
            }
            data[start].distance = 0;

            std::unordered_map<common::Edge*, int> w = graph->getWeights();

            std::vector<common::Node*> S = {};
            MinHeap Q;
            Q.push({0, start});
            for (common::Node* v : graph->getVertices()) {
                Q.push({data[v].distance, v});
            }

            while (!Q.empty()) {
                common::Node* u = Q.top().second;
                Q.pop();
                S.push_back(u);

                for (common::Node* neighbor : u->adj()) {
                    common::Edge* e = u->getEdgeTo(neighbor);
                    relax(e, w[e], data);
                }
            }

            return data;
        };

        static std::vector<common::Node*> getPathTo(common::Graph* graph, common::Node* start, common::Node* end)
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

        static void printPathTo(common::Graph* graph, common::Node* start, common::Node* end)
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
    
    private:
        static void relax(common::Edge* e, int w, std::unordered_map<common::Node*, Data>& data)
        {
            common::Node* u = e->getFirstNode();
            common::Node* v = e->getSecondNode();
            if (data[v].distance > data[u].distance + w) {
                data[v].distance = data[u].distance + w;
                data[v].parent = u;
            }
        };
    };
};