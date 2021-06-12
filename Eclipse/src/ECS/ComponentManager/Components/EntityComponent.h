#pragma once

#include "Global.h"

namespace Eclipse
{
	struct EntityComponent
	{
		EntityType Tag{ EntityType::ENT_UNASSIGNED };
		std::string Name{"Untagged Entity"};
		bool IsActive{ true };
	};
}
