#pragma once
#include "Graphics/Material/MaterialInstance/MaterialInstance.h"
#include "Graphics/EngineCompiler/MaterialCompiler/MaterialCompiler.h"
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
        MaterialCompiler gMaterialCompiler;

        std::ofstream MaterialFileWrite;
        std::fstream MaterialFileRead;

        unsigned int sphereVAO = 0;
        unsigned int indexCount = 0;
        ECVec3 LightPosition{ 0,0,0 };
        ECVec3 lightColor{ 0,0,0 };
        Mode EditingMode = Mode::NONE_INSTANCE;
        MaterialInstance CurrentMaterial;
        ECVec3 Rotation{ 0.0f, 0.0f, 0.0f };

        std::string AlbedoTexture;
        std::string NormalTexture;
        std::string MetallicTexture;
        std::string RoughnessTexture;
        std::string AoTexture;
        std::string HeightTexture;
        std::string MaterialName;

        unsigned int SelectedIndex = 0;
        std::string CurrentSelectedMaterial;
        bool Once = false;

        /*************************************************************************
          Material Editor Model
          - Inner
          - Outer
        *************************************************************************/
        unsigned int InnerEntity = 0;
        unsigned int OuterEntity = 0;

        MaterialEditorSettings();
        void BindMaterial(Shader& In, std::string MaterialName);
        void RenderSphere();
        void UpdateCurrentMaterial(Shader& ShaderIn, CameraComponent& Camin);
        void UpdateLights(Shader& MaterialEditorShader);
        void UpdateCamera(Shader& MaterialEditorShader, CameraComponent& MeshEditorCamera);
        void CreateMaterialInstance();
        void RenderMaterialScene();
        void ClearCurrentMaterial();
        void ClearTextureFields();
        void CreateModel();
        void UpdateInner(Shader& shdrpgm, CameraComponent& _camera, Entity ID);
    };
}