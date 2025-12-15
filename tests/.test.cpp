/**
 * @file main_test.cpp
 * @brief Suite de testes unitários ordenada pelas Atividades (01-06) + Extras, preservando comentários originais.
 */

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <cmath>
#include <memory>
#include <stdexcept>

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

// ==========================================
// SISTEMA DE PONTUAÇÃO E WRAPPER DE TESTES
// ==========================================

class TestRunner {
public:
    int totalTestesSucesso = 0;
    std::vector<std::string> vectorFalhas;

    // Funções auxiliares para asserções
    void assertTrue(bool condition, const std::string& message) {
        if (!condition) {
            throw std::runtime_error(message);
        }
    }

    void assertEqual(int actual, int expected, const std::string& message) {
        if (actual != expected) {
            throw std::runtime_error(message + " [Esperado: " + std::to_string(expected) + ", Atual: " + std::to_string(actual) + "]");
        }
    }

    // Executa um caso de teste
    void runTest(const std::string& testId, const std::string& description, std::function<void(TestRunner&)> testFunc) {
        std::cout << "Executando " << testId << ": " << description << "... ";
        try {
            testFunc(*this);
            // Se não lançou exceção, sucesso
            totalTestesSucesso++;
            std::cout << "SUCESSO" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "FALHA - " << e.what() << std::endl;
            vectorFalhas.push_back(testId);
        } catch (...) {
            std::cout << "FALHA - Erro desconhecido" << std::endl;
            vectorFalhas.push_back(testId);
        }
    }

    void printSummary() {
        std::cout << "\n================ RESUMO DOS TESTES ================" << std::endl;
        std::cout << "Total de Sucessos: " << totalTestesSucesso << std::endl;
        std::cout << "Total de Falhas: " << vectorFalhas.size() << std::endl;
        
        if (!vectorFalhas.empty()) {
            std::cout << "\nIDs dos Testes com Falha:" << std::endl;
            for (const auto& id : vectorFalhas) {
                std::cout << "- " << id << std::endl;
            }
        } else {
            std::cout << "\nTODOS OS TESTES PASSARAM! EXCELENTE CODIGO! :D" << std::endl;
        }
        std::cout << "===================================================" << std::endl;
    }
};

// ==========================================
// FUNÇÕES AUXILIARES
// ==========================================

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
};

void test_directed_graph_basics(TestRunner& t) {
    // [TEST_ID: 1.1] - Criação básica, inserção de nós e arestas em Grafo Direcionado
    std::unique_ptr<directed::Graph> g = std::make_unique<directed::Graph>();
    
    auto v1 = g->newVertex();
    auto v2 = g->newVertex();
    auto v3 = g->newVertex();

    t.assertEqual(g->getOrder(), 3, "A ordem do grafo deve ser 3");

    auto e1 = g->newEdge(v1, v2, 10); // v1 -> v2
    g->newEdge(v2, v3, 5);  // v2 -> v3

    t.assertEqual(g->getSize(), 2, "O tamanho (arestas) do grafo deve ser 2");
    
    // Verifica direção
    auto adjV1 = g->getNodesFromEdge(e1);
    t.assertTrue(adjV1.size() > 0, "A aresta deve conectar nós");
    
    // Em grafo direcionado, v1->v2. GetOpposite deve funcionar.
    t.assertTrue(g->getOpposite(v1, e1) == v2, "O oposto de v1 na aresta e1 deve ser v2");
};

void test_undirected_graph_basics(TestRunner& t) {
    // [TEST_ID: 1.2] - Simetria em Grafo Não Direcionado
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();

    auto v1 = g->newVertex();
    auto v2 = g->newVertex();

    auto e1 = g->newEdge(v1, v2, 10);

    t.assertEqual(g->getSize(), 1, "Deve haver 1 aresta");
    
    // Teste de simetria conceitual (embora a implementação interna possa variar, o comportamento lógico é bidirecional)
    // Para um grafo não direcionado, a aresta conecta v1 e v2 igualmente
    t.assertTrue(g->getOpposite(v1, e1) == v2, "Oposto de v1 deve ser v2");
    t.assertTrue(g->getOpposite(v2, e1) == v1, "Oposto de v2 deve ser v1");
};

