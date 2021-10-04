#pragma once
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"

namespace Eclipse
{
	struct PrefabComponent
	{
		META_DATA(PrefabComponent);
		bool IsChild = false;
		
		int PrefabID = -1;
	};
}
