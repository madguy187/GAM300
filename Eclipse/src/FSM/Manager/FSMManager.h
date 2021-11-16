#pragma once

#include "../StateDesigns/Base/FiniteStateMachine.h"
#include "../StateDesigns/Animation/AnimationFSM.h"

namespace Eclipse
{
	class FSMManager
	{
	public:
		void Init();
		void AddFSM(Entity ID);
		void RemoveFSM(Entity ID);
		AnimationFSM* FindFSM(Entity ID);
	private:
		std::unordered_map<Entity, AnimationFSM> m_AnimationFSMMap;
	};
}
