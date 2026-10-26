#include "flow_mesh.h"

namespace flow
{
    Mesh::~Mesh()
    {

    }

    std::vector<Vertex>& Mesh::GetVertices()
    {
        return m_vertices;
    }

    std::vector<unsigned int>& Mesh::GetIndices()
    {
        return m_indices;
    }

    unsigned int Mesh::GetIndexCount() const
    {
        return m_indices.size();
    }
}