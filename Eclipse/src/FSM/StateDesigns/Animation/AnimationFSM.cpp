#include "pch.h"
#include "AnimationFSM.h"

namespace Eclipse
{
	AnimationFSM::AnimationFSM() :
		FSM<AnimationState>{}, m_ID{ MAX_ENTITY }
	{}
	AnimationFSM::AnimationFSM(Entity ID) :
		FSM<AnimationState>{}, m_ID{ ID }
	{}

	void AnimationFSM::UpdateState(AnimationState state)
	{
		engine->gAnimationManager.UpdateAnimation(m_ID, engine->Game_Clock.get_DeltaTime());

		if (GetTimeInCurrentState() > 5.0f)
		{
			int temp = (int)state;

			if (temp < 2)
			{
				temp++;
				SetNextState((AnimationState)temp);
			}
			else
			{
				SetNextState((AnimationState)0);
			}
		}
	}

	void AnimationFSM::BeginState(AnimationState state)
	{
		engine->gAnimationManager.ChangeAnimationState(m_ID, state);
	}

	void AnimationFSM::EndState(AnimationState state)
	{
		(void)state;
	}

	void AnimationFSM::SetEntity(Entity ID)
	{
		m_ID = ID;
	}
}