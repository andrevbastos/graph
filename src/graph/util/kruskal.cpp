#include "graph/util/kruskal.hpp"

namespace util::Kruskal {
    std::vector<common::Edge*> getMST(common::Graph* g) {
        std::vector<common::Edge*> mstEdges;
        if (!g || g->getOrder() == 0) {
            return {};
        }

        auto w = g->getWeights();
        std::vector<common::Edge*> edges = g->getEdges();

        std::sort(edges.begin(), edges.end(), [&](common::Edge* e1, common::Edge* e2) {
            return w[e1] < w[e2];
        });

        for (auto vertex : g->getVertices()) {
            util::Set::makeSet(vertex);
        }

        for (auto edge : edges) {
            common::Node* u = edge->getFirstNode();
            common::Node* v = edge->getSecondNode();

            if (util::Set::findSet(u) != util::Set::findSet(v)) {
                mstEdges.push_back(edge);
                util::Set::unionSet(u, v);
            }
        }

        return mstEdges;
    }
}