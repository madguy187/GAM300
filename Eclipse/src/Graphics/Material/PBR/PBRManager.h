#pragma once
#include "Graphics/Material/MaterialInstance/MaterialInstance.h"

namespace Eclipse
{
    class PBRManager
    {
    public:
        std::unordered_map<std::string, std::shared_ptr<MaterialInstance>> AllMaterialInstances;

        void Init();
        void CheckUniform(unsigned int ID, CameraComponent& In);
        bool CheckMaterial(unsigned int ID, CameraComponent& In , MaterialComponent& Mat);

    public:
        void LoadMaterial(std::string NameOfMaterial);
        void BindMaterial(std::string MaterialName , Shader& In);
        void SetAlbedoConstant(Shader& In , glm::vec4& AlbedoValue);
        void SetMetallicConstant(Shader& In ,float metallicValue);
        void SetRoughnessConstant(Shader& In , float RoughnessValue);
        void SetInstanceFlag(Shader& In , bool instanceflag);
        void SetAOConstant(Shader& In, float AOValue);
        void UnBindMetallicTexture(Shader& In);
        void UnBindRoughnessTexture(Shader& In);
        void UnBindAOTexture(Shader& In);

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