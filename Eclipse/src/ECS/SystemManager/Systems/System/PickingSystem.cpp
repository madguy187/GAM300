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

	glm::vec3 rayDir2 = engine->gPicker.ComputeCursorRayDirection();
	unsigned int collision2 = engine->gDynamicAABBTree.RayCast(engine->gDynamicAABBTree.GetTreeRoot(), camera.eyePos, rayDir2);

	if (collision2 != MAX_ENTITY)
	{
		if (engine->gPicker.GetCurrentCollisionID() != MAX_ENTITY)
		{
			engine->MaterialManager.UnHighlight(engine->gPicker.GetCurrentCollisionID());
		}

		engine->gPicker.SetCurrentCollisionID(collision2);
		engine->MaterialManager.HighlightClick(collision2);

		engine->gPicker.UpdateAabb(collision2);
		engine->gDynamicAABBTree.UpdateData(collision2);
	}

	//for (auto& it : mEntities)
	//{
	//	auto& aabb = engine->world.GetComponent<AABBComponent>(it);
	//
	//	float t = 0.0f;
	//	glm::vec3 rayDir = engine->gPicker.ComputeCursorRayDirection();
	//	bool collision = engine->gPicker.RayAabb(camera.eyePos, rayDir, aabb.Min.ConvertToGlmVec3Type(), aabb.Max.ConvertToGlmVec3Type(), t);
	//
	//	if (collision && (t < tMin))
	//	{
	//		tMin = t;
	//		auto& material = engine->world.GetComponent<MaterialComponent>(it);
	//
	//		if (engine->gPicker.GetCurrentCollisionID() != MAX_ENTITY)
	//		{
	//			engine->MaterialManager.UnHighlight(engine->gPicker.GetCurrentCollisionID());
	//		}
	//
	//		engine->gPicker.SetCurrentCollisionID(it);
	//		engine->MaterialManager.HighlightClick(it);
	//
	//		engine->gPicker.UpdateAabb(it);
	//	}
	//}

	engine->Timer.tracker.system_end = glfwGetTime();
	engine->Timer.UpdateTimeContainer(engine->Timer.tracker);

	FrameMark
}
