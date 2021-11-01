#include "pch.h"
#include "Graphics/Instancing/DebugLights/DebugLights.h"

namespace Eclipse
{
    DebugLights::DebugLights()
    {

    }

    void DebugLights::Init()
    {
        LightVertices = engine->AssimpManager.Geometry["LightBulb"]->Vertices;
        LightIndices = engine->AssimpManager.Geometry["LightBulb"]->Indices;
    }

    void DebugLights::Addinstance(glm::mat4 TransXRotXScale)
    {
        instanceMatrix.push_back(TransXRotXScale);
    }

    void DebugLights::ResetInstancedDebugLights()
    {
        instanceMatrix.clear();
    }

    void DebugLights::RenderLights()
    {
        if (engine->editorManager->GetEditorWindow<SceneWindow>()->IsVisible)
        {
            if (instanceMatrix.size() >= 1)
            {
                DebugLights_.Init(LightVertices, LightIndices, instanceMatrix.size(), instanceMatrix);

                auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));
                engine->gFrameBufferManager->UseFrameBuffer(FrameBufferMode::FBM_SCENE);
                auto& shdrpgm = Graphics::shaderpgms["Instancing"];
                shdrpgm.Use();

                glUniform3f(glGetUniformLocation(shdrpgm.GetHandle(), "Colour"), 0.8, 0.8, 0.8);

                engine->MaterialManager.DoNotUpdateStencil();
                DebugLights_.Draw(shdrpgm, _camera);
            }
        }
    }

}