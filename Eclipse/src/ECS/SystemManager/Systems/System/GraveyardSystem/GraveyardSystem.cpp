#include "pch.h"
#include "GraveyardSystem.h"

namespace Eclipse
{
	void GraveyardSystem::Update()
	{
		if (HasFuneralStarted)
		{
			for (const auto& ent : m_Graveyard)
				engine->DestroyGameObject(ent);

			HasFuneralStarted = false;
			m_Graveyard.clear();
		}
	}

	bool GraveyardSystem::CommenceBurial(DestroyEvent de)
	{
		m_Graveyard.push_back(de.ID);
		HasFuneralStarted = de.IsDead;
		return false;
	}
}