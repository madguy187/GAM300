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
            auto& ModelVsGrid = engine->test.CheckOverlap(Model->SetAABB(Transform));
        }
    }
}
