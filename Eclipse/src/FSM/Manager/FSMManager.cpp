#include "pch.h"
#include "FSMManager.h"

namespace Eclipse
{
	void FSMManager::Init()
	{
	}

	void FSMManager::AddFSM(Entity ID)
	{
		AnimationFSM animFSM{ ID };
		m_AnimationFSMMap.insert({ ID, animFSM });
	}

	void FSMManager::RemoveFSM(Entity ID)
	{
		if (m_AnimationFSMMap.find(ID) != m_AnimationFSMMap.end())
			m_AnimationFSMMap.erase(ID);
	}

	AnimationFSM* FSMManager::FindFSM(Entity ID)
	{
		if (m_AnimationFSMMap.count(ID) != 0)
			return &m_AnimationFSMMap[ID];

		return nullptr;
	}
}
