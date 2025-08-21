#pragma once

#include <memory>
#include "ifcg/common/mesh.hpp"
#include "ifcg/graphics3D/geometry/mesh.hpp"

namespace mesh3D
{
    class MeshTree3D : public ifcg::Mesh
    {
    public:
        virtual ~MeshTree3D() = default;
	    MeshTree3D(const std::vector<Mesh*> &meshes);
        MeshTree3D* duplicate() const override;

        virtual void draw(glm::mat4 m = glm::mat4(1.0f)) override;
        virtual void setOutline(bool outline);
        
        virtual void translate(float tX, float tY, float tZ);
        virtual void scale(float sX, float sY, float sZ);
        virtual void rotate(float angle, float rX, float rY, float rZ);
        virtual void reflect(bool refX, bool refY, bool refZ);

        void addSubMesh(Mesh* mesh);
        Mesh* getSubMesh(int index) const;

        glm::mat4 model = glm::mat4(1.0f);

    private:
		std::vector<Mesh*> subMeshes;
        bool outline = false;
    };
};

using namespace mesh3D;