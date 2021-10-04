#include "pch.h"
#include "MaterialSystem.h"

namespace Eclipse
{
    void MaterialSystem::Init()
    {
        EDITOR_LOG_INFO("MaterialSystem Init");
    }

    void MaterialSystem::Update()
    {
        ZoneScopedN("Material System")

            engine->Timer.SetName({ SystemName::MATERIAL });
        engine->Timer.tracker.system_start = glfwGetTime();

        if (engine->MaterialManager.EnableHighlight == true)
        {
            // Materials Update ===============================
            for (auto const& entity : mEntities)
            {
                MaterialComponent& material = engine->world.GetComponent<MaterialComponent>(entity);

                // Update Shininess ===============================
                engine->MaterialManager.UpdateShininess(entity);

                if (engine->world.CheckComponent<ModelComponent>(entity))
                {
                    // HighLight Basic Models Start ===============================
                    engine->MaterialManager.Highlight3DModels(entity, engine->GraphicsManager.GetFrameBufferID(FrameBufferMode::FBM_SCENE));
                    // HighLight Basic Models End ===============================
                }
                else
                {
                    // HighLight Basic Models Start ===============================
                    engine->MaterialManager.HighlightBasicPrimitives(entity, engine->GraphicsManager.GetFrameBufferID(FrameBufferMode::FBM_SCENE));
                    // HighLight Basic Models End ===============================

                }
            }

            engine->MaterialManager.StencilBufferClear();
        }

        engine->Timer.tracker.system_end = glfwGetTime();
        engine->Timer.UpdateTimeContainer(engine->Timer.tracker);

        FrameMark
    }


}
