#include "pch.h"
#include "Graphics/Instancing/DebugBoxes/DebugBoxes.h"

namespace Eclipse
{
    DebugBoxes::DebugBoxes()
    {

    }

    void DebugBoxes::Init()
    {
        DebugBoxesVertices = engine->AssimpManager.Geometry["AdditionalCube"]->Vertices;
        DebugBoxesIndices = engine->AssimpManager.Geometry["AdditionalCube"]->Indices;
    }

    void DebugBoxes::Addinstance(glm::mat4 TransXRotXScale)
    {
        instanceMatrix.push_back(TransXRotXScale);
    }

    void DebugBoxes::ResetInstancedDebugBoxes()
    {
        instanceMatrix.clear();
    }

    void DebugBoxes::RenderDebugBoxes()
    {
        if (engine->editorManager->GetEditorWindow<SceneWindow>()->IsVisible)
        {
            if (instanceMatrix.size() >= 1)
            {
                DebugBoxes_.Init(DebugBoxesVertices, DebugBoxesIndices, instanceMatrix.size(), instanceMatrix);

                auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));
                auto& _camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));

                engine->gFrameBufferManager->UseFrameBuffer(FrameBufferMode::FBM_SCENE);
                auto& shdrpgm = Graphics::shaderpgms["Instancing"];
                shdrpgm.Use();

                GLuint DiffuseColour = shdrpgm.GetLocation("texDiff");
                GLuint CamPos = shdrpgm.GetLocation("camPos");

                glUniform3f(CamPos,
                    _camerapos.position.getX(),
                    _camerapos.position.getY(),
                    _camerapos.position.getZ());

                glUniform3f(DiffuseColour, 1.0f, 0.0f, 0.0f);

                engine->MaterialManager.DoNotUpdateStencil();
                DebugBoxes_.Draw(shdrpgm, _camera, RenderMode::Wireframe_Mode);
            }
        }
    }
}