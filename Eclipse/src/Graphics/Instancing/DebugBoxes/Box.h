#pragma once

#include "Graphics/Grid/BoundingBox.h"

#define UPPER_BOUND 50000

namespace Eclipse
{
    class AABB_
    {

    private:
        unsigned int VAO = 0;
        unsigned int VBO = 0;
        unsigned int EBO = 0;
        std::vector<float> Vertices;
        std::vector<unsigned int> Indices;
        std::vector<glm::vec3> Offsets;
        std::vector<glm::vec3> Sizes;
        int Counter = 0;
        unsigned int OffsetVBO = 0;
        unsigned int SizeVBO = 0;

        // Check if there is any Debug Boxes to render
        bool CheckToRender();
        // Check Flag to render
        bool ShouldRender();
        void Render(Shader& shader);

    public:
        bool DrawAABBS = true;

        AABB_();
        void Init();
        void AddInstance(BoundingRegion& br);
        void Cleanup();
        void Reset();
        void DrawAll(FrameBufferMode);
    };
}
