#pragma once
#include "flow_vertex.h"
#include <string>
#include <vector>
#include <span>

namespace flow
{
    struct Mesh
    {
    private:
        std::vector<unsigned int> m_indices;
        std::vector<Vertex> m_vertices;
    public:
        std::string name;

    public:
        Mesh(const std::vector<unsigned int> indices, const std::vector<Vertex> &vertices) : m_indices(indices), m_vertices(vertices) {}
        ~Mesh();
        std::vector<Vertex>& GetVertices();
        std::vector<unsigned int>& GetIndices();
        unsigned int GetIndexCount() const;
    };
}