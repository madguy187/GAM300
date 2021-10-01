#include "pch.h"
#include "MonoSystem.h"
#include "ECS/ComponentManager/Components/ScriptComponent.h"

namespace Eclipse
{
	void MonoSystem::Init()
	{
		for (auto entity : mEntities)
		{
			ScriptComponent& scriptCom = engine->world.GetComponent<ScriptComponent>(entity);

			for (auto& script : scriptCom.scriptList)
			{
				if (!script.obj)
					script.obj = engine->mono.CreateMonoObject(script.scriptName, entity);
			}
		}
	}

	void MonoSystem::Update()
	{
		ZoneScopedN("Mono System")
		engine->Timer.SetName({ SystemName::MONO });
		engine->Timer.tracker.system_start = glfwGetTime();

		for (auto entity : mEntities)
		{
			ScriptComponent& scriptCom = engine->world.GetComponent<ScriptComponent>(entity);

			for (auto& script : scriptCom.scriptList)
			{
				engine->mono.Update(&script);
			}
		}

		engine->Timer.tracker.system_end = glfwGetTime();
		engine->Timer.UpdateTimeContainer(engine->Timer.tracker);

		FrameMark
	}

	void MonoSystem::Terminate()
	{
		for (auto entity : mEntities)
		{
			ScriptComponent& scriptCom = engine->world.GetComponent<ScriptComponent>(entity);

			for (auto& script : scriptCom.scriptList)
				script.obj = nullptr;
		}

		engine->mono.RestartMono();
	}
}