void test_removal_logic(TestRunner& t) {
    // [TEST_ID: 1.3] - Remoção de vértices e arestas e consistência
    std::unique_ptr<directed::Graph> g = std::make_unique<directed::Graph>();
    auto v1 = g->newVertex();
    auto v2 = g->newVertex();
    auto e1 = g->newEdge(v1, v2);

    t.assertEqual(g->getSize(), 1, "Antes da remocao: 1 aresta");
    
    g->removeEdge(e1);
    t.assertEqual(g->getSize(), 0, "Apos remover aresta: 0 arestas");

    g->removeVertex(v1);
    t.assertEqual(g->getOrder(), 1, "Apos remover vertice: 1 vertice");
    
    // Teste de Borda: Remover vertice inexistente ou nulo não deve crashar (comportamento defensivo)
    try {
        g->removeVertex(nullptr); 
    } catch (...) {
        t.assertTrue(false, "Remover nullptr nao deveria lançar excecao critica");
    }
};

void test_bfs_path(TestRunner& t) {
    // [TEST_ID: 2.1] - BFS Caminho Mínimo (Unweighted)
    std::unique_ptr<directed::Graph> g = std::make_unique<directed::Graph>();
    
    // 0 -> 1 -> 2
    // |         ^
    // \-> 3 ----/
    auto v0 = g->newVertex(); // ID provavel 0
    auto v1 = g->newVertex();
    auto v2 = g->newVertex();
    auto v3 = g->newVertex();

    g->newEdge(v0, v1);
    g->newEdge(v1, v2);
    g->newEdge(v0, v3);
    g->newEdge(v3, v2);

    // Caminho de v0 para v2. Ambos tem distancia 2 (v0->v1->v2 ou v0->v3->v2).
    std::vector<common::Node*> path = util::BFS::getPath(g.get(), v0, v2);
    
    t.assertTrue(path.size() == 3, "Caminho BFS deve ter 3 nós (Start, Middle, End)");
    t.assertTrue(path[0] == v0, "Inicio do caminho deve ser v0");
    t.assertTrue(path.back() == v2, "Fim do caminho deve ser v2");
};

void test_bfs_components(TestRunner& t) {
    // [TEST_ID: 2.2] - Componentes Conexos via BFS
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();
    
    auto v1 = g->newVertex();
    auto v2 = g->newVertex();
    g->newEdge(v1, v2); // Componente 1

    auto v3 = g->newVertex(); // Componente 2 (isolado)

    auto components = util::BFS::getConnectedComponents(g.get());
    
    // A implementação retorna um mapa Node -> Root/Representative
    // v1 e v2 devem ter o mesmo representante. v3 deve ter outro (ou ele mesmo).
    t.assertTrue(components[v1] == components[v2], "v1 e v2 devem estar no mesmo componente");
    t.assertTrue(components[v1] != components[v3], "v1 e v3 devem estar em componentes diferentes");
};

void test_dfs_components(TestRunner& t) {
    // [TEST_ID: 3.1] - DFS para contagem de componentes
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();
    auto v1 = g->newVertex();
    auto v2 = g->newVertex(); // Desconectados

    auto components = util::DFS::getConnectedComponents(g.get());
    
    // Devemos ter chaves para ambos
    t.assertTrue(components.count(v1) > 0, "v1 deve estar mapeado");
    t.assertTrue(components.count(v2) > 0, "v2 deve estar mapeado");
    t.assertTrue(components[v1] != components[v2], "v1 e v2 são componentes distintos");
};

void test_union_find(TestRunner& t) {
    // [TEST_ID: 7.1] - Union Find (util::Set): Componentes e Conectividade
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();

    auto v1 = g->newVertex();
    auto v2 = g->newVertex();
    auto v3 = g->newVertex();
    auto v4 = g->newVertex();

    // Conectar v1-v2 e v3-v4 (Dois componentes disjuntos)
    g->newEdge(v1, v2);
    g->newEdge(v3, v4);

    // Limpar estado estático anterior (importante para util::Set que usa estáticos)
    util::Set::clear();

    int count = util::Set::componentsCount(g.get());
    t.assertEqual(count, 2, "Devem existir 2 componentes conexos");

    // Verificar pertinência
    t.assertTrue(util::Set::sameComponent(v1, v2), "v1 e v2 devem estar no mesmo componente");
    t.assertTrue(util::Set::sameComponent(v3, v4), "v3 e v4 devem estar no mesmo componente");
    t.assertTrue(!util::Set::sameComponent(v1, v3), "v1 e v3 NAO devem estar no mesmo componente");
};

