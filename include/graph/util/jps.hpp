#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <cmath>

#include "graph/util/node_data.hpp"
#include "graph/undirected/lw_graph.hpp"

namespace util {
    typedef std::pair<double, int> PQElement;
    typedef std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> PQLoc;

    class JumpPointSearchLw {
    public:
        JumpPointSearchLw(undirected::lwGraph<Vertex2D>& graph, int width, int height) 
            : grid(graph), width(width), height(height) 
        {
        };

        std::vector<int> find(int startId, int endId, std::function<double(const Vertex2D&, const Vertex2D&)> heuristic) {
            if (startId < 0 || startId >= width * height || endId < 0 || endId >= width * height) return {};
            if (startId == endId) return {startId};

            Vertex2D start = toVertex(startId);
            Vertex2D target = toVertex(endId);

            PQLoc openSet;
            std::unordered_map<int, double> gScore;
            std::unordered_map<int, int> parent;

            openSet.push({heuristic(start, target), startId});
            gScore[startId] = 0.0;
            parent[startId] = -1;

            while (!openSet.empty()) {
                int currentId = openSet.top().second;
                openSet.pop();

                Vertex2D current = toVertex(currentId);
                if (current == target) {
                    std::vector<int> path;
                    int curr = currentId;
                    while (curr != -1) {
                        path.push_back(curr);
                        curr = parent[curr];
                    }
                    std::reverse(path.begin(), path.end());
                    return path;
                }

                Vertex2D p = (parent[currentId] == -1) ? none : toVertex(parent[currentId]);
                for (const auto& next : successors(current, p, target)) {
                    int nextId = toIndex(next);
                    double d = dist(current, next);
                    double tentative_gScore = gScore[currentId] + d;

                    if (gScore.find(nextId) == gScore.end() || tentative_gScore < gScore[nextId]) {
                        gScore[nextId] = tentative_gScore;
                        parent[nextId] = currentId;
                        double fScore = tentative_gScore + heuristic(next, target);
                        openSet.push({fScore, nextId});
                    }
                }
            }

            return {};
        };

    private:
        undirected::lwGraph<Vertex2D> grid;
        int width, height;

        const Vertex2D none {-1, -1};
        const std::vector<Vertex2D> directions {
            {1, 0}, {-1, 0}, {0, -1}, {0, 1},
            {1, 1}, {-1, 1}, {1, -1}, {-1, -1}
        };

        int toIndex(const Vertex2D& loc) const {
            return static_cast<int>(loc.y) * width + static_cast<int>(loc.x);
        }

        Vertex2D toVertex(int index) const {
            return { static_cast<double>(index % width), static_cast<double>(index / width) };
        }

        double dist(const Vertex2D& a, const Vertex2D& b) const {
            double dx = std::abs(a.x - b.x);
            double dy = std::abs(a.y - b.y);
            return (dx + dy) + (std::sqrt(2.0) - 2.0) * std::min(dx, dy);
        }

        bool inBounds(const Vertex2D& loc) const noexcept { 
            return 0 <= loc.x && loc.x < width && 0 <= loc.y && loc.y < height; 
        };

        bool passable(const Vertex2D& loc) const { 
            if (!inBounds(loc)) return false;
            return !grid.adj(toIndex(loc)).empty(); 
        };

        bool forced(const Vertex2D& loc, const Vertex2D& parent, const Vertex2D& travelDir) {
            const Vertex2D dir {(loc - parent).direction()};
            if(travelDir.x != 0 && travelDir.y != 0) {
                if((dir.x == travelDir.x && dir.y == -travelDir.y) || 
                    (dir.x == -travelDir.x && dir.y == travelDir.y)){
                    return true;
                }
            } else if(dir.x != 0 && dir.y != 0){
                return true;
            }
            return false;
        };

        bool validMove(const Vertex2D& loc, const Vertex2D& dir) {
            const auto nextLoc {loc + dir};
            if (!inBounds(nextLoc) || !passable(nextLoc)) return false;

            if(dir.x != 0 && dir.y != 0){
                return passable(loc + Vertex2D{dir.x, 0}) || passable(loc + Vertex2D{0, dir.y});
            }
            return true;
        };

        std::vector<Vertex2D> getNeighbours(const Vertex2D& vertex, const std::vector<Vertex2D>& dirs) {
            std::vector<Vertex2D> result;
            for(auto dir : dirs){
                if(validMove(vertex, dir)){
                    result.push_back(vertex + dir);
                }
            }
            return result;
        };

        std::vector<Vertex2D> prunedNeighbours(const Vertex2D& current, const Vertex2D& parent) {
            if (parent == none) {
                return getNeighbours(current, directions);
            }

            std::vector<Vertex2D> result;
            const auto dir {(current - parent).direction()};
            
            if (dir.x != 0 && dir.y != 0) { // Diagonal
                const Vertex2D dirX {dir.x, 0};
                const Vertex2D dirY {0, dir.y};

                if (validMove(current, dir)) result.push_back(current + dir);
                if (validMove(current, dirX)) result.push_back(current + dirX);
                if (validMove(current, dirY)) result.push_back(current + dirY);

                const auto previous {current - dir};
                if (!validMove(previous, dirX) && validMove(previous, dirX * 2)) {
                    result.push_back(previous + (dirX * 2));
                }
                if (!validMove(previous, dirY) && validMove(previous, dirY * 2)) {
                    result.push_back(previous + (dirY * 2));
                }
            } else {
                if (validMove(current, dir)) {
                    result.push_back(current + dir);

                    const Vertex2D invertedDir {dir.y, dir.x};
                    if (!validMove(current, invertedDir) && validMove(current, invertedDir + dir)) {
                        result.push_back(current + invertedDir + dir);
                    }
                    if (!validMove(current, -invertedDir) && validMove(current, -invertedDir + dir)) {
                        result.push_back(current - invertedDir + dir);
                    }
                }
            }
            return result;
        };

        Vertex2D jump(const Vertex2D& source, const Vertex2D& target, const Vertex2D& direction) {
            auto newLoc {source + direction};

            if (!validMove(source, direction)) return none;
            if (newLoc == target) return newLoc;

            for (const auto& next : prunedNeighbours(newLoc, source)) {
                if (forced(next, newLoc, direction)) {
                    return newLoc;
                }
            }
            if(direction.x != 0 && direction.y != 0){
                for(const auto& new_dir: {Vertex2D{direction.x, 0}, Vertex2D{0, direction.y}}){
                    const auto jumpPoint {jump(newLoc, target, new_dir)};
                    if(jumpPoint != none){
                        return newLoc;
                    }
                }
            }
            return jump(newLoc, target, direction);
        };

        std::vector<Vertex2D> successors(const Vertex2D& source, const Vertex2D& parent, const Vertex2D& target) {
            std::vector<Vertex2D> successorsList;
            auto neighboursList {prunedNeighbours(source, parent)};
            for(const auto& n : neighboursList){
                auto jumpPoint {jump(source, target, (n - source).direction())};
                if(jumpPoint != none){
                    successorsList.push_back(jumpPoint);
                }
            }
            return successorsList;
        };
    };
}
