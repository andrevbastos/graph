/**
 * @file graph_tests.cpp
 * @brief Suite de testes unitários utilizando Google Test.
 */

#include <gtest/gtest.h>
#include <memory>
#include <vector>

// Inclusão dos headers do projeto
#include "graph/directed/graph.hpp"
#include "graph/undirected/graph.hpp"
#include "graph/util/bfs.hpp"
#include "graph/util/dfs.hpp"
#include "graph/util/dijkstra.hpp"
#include "graph/util/floyd.hpp"
#include "graph/util/union_find.hpp"
#include "graph/util/goodman.hpp"
#include "graph/util/hierholzer.hpp"
#include "graph/util/kruskal.hpp"
#include "graph/util/prim.hpp"
#include "graph/util/postman.hpp"
#include "graph/util/a_star.hpp"
#include "graph/util/path_smoothing.hpp"
#include "graph/util/theta_star.hpp"

int calculateMSTCost(common::Graph* g, const std::vector<common::Edge*>& edges) {
    int total = 0;
    auto weights = g->getWeights();
    for (auto* edge : edges) {
        if (weights.count(edge)) {
            total += weights.at(edge);
        } else {
            total += 1; 
        }
    }
    return total;
}

TEST(GraphBasicsTest, DirectedGraphCreationAndInsertion) {
    std::unique_ptr<directed::Graph> g = std::make_unique<directed::Graph>();
    
    auto v1 = g->newVertex();
    auto v2 = g->newVertex();
    auto v3 = g->newVertex();

    EXPECT_EQ(g->getOrder(), 3) << "A ordem do grafo deve ser 3";

    auto e1 = g->newEdge(v1, v2, 10); // v1 -> v2
    g->newEdge(v2, v3, 5);            // v2 -> v3

    EXPECT_EQ(g->getSize(), 2) << "O tamanho (arestas) do grafo deve ser 2";
    
    EXPECT_EQ(g->getOpposite(v1, e1), v2) << "O oposto de v1 na aresta e1 deve ser v2";
}

TEST(GraphBasicsTest, UndirectedGraphSymmetry) {
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();

    auto v1 = g->newVertex();
    auto v2 = g->newVertex();
    auto e1 = g->newEdge(v1, v2, 10);

    EXPECT_EQ(g->getSize(), 1) << "Deve haver 1 aresta";
    
    EXPECT_EQ(g->getOpposite(v1, e1), v2) << "Oposto de v1 deve ser v2";
    EXPECT_EQ(g->getOpposite(v2, e1), v1) << "Oposto de v2 deve ser v1";
}

TEST(GraphBasicsTest, RemovalLogic) {
    std::unique_ptr<directed::Graph> g = std::make_unique<directed::Graph>();
    auto v1 = g->newVertex();
    auto v2 = g->newVertex();
    auto e1 = g->newEdge(v1, v2);

    EXPECT_EQ(g->getSize(), 1) << "Antes da remocao: 1 aresta";
    
    g->removeEdge(e1);
    EXPECT_EQ(g->getSize(), 0) << "Apos remover aresta: 0 arestas";

    g->removeVertex(v1);
    EXPECT_EQ(g->getOrder(), 1) << "Apos remover vertice: 1 vertice";
    
    // Comportamento defensivo: não deve lançar exceção ao remover nullptr
    EXPECT_NO_THROW({
        g->removeVertex(nullptr); 
    }) << "Remover nullptr nao deveria lançar excecao critica";
}

TEST(BFSTest, ShortestPathUnweighted) {
    std::unique_ptr<directed::Graph> g = std::make_unique<directed::Graph>();
    
    auto v0 = g->newVertex();
    auto v1 = g->newVertex();
    auto v2 = g->newVertex();
    auto v3 = g->newVertex();

    g->newEdge(v0, v1);
    g->newEdge(v1, v2);
    g->newEdge(v0, v3);
    g->newEdge(v3, v2);

    std::vector<common::Node*> path = util::BFS::getPath(g.get(), v0, v2);
    
    ASSERT_EQ(path.size(), 3) << "Caminho BFS deve ter 3 nós (Start, Middle, End)";
    EXPECT_EQ(path.front(), v0) << "Inicio do caminho deve ser v0";
    EXPECT_EQ(path.back(), v2) << "Fim do caminho deve ser v2";
}

TEST(BFSTest, ConnectedComponents) {
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();
    
    auto v1 = g->newVertex();
    auto v2 = g->newVertex();
    g->newEdge(v1, v2); // Componente 1
    auto v3 = g->newVertex(); // Componente 2 (isolado)

    auto components = util::BFS::getConnectedComponents(g.get());
    
    EXPECT_EQ(components[v1], components[v2]) << "v1 e v2 devem estar no mesmo componente";
    EXPECT_NE(components[v1], components[v3]) << "v1 e v3 devem estar em componentes diferentes";
}

