#include "pch.h"
#include "MaterialSystem.h"

namespace Eclipse
{
    void MaterialSystem::Init()
    {

    }

    void MaterialSystem::Update()
    {
        for (auto const& entity : mEntities) 
        {
            MaterialComponent& material = engine->world.GetComponent<MaterialComponent>(entity);
            MaterialManager_.UpdateShininess(material);
        }
    }
}
