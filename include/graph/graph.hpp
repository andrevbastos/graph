#include <iostream>
#include <string>
#include <fstream>

#include "graph/common/graph.hpp"
#include "graph/common/node.hpp"
#include "graph/directed/graph.hpp"
#include "graph/directed/node.hpp"
#include "graph/undirected/graph.hpp"
#include "graph/undirected/node.hpp"
#include "graph/util/bfs.hpp"
#include "graph/util/dfs.hpp"
#include "graph/util/goodman.hpp"
#include "graph/util/union_find.hpp"
#include "graph/util/dijkstra.hpp"
#include "graph/util/floyd.hpp"
#include "graph/util/a_star.hpp"
#include "graph/util/hierholzer.hpp"
#include "graph/util/kruskal.hpp"
#include "graph/util/prim.hpp"
#include "json/json.hpp"

using json = nlohmann::json;
static common::Graph* loadGraphFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filePath << std::endl;
        return nullptr;
    }

    json data = json::parse(file);

    common::Graph* graph = nullptr;
    if (data["directed"]) {
        using namespace directed;
        graph = new Graph();

        for (const auto& nodeData : data["nodes"]) {
            graph->newVertex(nodeData["label"]);
        }

        for (const auto& edgeData : data["edges"]) {
            graph->newEdge(edgeData["source"], edgeData["target"]);
        }
    } else {
        using namespace undirected;
        graph = new Graph();

        for (const auto& nodeData : data["nodes"]) {
            graph->newVertex(nodeData["label"]);
        }

        for (const auto& edgeData : data["edges"]) {
            graph->newEdge(edgeData["source"], edgeData["target"]);
        }
    }

    return graph;
}