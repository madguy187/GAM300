#include "pch.h"
#include "MaterialSystem.h"

namespace Eclipse
{
    void MaterialSystem::Init()
    {
        EDITOR_LOG_INFO("MaterialSystem Init");
        engine->gPBRManager->Init();
    }

    void MaterialSystem::Update()
    {
        ZoneScopedN("Material System")
        engine->Timer.SetName({ SystemName::MATERIAL });
        engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());


        //for (auto const& entityID : mEntities)
        //{
        //    engine->Test.CheckUniform(entityID);
        //}

        engine->gPBRManager->PBRScene();

        engine->MaterialManager.StencilBufferClear();

        engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
        engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
        FrameMark
    }
}
