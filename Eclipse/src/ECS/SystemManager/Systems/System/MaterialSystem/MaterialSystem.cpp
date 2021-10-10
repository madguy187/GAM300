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
        engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());

        if (engine->MaterialManager.EnableHighlight == true)
        {
            // Materials Update ===============================
            for (auto const& entity : mEntities)
            {
                // Update Shininess ===============================
                engine->MaterialManager.UpdateShininess(entity);

                if (engine->world.CheckComponent<ModelComponent>(entity) == true)
                {
                    // HighLight Basic Models Start ===============================
                    engine->MaterialManager.Highlight3DModels(entity, FrameBufferMode::FBM_SCENE);
                    // HighLight Basic Models End ===============================
                }
                else
                {
                    // HighLight Basic Models Start ===============================
                    engine->MaterialManager.HighlightBasicPrimitives(entity, FrameBufferMode::FBM_SCENE);
                    // HighLight Basic Models End ===============================

                }
            }

            engine->MaterialManager.StencilBufferClear();
        }

        engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
        engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
        FrameMark
    }
}