TEST(DFSTest, ConnectedComponents) {
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();
    auto v1 = g->newVertex();
    auto v2 = g->newVertex();

    auto components = util::DFS::getConnectedComponents(g.get());
    
    EXPECT_GT(components.count(v1), 0) << "v1 deve estar mapeado";
    EXPECT_GT(components.count(v2), 0) << "v2 deve estar mapeado";
    EXPECT_NE(components[v1], components[v2]) << "v1 e v2 são componentes distintos";
}

TEST(HierholzerTest, EulerianCycle) {
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();
    
    auto v0 = g->newVertex();
    auto v1 = g->newVertex();
    auto v2 = g->newVertex();
    auto v3 = g->newVertex();
    auto v4 = g->newVertex();

    g->newEdge(v0, v1); g->newEdge(v1, v2); g->newEdge(v2, v0);
    g->newEdge(v2, v3); g->newEdge(v3, v4); g->newEdge(v4, v2);

    EXPECT_TRUE(util::Hierholzer::isEulerian(g.get())) << "O grafo deve ser identificado como Euleriano";

    auto cycle = util::Hierholzer::findEulerianCycle(g.get());
    EXPECT_EQ(cycle.size(), 7) << "O ciclo deve ter N_arestas + 1 vertices na sequencia";
    EXPECT_EQ(cycle.front(), cycle.back()) << "O ciclo Euleriano deve começar e terminar no mesmo vertice";
}

TEST(HierholzerTest, NonEulerianCycle) {
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();
    auto v0 = g->newVertex(); auto v1 = g->newVertex(); auto v2 = g->newVertex();

    g->newEdge(v0, v1); g->newEdge(v1, v2);

    EXPECT_FALSE(util::Hierholzer::isEulerian(g.get())) << "Grafo linear nao deve ser Euleriano";
    auto cycle = util::Hierholzer::findEulerianCycle(g.get());
    EXPECT_TRUE(cycle.empty()) << "Se nao for Euleriano, deve retornar vetor vazio";
}

TEST(ShortestPathTest, Dijkstra) {
    std::unique_ptr<directed::Graph> g = std::make_unique<directed::Graph>();
    
    auto v0 = g->newVertex();
    auto v1 = g->newVertex();
    auto v2 = g->newVertex();

    g->newEdge(v0, v1, 10);
    g->newEdge(v0, v2, 100);
    g->newEdge(v1, v2, 10);

    auto path = util::Dijkstra::getPathTo(g.get(), v0, v2);

    ASSERT_EQ(path.size(), 3) << "Dijkstra deve preferir o caminho indireto porem mais barato";
    EXPECT_EQ(path[1], v1) << "Caminho deve passar por v1";
}

TEST(ShortestPathTest, FloydWarshall) {
    std::unique_ptr<directed::Graph> g = std::make_unique<directed::Graph>();
    
    auto v0 = g->newVertex();
    auto v1 = g->newVertex();
    auto v2 = g->newVertex();

    g->newEdge(v0, v1, 2);
    g->newEdge(v1, v2, 3);
    g->newEdge(v0, v2, 10);

    auto result = util::Floyd::computeAllPairs(g.get());
    auto distMatrix = result.first;

    ASSERT_EQ(distMatrix.size(), 3) << "Matriz de distancias deve ser 3x3";
    EXPECT_EQ(distMatrix[0][1], 2);
    EXPECT_EQ(distMatrix[0][2], 5) << "Distancia 0->2 deve ser 5 (caminho indireto)";
    EXPECT_GT(distMatrix[1][0], 1000) << "Grafo direcionado: 1->0 deve ser inalcancavel";

    auto path = util::Floyd::getShortestPath(g.get(), v0, v2, result.second);
    EXPECT_EQ(path.size(), 3);
}

TEST(DisjointSetTest, UnionFindConnectivity) {
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();

    auto v1 = g->newVertex(); auto v2 = g->newVertex();
    auto v3 = g->newVertex(); auto v4 = g->newVertex();

    g->newEdge(v1, v2);
    g->newEdge(v3, v4);

    util::Set::clear();

    EXPECT_EQ(util::Set::componentsCount(g.get()), 2) << "Devem existir 2 componentes conexos";
    EXPECT_TRUE(util::Set::sameComponent(v1, v2));
    EXPECT_TRUE(util::Set::sameComponent(v3, v4));
    EXPECT_FALSE(util::Set::sameComponent(v1, v3));
}

TEST(GoodmanTest, GoodmanAlgorithmExecution) {
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();
    auto v1 = g->newVertex(); auto v2 = g->newVertex(); auto v3 = g->newVertex();
    
    g->newEdge(v1, v2); g->newEdge(v2, v3); g->newEdge(v3, v1);

    EXPECT_NO_THROW({
        int result = util::Goodman::goodmanAlgorithm(g.get());
        EXPECT_GE(result, 0) << "Goodman deve retornar um valor inteiro valido >= 0";
    });
}

