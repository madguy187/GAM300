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
            engine->MaterialManager.UpdateShininess(material);
        }

        engine->MaterialManager.Highlight(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), GL_FILL);
        engine->GraphicsManager.StencilBufferClear();

    }
}
