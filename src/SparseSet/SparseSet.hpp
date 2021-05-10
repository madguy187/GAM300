#pragma once
#include "Global.h"

namespace Eclipse
{
	template <typename T, unsigned PAGE_SIZE>
	class SparseSet
	{
		struct Page
		{
			std::array<T, PAGE_SIZE> pArray;
		};

		std::vector<Page> dense;
		std::vector<Page> sparse;
		unsigned _size = 0; // current size of dense
		unsigned _capacity = 0;

		__forceinline T GetPageIndex(T index)	{ return index / PAGE_SIZE; }
		__forceinline T GetIndex(T index)		{ return index % PAGE_SIZE; }
	public:
		__forceinline unsigned GetSize() const { return _size; }

		const T NOT_FOUND = -1;

		SparseSet();
		~SparseSet();
		T Search(T index);
		unsigned Insert(T index);
		void Delete(T index);
		void Clear();
		void Print();
		void AddPage();
	};
}

#include "SparseSet.cpp"