#include "pch.h"
#include "Graphics/Grid/AABB.h"

namespace Eclipse
{
    AABB::AABB() :
        minX(0.0f),
        minY(0.0f),
        minZ(0.0f),
        maxX(0.0f),
        maxY(0.0f),
        maxZ(0.0f),
        surfaceArea(0.0f)
    {

    }

    AABB::AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) :
        minX(minX),
        minY(minY),
        minZ(minZ),
        maxX(maxX),
        maxY(maxY),
        maxZ(maxZ)
    {
        surfaceArea = CalculateSurfaceArea();
    }

    AABB AABB::Merge(const AABB& rhs) const
    {
        ECVec3 min = { std::min<float>(minX, rhs.minX), std::min<float>(minY, rhs.minY), std::min<float>(minZ, rhs.minZ) };
        ECVec3 max = { std::max<float>(maxX, rhs.maxX), std::max<float>(maxY, rhs.maxY), std::max<float>(maxZ, rhs.maxZ) };

        return AABB({ min.getX() , min.getY() , min.getZ(), max.getX() , max.getY() , max.getZ() });
    }

    AABB AABB::Intersection(const AABB& rhs) const
    {
        ECVec3 min = { std::min<float>(minX, rhs.minX), std::min<float>(minY, rhs.minY), std::min<float>(minZ, rhs.minZ) };
        ECVec3 max = { std::max<float>(maxX, rhs.maxX), std::max<float>(maxY, rhs.maxY), std::max<float>(maxZ, rhs.maxZ) };

        return AABB({ min.getX() , min.getY() , min.getZ(), max.getX() , max.getY() , max.getZ() });
    }

    float AABB::GetWidth()
    {
        return maxX - minX;
    }

    float AABB::GetHeight()
    {
        return maxY - minY;
    }

    float AABB::GetDepth()
    {
        return maxZ - minZ;
    }

    float AABB::CalculateSurfaceArea()
    {
        return 2.0f * (GetWidth() * GetHeight() + GetWidth() * GetDepth() + GetHeight() * GetDepth());
    }

    bool AABB::Overlaps(const AABB& other) const
    {
        return maxX > other.minX && minX < other.maxX&&
            maxY > other.minY && minY < other.maxY&&
            maxZ > other.minZ && minZ < other.maxZ;
    }

    bool AABB::Contains(const AABB& other) const
    {
        return other.minX >= minX &&
            other.maxX <= maxX &&
            other.minY >= minY &&
            other.maxY <= maxY &&
            other.minZ >= minZ &&
            other.maxZ <= maxZ;
    }
}