TEST(MSTTest, Prim) {
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();
    auto v0 = g->newVertex(); auto v1 = g->newVertex(); 
    auto v2 = g->newVertex(); auto v3 = g->newVertex();

    g->newEdge(v0, v1, 1); g->newEdge(v1, v2, 2); g->newEdge(v2, v3, 3);
    g->newEdge(v0, v2, 4); g->newEdge(v1, v3, 5);

    std::vector<common::Edge*> mst = util::Prim::getMST(g.get());

    EXPECT_EQ(mst.size(), 3) << "MST de grafo com 4 vertices deve ter 3 arestas";
    EXPECT_EQ(calculateMSTCost(g.get(), mst), 6) << "Custo total da MST deve ser 6";
}

TEST(MSTTest, Kruskal) {
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();
    auto v0 = g->newVertex(); auto v1 = g->newVertex(); 
    auto v2 = g->newVertex(); auto v3 = g->newVertex();

    g->newEdge(v0, v1, 1); g->newEdge(v1, v2, 2); g->newEdge(v2, v3, 3);
    g->newEdge(v0, v2, 4); g->newEdge(v1, v3, 5);

    util::Set::clear();
    std::vector<common::Edge*> mst = util::Kruskal::getMST(g.get());

    EXPECT_EQ(mst.size(), 3);
    EXPECT_EQ(calculateMSTCost(g.get(), mst), 6);
}

TEST(AStarTest, PathfindingWithHeuristic) {
    std::unique_ptr<directed::Graph> g = std::make_unique<directed::Graph>();
    
    auto n1 = g->newVertex();
    auto n2 = g->newVertex();
    auto n3 = g->newVertex();

    g->newEdge(n1, n2, 5);
    g->newEdge(n1, n3, 10);
    g->newEdge(n2, n3, 2);

    auto path = util::AStar(g.get(), n1->getId(), n2->getId(), util::heuristics::zeroHeuristic);
    EXPECT_EQ(path.size(), 2) << "A* deve encontrar caminho direto em caso sem obstaculo";
}

TEST(ThetaStarTest, PathfindingWithLineOfSight) {
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();
    
    auto n1 = g->newVertex(std::pair<double, double>{0.0, 0.0});
    auto n2 = g->newVertex(std::pair<double, double>{1.0, 1.0});
    auto n3 = g->newVertex(std::pair<double, double>{2.0, 0.0});

    g->newEdge(n1, n2, 14); // cost 1.4 approx * 10
    g->newEdge(n2, n3, 14);

    // Without Line of Sight (or callback returning false), should follow edges
    auto pathNoLos = util::ThetaStar(g.get(), n1->getId(), n3->getId(), 
        util::heuristics::euclideanHeuristic2D, 
        [](common::Node*, common::Node*) { return false; });
    
    ASSERT_EQ(pathNoLos.size(), 3);
    EXPECT_EQ(pathNoLos[0], n1);
    EXPECT_EQ(pathNoLos[1], n2);
    EXPECT_EQ(pathNoLos[2], n3);

    // With Line of Sight, should skip n2 and go directly to n3
    auto pathLos = util::ThetaStar(g.get(), n1->getId(), n3->getId(), 
        util::heuristics::euclideanHeuristic2D, 
        [](common::Node*, common::Node*) { return true; });
    
    ASSERT_EQ(pathLos.size(), 2);
    EXPECT_EQ(pathLos[0], n1);
    EXPECT_EQ(pathLos[1], n3);
}

TEST(ThetaStarTest, LwThetaStarWithLineOfSight) {
    undirected::lwGraph<util::Vertex2D> g(3);
    g.setVertex(0, util::Vertex2D(0.0, 0.0));
    g.setVertex(1, util::Vertex2D(1.0, 1.0));
    g.setVertex(2, util::Vertex2D(2.0, 0.0));
    g.addEdge(0, 1, 1.414);
    g.addEdge(1, 2, 1.414);

    auto heuristic = [](const util::Vertex2D& a, const util::Vertex2D& b) {
        return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
    };

    // Without Line of Sight
    auto pathNoLos = util::lwThetaStar(g, 0, 2, heuristic, nullptr);
    ASSERT_EQ(pathNoLos.size(), 3);
    EXPECT_EQ(pathNoLos[0], 0);
    EXPECT_EQ(pathNoLos[1], 1);
    EXPECT_EQ(pathNoLos[2], 2);

    // With Line of Sight
    auto pathLos = util::lwThetaStar(g, 0, 2, heuristic, [](int, int) { return true; });
    ASSERT_EQ(pathLos.size(), 2);
    EXPECT_EQ(pathLos[0], 0);
    EXPECT_EQ(pathLos[1], 2);
}

TEST(PostmanTest, ChinesePostmanRoute) {
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();

    auto v0 = g->newVertex();
    auto v1 = g->newVertex();
    auto v2 = g->newVertex();

    g->newEdge(v0, v1, 2);
    g->newEdge(v1, v2, 3);

    auto route = util::Postman::findDeliveryRoute(g.get());

    ASSERT_FALSE(route.empty()) << "A rota nao deve ser vazia";
    EXPECT_EQ(route.front(), route.back()) << "A rota do carteiro deve ser um ciclo fechado";
    EXPECT_GE(route.size(), 4) << "A rota deve cobrir ida e volta";
}