#pragma once

#include "Global.h"
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"
#include "imgui.h"

namespace Eclipse
{
	struct EntityComponent
	{
		META_DATA(EntityComponent);
		EntityType Tag{ EntityType::ENT_UNASSIGNED };
		std::string Name{"Untagged Entity"};
		int LayerIndex{ -1 };

		// DONT USE, FOR EDITOR STUFF
		bool IsActive{ false };
		std::vector<Entity> Child;
		std::vector<Entity> Parent;
		bool IsAChild = false;
		bool hightLightChild = false;
		float ImguiIndentValue = 20.f;
	};
}
