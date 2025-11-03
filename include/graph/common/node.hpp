#pragma once

#include <vector>
#include <unordered_map>
#include <string>

namespace common {
    class Edge; 
       
    class Node
    {
    public:
        Node(int id, const std::string& label)
            : _id(id), _label(label) {};

        virtual void linkTo(common::Node* neighbor, common::Edge* edge) { _adj[neighbor] = edge; };
        virtual void unlinkFrom(common::Node* neighbor) { _adj.erase(neighbor); };
    
        int getId() const { return _id; };
        const std::string& getLabel() const { return _label; };
        Edge* getEdgeTo(common::Node* neighbor) const { return (_adj.count(neighbor) > 0) ? _adj.at(neighbor) : nullptr; };
        int getOrder() const { return _adj.size(); }
        
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

        bool adjTo(common::Node* neighbor) const {
            return _adj.count(neighbor) > 0;
        };
    
    protected:
        std::unordered_map<common::Node*, common::Edge*> _adj;

    private:
        int _id;
        std::string _label;
    };
}