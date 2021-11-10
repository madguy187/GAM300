#include "pch.h"
#include "FrogFSM.h"

namespace Eclipse
{
	FrogFSM::FrogFSM(Entity ID) :
		FSM<AnimationState>{}, m_ID{ ID }
	{
	}

	void FrogFSM::UpdateState(AnimationState state)
	{

	}

	void FrogFSM::BeginState(AnimationState state)
	{
	}

	void FrogFSM::EndState(AnimationState state)
	{
	}
}