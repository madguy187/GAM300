#include "pch.h"
#include "Graphics/Instancing/DebugSpotLights/DebugSpotLights.h"

namespace Eclipse
{
    DebugSpotLights::DebugSpotLights()
    {
    }

    void DebugSpotLights::Init()
    {
        SpotLightVertices = engine->AssimpManager.Geometry["SpotLight"]->Vertices;
        SpotLightIndices = engine->AssimpManager.Geometry["SpotLight"]->Indices;
    }

    void DebugSpotLights::Addinstance(glm::mat4 TransXRotXScale)
    {
        instanceMatrix.push_back(TransXRotXScale);
    }

    void DebugSpotLights::ResetInstancedDebugSpotLights()
    {
        instanceMatrix.clear();
    }
    void DebugSpotLights::RenderSpotLights()
    {
        if (engine->editorManager->GetEditorWindow<SceneWindow>()->IsVisible)
        {
            if (instanceMatrix.size() >= 1)
            {
                DebugSpotLights_.Init(SpotLightVertices, SpotLightIndices, instanceMatrix.size(), instanceMatrix);

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

                glUniform3f(DiffuseColour,
                    engine->AssimpManager.Geometry["SpotLight"]->Diffuse.r,
                    engine->AssimpManager.Geometry["SpotLight"]->Diffuse.g,
                    engine->AssimpManager.Geometry["SpotLight"]->Diffuse.b);

                engine->MaterialManager.DoNotUpdateStencil();
                DebugSpotLights_.Draw(shdrpgm, _camera);
            }
        }
    }
}