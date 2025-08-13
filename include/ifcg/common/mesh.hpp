#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ifcg/shader/vao.hpp"
#include "ifcg/shader/vbo.hpp"
#include "ifcg/shader/ebo.hpp"
#include "ifcg/shader/shader.hpp"

namespace ifcg
{
    class Mesh
    {
    public:
        virtual ~Mesh() = default;
        virtual Mesh* duplicate() const = 0;

        virtual void draw(glm::mat4 m = glm::mat4(1.0f)) = 0;
        virtual void setOutline(bool outline) = 0;

        virtual void translate(float tX, float tY, float tZ = 0) = 0;
        virtual void scale(float sX, float sY, float sZ = 0) = 0;
        virtual void rotate(float angle, float rX, float rY, float rZ = 0) = 0;
        virtual void reflect(bool refX, bool refY, bool refZ = 0) = 0;

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        glm::mat4 model;
    private:
        bool outline = false;
    };
}