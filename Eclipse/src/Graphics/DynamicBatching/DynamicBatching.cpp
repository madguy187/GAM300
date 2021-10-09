#include "pch.h"
#include "DynamicBatching.h"

namespace Eclipse
{
    void DynamicBatch::CreateBuffers()
    {
        // Cr8 buffers
        glCreateVertexArrays(1, &m_QuadVA);
        glCreateBuffers(1, &m_QuadVB);
        glCreateBuffers(1, &m_QuadVIB);
        glBindVertexArray(m_QuadVA);

        glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 100000, nullptr, GL_DYNAMIC_DRAW);

        // set vertex attribute pointers
        // Vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // Textures C0ordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, TextureCoodinates)));

        // Normsals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));

        // Tangents
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Tangents)));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadVIB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, engine->AssimpManager.Geometry["Hello"]->Indices.size() * sizeof(unsigned int), &engine->AssimpManager.Geometry["Hello"]->Indices[0], GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    void DynamicBatch::ResetBuffer()
    {
        DynamicVertices.clear();
    }
}