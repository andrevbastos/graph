#pragma once

#include <math.h>
#include "ifcg/graphics2D/geometry/mesh.hpp"

namespace mesh2D
{
    class Circle2D : public Mesh2D
    {
    public:
        Circle2D(int precision, GLuint shaderID);
    private:
        std::vector<Vertex> vertices(int precision);
        std::vector<GLuint> indices(int precision);
    };
}