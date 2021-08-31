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
        for (auto& M : engine->AssimpManager.GetContainer())
        {
            auto& Model = M.second;
            auto& Transform = engine->world.GetComponent<TransformComponent>(M.first);
            auto ModelVsGrid = engine->CollisionGridTree.CheckOverlapAgainstGrid(Model->SetAABB(Transform));

            if (ModelVsGrid.size() >= 1)
            {
                auto& Transform1 = engine->world.GetComponent<TransformComponent>(M.first);
                Transform1.position = engine->GraphicsManager.GridManager->gridArray[ModelVsGrid[0]].CenterPoint;
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
            if (MouseVsGrid.size() >= 1)
            {
                std::cout << " - Collided With -" << std::endl;

                for (int i = 0; i < MouseVsGrid.size(); i++)
                {
                    std::cout << "Tile ID : " << MouseVsGrid[i] << std::endl;
                }
            }
        }
    }
}
