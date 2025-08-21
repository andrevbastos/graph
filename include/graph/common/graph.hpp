#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <iomanip>
#include <cassert>
#include <unordered_map>

#include "graph/node.hpp"
#include "graph/edge.hpp"
#include "ifcg/ifcg.hpp"
#include "ifcg/graphics3D/geometry/mesh.hpp"
#include "ifcg/graphics3D/geometry/meshTree.hpp"
#include "ifcg/graphics3D/geometry/sphere.hpp"

namespace abstract {
    class Graph
    {
    public:
        virtual ~Graph() = default;
        
        Node* newVertex(Vertex glVertex = Vertex(0.0f, 0.0f));
        Edge* newEdge(int v1_ID, int v2_ID);
        Edge* newEdge(Node* v1_ID, Node* v2_ID);
        
        void removeVertex(int v_ID);
        void removeVertex(Node* v_ID);
        void removeEdge(int e_ID);
        void removeEdge(Edge* e_ID);
        
        Node* getVertex(int v_ID) const;
        Edge* getEdge(int e_ID) const;
        Node* getOpposite(int v_ID, int e_ID) const;
        Node* getOpposite(Node* v, Edge* e) const;
        std::vector<Node*> getVertices() const;
        std::vector<Edge*> getEdges() const;
        std::vector<Node*> getNodesFromEdge(int e_ID) const;
        std::vector<Node*> getNodesFromEdge(Edge* e) const;

        // Utility
        virtual Graph* makeSubGraph(const std::vector<int>& targetVertices, const std::vector<int>& targetEdges) const = 0;
        
        int getOrder() const;
        int getSize() const;
        float getDensity() const;
        void fastPrint() const;
        void print() const;
    
        // will be encapsulated later
        void addToRenderQueue();
        MeshTree3D* getMesh();
    
    protected:
        std::unordered_map<int, std::unique_ptr<Node>> vertices;
        std::unordered_map<int, std::unique_ptr<Edge>> edges;

    private:
        int nextNodeId = 0;
        int nextEdgeId = 0;
        
        // will be encapsulated later
        Mesh3D* vertexToRenderQueue(Node* v);
        Mesh3D* edgeToRenderQueue(Edge* e_ID);
        Mesh3D* makeLine(glm::vec3 origin, glm::vec3 end);
        
        MeshTree3D* mesh = nullptr;
        Sphere3D* sphere = new Sphere3D(10, IFCG::shader.id);
    };
}