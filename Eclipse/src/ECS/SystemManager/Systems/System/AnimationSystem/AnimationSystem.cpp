#include "pch.h"
#include "AnimationSystem.h"

void Eclipse::AnimationSystem::Init()
{
}

void Eclipse::AnimationSystem::Update()
{
	for (auto& it : mEntities)
	{
		engine->gAnimationManager.UpdateAnimation(it, engine->Game_Clock.get_DeltaTime());
	}
}