#pragma once
#include "Global.h"
#include "pch.h"

namespace Eclipse
{
	static void setEnabled(Entity entity, MonoString* scriptName, int value)
	{
		ScriptComponent& scriptComp = engine->world.GetComponent<ScriptComponent>(entity);
		std::cout << value << std::endl;
		for (auto& var : scriptComp.scriptList)
		{
			std::cout << var.scriptName << " " << mono_string_to_utf8(scriptName) << std::endl;
			if (var.scriptName == mono_string_to_utf8(scriptName))
			{
				std::cout << "set to false" << std::endl;
				var.enabled = value;
			}
		}
	}
}