#include "ifcg/graphics3D/geometry/meshTree.hpp"

namespace mesh3D
{
    MeshTree3D::MeshTree3D(const std::vector<Mesh*> &meshes)
    {
        this->subMeshes = meshes;
    };

    MeshTree3D* MeshTree3D::duplicate() const
    {
        MeshTree3D* newTree = new MeshTree3D({});
        newTree->model = model;

        for (const auto* child : subMeshes)
        {
            if (child)
            {
                newTree->addSubMesh(child->duplicate());
            }
        }

        return newTree;
    }

    void MeshTree3D::draw(glm::mat4 m)
    {
        m *= model;

        if (!subMeshes.empty()) {
            for (Mesh* mesh : subMeshes) {
                mesh->draw(m);
            }
        }
    };

    void MeshTree3D::setOutline(bool outline) 
    {
        if (!subMeshes.empty()) {
            for (Mesh* mesh : subMeshes) {
                mesh->setOutline(outline);
            }
        } else {
            this->outline = outline; 
        }
    };

	void MeshTree3D::translate(float tX, float tY, float tZ)
	{
		model = glm::translate(model, glm::vec3(tX, tY, tZ));
	};

    void MeshTree3D::scale(float sX, float sY, float sZ) 
    {
		model = glm::scale(model, glm::vec3(sX, sY, sZ));
    };


	void MeshTree3D::rotate(float angle, float rX, float rY, float rZ)
	{
		glm::mat4 modelSave = model;
		model = glm::mat4(1.0f);
    	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(rX, rY, rZ));		
		
		model = modelSave * rotation * model;
	};

    void MeshTree3D::reflect(bool refX, bool refY, bool refZ)
    {
        glm::mat4 m = glm::mat4(1.0f);
        m[0][0] = (refX) ? -1.0f : 1.0f;
        m[1][1] = (refY) ? -1.0f : 1.0f;
        m[2][2] = (refZ) ? -1.0f : 1.0f;
        model *= m;
    };

    void MeshTree3D::addSubMesh(Mesh* mesh) 
    { 
        subMeshes.push_back(mesh); 
    }

    ifcg::Mesh* MeshTree3D::getSubMesh(int index) const
    {
        if (index >= 0 && index < subMeshes.size())
        {
            return subMeshes[index];
        }
        return nullptr;
    }

};