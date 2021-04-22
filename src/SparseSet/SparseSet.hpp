#pragma once
#include "Global.h"

namespace Eclipse
{
	template <typename T>
	class SparseSet
	{
		T* dense;
		int* sparse;
		unsigned _size = 0;
	public:
		SparseSet(unsigned size);
		~SparseSet();
		void Insert();
		void Delete(unsigned index);
		T* Search(unsigned index);
		void Clear();
	};
}

#include "SparseSet.cpp"