#pragma once

#include "Graphics/Grid/bounds.h"

#define UPPER_BOUND 10000

namespace Eclipse
{
    class AABB_
    {

    private:
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        std::vector<float> Vertices;
        std::vector<unsigned int> Indices;

        bool DrawAABBS = true;
        std::vector<glm::vec3> Offsets;
        std::vector<glm::vec3> Sizes;
        int Counter = 0;
        unsigned int OffsetVBO;
        unsigned int SizeVBO;

        // Check if there is any Debug Boxes to render
        bool CheckToRender();
        // Check Flag to render
        bool ShouldRender();
        void Render(Shader shader);

    public:

        AABB_();
        void Init();
        void AddInstance(BoundingRegion& br);
        void Cleanup();
        void Reset();
        void DrawAll(unsigned int FramebufferID);
    };
}
