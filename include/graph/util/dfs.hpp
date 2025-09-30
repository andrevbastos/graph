#pragma once

#include "graph/common/graph.hpp"

namespace util {
    class DFS {
    public:
        enum Color { WHITE, GRAY, BLACK };
        struct Data {
            Data()
            : parent(nullptr), color(WHITE) {};
    
            common::Node* parent;
            Color color;
            int opening;
            int closing;
        };
        static std::unordered_map<common::Node*, DFS::Data> getGraphData(common::Graph* graph);

        static void printData(common::Graph* graph);
        static void tableData(common::Graph* graph);

        static std::unordered_map<common::Node*, common::Node*> getConnectedComponents(common::Graph* graph);
        static void printComponents(common::Graph* graph);

    private:
        static void visit(common::Node* v, std::unordered_map<common::Node*, DFS::Data> &data, int &time);
        static void visitComponent(common::Node* u, common::Node* root, std::unordered_map<common::Node*, common::Node*>& components, std::unordered_map<common::Node*, Data>& nodeData);
    };
}