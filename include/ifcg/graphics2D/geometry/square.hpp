#pragma once

#include "ifcg/graphics2D/geometry/mesh.hpp"

namespace mesh2D
{
    class Square2D : public Mesh2D
    {
    public:
        Square2D(GLuint shaderID);
    };
}