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
		void Insert(Entity ent, T component)
		{
			unsigned index = set.Insert(ent);
			components[index] = component;
		}

		void Delete(Entity ent)
		{
			unsigned lastElement = set.GetSize();
			std::swap(components[ent], components[--lastElement]);
			set.Delete(ent);
		}

		void Clear()
		{
			set.Clear();
		}

		T& GetComponent(Entity ent)
		{
			auto index = set.Search(ent);
			assert(index != set.NOT_FOUND, "ENTITY DOES NOT EXIST.");
			return components[index];
		}
	};
}