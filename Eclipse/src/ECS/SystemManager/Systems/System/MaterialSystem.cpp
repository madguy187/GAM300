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

            if (engine->MaterialManager.EnableHighlight == true)
            {
                if (material.Highlight == true)
                {
                    if (engine->world.CheckComponent<RenderComponent>(entity))
                    {
                        // HighLight BASIC Start ===============================

                        glStencilFunc(GL_NOTEQUAL, 2, 0xFF);
                        glStencilMask(0xFF);
                        glDisable(GL_DEPTH_TEST);

                        engine->MaterialManager.Highlight(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), entity, GL_FILL);
                        // HighLight BASIC End ===============================
                            // Clear stencil buffer
                        glStencilFunc(GL_ALWAYS, 0, 0xFF);
                    }
                }
            }
        }

        // HighLight Models Start ===============================

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0xFF);
        glDisable(GL_DEPTH_TEST);

        engine->MaterialManager.Highlight(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), GL_FILL);
        engine->MaterialManager.Highlight(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::GAMEVIEW)->GetFrameBufferID(), GL_FILL);
        // HighLight Models End ===============================

        // StencilBuffer Clear - Darren to check again next time ===============================
        engine->GraphicsManager.StencilBufferClear();
    }
}
