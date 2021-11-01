#include "pch.h"
#include "Graphics/Instancing/DebugSpheres/DebugSphere.h"

namespace Eclipse
{
    DebugSphere::DebugSphere()
    {

    }

    void DebugSphere::Init()
    {
        SphereVertices = engine->AssimpManager.Geometry["BoundingSphere"]->Vertices;
        SphereIndices = engine->AssimpManager.Geometry["BoundingSphere"]->Indices;
    }

    void DebugSphere::Addinstance(glm::mat4 TransXRotXScale)
    {
        instanceMatrix.push_back(TransXRotXScale);
    }

    void DebugSphere::ResetInstancedDebugSpheres()
    {
        instanceMatrix.clear();
    }

    void DebugSphere::PrepareData()
    {
        DebugSpheres.Init(SphereVertices, SphereIndices, 1, instanceMatrix);
    }

    void DebugSphere::RenderBoundingSpheres()
    {
        if (engine->editorManager->GetEditorWindow<SceneWindow>()->IsVisible)
        {
            if (instanceMatrix.size() >= 1)
            {
                DebugSpheres.Init(SphereVertices, SphereIndices, instanceMatrix.size(), instanceMatrix);

                auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));
                engine->gFrameBufferManager->UseFrameBuffer(FrameBufferMode::FBM_SCENE);
                auto& shdrpgm = Graphics::shaderpgms["Instancing"];
                shdrpgm.Use();

                glUniform3f(glGetUniformLocation(shdrpgm.GetHandle(), "Colour"), 1.0, 1.0, 1.0);

                engine->MaterialManager.DoNotUpdateStencil();
                DebugSpheres.Draw(shdrpgm, _camera);
            }
        }
    }
}