#include "flow_model.h"

namespace flow {
    Mesh *Model::GetMesh(const std::string &meshName)
    {
        if (!meshes.contains(meshName))
            return nullptr;
        return meshes[meshName].get();
    }

    void Model::AddMesh(const std::string &meshName, const std::vector<unsigned int> &indices, const std::vector<Vertex> &vertices)
    {
        std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(indices, vertices);
        mesh->name = meshName;
        meshes[meshName] = std::move(mesh);
    }
}