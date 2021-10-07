#include "pch.h"
#include "PrefabSystem.h"

namespace Eclipse
{
	void PrefabSystem::Update()
	{
		ZoneScopedN("Prefeb System")
		engine->Timer.SetName({ SystemName::PREFEB });
		engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());
		engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
		engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
		FrameMark
	}
}