#include "pch.h"

namespace Eclipse
{
	template <typename T>
	SparseSet<T>::SparseSet(unsigned size) :
		_size {size}
	{
		dense = 
	}

	template <typename T>
	SparseSet<T>::~SparseSet()
	{
		if (dense && sparse)
		{
			delete[] dense;
			delete[] sparse;
		}
	}

	template <typename T>
	void SparseSet<T>::Insert()
	{

	}

	template <typename T>
	void SparseSet<T>::Delete(unsigned index)
	{

	}

	template <typename T>
	T* SparseSet<T>::Search(unsigned index)
	{
		if (index > size)
			return nullptr;
	}

	template <typename T>
	void SparseSet<T>::Clear()
	{
		_size = 0;
	}
}