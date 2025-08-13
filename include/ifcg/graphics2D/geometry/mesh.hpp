#pragma once

#include "ifcg/common/mesh.hpp"

namespace mesh2D
{
    class Mesh2D : public ifcg::Mesh
    {
    public:
        Mesh2D(std::vector<Vertex> vertices, std::vector<GLuint> indices, GLuint shaderID);
        Mesh2D(const std::vector<Mesh2D*>& meshes, GLuint shaderID);

        virtual void draw(glm::mat4 m);

        virtual void transform(glm::mat4 t);
        virtual void translate(float tX, float tY);
        virtual void scale(float sX, float sY);
        virtual void rotate(float angle);

    private:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        GLuint shaderID;
        VAO vao;
        
		glm::mat4 model = glm::mat4(1.0f);
		std::vector<Mesh2D*> subMeshes;
    };
};

using namespace mesh2D;