#include "pch.h"
#include "Graphics/Instancing/DebugManager/DebugManager.h"

namespace Eclipse
{
    DebugManager::DebugManager()
    {

    }

    void DebugManager::Init()
    {
        DebugBoxes.Init();

        SphereVertices = engine->AssimpManager.Geometry["BoundingSphere"]->Vertices;
        SphereIndices = engine->AssimpManager.Geometry["BoundingSphere"]->Indices;
    }

    void DebugManager::Reset()
    {
        ResetInstancedDebugBoxes();
        ResetInstancedDebugSpheres();
    }
}

namespace Eclipse
{
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

        RenderBoundingSpheres();
    }
}

namespace Eclipse
{
    void DebugManager::Addinstance(glm::mat4 TransXRotXScale)
    {
        instanceMatrix.push_back(TransXRotXScale);
    }

    void DebugManager::ResetInstancedDebugSpheres()
    {
        instanceMatrix.clear();
    }

    void DebugManager::PrepareData()
    {
        DebugSpheres.Init(SphereVertices, SphereIndices, 1, instanceMatrix);
    }

    void DebugManager::RenderBoundingSpheres()
    {
        if (engine->editorManager->GetEditorWindow<SceneWindow>()->IsVisible)
        {
            if (instanceMatrix.size() >= 1)
            {
                DebugSpheres.Init(SphereVertices, SphereIndices, instanceMatrix.size(), instanceMatrix);

                auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));
                engine->gFrameBufferManager->UseFrameBuffer(FrameBufferMode::FBM_SCENE);
                auto& shdrpgm = Graphics::shaderpgms["Test"];
                shdrpgm.Use();
                engine->MaterialManager.DoNotUpdateStencil();
                DebugSpheres.Draw(shdrpgm, _camera);
            }
        }
    }
}