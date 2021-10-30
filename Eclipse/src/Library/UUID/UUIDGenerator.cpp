#include "pch.h"
#include "UUIDGenerator.h"

namespace Eclipse
{
    EUUID UUIDGenerator::GenerateUUID()
    {
        auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        uint64_t temp = static_cast<uint64_t>(time) << 32;

        temp |= Random::Rand(static_cast<uint32_t>(1), UINT32_MAX);

        return EUUID(temp);
    }
}