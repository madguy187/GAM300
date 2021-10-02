#include "pch.h"
#include "ChildSystem.h"

namespace Eclipse
{
	void Eclipse::ChildSystem::Update()
	{
		if (!engine->GetPlayState()) return;

		for (auto& entity : mEntities)
		{
			// calculate offset
		}
	}
}
