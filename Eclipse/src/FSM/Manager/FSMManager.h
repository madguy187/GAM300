#pragma once

#include "../StateDesigns/Base/FiniteStateMachine.h"

namespace Eclipse
{
	class FSMManager
	{
	public:
		void Init();
		
		/*template <typename T, class... Args>
		void AddFSM(Args&&... args);

		template <typename T>
		T* FindFSM();

		template <typename T>
		void Update(up);*/
	private:
		std::unordered_map<Entity, AnimationFSM> m_AnimationFSMMap;
	};

	/*template<typename T, class ...Args>
	inline void FSMManager::AddFSM(Args && ...args)
	{
		auto typeID = typeid(T).name();
		T fsm{ std::forward<Args>(args)... };
		m_FSMMap.insert({typeID, fsm});
	}

	template<typename T>
	inline T* FSMManager::FindFSM()
	{
		const std::string name = typeid(T).name;

		if (m_FSMMap.count(name) != 0)
			return &m_FSMMap[name];

		return nullptr;
	}

	template<typename T>
	inline void FSMManager::Update()
	{

	}*/
}
