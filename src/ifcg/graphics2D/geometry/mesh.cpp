#include "ifcg/graphics2D/geometry/mesh.hpp"

namespace mesh2D
{
    Mesh2D::Mesh2D(std::vector<Vertex> vertices, std::vector<GLuint> indices, GLuint shaderID)
        : vertices(vertices), indices(indices), shaderID(shaderID)
    {
		this->vao = vao;
        vao.bind();

        VBO vbo(vertices);
        EBO ebo(indices);
		
        vao.linkAttrib(vbo, 0, 2, GL_FLOAT, sizeof(Vertex), (void*) 0);
        vao.linkAttrib(vbo, 1, 4, GL_FLOAT, sizeof(Vertex), (void*) (2 * sizeof(float)));

        vao.unbind();
        vbo.unbind();
        ebo.unbind();
    };

    Mesh2D::Mesh2D(const std::vector<Mesh2D*>& meshes, GLuint shaderID)
        : shaderID(shaderID)
    {
        this->subMeshes = meshes;
    };
        
    void Mesh2D::draw(glm::mat4 m = glm::mat4(1.0f))
    {
        GLuint modelLoc = glGetUniformLocation(shaderID, "model");
        m *= model;

        if (!subMeshes.empty()) {
            for (Mesh2D* mesh : subMeshes) {
                mesh->draw(m);
            }
        } else {
            vao.bind();
    
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));
            glDrawElements(
                GL_TRIANGLES,
                indices.size(),
                GL_UNSIGNED_INT,
                (void*)0
            );

            vao.unbind();
        }
    };

	void Mesh2D::transform(glm::mat4 t)
	{
		model *= t;
	};

	void Mesh2D::translate(float tX, float tY)
	{
		model = glm::translate(model, glm::vec3(tX, tY, 0.0f));
	}
	
    void Mesh2D::scale(float x, float y) 
    {
		model = glm::scale(model, glm::vec3(x, y, 1.0f));
    }

	void Mesh2D::rotate(float angle)
	{
		glm::mat4 modelSave = model;
		model = glm::mat4(1.0f);
    	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));		
		
		model = modelSave * rotation * model;
	}
};
