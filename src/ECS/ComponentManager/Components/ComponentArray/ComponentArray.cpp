#include "pch.h"
#include "ComponentArray.h"

namespace Eclipse
{
	template <typename T>
	void ComponentArray<T>::Insert(Entity ent, T component)
	{
		unsigned index = set.Insert(ent);
		components[index] = component;
	}

	template <typename T>
	void ComponentArray<T>::Delete(Entity ent)
	{
		unsigned lastElement = set.GetSize();
		std::swap(components[ent], components[--lastElement]);
		set.Delete(ent);
	}

	template <typename T>
	void ComponentArray<T>::Clear()
	{
		set.Clear();
	}

	template <typename T>
	T& ComponentArray<T>::GetComponent(Entity ent)
	{
		return components[ent];
	}
}