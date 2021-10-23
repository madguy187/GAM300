#pragma once

#include "Global.h"
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"

namespace Eclipse
{
	struct EntityComponent
	{
		META_DATA(EntityComponent);
		EntityType Tag{ EntityType::ENT_UNASSIGNED };
		std::string Name{"Untagged Entity"};
		// DONT USE, FOR EDITOR STUFF
		bool IsActive{ false };
		// For Script Testing
		std::vector<Entity> Child;
		std::vector<Entity> Parent;
		bool IsAChild = false;
		ImGuiTreeNodeFlags TreeactiveFlag = ImGuiTreeNodeFlags_OpenOnDoubleClick;
	};
}
