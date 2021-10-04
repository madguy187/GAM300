#pragma once
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"

namespace Eclipse
{
	struct PrefabComponent
	{
		META_DATA(PrefabComponent);
		bool IsChild = false;
		
		long long unsigned int PrefabID = 0;
	};
}
