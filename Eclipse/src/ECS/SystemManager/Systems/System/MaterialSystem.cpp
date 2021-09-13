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
        if (engine->MaterialManager.EnableHighlight == true)
        {
            // Materials Update ===============================
            for (auto const& entity : mEntities)
            {
                MaterialComponent& material = engine->world.GetComponent<MaterialComponent>(entity);

                // Update Shininess ===============================
                engine->MaterialManager.UpdateShininess(material);

                // Update Materials ===============================
                engine->MaterialManager.UpdateMaterial(material);

                if (engine->world.CheckComponent<ModeLInforComponent>(entity))
                {
                    // HighLight Basic Models Start ===============================
                    engine->MaterialManager.Highlight3DModels(material, entity, engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID());
                    // HighLight Basic Models End ===============================
                }
                else
                {
                    // HighLight Basic Models Start ===============================
                    engine->MaterialManager.HighlightBasicPrimitives(material, entity, engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID());
                    // HighLight Basic Models End ===============================
                }
            }

           // // HighLight Models Start ===============================
           // engine->MaterialManager.MeshHighlight(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), GL_FILL);
           // // HighLight Models End ===============================

            engine->MaterialManager.StencilBufferClear();
        }
    }
}
