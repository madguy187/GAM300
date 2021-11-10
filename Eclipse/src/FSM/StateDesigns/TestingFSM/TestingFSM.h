#pragma once

#include "../../FSM/FiniteStateMachine.h"
#include "Global.h"

namespace Eclipse
{
	class TestingFSMClass : public FSM<TestState>
	{
	public:
		TestingFSMClass(Entity ID);
		void UpdateState(TestState state) override;
		void BeginState(TestState state) override;
		void EndState(TestState state) override;
	private:
		Entity m_ID{ MAX_ENTITY };
	};
}