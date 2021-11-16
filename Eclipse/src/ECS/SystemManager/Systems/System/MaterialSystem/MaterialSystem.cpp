#include "pch.h"
#include "MaterialSystem.h"
#include "Editor/Windows/MaterialEditor/MaterialEditor.h"

namespace Eclipse
{
    void MaterialSystem::Init()
    {
        engine->gPBRManager->woodTexture = Graphics::FindTextures("brick").GetHandle();

        auto& shdrpgm3 = Graphics::shaderpgms["Blur"];
        shdrpgm3.Use();
        shdrpgm3.setInt("image", 0);
    }

    void MaterialSystem::Update()
    {
        ZoneScopedN("Material System")
            engine->Timer.SetName({ SystemName::MATERIAL });
        engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());

        auto* MaterialEditor = engine->editorManager->GetEditorWindow<MaterialEditorWindow>();

        if (engine->InputManager->GetKeyTriggered(InputKeycode::Key_M))
        {
            if (!MaterialEditor->IsVisible)
            {
                MaterialEditor->IsVisible = true;
            }
            else
            {
                // Whenever we open up the material Editor , we Will Clear it.
                MaterialEditor->ColorPicker = ECVec3{ 1.0f };
                engine->gPBRManager->gMaterialEditorSettings->ClearCurrentMaterial();
                engine->gPBRManager->gMaterialEditorSettings->ClearTextureFields();
                MaterialEditor->IsVisible = false;
            }
        }

        // Material Node Editor
        engine->gPBRManager->gMaterialEditorSettings->RenderMaterialScene();


        // Bloom Scene
        //engine->gPBRManager->BloomScene();
        
        auto& cam = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

        //engine->gFrameBufferManager->UseFrameBuffer(FrameBufferMode::FBM_SCENE);

        // bind pre-computed IBL data
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, engine->gPBRManager->IrradianceSettings.irradianceMap);

        auto& background_ = Graphics::shaderpgms["background"];
        background_.Use();
        background_.setMat4("view", cam.viewMtx);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, engine->gPBRManager->IrradianceSettings.envCubemap);
        engine->gPBRManager->renderCube();

        engine->MaterialManager.StencilBufferClear();

        engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
        engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
        FrameMark
    }
}
