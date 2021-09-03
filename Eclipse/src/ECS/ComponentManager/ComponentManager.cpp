#include "pch.h"
#include "ComponentManager.h"

namespace Eclipse
{
	void ComponentManager::Clear()
	{
		for (size_t i = 0; i < mComponentArrays.size(); i++)
		{
			mComponentArrays[i].Clear(*(mComponentArrays[i]._container));
		}
	}
	std::vector<std::string>& ComponentManager::GetAllComponentNames()
	{
		return mComponentNames;
	}
}