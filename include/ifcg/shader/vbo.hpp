#pragma once

/**
* \author André B.
* \date February 27, 2025
* \version March 24, 2025
* \brief Constructor for VBO class
**/

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "ifcg/common/vertex.hpp"

namespace vbo
{
    class VBO
    {
    public:
        GLuint _id;
        VBO(const std::vector<Vertex>& vertices);
    
        void bind();
        void unbind();
        void destroy();
    };
}

using namespace vbo;