#pragma once

#include "Graphics/Grid/bounds.h"

#define UPPER_BOUND 100

namespace Eclipse
{
    class AABB
    {

    private:
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

    public:
        bool DrawAABBS = false;
        std::vector<glm::vec3> Position;
        std::vector<glm::vec3> AABB_Size;

        AABB();
        void Init();
        void Render(Shader shader, CameraComponent& camera);
        void AddInstance(BoundingRegion br, glm::vec3 pos);
        void Cleanup();
        void Reset();
        void DrawAll(unsigned int FramebufferID);
    };
}
