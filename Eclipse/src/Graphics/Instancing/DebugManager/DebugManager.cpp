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
        if (Visible == true)
        {
            engine->MaterialManager.DoNotUpdateStencil();
            if (engine->editorManager->GetEditorWindow<SceneWindow>()->IsVisible)
            {
                DebugBoxes.DrawAll(FrameBufferMode::FBM_SCENE);
            }

            RenderBoundingSpheres();
        }
    }

    void DebugManager::AddBoundingRegion(glm::mat4 model, CameraComponent& _camera, Entity EntityID)
    {
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

                    engine->gDebugDrawManager->Addinstance(model);
                }
                break;

                }
            }
        }
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
        if (Visible == true)
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
                    engine->MaterialManager.DoNotUpdateStencil();
                    DebugSpheres.Draw(shdrpgm, _camera);
                }
            }
        }
    }
}