#pragma once
#include "Graphics/Material/MaterialInstance/MaterialInstance.h"

namespace Eclipse
{
    class PBRManager
    {
    public:
        //std::vector<MaterialInstance> MaterialInstances;
        std::unordered_map<std::string, std::shared_ptr<MaterialInstance>> AllMaterialInstances;

        void Init();
        void CheckUniform(unsigned int ID);

    public:
        void LoadMaterial(std::string NameOfMaterial);
        void BindMaterial(std::string MaterialName , Shader& In);

    public:
        unsigned int sphereVAO = 0;
        unsigned int indexCount;
        void RenderSphere();
        void UpdateLoop();
        void PBRScene();
        unsigned int loadTexture(char const* path);
        void NonTexturedUpdateLoop();
    };
}