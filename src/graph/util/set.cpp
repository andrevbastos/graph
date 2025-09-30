#include "graph/util/set.hpp"

namespace util {
    std::unordered_map<common::Node*, common::Node*> Set::_p;
    std::unordered_map<common::Node*, int> Set::_rank;

    int Set::componentsCount(common::Graph* g) {
        if (!g) return 0;
        std::unordered_map<common::Node*, common::Node*> components = connectedComponents(g);

        std::vector<common::Node*> roots;
        for (const auto& pair : components) {
            common::Node* root = pair.second;
            if (std::find(roots.begin(), roots.end(), root) == roots.end()) {
                roots.push_back(root);
            }
        }
        return roots.size();
    };

    std::unordered_map<common::Node*, common::Node*> Set::connectedComponents(common::Graph* g)
    {
        if (!g) {
            _p.clear();
            _rank.clear();
            return {};
        }

        for (auto* n : g->getVertices()) {
            makeSet(n);
        }
        for (auto* e : g->getEdges()) {
            auto* n1 = e->getFirstNode();
            auto* n2 = e->getSecondNode();
            if (findSet(n1) != findSet(n2)) {
                unionSet(n1, n2);
            }
        }

        std::unordered_map<common::Node*, common::Node*> result;
        for (auto* n : g->getVertices()) {
            result[n] = findSet(n);
        }

        _p.clear();
        _rank.clear();

        return result;
    };

    bool Set::sameComponent(common::Node* n1, common::Node* n2)
    {
        if (findSet(n1) == findSet(n2)) return true;
        return false;
    };

    void Set::printComponents(common::Graph* g)
    {
        if (!g) return;

        auto components = connectedComponents(g);
        std::unordered_map<common::Node*, std::vector<common::Node*>> sets;

        for (const auto& pair : components) {
            common::Node* node = pair.first;
            common::Node* root = pair.second;
            sets[root].push_back(node);
        }

        if (sets.size() > 1) {
            std::cout << "The graph is not connected." << std::endl;
            std::cout << "It has " << sets.size() << " connected components:" << std::endl;

            int setId = 1;
            for (const auto& pair : sets) {
                std::cout << "\tSet " << setId++ << ": ";
                for (const auto& node : pair.second) {
                    std::cout << node->getId() << " ";
                }
                std::cout << std::endl;
            }
        } else if (sets.size() == 1) {
            std::cout << "The graph is connected." << std::endl;
        } else {
            std::cout << "The graph is empty." << std::endl;
        }
    };

    void Set::makeSet(common::Node* n)
    {
        _p[n] = n;
        _rank[n] = 0;
    };

    common::Node* Set::findSet(common::Node* n)
    {
        common::Node* p = _p[n];
        if (n != p) {
            p = findSet(p);
            _p[n] = p;
        }
        return p;
    };

    void Set::unionSet(common::Node* n1, common::Node* n2)
    {
        link(findSet(n1), findSet(n2));
    };

    void Set::link(common::Node* n1, common::Node* n2)
    {
        if (_rank[n1] > _rank[n2]) {
            _p[n2] = n1;
        } else {
            _p[n1] = n2;
            if (_rank[n1] == _rank[n2]) {
                _rank[n2]++;
            }
        }
    };
}