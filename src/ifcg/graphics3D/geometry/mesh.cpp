#include "ifcg/graphics3D/geometry/mesh.hpp"

namespace mesh3D
{
    Mesh3D::Mesh3D(const Mesh3D& other)
        : vertices(other.vertices), indices(other.indices), vao(other.vao), shaderID(other.shaderID), model(other.model) {}

    Mesh3D::Mesh3D(std::vector<Vertex> vertices, std::vector<GLuint> indices, GLuint shaderID)
        : vertices(vertices), indices(indices), shaderID(shaderID)
    {
        vao.bind();

        VBO vbo(vertices);
        EBO ebo(indices);
		
        vao.linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*) 0);
        vao.linkAttrib(vbo, 1, 4, GL_FLOAT, sizeof(Vertex), (void*) (3 * sizeof(float)));

        vao.unbind();
        vbo.unbind();
        ebo.unbind();
    };

    Mesh3D* Mesh3D::duplicate() const
    {
        return new Mesh3D(*this);
    };

    void Mesh3D::draw(glm::mat4 m)
    {
        GLuint modelLoc = glGetUniformLocation(shaderID, "model");
        m *= model;

        vao.bind();
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));
        glDrawElements(
            GL_TRIANGLES,
            indices.size(),
            GL_UNSIGNED_INT,
            (void*)0
        );

        if (outline) {
            drawOutline();
        }

        vao.unbind();
    }

    void Mesh3D::drawOutline()
    {
        vao.bind();

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_DEPTH_TEST);
        glLineWidth(1.0f);
        glDrawElements(
            GL_TRIANGLES, 
            indices.size(), 
            GL_UNSIGNED_INT, 
            0
        );
        glEnable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        vao.unbind();
    }

    void Mesh3D::setOutline(bool outline) 
    {
        this->outline = outline; 
    };

	void Mesh3D::translate(float tX, float tY, float tZ)
	{
		model = glm::translate(model, glm::vec3(tX, tY, tZ));
	};

    void Mesh3D::scale(float sX, float sY, float sZ) 
    {
		glm::mat4 modelSave = model;
    	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(sX, sY, sZ));	

        model = modelSave * scale;
    };

	void Mesh3D::rotate(float angle, float rX, float rY, float rZ)
	{
		glm::mat4 modelSave = model;
		model = glm::mat4(1.0f);
    	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(rX, rY, rZ));		
		
		model = modelSave * rotation * model;
	};

    void Mesh3D::reflect(bool refX, bool refY, bool refZ)
    {
        glm::mat4 m = glm::mat4(1.0f);
        m[0][0] = (refX) ? -1.0f : 1.0f;
        m[1][1] = (refY) ? -1.0f : 1.0f;
        m[2][2] = (refZ) ? -1.0f : 1.0f;
        model *= m;
    };

};