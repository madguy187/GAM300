#include "pch.h"
#include "AISystem.h"
namespace Eclipse
{
	void AISystem::Update()
	{
		for (auto const& entity : mEntities)
		{
			engine->gAI.patrol(entity);
		}
	}
}