#include "pch.h"
#include "TestingFSM.h"

namespace Eclipse
{
	TestingFSMClass::TestingFSMClass(Entity ID) :
		FSM<TestState>{}, m_ID{ ID }
	{
	}

	void TestingFSMClass::UpdateState(TestState state)
	{
	}

	void TestingFSMClass::BeginState(TestState state)
	{
	}

	void TestingFSMClass::EndState(TestState state)
	{
	}
}