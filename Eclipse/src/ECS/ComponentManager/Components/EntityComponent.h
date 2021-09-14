#pragma once

#include "Global.h"

namespace Eclipse
{
	struct EntityComponent
	{
		EntityType Tag{ EntityType::ENT_UNASSIGNED };
		std::string Name{"Untagged Entity"};
		// DONT USE, FOR EDITOR STUFF
		bool IsActive{ false };
		// For Script Testing
		std::vector<std::string> ScriptListComTest;
	};
}
