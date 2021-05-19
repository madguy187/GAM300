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

		SparseSet() = default;
		~SparseSet() = default;

		T Search(T index)
		{
			if (index > _capacity)
				return NOT_FOUND;

			T sparseIndex = sparse[GetPageIndex(index)].pArray[GetIndex(index)];

			if (sparseIndex < _size && dense[GetPageIndex(sparseIndex)].pArray[GetIndex(sparseIndex)] == index)
				return sparseIndex;

			return NOT_FOUND;
		}

		unsigned Insert(T index)
		{
			while (index >= _capacity)
				AddPage();

			if (Search(index) != NOT_FOUND) return NOT_FOUND;

			dense[GetPageIndex(_size)].pArray[GetIndex(_size)] = index;
			sparse[GetPageIndex(index)].pArray[GetIndex(index)] = _size;

			return _size++;
		}

		void Delete(T index)
		{
			if (Search(index) == NOT_FOUND) return;

			T temp = dense[GetPageIndex(_size - 1)].pArray[GetIndex(_size - 1)]; // taking last element in dense

			T sparseIndex = sparse[GetPageIndex(index)].pArray[GetIndex(index)];
			dense[GetPageIndex(sparseIndex)].pArray[GetIndex(sparseIndex)] = temp; // point deleted index to last element

			sparse[GetPageIndex(temp)].pArray[GetIndex(temp)] = sparseIndex; // last element is now deleted index

			_size--;
		}

		void Clear()
		{
			_size = 0;
		}

		void Print()
		{
			std::cout << "Dense" << std::endl;
			for (unsigned i = 0; i < _size; i++)
			{
				std::cout << dense[GetPageIndex(i)].pArray[GetIndex(i)] << " ";
			}

			std::cout << std::endl;
			std::cout << "Sparse" << std::endl;
			for (unsigned i = 0; i < _capacity; i++)
			{
				std::cout << sparse[GetPageIndex(i)].pArray[GetIndex(i)] << " ";
			}
			std::cout << std::endl;
		}

		void AddPage()
		{

			sparse.push_back(Page{});
			dense.push_back(Page{});
			_capacity += PAGE_SIZE;
		}
	};
}