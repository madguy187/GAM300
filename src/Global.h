#pragma once
#include "pch.h"

using Entity = uint32_t;
const Entity MAX_ENTITY = 10000;

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENT = 32;
using Signature = std::bitset<MAX_COMPONENT>;