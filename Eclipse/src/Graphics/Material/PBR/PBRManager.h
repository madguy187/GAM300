#pragma once
#include "Graphics/Material/MaterialInstance/MaterialInstance.h"
#include "Graphics/Material/MaterialEditorSettings/MaterialEditorSettings.h"

namespace Eclipse
{
    enum class MaterialType
    {
        MT_NONE = 0,
        MT_ALBEDO = 1,
        MT_NORMAL = 2,
        MT_METALLIC = 3,
        MT_ROUGHNESS = 4,
        MT_AO = 5,
        MT_HEIGHT = 6,
        MT_MAXCOUNT
    };

    struct MaterialTextue
    {
        MaterialTextue(MaterialType type, unsigned int id) : Type(type), Id(id) {}
        MaterialType Type = MaterialType::MT_NONE;
        unsigned int Id = 0;
    };

    typedef std::unordered_map<MaterialType, unsigned int> map1;

    class PBRManager
    {
    public:
        std::unordered_map<std::string, std::unique_ptr<MaterialInstance>> AllMaterialInstances;
        std::unordered_map<std::string, map1> AllMaterialTextures;
        std::unique_ptr<MaterialEditorSettings> gMaterialEditorSettings;
        std::unordered_map<std::string, ECVec3> BaseReflectivity;
        std::vector<std::string> AllMaterialInstName;

        PBRManager();
        void Init();
        void InitialiseBaseReflectivity();
        void GenerateMaterialTexture(std::string FolderName, std::string TextureName);
        void LoadAllTextures();
        void NonTexturedUniform(unsigned int ID, CameraComponent& In);
        void TexturedUniform(unsigned int ID, CameraComponent& In);

        // For Tianyu
        unsigned int GetMaterialTextureID(std::string FolderName, std::string TextureName);
        MaterialType GetMaterialType(std::string TextureName);

    public:
        void ClearAllMaterialInstances();
        void ClearAllMaterialInstNames();

    public:
        void LoadMaterial(std::string NameOfMaterial);
        void BindMaterial(std::string MaterialName, Shader& In);
        void SetAlbedoConstant(Shader& In, glm::vec4 AlbedoValue);
        void SetMetallicConstant(Shader& In, float metallicValue);
        void SetRoughnessConstant(Shader& In, float RoughnessValue);
        void SetInstanceFlag(Shader& In, bool instanceflag);
        void SetAOConstant(Shader& In, float AOValue);
        void UnBindMetallicTexture(Shader& In);
        void UnBindRoughnessTexture(Shader& In);
        void UnBindAOTexture(Shader& In);
        void SetSurfaceColour(Shader& In, glm::vec3 SurfaceColour_);
        void SetSurfaceColour(Shader& In, ECVec3 SurfaceColour_);
        bool CheckMaterialExist(MaterialComponent& in);

    public:
        unsigned int sphereVAO = 0;
        unsigned int indexCount;
        void UpdateLoop();
        void PBRScene();
        unsigned int loadTexture(char const* path);
        void NonTexturedUpdateLoop();
        void Clear(std::string& TextureName, MaterialType In);
    };
}