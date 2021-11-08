#include "pch.h"
#include "AISystem.h"
namespace Eclipse
{
	void AISystem::Update()
	{
		ZoneScopedN("AI System")
		engine->Timer.SetName({ SystemName::AI });
		engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());
		for (auto const& entity : mEntities)
		{
			//engine->gAI.patrol(entity);
		}
		engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
		engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
		FrameMark
	}
}