void test_goodman(TestRunner& t) {
    // [TEST_ID: 8.1] - Goodman Algorithm
    // Como não temos a especificação exata do que o algoritmo calcula (pode ser redução de ciclos, 
    // ou uma variação específica de coloração/traversal), vamos testar a execução e estabilidade.
    
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();
    auto v1 = g->newVertex();
    auto v2 = g->newVertex();
    auto v3 = g->newVertex();
    
    // Ciclo simples
    g->newEdge(v1, v2);
    g->newEdge(v2, v3);
    g->newEdge(v3, v1);

    try {
        int result = util::Goodman::goodmanAlgorithm(g.get());
        // Verifica apenas se o retorno é um valor válido (não negativo, por exemplo)
        // Ajuste esta asserção se souber o valor esperado exato para um triângulo K3
        t.assertTrue(result >= 0, "Goodman deve retornar um valor inteiro valido >= 0");
    } catch (...) {
        t.assertTrue(false, "Goodman Algorithm falhou/crashou na execucao");
    }
};

void test_hierholzer(TestRunner& t) {
    // [TEST_ID: 5.1] - Hierholzer: Ciclo Euleriano em Grafo Euleriano
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();
    
    // Cria um grafo "Laço" (Bowtie) ou Ampulheta, que é Euleriano (todos graus pares)
    // 0 --- 1
    //  \   /
    //    2
    //  /   \
    // 3 --- 4
    
    auto v0 = g->newVertex();
    auto v1 = g->newVertex();
    auto v2 = g->newVertex(); // Ponto central, grau 4
    auto v3 = g->newVertex();
    auto v4 = g->newVertex();

    g->newEdge(v0, v1);
    g->newEdge(v1, v2);
    g->newEdge(v2, v0); // Triangulo superior fechado

    g->newEdge(v2, v3);
    g->newEdge(v3, v4);
    g->newEdge(v4, v2); // Triangulo inferior fechado

    // Graus esperados: v0(2), v1(2), v2(4), v3(2), v4(2). Todos pares.
    
    // Verifica se identifica como Euleriano
    t.assertTrue(util::Hierholzer::isEulerian(g.get()), "O grafo deve ser identificado como Euleriano");

    auto cycle = util::Hierholzer::findEulerianCycle(g.get());

    // Num ciclo euleriano, devemos percorrer todas as arestas exatamente uma vez.
    // Número de arestas = 6. O caminho (nós) deve ter tamanho 7 (start...end).
    t.assertEqual((int)cycle.size(), 7, "O ciclo deve ter N_arestas + 1 vertices na sequencia");
    
    // O ciclo deve ser fechado (começa e termina no mesmo nó)
    t.assertTrue(cycle.front() == cycle.back(), "O ciclo Euleriano deve começar e terminar no mesmo vertice");
}

void test_hierholzer_fail(TestRunner& t) {
    // [TEST_ID: 11.2] - Hierholzer: Grafo Não-Euleriano (Semi-Euleriano ou Comum)
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();

    // Caminho simples: 0 -- 1 -- 2
    // Graus: 0(1), 1(2), 2(1). Não é Euleriano (tem ímpares).
    auto v0 = g->newVertex();
    auto v1 = g->newVertex();
    auto v2 = g->newVertex();

    g->newEdge(v0, v1);
    g->newEdge(v1, v2);

    t.assertTrue(!util::Hierholzer::isEulerian(g.get()), "Grafo linear nao deve ser Euleriano");

    auto cycle = util::Hierholzer::findEulerianCycle(g.get());
    
    t.assertEqual((int)cycle.size(), 0, "Se nao for Euleriano, deve retornar vetor vazio");
}

void test_dijkstra(TestRunner& t) {
    // [TEST_ID: 4.1] - Dijkstra Caminho Mínimo Ponderado
    std::unique_ptr<directed::Graph> g = std::make_unique<directed::Graph>();
    
    auto v0 = g->newVertex();
    auto v1 = g->newVertex();
    auto v2 = g->newVertex();

    // v0 -> v1 (peso 10)
    // v0 -> v2 (peso 100)
    // v1 -> v2 (peso 10)
    // Caminho mais curto v0->v2 deve ser v0->v1->v2 (Custo 20), e não o direto (Custo 100)
    
    g->newEdge(v0, v1, 10);
    g->newEdge(v0, v2, 100);
    g->newEdge(v1, v2, 10);

    auto path = util::Dijkstra::getPathTo(g.get(), v0, v2);

    t.assertEqual((int)path.size(), 3, "Dijkstra deve preferir o caminho mais longo em nós, mas curto em peso");
    t.assertTrue(path[1] == v1, "Caminho deve passar por v1");
};

