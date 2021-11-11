#pragma once
#include "Global.h"
#include "pch.h"
#include "ECS/SystemManager/Systems/System/EntityCompSystem/EntityCompSystem.h"

namespace Eclipse
{
	static void Invoke(Entity ent, MonoString* scriptName, MonoString* funcName, float timer)
	{
		ScriptComponent & scriptComp = engine->world.GetComponent<ScriptComponent>(ent);

		MonoMethod* method = nullptr;

		for (auto& var : scriptComp.scriptList)
		{
			if (mono_string_to_utf8(scriptName) != var->scriptName) continue;

			method = engine->mono.GetMethodFromClass(engine->mono.GetScriptMonoClass(var->scriptName), mono_string_to_utf8(funcName));
			if (method != nullptr)
			{
				engine->mono.AddInvoke(var, timer, method);
				return;
			}
		}
	}

	static MonoObject* Find(MonoString* entName)
	{
		Entity ent = engine->world.GetSystem<EntityCompSystem>()->FindEntity(mono_string_to_utf8(entName));
		if (ent == MAX_ENTITY) return nullptr;

		return engine->mono.CreateGameObjectClass(ent, "");
	}
}