#pragma once
#include "Global.h"
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"

namespace Eclipse
{
	struct PrefabComponent
	{
		META_DATA(PrefabComponent);
		bool IsChild = false;
		
		EUUID PrefabID = 0;
	};
}
