#pragma once
#include "Global.h"
#include "SparseSet/SparseSet.hpp"

namespace Eclipse
{
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() {};
	};

	template <typename T>
	class ComponentArray : public IComponentArray
	{
		SparseSet<Entity, MAX_PAGE_SIZE> set;
		std::vector<T> components;

	public:
		void Insert(Entity ent, T component)
		{
			unsigned index = set.Insert(ent);
			while(index >= components.size())
				components.push_back(T{});

			components[index] = component;
		}

		void Delete(Entity ent)
		{
			unsigned lastElement = set.GetSize();
			std::swap(components[set.Search(ent)], components[--lastElement]);
			set.Delete(ent);
		}

		void Clear()
		{
			set.Clear();
		}

		T& GetComponent(Entity ent)
		{
			auto index = set.Search(ent);
			assert(index != set.NOT_FOUND);
			return components[index];
		}
	};
}