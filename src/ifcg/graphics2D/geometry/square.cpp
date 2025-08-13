#include "ifcg/graphics2D/geometry/square.hpp"

namespace mesh2D
{
    Square2D::Square2D(GLuint shaderID)
        : Mesh2D({
            Vertex{1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
            Vertex{1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f },
            Vertex{-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
            Vertex{-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f }
        },
        {
            0, 1, 3,
            1, 2, 3
        },
        shaderID
    ) {}
};