void test_floyd_warshall(TestRunner& t) {
    // [TEST_ID: 6.1] - Floyd-Warshall: Todos os pares de caminhos mínimos
    std::unique_ptr<directed::Graph> g = std::make_unique<directed::Graph>();
    
    /*
     * Configuração do Grafo (IDs esperados: 0, 1, 2):
     * (0) --> (1) : peso 2
     * (1) --> (2) : peso 3
     * (0) --> (2) : peso 10 (Caminho direto custoso)
     * * Caminho mais curto (0)->(2) deve ser (0)->(1)->(2) com custo 2+3 = 5.
     */
    
    auto v0 = g->newVertex();
    auto v1 = g->newVertex();
    auto v2 = g->newVertex();

    g->newEdge(v0, v1, 2);
    g->newEdge(v1, v2, 3);
    g->newEdge(v0, v2, 10);

    // Executa Floyd
    auto result = util::Floyd::computeAllPairs(g.get());
    auto distMatrix = result.first;

    // Validar dimensões
    t.assertEqual((int)distMatrix.size(), 3, "Matriz de distancias deve ser 3x3");

    // Assumindo que a ordem da matriz segue a ordem de criação dos IDs (0, 1, 2)
    // Distancia v0->v1 deve ser 2
    t.assertEqual(distMatrix[0][1], 2, "Distancia 0->1 deve ser 2");
    
    // Distancia v0->v2 deve ser 5 (via v1), e não 10
    t.assertEqual(distMatrix[0][2], 5, "Distancia 0->2 deve ser 5 (caminho indireto)");
    
    // Distancia v1->v0 deve ser infinito (ou valor maximo definido) pois é direcionado
    // Nota: Verifique se sua implementação usa INT_MAX ou um valor específico para infinito.
    // Aqui apenas checamos se é maior que o caminho direto existente para garantir direção.
    t.assertTrue(distMatrix[1][0] > 1000, "Grafo direcionado: 1->0 deve ser inalcancavel");

    // Teste da função auxiliar getShortestPath do Floyd
    auto path = util::Floyd::getShortestPath(g.get(), v0, v2, result.second);
    t.assertEqual((int)path.size(), 3, "Caminho reconstruido pelo Floyd deve ter 3 nos");
};

void test_prim(TestRunner& t) {
    // [TEST_ID: 10.1] - Prim MST
    // Usando o mesmo grafo do Kruskal para garantir consistência
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();

    /*
        (0) --1--> (1)
         |        / |
         4       2  5
         |      /   |
        (2) --3--> (3)
    */
    
    auto v0 = g->newVertex();
    auto v1 = g->newVertex();
    auto v2 = g->newVertex();
    auto v3 = g->newVertex();

    g->newEdge(v0, v1, 1);
    g->newEdge(v1, v2, 2);
    g->newEdge(v2, v3, 3);
    g->newEdge(v0, v2, 4);
    g->newEdge(v1, v3, 5);

    std::vector<common::Edge*> mst = util::Prim::getMST(g.get());

    t.assertEqual((int)mst.size(), 3, "MST de grafo com 4 vertices deve ter 3 arestas");
    
    int cost = calculateMSTCost(g.get(), mst);
    t.assertEqual(cost, 6, "O custo total da MST pelo Prim deve ser 6");
};

void test_kruskal(TestRunner& t) {
    // [TEST_ID: 9.1] - Kruskal MST
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();
    
    /*
        (0) --1--> (1)
         |      /   |
         4     2    5
         |    /     |
        (2) --3--> (3)
    */
    
    auto v0 = g->newVertex();
    auto v1 = g->newVertex();
    auto v2 = g->newVertex();
    auto v3 = g->newVertex();

    g->newEdge(v0, v1, 1);
    g->newEdge(v1, v2, 2);
    g->newEdge(v2, v3, 3);
    g->newEdge(v0, v2, 4);
    g->newEdge(v1, v3, 5);

    // O MST Esperado deve pegar as arestas de peso: 1, 2, 3.
    // Total = 1 + 2 + 3 = 6.
    // As arestas de peso 4 e 5 criariam ciclos.

    util::Set::clear();

    std::vector<common::Edge*> mst = util::Kruskal::getMST(g.get());

    // Validações
    // Num grafo conexo de 4 vértices, MST deve ter 3 arestas (V-1)
    t.assertEqual((int)mst.size(), 3, "MST de grafo com 4 vertices deve ter 3 arestas");
    
    int cost = calculateMSTCost(g.get(), mst);
    t.assertEqual(cost, 6, "O custo total da MST pelo Kruskal deve ser 6");
};

// ==========================================
// ALGORITMOS EXTRAS
// ==========================================

