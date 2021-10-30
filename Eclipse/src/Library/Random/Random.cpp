#include "pch.h"
#include "Random.h"

namespace Eclipse
{
	std::mt19937 Random::mt_rand{ std::random_device{}() };
}