#include "pch.h"
#include "MaterialSystem.h"

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

        engine->gPBRManager->gMaterialEditorSettings->RenderMaterialScene();
        
        //engine->gPBRManager->BloomScene();

        engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
        engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
        FrameMark
    }
}
