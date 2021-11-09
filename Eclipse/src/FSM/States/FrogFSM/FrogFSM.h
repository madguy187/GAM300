#pragma once

#include "../../FSM/FiniteStateMachine.h"
#include "Global.h"

namespace Eclipse
{
	class FrogFSM : public FSM<AnimationState>
	{
	public:
		FrogFSM(Entity ID);
		void UpdateState(AnimationState state) override;
		void BeginState(AnimationState state) override;
		void EndState(AnimationState state) override;
	private:
		Entity m_ID{ MAX_ENTITY };
	};
}