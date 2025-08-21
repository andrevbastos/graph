#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <iomanip>
#include <unordered_map> // !!!!!!

#include "graph/edge.hpp"
#include "ifcg/ifcg.hpp"
#include "ifcg/graphics3D/geometry/mesh.hpp"
#include "ifcg/graphics3D/geometry/meshTree.hpp"
#include "ifcg/graphics3D/geometry/sphere.hpp"

// VOU TER QUE USAR HASH MAP MESMO!!!!!!!!!!!!!
// QUE DESGRAÇA
namespace abstract {
    class Graph
    {
    public:
        virtual ~Graph() = default;
        
        Node* newVertex(Vertex glVertex = Vertex(0.0f, 0.0f)); //
        Edge* newEdge(int n1, int n2); //
        Edge* newEdge(Node* n1, Node* n2); //
        
        void removeVertex(int n);
        void removeVertex(Node* n);
        void removeEdge(int e);
        void removeEdge(Edge* e);
        
        std::vector<Node*> getVertices() const; //
        std::vector<Edge*> getEdges() const; //
        std::vector<Node*> getNodesFromEdge(int e) const;
        std::vector<Node*> getNodesFromEdge(Edge* e) const;
        virtual std::vector<Edge*> getExitEdgesFromNode(int e) const = 0;
        virtual std::vector<Edge*> getExitEdgesFromNode(Edge* e) const = 0;
        virtual std::vector<Edge*> getEntryEdgesFromNode(int e) const = 0;
        virtual std::vector<Edge*> getEntryEdgesFromNode(Edge* e) const = 0;
        
        Node* getVertex(int i); //
        Edge* getEdge(int i); //
        Node* getOpposite(int n, int e) const;
        Node* getOpposite(Node* n, Edge* e) const;
    
        int getOrder() const; //
        int getSize() const; //
        float getDensity() const; //
    
        virtual int getEntryDegree(int n) const = 0;
        virtual int getEntryDegree(Node* n) const = 0;
        virtual int getExitDegree(int n) const = 0;
        virtual int getExitDegree(Node* n) const = 0;
        virtual std::vector<int> getDegreeSequence() const = 0;
        virtual std::vector<Node*> getEntryNeightbors(int n) const = 0;
        virtual std::vector<Node*> getEntryNeightbors(Node* n) const = 0;
        virtual std::vector<Node*> getExitNeightbors(int n) const = 0;
        virtual std::vector<Node*> getExitNeightbors(Node* n) const = 0;
        virtual std::vector<Edge*> getEntryEdges(int n) const = 0;
        virtual std::vector<Edge*> getEntryEdges(Node* n) const = 0;
        virtual std::vector<Edge*> getExitEdges(int n) const = 0;
        virtual std::vector<Edge*> getExitEdges(Node* n) const = 0;
    
        Graph* makeSubGraph(const std::vector<int>& targetVertices, const std::vector<int>& targetEdges) const; //
        // Graph* makeSubGraph(const std::vector<Node*>& targetVertices, const std::vector<Edge*>& targetEdges) const; ??? talvez
    
        void fastPrint() const; //
        void print() const; //
    
        void addToRenderQueue(); //
        Mesh3D* getMesh() const; //
        
    private:
        Mesh3D* vertexToRenderQueue(Node* v); //
        Mesh3D* edgeToRenderQueue(Edge* e); //
        Mesh3D* makeLine(glm::vec3 origin, glm::vec3 end); //
    
        std::vector<std::unique_ptr<Node>> vertices;
        std::vector<std::unique_ptr<Edge>> edges;
        MeshTree3D *mesh = nullptr;
        Sphere3D *sphere = new Sphere3D(10, IFCG::shader.id);
    };
}