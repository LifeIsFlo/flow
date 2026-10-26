#pragma once
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <span>
#include "flow_mesh.h"

namespace flow
{
    struct Model
    {
    private:
        std::map<std::string, std::unique_ptr<Mesh>> meshes;
    public:
        std::string name;

    public:
        Mesh *GetMesh(const std::string &meshName);
        void AddMesh(const std::string &meshName, const std::vector<unsigned int> &indices, const std::vector<Vertex> &vertices);
    };
}