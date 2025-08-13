#pragma once

#include <math.h>
#include "ifcg/graphics3D/geometry/mesh.hpp"

namespace mesh3D
{
    class Sphere3D : public Mesh3D
    {
    public:
        Sphere3D(int precision, GLuint shaderID);
    private:
        std::vector<Vertex> vertices(int precision);
        std::vector<GLuint> indices(int precision);
    };
};