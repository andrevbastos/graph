#include "ifcg/graphics3D/geometry/pyramid.hpp"

namespace mesh3D
{
    Pyramid3D::Pyramid3D(GLuint shaderID)
        : Mesh3D({
            Vertex{+1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
            Vertex{+1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
            Vertex{-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
            Vertex{-1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
    
            Vertex{-1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
            Vertex{+1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
            Vertex{0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    
            Vertex{+1.0f, -1.0f, +1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
            Vertex{+1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
            Vertex{0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f},
    
            Vertex{+1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
            Vertex{-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
            Vertex{0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    
            Vertex{-1.0f, -1.0f, +1.0f, 0.0f, 1.0f, 0.0f, 1.0f},
            Vertex{-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f},
            Vertex{0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f}
        },
        {
            0, 1, 3,
            1, 2, 3,
    
            4, 5, 6,
    
            7, 8, 9,
    
            10, 11, 12,
    
            13, 14, 15
        },
        shaderID
    ) {}
}
