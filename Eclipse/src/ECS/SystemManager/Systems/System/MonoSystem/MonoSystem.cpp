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
		for (auto entity : mEntities)
		{
			ScriptComponent& scriptCom = engine->world.GetComponent<ScriptComponent>(entity);

			for (auto& script : scriptCom.scriptList)
			{
				engine->mono.Update(&script);
			}
		}
	}
}