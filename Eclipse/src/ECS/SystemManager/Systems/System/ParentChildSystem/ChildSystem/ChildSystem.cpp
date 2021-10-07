#include "pch.h"
#include "ChildSystem.h"
#include "ECS/ComponentManager/Components/ParentComponent.h"
#include "ECS/ComponentManager/Components/ChildComponent.h"

namespace Eclipse
{
	void Eclipse::ChildSystem::Update()
	{
		ZoneScopedN("Child System")
		//if (!engine->GetPlayState()) return;

		//for (auto& entity : mEntities)
		//{
		//	// calculate offset
		//}
		engine->Timer.SetName({ SystemName::CHILD });
		engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());
		engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
		engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
		FrameMark
	}
}
