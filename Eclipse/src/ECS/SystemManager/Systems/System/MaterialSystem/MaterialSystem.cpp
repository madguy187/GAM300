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

        // Material Node Editor
        engine->gPBRManager->gMaterialEditorSettings->RenderMaterialScene();

        //engine->gFrameBufferManager->UseFrameBuffer(FrameBufferMode::FBM_SCENE);
        //auto& cam = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
        //auto& background_ = Graphics::shaderpgms["background"];
        //background_.Use();
        //background_.setMat4("view", cam.viewMtx);
        //glDepthMask(GL_FALSE);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_CUBE_MAP, engine->gPBRManager->IrradianceSettings.envCubemap);
        //engine->gPBRManager->renderCube();

        engine->MaterialManager.StencilBufferClear();

        engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
        engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
        FrameMark
    }
}
