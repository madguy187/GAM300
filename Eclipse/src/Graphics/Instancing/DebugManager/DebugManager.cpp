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
        DebugSpheres.Init();
        LightIcons.Init();
        SpotLightIcons.Init();
    }

    void DebugManager::Reset()
    {
        ResetInstancedDebugBoxes();
        DebugSpheres.ResetInstancedDebugSpheres();
        LightIcons.ResetInstancedDebugLights();
        SpotLightIcons.ResetInstancedDebugSpotLights();
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
        if (Visible == true)
        {
            engine->MaterialManager.DoNotUpdateStencil();
            if (engine->editorManager->GetEditorWindow<SceneWindow>()->IsVisible)
            {
                DebugBoxes.DrawAll(FrameBufferMode::FBM_SCENE);
            }

            DebugSpheres.RenderBoundingSpheres();
        }

        if (engine->IsScenePlaying() == false)
        {
            LightIcons.RenderLights();
            SpotLightIcons.RenderSpotLights();
        }

        engine->MaterialManager.StencilBufferClear();
    }

    void DebugManager::AddBoundingRegion(glm::mat4 model, CameraComponent& _camera, Entity EntityID)
    {
        (void)model;

        if (Visible == false)
            return;

        if (engine->world.CheckComponent<TransformComponent>(EntityID) == false)
            return;

        auto& Transform = engine->world.GetComponent<TransformComponent>(EntityID);

        if (_camera.camType == CameraComponent::CameraType::Editor_Camera)
        {
            if (engine->world.CheckComponent<CollisionComponent>(EntityID) == false)
            {
                BoundingRegion br(Transform.position.ConvertToGlmVec3Type(), Transform.scale.ConvertToGlmVec3Type());
                engine->gDebugDrawManager->DebugBoxes.AddInstance(br);
            }
            else
            {
                auto& BodyShape = engine->world.GetComponent<CollisionComponent>(EntityID);

                switch (BodyShape.shape.shape)
                {

                case PxShapeType::Px_CUBE:
                {
                    // Below are half extents , we take transform for now
                    //glm::vec3 Scale = glm::vec3{ BodyShape.shape.hx, BodyShape.shape.hy ,  BodyShape.shape.hz };
                    //BoundingRegion br(Transform.position.ConvertToGlmVec3Type(), Scale);
                    //engine->gDebugDrawManager->DebugBoxes.AddInstance(br);

                    BoundingRegion br(Transform.position.ConvertToGlmVec3Type(), Transform.scale.ConvertToGlmVec3Type());
                    engine->gDebugDrawManager->DebugBoxes.AddInstance(br);
                }
                break;

                case PxShapeType::Px_SPHERE:
                {
                    glm::vec3 RadiusScale = glm::vec3{ BodyShape.shape.radius, BodyShape.shape.radius ,  BodyShape.shape.radius };

                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, Transform.position.ConvertToGlmVec3Type());
                    model = glm::rotate(model, glm::radians(Transform.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
                    model = glm::rotate(model, glm::radians(Transform.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
                    model = glm::rotate(model, glm::radians(Transform.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
                    model = glm::scale(model, RadiusScale);

                    engine->gDebugDrawManager->DebugSpheres.Addinstance(model);
                }
                break;

                }
            }
        }
    }
}
