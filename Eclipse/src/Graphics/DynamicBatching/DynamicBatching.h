#pragma once

namespace Eclipse
{
    class DynamicBatch
    {
    public:
        unsigned int m_QuadVA = 0;
        unsigned int m_QuadVB = 0;
        unsigned int m_QuadVIB = 0;
        std::vector<Vertex> DynamicVertices;
        std::vector<unsigned int> Indices;

        void CreateBuffers();
        void ResetBuffer();
    };
}