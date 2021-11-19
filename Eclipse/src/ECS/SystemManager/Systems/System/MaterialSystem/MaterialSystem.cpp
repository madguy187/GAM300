#include "pch.h"
#include "MaterialSystem.h"
#include "Editor/Windows/MaterialEditor/MaterialEditor.h"

namespace Eclipse
{
    void MaterialSystem::Init()
    {
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

        // Sky
        engine->gPBRManager->RenderSky(FrameBufferMode::FBM_SCENE);

        engine->MaterialManager.StencilBufferClear();

        engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
        engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
        FrameMark
    }
}
