#include "pch.h"
#include "AnimationSystem.h"

bool switchState = false;

void Eclipse::AnimationSystem::Init()
{
}

void Eclipse::AnimationSystem::Update()
{
	for (auto& it : mEntities)
	{
		engine->gAnimationManager.UpdateAnimation(it, engine->Game_Clock.get_DeltaTime());
		
		auto& animation = engine->world.GetComponent<AnimationComponent>(it);
		int currTicks = animation.m_CurrentAnimation.m_TicksPerSecond;

		if (ImGui::IsMouseClicked(1))
		{
			if (!switchState)
			{
				engine->gAnimationManager.ChangeAnimationState(it, AnimationState::RUN);
				switchState = true;
			}
			
			--currTicks;
			animation.m_CurrentAnimation.m_TicksPerSecond = currTicks;
		}
	}
}
