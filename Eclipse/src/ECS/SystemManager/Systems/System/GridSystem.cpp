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
            auto& ModelVsGrid = engine->CollisionGridTree.CheckOverlap(Model->SetAABB(Transform));
        }

        auto& camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

        if (ImGui::IsMouseClicked(0))
        {
            glm::vec3 rayDir = engine->gPicker.ComputeCursorRayDirection();
            float t;
            auto& MouseVsGrid = engine->CollisionGridTree.SecondCheckOverlap(camera.eyePos, rayDir, t);

            if (MouseVsGrid.size() >= 1)
            {
                std::cout << " Test Colision " << std::endl;
            }
        }
    }
}
