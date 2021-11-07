#pragma once
#include "Global.h"
#include "pch.h"

namespace Eclipse
{
	static void setEnabled(Entity entity, int value)
	{
		ScriptComponent& scriptComp = engine->world.GetComponent<ScriptComponent>(entity);
	}
}