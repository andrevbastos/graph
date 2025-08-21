#include "graph/common/graph.hpp"

namespace directed {
    class Graph : public abstract::Graph
    {
    public:
        virtual Graph* makeSubGraph(const std::vector<int>& targetVertices, const std::vector<int>& targetEdges) const { return nullptr; };
    };
}