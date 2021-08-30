#include "pch.h"
#include "GridSystem.h"

namespace Eclipse
{
    void GridSystem::Init()
    {

    }

    void GridSystem::Update()
    {
        for (auto& M : engine->AssimpManager.GetContainer())
        {
            auto& Model = M.second;
            auto& Transform = engine->world.GetComponent<TransformComponent>(M.first);
            auto ModelVsGrid = engine->CollisionGridTree.CheckOverlap(Model->SetAABB(Transform));

            //Entity i = ModelVsGrid[0];

            //if (ModelVsGrid.size() >= 1)
            //{
            //    for (int i = 0; i < ModelVsGrid.size(); i++)
            //    {
            //        auto& Transform1 = engine->world.GetComponent<TransformComponent>(M.first);
            //        Transform1.position = engine->GraphicsManager.GridManager->gridArray[i].CenterPoint;
            //    }
            //}
        }

        auto& camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

        if (ImGui::IsMouseClicked(0))
        {
            glm::vec3 rayDir = engine->gPicker.ComputeCursorRayDirection();
            float t;
            auto& MouseVsGrid = engine->CollisionGridTree.SecondCheckOverlap(camera.eyePos, rayDir, t);

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
