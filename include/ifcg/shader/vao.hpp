#pragma once

/**
* \author André B.
* \date February 27, 2025
* \version March 13, 2025
* \brief Constructor for VAO class
**/

#include <glad/glad.h>
#include "ifcg/shader/vbo.hpp"

namespace vao
{
    class VAO
    {
    public:
        GLuint _id;
        VAO();
    
        void linkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
        void bind();
        void unbind();
        void destroy();
    };
}

using namespace vao;