#pragma once

namespace Eclipse
{
    class PBRManager
    {
        struct MaterialInstance
        {
            std::string Name = "Default";
            unsigned int albedo = 0;
            unsigned int normal = 0;
            unsigned int metallic = 0;
            unsigned int roughness = 0;
            unsigned int ao = 0;
        };

    public:
        std::vector<MaterialInstance> MaterialInstances;

        void SetShader();
        void CheckUniform(unsigned int ID);

    public:
        unsigned int sphereVAO = 0;
        unsigned int indexCount;
        void RenderSphere();
        void UpdateLoop();
        void PBRScene();
    };
}