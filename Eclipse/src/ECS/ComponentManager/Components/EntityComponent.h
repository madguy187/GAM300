#pragma once

#include "Global.h"

namespace Eclipse
{
	struct EntityComponent
	{
		EntityType Tag{ EntityType::ENT_UNASSIGNED };
		std::string Name;
		bool IsActive{ true };
	};
}
