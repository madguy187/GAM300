#pragma once
#include "pch.h"

using Entity = uint32_t;
static constexpr Entity MAX_ENTITY = 10000;

using ComponentType = std::uint8_t;
static constexpr ComponentType MAX_COMPONENT = 32;
using Signature = std::bitset<MAX_COMPONENT>;