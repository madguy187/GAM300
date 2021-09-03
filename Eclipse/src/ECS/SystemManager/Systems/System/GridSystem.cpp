#include "pch.h"
#include "GridSystem.h"

namespace Eclipse
{
    void GridSystem::Init()
    {
        EDITOR_LOG_INFO("GridSystem Init");

        // Create Grid =============================
        engine->GraphicsManager.GridManager->Init();
        engine->GraphicsManager.GridManager->DebugPrint();
    }

    void GridSystem::Update()
    {
        for (auto& it : mEntities)
        {
            auto& aabb = engine->world.GetComponent<AabbComponent>(it);
            auto& Transform = engine->world.GetComponent<TransformComponent>(it);

            auto& ModelVsGrid = engine->CollisionGridTree.CheckOverlapAgainstGrid(DYN_AABB::SetAABB(Transform, aabb));

            if (engine->CollisionGridTree.NumberOfIntersections(ModelVsGrid))
            {
                Transform.position = engine->GraphicsManager.GridManager->gridArray[ModelVsGrid[0]].CenterPoint;
            }
        }

        // Testing Code == Testing Picking Against my Grid  TESTING ONLY BUT WORKS
        auto& camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

        if (ImGui::IsMouseClicked(0))
        {
            glm::vec3 rayDir = engine->gPicker.ComputeCursorRayDirection();
            float t;
            auto& MouseVsGrid = engine->CollisionGridTree.CheckMouseOverlapAgainstGrid(camera.eyePos, rayDir, t);

            // Test Code
           /* if (MouseVsGrid.size() >= 1)
            {
                std::cout << " - Collided With -" << std::endl;

                for (int i = 0; i < MouseVsGrid.size(); i++)
                {
                    std::cout << "Tile ID : " << MouseVsGrid[i] << std::endl;
                }
            }*/
        }
    }
}
