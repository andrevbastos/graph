/**
 * @file node.hpp
 * @author André V Bastos (andrevbastos)
 * @brief Definition of the Node class for graph representation.
 */

#pragma once

#include <vector>
#include <unordered_map>
#include <any>

namespace common {
    class Edge; 
       
    class Node
    {
    public:
        Node(int id, std::any data = {});

        virtual void linkTo(common::Node* neighbor, common::Edge* edge);
        virtual void unlinkFrom(common::Node* neighbor);
    
        int getId() const;
        Edge* getEdgeTo(common::Node* neighbor) const;
        int getOrder() const;
        std::any getData() const;
        bool hasData() const;
        
        std::vector<common::Node*> adj() const;
        std::vector<common::Edge*> edges() const;
        bool adjTo(common::Node* neighbor) const;
    
    protected:
        std::unordered_map<common::Node*, common::Edge*> _adj;

    private:
        int _id;
        std::any _data;
    };
}