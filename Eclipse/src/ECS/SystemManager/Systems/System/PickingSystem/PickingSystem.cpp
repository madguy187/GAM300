#include "pch.h"
#include "../PickingSystem/PickingSystem.h"

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
	engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());

	auto& camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
	float tMin = (std::numeric_limits<float>::max)();

	glm::vec3 rayDir = engine->gPicker.ComputeCursorRayDirection();
	unsigned int collisionID = engine->gDynamicAABBTree.RayCast(engine->gDynamicAABBTree.GetTreeRoot(), camera.eyePos, rayDir, tMin);

	if (collisionID != MAX_ENTITY)
	{
		if (engine->gPicker.GetCurrentCollisionID() != MAX_ENTITY)
		{
			engine->MaterialManager.UnHighlight(engine->gPicker.GetCurrentCollisionID());
		}
	
		engine->gPicker.SetCurrentCollisionID(collisionID);
		engine->MaterialManager.HighlightClick(collisionID);
	
		//engine->gPicker.UpdateAabb(collisionID);
		//engine->gDynamicAABBTree.UpdateData(collisionID);
	}

	engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
	engine->Timer.UpdateTimeContainer(engine->Timer.tracker);

	FrameMark
}
