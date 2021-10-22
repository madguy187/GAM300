#pragma once
#include "Graphics/Material/MaterialInstance/MaterialInstance.h"

namespace Eclipse
{
    enum class Mode
    {
        NONE_INSTANCE = 0,
        CREATE_INSTANCE = 1,
        UPDATE_INSTANCE = 2,
        MAXCOUNT_INSTANCE
    };

    class MaterialEditorSettings
    {
    public:
        unsigned int sphereVAO = 0;
        unsigned int indexCount = 0;
        ECVec3 LightPosition{ 0,0,0 };
        ECVec3 lightColor{ 0,0,0 };
        Mode EditingMode = Mode::NONE_INSTANCE;
        std::shared_ptr<MaterialInstance> CurrentMaterial;
        ECVec3 Rotation{ 0.0f, 0.0f, 0.0f };

        std::string AlbedoTexture;
        std::string NormalTexture;
        std::string MetallicTexture;
        std::string RoughnessTexture;
        std::string AoTexture;
        std::string MaterialName;

        MaterialEditorSettings();
        void BindMaterial(std::string MaterialName, Shader& In);
        void RenderSphere();
        void UpdateCurrentMaterial();
        void UpdateLights(Shader& MaterialEditorShader);
        void UpdateCamera(Shader& MaterialEditorShader, CameraComponent& MeshEditorCamera);
    };
}