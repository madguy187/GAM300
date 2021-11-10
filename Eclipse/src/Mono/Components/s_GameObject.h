#pragma once
#include "Global.h"
#include "pch.h"

namespace Eclipse
{
	static void setEnabled(Entity entity, MonoString* scriptName, int value)
	{
		ScriptComponent& scriptComp = engine->world.GetComponent<ScriptComponent>(entity);
		for (auto& var : scriptComp.scriptList)
		{
			if (var->scriptName == mono_string_to_utf8(scriptName))
			{
				var->enabled = value;
				return;
			}
		}
	}

	static MonoObject* getBehavior(Entity entity, MonoString* behaviorName)
	{
		ScriptComponent& scriptComp = engine->world.GetComponent<ScriptComponent>(entity);

		for (auto& var : scriptComp.scriptList)
		{
			if (var->scriptName == mono_string_to_utf8(behaviorName))
			{
				return var->obj;
			}
		}
	}
}