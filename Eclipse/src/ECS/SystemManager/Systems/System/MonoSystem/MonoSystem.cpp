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
				{
					script.obj = engine->mono.CreateMonoObject(script.scriptName, entity);
					engine->mono.LoadAllFields(&script);
				}
			}

			for (auto& script : scriptCom.scriptList)
			{
				engine->mono.Start(&script);
			}
		}
	}

	void MonoSystem::Update()
	{
		ZoneScopedN("Mono System")
		engine->Timer.SetName({ SystemName::MONO });
		engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());

		for (auto entity : mEntities)
		{
			ScriptComponent& scriptCom = engine->world.GetComponent<ScriptComponent>(entity);

			for (auto& script : scriptCom.scriptList)
			{
				if (!script.enabled) continue;

				if (engine->mono.fixUpdate)
					engine->mono.FixedUpdate(&script);
				else
					engine->mono.Update(&script);
			}
		}

		engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
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
	}
}