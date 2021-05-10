#include "pch.h"

namespace Eclipse
{
	template <typename T, unsigned PAGE_SIZE>
	SparseSet<T, PAGE_SIZE>::SparseSet()
	{
	}

	template <typename T, unsigned PAGE_SIZE>
	SparseSet<T, PAGE_SIZE>::~SparseSet()
	{
	}

	template <typename T, unsigned PAGE_SIZE>
	unsigned SparseSet<T, PAGE_SIZE>::Insert(T index)
	{
		while (index >= _capacity)
			AddPage();

		if (Search(index) != NOT_FOUND) return;

		dense[GetPageIndex(_size)].pArray[GetIndex(_size)] = index;
		sparse[GetPageIndex(index)].pArray[GetIndex(index)] = _size;

		return _size++;
	}

	template <typename T, unsigned PAGE_SIZE>
	void SparseSet<T, PAGE_SIZE>::Delete(T index)
	{
		if (Search(index) == NOT_FOUND) return;

		T temp = dense[GetPageIndex(_size - 1)].pArray[GetIndex(_size - 1)]; // taking last element in dense

		T sparseIndex = sparse[GetPageIndex(index)].pArray[GetIndex(index)];
		dense[GetPageIndex(sparseIndex)].pArray[GetIndex(sparseIndex)] = temp; // point deleted index to last element

		sparse[GetPageIndex(temp)].pArray[GetIndex(temp)] = sparseIndex; // last element is now deleted index

		_size--;
	}

	template <typename T, unsigned PAGE_SIZE>
	T SparseSet<T, PAGE_SIZE>::Search(T index)
	{
		if (index > _capacity)
			return NOT_FOUND;

		T sparseIndex = sparse[GetPageIndex(index)].pArray[GetIndex(index)];

		if (sparseIndex < _size && dense[GetPageIndex(sparseIndex)].pArray[GetIndex(sparseIndex)] == index)
			return dense[GetPageIndex(sparseIndex)].pArray[GetIndex(sparseIndex)];

		return NOT_FOUND;
	}

	template <typename T, unsigned PAGE_SIZE>
	void SparseSet<T, PAGE_SIZE>::Clear()
	{
		_size = 0;
	}

	template <typename T, unsigned PAGE_SIZE>
	void SparseSet<T, PAGE_SIZE>::AddPage()
	{

		sparse.push_back(Page{});
		dense.push_back(Page{});
		_capacity += PAGE_SIZE;
	}

	template <typename T, unsigned PAGE_SIZE>
	void SparseSet<T, PAGE_SIZE>::Print()
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
}