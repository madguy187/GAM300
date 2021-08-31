#include "pch.h"
#include "PickingSystem.h"

void Eclipse::PickingSystem::Init()
{
}

void Eclipse::PickingSystem::Update()
{
	auto& camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

	if (ImGui::IsMouseClicked(0))
	{
		for (auto& it : mEntities)
		{
			auto& aabb = engine->world.GetComponent<AabbComponent>(it);

			float t;
			glm::vec3 rayDir = engine->gPicker.ComputeCursorRayDirection();
			bool collision = engine->gPicker.RayAabb(camera.eyePos, rayDir, aabb.min.ConvertToGlmVec3Type(), aabb.max.ConvertToGlmVec3Type(), t);

			if (collision)
			{
				auto& material = engine->world.GetComponent<MaterialComponent>(it);
				
				if (material.Highlight)
				{
					engine->gPicker.SetCurrentCollisionID(MAX_ENTITY);
					engine->MaterialManager.UnHighlight(it);
				}
				else
				{
					engine->gPicker.SetCurrentCollisionID(it);
					engine->MaterialManager.HighlightClick(it);
				}			
			}
			else
			{
				engine->gPicker.SetCurrentCollisionID(MAX_ENTITY);
				engine->MaterialManager.UnHighlight(it);
			}

			engine->gPicker.UpdateAabb(it);
		}
	}
}