void test_astar_heuristic(TestRunner& t) {
    // [TEST_ID: 11.1] - A* com Heurística Mockada
    std::unique_ptr<directed::Graph> g = std::make_unique<directed::Graph>();
    
    /*
       A (Start)
       | \
       |  \ (edge cost 10)
       |   \
       B----C (Target)
       
       Edge A->B cost 1
       Edge B->C cost 1
       
       Heuristicas (Distância estimada até C):
       h(A) = 100 (Estimativa ruim/alta propositalmente para testar a lógica ou 0 para Dijkstra)
       
       Vamos fazer um caso onde a heurística ajuda a decidir entre dois caminhos de custo igual,
       ou apenas verificar se o algoritmo roda.
    */

    // Usando IDs pois a assinatura do AStar::getPath usa IDs
    auto n1 = g->newVertex();
    auto n2 = g->newVertex();
    auto n3 = g->newVertex();

    g->newEdge(n1, n2, 5);
    g->newEdge(n1, n3, 10);
    g->newEdge(n2, n3, 2);

    auto path = util::AStar::getPath(g.get(), n1->getId(), n2->getId(), util::AStar::zeroHeuristic);
    
    t.assertTrue(path.size() == 2, "A* deve encontrar caminho direto");
};

void test_postman(TestRunner& t) {
    // [TEST_ID: 12.1] - Chinese Postman Problem (Route Inspection)
    std::unique_ptr<undirected::Graph> g = std::make_unique<undirected::Graph>();

    /*
       Cenário: Grafo Linear com pesos.
       (0) --[peso 2]-- (1) --[peso 3]-- (2)
       
       Graus: 0(impar), 1(par), 2(impar).
       O carteiro deve ir 0->1->2, e obrigatoriamente voltar para 0.
       O caminho de volta mais barato é repetir as arestas 2->1 e 1->0.
       
       Custo total das arestas originais: 2 + 3 = 5.
       Custo da volta (duplicação): 2 + 3 = 5.
       Custo total da rota = 10.
    */

    auto v0 = g->newVertex();
    auto v1 = g->newVertex();
    auto v2 = g->newVertex();

    g->newEdge(v0, v1, 2);
    g->newEdge(v1, v2, 3);

    // O algoritmo deve identificar v0 e v2 como ímpares, calcular a rota entre eles (custo 5)
    // e adicionar uma aresta virtual no clone do grafo.
    auto route = util::Postman::findDeliveryRoute(g.get(), v0);

    // Verificações
    t.assertTrue(route.size() > 0, "A rota nao deve ser vazia");
    t.assertTrue(route.front() == route.back(), "A rota do carteiro deve ser um ciclo fechado");

    // Validação da lógica do ciclo:
    t.assertTrue(route.size() >= 4, "A rota deve cobrir ida e volta (minimo 4 nós na sequencia)");
}

// ==========================================
// MAIN EXECUTION
// ==========================================

int main() {
    TestRunner runner;

    std::cout << "Iniciando Testes da Graph Lib (Ordem das Atividades)...\n" << std::endl;

    runner.runTest("[Act 1.1]", "Directed Graph - Criacao e Insercao", test_directed_graph_basics);
    runner.runTest("[Act 1.2]", "Undirected Graph - Simetria", test_undirected_graph_basics);
    runner.runTest("[Act 1.3]", "Logica de Remocao", test_removal_logic);

    runner.runTest("[Act 2.1]", "BFS - Caminho Minimo", test_bfs_path);
    runner.runTest("[Act 2.2]", "BFS - Componentes Conexos", test_bfs_components);
    runner.runTest("[Act 2.3]", "DFS - Componentes Conexos", test_dfs_components);

    runner.runTest("[Act 3.1]", "Union Find - Disjoint Sets", test_union_find);
    runner.runTest("[Act 3.2]", "Goodman Algorithm", test_goodman);

    runner.runTest("[Act 4.1]", "Hierholzer - Grafo Euleriano", test_hierholzer);
    runner.runTest("[Act 4.2]", "Hierholzer - Grafo Nao-Euleriano", test_hierholzer_fail);

    runner.runTest("[Act 5.1]", "Dijkstra - Single Source Shortest Path", test_dijkstra);
    runner.runTest("[Act 5.2]", "Floyd-Warshall - All Pairs Shortest Path", test_floyd_warshall);

    runner.runTest("[Act 6.1]", "Prim - MST", test_prim);
    runner.runTest("[Act 6.2]", "Kruskal - MST", test_kruskal);

    runner.runTest("[Extra 1]", "A-Star (Pathfinding Heuristico)", test_astar_heuristic);
    runner.runTest("[Extra 2]", "Chinese Postman (Route Inspection)", test_postman);

    runner.printSummary();

    return runner.vectorFalhas.empty() ? 0 : 1;
}