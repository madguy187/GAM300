#include "pch.h"
#include "Graphics/Instancing/DebugManager/DebugManager.h"

namespace Eclipse
{
    DebugManager::DebugManager()
    {

    }

    void DebugManager::ResetInstancedDebugBoxes()
    {
        DebugBoxes.Reset();
    }

    void DebugManager::Render()
    {
        engine->MaterialManager.DoNotUpdateStencil();
        if (engine->editorManager->GetEditorWindow<SceneWindow>()->IsVisible)
        {
            DebugBoxes.DrawAll(FrameBufferMode::FBM_SCENE);
        }
    }
}