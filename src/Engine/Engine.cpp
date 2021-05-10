#include "pch.h"
#include "Engine.h"
#include "SparseSet/SparseSet.hpp"
#include "TypeID\TypeID.h"

unsigned TypeID<int>::_counter = 0;
unsigned TypeID<char>::_counter = 0;

void Eclipse::Engine::Init()
{

}

void Eclipse::Engine::Run()
{
	TypeID<int> id;
	id.RegisterID<int&>();
	id.RegisterID<char>();

	std::cout << id.GetID<int>() << std::endl;
	std::cout << id.GetID<char>() << std::endl;
	std::cout << id.GetID<char>() << std::endl;
	std::cout << id.GetID<int>() << std::endl;

	std::cout << std::endl;

	TypeID<char> id2;
	std::cout << id2.GetID<int>() << std::endl;
	std::cout << id2.GetID<char>() << std::endl;
	std::cout << id2.GetID<char>() << std::endl;
	std::cout << id2.GetID<int>() << std::endl;
	std::cout << id2.GetID<int&>() << std::endl;
	while (true)
	{

	}
}
