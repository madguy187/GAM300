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
		if (GetTimeInCurrentState() > 5.0f)
		{
			int temp = (int)state;

			if (temp < 2)
			{
				temp++;
				SetNextState((TestState)temp);
			}
			else
			{
				SetNextState((TestState)0);
			}
		}
	}

	void TestingFSMClass::BeginState(TestState state)
	{
	}

	void TestingFSMClass::EndState(TestState state)
	{
	}
}