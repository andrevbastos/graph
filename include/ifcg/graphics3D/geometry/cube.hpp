#pragma once

#include "ifcg/graphics3D/geometry/mesh.hpp"

namespace mesh3D
{
    class Cube3D : public Mesh3D
    {
    public:
        Cube3D(GLuint shaderID);
    };
}
