#include "ifcg/shader/vao.hpp"

namespace vao
{
    VAO::VAO()
    {
        glGenVertexArrays(1, &_id);
    }

    void VAO::linkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
    {
        vbo.bind();
        glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
        glEnableVertexAttribArray(layout);
    }

    void VAO::bind()
    {
        glBindVertexArray(_id);
    }

    void VAO::unbind()
    {
        glBindVertexArray(0);
    }

    void VAO::destroy()
    {
        glDeleteVertexArrays(1, &_id);
    }
}