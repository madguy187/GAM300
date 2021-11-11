#include "pch.h"
#include "AnimationSystem.h"

void Eclipse::AnimationSystem::Init()
{
	for (auto& it : mEntities)
	{
		auto& anim = engine->world.GetComponent<AnimationComponent>(it);
		engine->gFSM.AddFSM(it);
		engine->gFSM.FindFSM(it)->SetState(anim.m_CurrentAnimation.m_AnimationState);
	}
}

void Eclipse::AnimationSystem::Update()
{
	for (auto& it : mEntities)
	{
		// engine->gFSM.FindFSM(it)->UpdateFSM(engine->Game_Clock.get_fixedDeltaTime());
		engine->gAnimationManager.UpdateAnimation(it, engine->Game_Clock.get_DeltaTime());	
	}
}
