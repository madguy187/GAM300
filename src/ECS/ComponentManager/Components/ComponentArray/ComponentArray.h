#pragma once
#include "Global.h"
#include "SparseSet/SparseSet.hpp"

namespace Eclipse
{
	class IComponentArray
	{
		
	};

	template <typename T>
	class ComponentArray : public IComponentArray
	{
		SparseSet<Entity, MAX_PAGE_SIZE> set;
		std::array<T, MAX_ENTITY> components;
	public:
		void Insert(Entity ent, T component);
		void Delete(Entity ent);
		void Clear();

		T& GetComponent(Entity ent);
	};
}