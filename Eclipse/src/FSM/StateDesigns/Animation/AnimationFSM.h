#pragma once

#include "../Base/FiniteStateMachine.h"
#include "Global.h"

namespace Eclipse
{
	class AnimationFSM : public FSM<AnimationState>
	{
	public:
		AnimationFSM();
		AnimationFSM(Entity ID);
		void UpdateState(AnimationState state) override;
		void BeginState(AnimationState state) override;
		void EndState(AnimationState state) override;
		void SetEntity(Entity ID);
	private:
		Entity m_ID{ MAX_ENTITY };
	};
}