/**
 * @file edge.hpp
 * @author André V Bastos (andrevbastos)
 * @brief Definition of the Edge class for graph representation.
 */

#pragma once

#include <cassert>
#include <utility>

namespace common {
    class Node;

    class Edge {
    public:
        Edge(int id, common::Node* n1, common::Node* n2);

        int getId() const;
        Node* getFirstNode() const;
        Node* getSecondNode() const;

        virtual void linkNodes() = 0;
        
    protected:
        std::pair<common::Node*, common::Node*> _nodes;

    private:
        int _id;
    };
};