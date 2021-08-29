#pragma once

#include "Graphics/Grid/bounds.h"

#define UPPER_BOUND 100

namespace Eclipse
{
    class AABB_
    {

    private:
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

    public:
        bool DrawAABBS = true;
        std::vector<glm::vec3> offsets;
        std::vector<glm::vec3> sizes;
        int Counter = 0;
        unsigned int offsetVBO, sizeVBO;

        AABB_();
        void Init();
        void Render(Shader shader, CameraComponent& camera);
        void AddInstance(BoundingRegion br, glm::vec3 pos, glm::vec3 size);
        void Cleanup();
        void Reset();
        void DrawAll(unsigned int FramebufferID);
    };
}
