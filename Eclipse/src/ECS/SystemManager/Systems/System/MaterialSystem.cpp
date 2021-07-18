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
        // Materials Update ===============================
        for (auto const& entity : mEntities)
        {
            MaterialComponent& material = engine->world.GetComponent<MaterialComponent>(entity);
            engine->MaterialManager.UpdateShininess(material);
        }

        // HighLight Models Start ===============================
        engine->MaterialManager.Highlight(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), GL_FILL);
        // HighLight Models End ===============================

        // StencilBuffer Clear - Darren to check again next time ===============================
        engine->GraphicsManager.StencilBufferClear();
    }
}
