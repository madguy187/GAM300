#pragma once
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"
#include "mono/Manager/MonoManager.h"

namespace Eclipse
{
	struct ScriptComponent
	{
		META_DATA(ScriptComponent);
		// For Script Testing
		std::vector<MonoScript> scriptList;
	};
}