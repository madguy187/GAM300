#include "pch.h"
#include "Graphics/Instancing/DebugManager/DebugManager.h"

namespace Eclipse
{
    DebugManager::DebugManager()
    {

    }

    void DebugManager::Init()
    {
        //DebugBoxes.Init();
        DebugSpheres.Init();
        LightIcons.Init();
        SpotLightIcons.Init();
        DebugBoxes_.Init();
    }

    void DebugManager::Reset()
    {
        ResetInstancedDebugBoxes();
        DebugSpheres.ResetInstancedDebugSpheres();
        LightIcons.ResetInstancedDebugLights();
        SpotLightIcons.ResetInstancedDebugSpotLights();
        DebugBoxes_.ResetInstancedDebugBoxes();
    }
}

namespace Eclipse
{
    void DebugManager::ResetInstancedDebugBoxes()
    {
        //DebugBoxes.Reset();
    }

    void DebugManager::Render()
    {
        if (Visible == true)
        {
            DebugBoxes_.RenderDebugBoxes();
            DebugSpheres.RenderBoundingSpheres();
        }

        LightIcons.RenderLights();
        SpotLightIcons.RenderSpotLights();
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
                //BoundingRegion br(Transform.position.ConvertToGlmVec3Type(), Transform.scale.ConvertToGlmVec3Type());
                //engine->gDebugDrawManager->DebugBoxes.AddInstance(br);
                glm::mat4 model1 = glm::mat4(1.0f);
                model1 = glm::translate(model1, Transform.position.ConvertToGlmVec3Type());
                model1 = model1 * Transform.UpdateRotation();
                model1 = glm::scale(model1, Transform.scale.ConvertToGlmVec3Type());

                DebugBoxes_.Addinstance(model1);
            }
            else
            {
                auto& BodyShape = engine->world.GetComponent<CollisionComponent>(EntityID);

                switch (BodyShape.shape.shape)
                {

                case PxShapeType::Px_CUBE:
                {
                    auto& collision = engine->world.GetComponent<CollisionComponent>(EntityID);
                    ECVec3 temp{ 2.0f * collision.shape.hx,2.0f * collision.shape.hy,2.0f * collision.shape.hz };

                    glm::mat4 model1 = glm::mat4(1.0f);
                    model1 = glm::translate(model1, Transform.position.ConvertToGlmVec3Type());
                    model1 = model1 * Transform.UpdateRotation();
                    model1 = glm::scale(model1, temp.ConvertToGlmVec3Type());

                    engine->gDebugDrawManager->DebugBoxes_.Addinstance(model1);
                }
                break;

                case PxShapeType::Px_SPHERE:
                {
                    glm::vec3 RadiusScale = glm::vec3{ BodyShape.shape.radius, BodyShape.shape.radius ,  BodyShape.shape.radius };

                    glm::mat4 model1 = glm::mat4(1.0f);
                    model1 = glm::translate(model1, Transform.position.ConvertToGlmVec3Type());
                    model1 = model1 * Transform.UpdateRotation();
                    model1 = glm::scale(model1, RadiusScale);

                    engine->gDebugDrawManager->DebugSpheres.Addinstance(model1);
                }
                break;

                }
            }
        }
    }
}
