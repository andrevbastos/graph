#pragma once

#include <vector>
#include <unordered_map>

#include "ifcg/common/vertex.hpp"
#include "ifcg/graphics3D/geometry/mesh.hpp"

namespace common {
    class Edge; 
       
    class Node
    {
    public:
        Node(int id)
            : _id(id) {};
    
        virtual void linkTo(common::Node* neighbor, common::Edge* edge) { _adj[neighbor] = edge; };
        virtual void unlinkFrom(common::Node* neighbor) { _adj.erase(neighbor); };
    
        int getId() const { return _id; };
        Edge* getEdgeTo(common::Node* neighbor) const { return (_adj.count(neighbor) > 0) ? _adj.at(neighbor) : nullptr; };
        
        std::vector<common::Node*> adj() const  {
            std::vector<common::Node*> result;
            result.reserve(_adj.size());
            for (const auto &pair : _adj)
                result.push_back(pair.first);

            return result;
        };
        std::vector<common::Edge*> edges() const  {
            std::vector<common::Edge*> result;
            result.reserve(_adj.size());
            for (const auto &pair : _adj)
                result.push_back(pair.second);

            return result;
        };
    
    protected:
        std::unordered_map<common::Node*, common::Edge*> _adj;

    private:
        int _id;
    };
}