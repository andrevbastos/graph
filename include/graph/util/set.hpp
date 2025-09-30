#pragma once

#include <unordered_map>
#include <vector>

#include "graph/common/graph.hpp"
#include "graph/common/node.hpp"

namespace util {
    class Set {
    public:
        static std::unordered_map<common::Node*, common::Node*> connectedComponents(common::Graph* g);
        static bool sameComponent(common::Node* n1, common::Node* n2);
        static int componentsCount(common::Graph* g);

        static void printComponents(common::Graph* g);

        static void clear() {
            _p.clear();
            _rank.clear();
        };

    private:
        static void makeSet(common::Node* n);
        static common::Node* findSet(common::Node* n);
        static void unionSet(common::Node* n1, common::Node* n2);
        static void link(common::Node* n1, common::Node* n2);

        static std::unordered_map<common::Node*, common::Node*> _p;
        static std::unordered_map<common::Node*, int> _rank;
    };
};