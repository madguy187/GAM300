#include "pch.h"
#include "MaterialSystem.h"
#include "Editor/Windows/MaterialEditor/MaterialEditor.h"

namespace Eclipse
{
    void MaterialSystem::Init()
    {

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
                MaterialEditor->ColorPicker = ECVec3{ 1.0f };
                engine->gPBRManager->gMaterialEditorSettings->ClearCurrentMaterial();
                engine->gPBRManager->gMaterialEditorSettings->ClearTextureFields();
                MaterialEditor->IsVisible = false;
            }
        }

        engine->gPBRManager->gMaterialEditorSettings->RenderMaterialScene();
        engine->MaterialManager.StencilBufferClear();

        engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
        engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
        FrameMark
    }
}
