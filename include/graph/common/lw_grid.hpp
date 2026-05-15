#pragma once

#include <vector>
#include <cmath>
#include <memory>

#include "graph/undirected/lw_graph.hpp"
#include "graph/util/node_data.hpp"

namespace common {
    class lwGrid {
    public:
        lwGrid(int w, int h, std::vector<unsigned int> data) 
            : width(w), height(h), grid(data) {}

        lwGrid(const common::lwGrid& other) 
            : width(other.width), height(other.height), grid(other.grid) {}

        bool isPassable(int x, int y) const {
            if (x < 0 || x >= width || y < 0 || y >= height) return false;
            return grid[y * width + x] == 1;
        }

        bool isPassable(int nodeId) const {
            if (nodeId < 0 || nodeId >= static_cast<int>(grid.size())) return false;
            return grid[nodeId] == 1;
        }

        void setPassable(int x, int y, bool passable) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                grid[y * width + x] = passable ? 1 : 0;
            }
        }

        void setPassable(int nodeId, bool passable) {
            if (nodeId >= 0 && nodeId < static_cast<int>(grid.size())) {
                grid[nodeId] = passable ? 1 : 0;
            }
        }

        int getWidth() const { return width; }
        int getHeight() const { return height; }

    private:
        int width;
        int height;

        std::vector<unsigned int> grid;
    };
}
