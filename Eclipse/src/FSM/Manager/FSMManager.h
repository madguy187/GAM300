#pragma once

#include "FSM/FSM/FiniteStateMachine.h"

namespace Eclipse
{
	class FSMManager
	{
	public:
		void Init();
	private:
		std::unordered_map<std::string, std::any> m_FSMMap;
	};
}
