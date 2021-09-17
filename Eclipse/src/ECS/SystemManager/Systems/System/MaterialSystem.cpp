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

				if (engine->world.CheckComponent<ModeLInforComponent>(entity))
				{
					// HighLight Basic Models Start ===============================
					engine->MaterialManager.Highlight3DModels(material, entity, engine->GraphicsManager.GetFrameBufferID(FrameBufferMode::SCENEVIEW));
					// HighLight Basic Models End ===============================
				}
				else
				{
					// HighLight Basic Models Start ===============================
					engine->MaterialManager.HighlightBasicPrimitives(material, entity, engine->GraphicsManager.GetFrameBufferID(Eclipse::FrameBufferMode::SCENEVIEW));
					// HighLight Basic Models End ===============================

				}
			}

			engine->MaterialManager.StencilBufferClear();
		}
	}
}
