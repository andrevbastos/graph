#pragma once

#include "ifcg/common/mesh.hpp"

namespace mesh3D
{
    class Mesh3D : public ifcg::Mesh
    {
    public:
        virtual ~Mesh3D() = default;
        Mesh3D(std::vector<Vertex> vertices, std::vector<GLuint> indices, GLuint shaderID);
        Mesh3D(const Mesh3D &other);
        Mesh3D* duplicate() const override;

        virtual void draw(glm::mat4 m = glm::mat4(1.0f)) override;
        virtual void drawOutline();
        virtual void setOutline(bool outline) override;
        
        virtual void translate(float tX, float tY, float tZ) override;
        virtual void scale(float sX, float sY, float sZ) override;
        virtual void rotate(float angle, float rX, float rY, float rZ) override;
        virtual void reflect(bool refX, bool refY, bool refZ) override;

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        glm::mat4 model = glm::mat4(1.0f);

    private:
        GLuint shaderID;
        VAO vao;
        
        bool outline = false;
    };
};

using namespace mesh3D;