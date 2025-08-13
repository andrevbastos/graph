#pragma once

#include "ifcg/graphics2D/geometry/mesh.hpp"

namespace mesh2D
{
    class Triangle2D : public Mesh2D
    {
    public:
        Triangle2D(GLuint shaderID);
    };
}