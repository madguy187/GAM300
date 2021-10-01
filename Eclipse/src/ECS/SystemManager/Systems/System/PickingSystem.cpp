#include "pch.h"
#include "PickingSystem.h"

void Eclipse::PickingSystem::Init()
{
}

void Eclipse::PickingSystem::Update()
{
	// Probably use for the game in the future
}

void Eclipse::PickingSystem::EditorUpdate()
{
	ZoneScopedN("Picking System")

	engine->Timer.SetName({ SystemName::PICKING });
	engine->Timer.tracker.system_start = glfwGetTime();

	auto& camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
	float tMin = (std::numeric_limits<float>::max)();

	for (auto& it : mEntities)
	{
		auto& aabb = engine->world.GetComponent<AABBComponent>(it);

		float t = 0.0f;
		glm::vec3 rayDir = engine->gPicker.ComputeCursorRayDirection();
		bool collision = engine->gPicker.RayAabb(camera.eyePos, rayDir, aabb.Min.ConvertToGlmVec3Type(), aabb.Max.ConvertToGlmVec3Type(), t);

		if (collision && (t < tMin))
		{
			tMin = t;
			auto& material = engine->world.GetComponent<MaterialComponent>(it);

			if (engine->gPicker.GetCurrentCollisionID() != MAX_ENTITY)
			{
				engine->MaterialManager.UnHighlight(engine->gPicker.GetCurrentCollisionID());
			}

			engine->gPicker.SetCurrentCollisionID(it);
			engine->MaterialManager.HighlightClick(it);

			engine->gPicker.UpdateAabb(it);
			//engine->gDynamicAABBTree.UpdateData(it);
		}
	}

	engine->Timer.tracker.system_end = glfwGetTime();
	engine->Timer.UpdateTimeContainer(engine->Timer.tracker);

	FrameMark
}
