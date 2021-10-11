#pragma once

namespace Eclipse
{
    class PBRManager
    {
    public:
        unsigned int sphereVAO = 0;
        unsigned int indexCount;
        unsigned int albedo = 0;
        unsigned int normal = 0;
        unsigned int metallic = 0;
        unsigned int roughness = 0;
        unsigned int ao = 0;

        void SetShader();
        void RenderSphere();
        void UpdateLoop();

        void PBRScene();
        unsigned int loadTexture(char const* path);
    };
}