#pragma once
#include "pch.h"

#define EPSILON 0.000001f

// Entities
using Entity = uint32_t;
static constexpr Entity MAX_ENTITY = 10000;

// Components and Signature
using ComponentType = std::uint8_t;
static constexpr ComponentType MAX_COMPONENT = 32;
using Signature = std::bitset<MAX_COMPONENT>;
static constexpr unsigned MAX_PAGE_SIZE = 10;

enum class EditorWindowType
{
	UNASSIGNED,
	HIERARCHY,
	INSPECTOR,
	SCENE,
	ASSETS,
	PROFILER,
	CONSOLELOG,
	GAMEVIEW
};

enum class EditorMenuType
{
	UNASSIGNED,
	FILE,
	EDIT,
	WINDOWS,
	HELP
};
