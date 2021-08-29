#include "pch.h"
#include "GridSystem.h"

void Eclipse::GridSystem::Init()
{

}

void Eclipse::GridSystem::Update()
{
    for (auto& M : engine->AssimpManager.GetContainer())
    {
        unsigned int EntityID = M.first;
        auto& Model = M.second;

        auto& ModelVsGrid = engine->test.CheckOverlap(Model->getAABB());

        if (ModelVsGrid.size() >= 1)
        {
            auto& unoccupied = engine->GraphicsManager.GridManager->GetOccupiedTiles(ModelVsGrid);
            auto& Coordinates = engine->world.GetComponent<TransformComponent>(EntityID);
        }
    }
}
