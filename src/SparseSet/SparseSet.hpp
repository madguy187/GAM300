#pragma once
#include "Global.h"

namespace Eclipse
{
	template <unsigned PAGE_SIZE>
	class SparseSet
	{
		struct Page
		{
			std::array<int, PAGE_SIZE> pArray;
		};

		std::vector<Page> dense;
		std::vector<Page> sparse;
		unsigned _size = 0; // current size of dense
		unsigned _capacity = 0;

		__forceinline int GetPageIndex(int index)	{ return index / PAGE_SIZE; }
		__forceinline int GetIndex(int index)		{ return index % PAGE_SIZE; }
	public:
		SparseSet();
		~SparseSet();
		void Insert(int index);
		void Delete(int index);
		int Search(int index);
		void Clear();
		void Print();
		void AddPage();
	};
}

#include "SparseSet.cpp"