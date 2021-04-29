#include "pch.h"

namespace Eclipse
{
	template <unsigned PAGE_SIZE>
	SparseSet<PAGE_SIZE>::SparseSet()
	{
	}

	template <unsigned PAGE_SIZE>
	SparseSet<PAGE_SIZE>::~SparseSet()
	{
	}

	template <unsigned PAGE_SIZE>
	void SparseSet<PAGE_SIZE>::Insert(int index)
	{
		while (index >= _capacity)
			AddPage();

		if (Search(index) != -1) return;

		dense[GetPageIndex(_size)].pArray[GetIndex(_size)] = index;
		sparse[GetPageIndex(index)].pArray[GetIndex(index)] = _size;

		_size++;
	}

	template <unsigned PAGE_SIZE>
	void SparseSet<PAGE_SIZE>::Delete(int index)
	{
		if (Search(index) == -1) return;

		int temp = dense[GetPageIndex(_size - 1)].pArray[GetIndex(_size - 1)]; // taking last element in dense

		int sparseIndex = sparse[GetPageIndex(index)].pArray[GetIndex(index)];
		dense[GetPageIndex(sparseIndex)].pArray[GetIndex(sparseIndex)] = temp; // point deleted index to last element

		sparse[GetPageIndex(temp)].pArray[GetIndex(temp)] = sparseIndex; // last element is now deleted index

		_size--;
	}

	template <unsigned PAGE_SIZE>
	int SparseSet<PAGE_SIZE>::Search(int index)
	{
		if (index > _capacity)
			return -1;

		int sparseIndex = sparse[GetPageIndex(index)].pArray[GetIndex(index)];

		if (sparseIndex < _size && dense[GetPageIndex(sparseIndex)].pArray[GetIndex(sparseIndex)] == index)
			return sparseIndex;

		return -1;
	}

	template <unsigned PAGE_SIZE>
	void SparseSet<PAGE_SIZE>::Clear()
	{
		_size = 0;
	}

	template <unsigned PAGE_SIZE>
	void SparseSet<PAGE_SIZE>::AddPage()
	{

		sparse.push_back(Page{});
		dense.push_back(Page{});
		_capacity += PAGE_SIZE;
	}

	template <unsigned PAGE_SIZE>
	void SparseSet<PAGE_SIZE>::Print()
	{
		std::cout << "Dense" << std::endl;
		for (int i = 0; i < _size; i++)
		{
			std::cout << dense[GetPageIndex(i)].pArray[GetIndex(i)] << " ";
		}

		std::cout << std::endl;
		std::cout << "Sparse" << std::endl;
		for (int i = 0; i < _capacity; i++)
		{
			std::cout << sparse[GetPageIndex(i)].pArray[GetIndex(i)] << " ";
		}
	}
}