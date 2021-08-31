#include "pch.h"
#include "Graphics/Grid/AABB.h"

namespace Eclipse
{
    DYN_AABB::DYN_AABB() :
        minX(0.0f),
        minY(0.0f),
        minZ(0.0f),
        maxX(0.0f),
        maxY(0.0f),
        maxZ(0.0f),
        surfaceArea(0.0f)
    {

    }

    DYN_AABB::DYN_AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) :
        minX(minX),
        minY(minY),
        minZ(minZ),
        maxX(maxX),
        maxY(maxY),
        maxZ(maxZ)
    {
        surfaceArea = CalculateSurfaceArea();
    }

    DYN_AABB DYN_AABB::Merge(const DYN_AABB& rhs) const
    {
        ECVec3 min = { std::min<float>(minX, rhs.minX), std::min<float>(minY, rhs.minY), std::min<float>(minZ, rhs.minZ) };
        ECVec3 max = { std::max<float>(maxX, rhs.maxX), std::max<float>(maxY, rhs.maxY), std::max<float>(maxZ, rhs.maxZ) };

        return DYN_AABB({ min.getX() , min.getY() , min.getZ(), max.getX() , max.getY() , max.getZ() });
    }

    DYN_AABB DYN_AABB::Intersection(const DYN_AABB& rhs) const
    {
        ECVec3 min = { std::min<float>(minX, rhs.minX), std::min<float>(minY, rhs.minY), std::min<float>(minZ, rhs.minZ) };
        ECVec3 max = { std::max<float>(maxX, rhs.maxX), std::max<float>(maxY, rhs.maxY), std::max<float>(maxZ, rhs.maxZ) };

        return DYN_AABB({ min.getX() , min.getY() , min.getZ(), max.getX() , max.getY() , max.getZ() });
    }

    float DYN_AABB::GetWidth()
    {
        return maxX - minX;
    }

    float DYN_AABB::GetHeight()
    {
        return maxY - minY;
    }

    float DYN_AABB::GetDepth()
    {
        return maxZ - minZ;
    }

    unsigned int DYN_AABB::GetEntityID()
    {
        return EntityID;
    }

    bool DYN_AABB::CheckIsGrid()
    {
        return IsGrid;
    }

    void DYN_AABB::SetEntityID(unsigned int id)
    {
        EntityID = id;
    }

    ECVec3 DYN_AABB::GetMinimum(TransformComponent& in)
    {
        glm::vec3 halfExt = in.scale.ConvertToGlmVec3Type() / 2.0f;
        ECVec3 min = ECVec3{ in.position.getX() - halfExt.x, in.position.getY() - halfExt.y, in.position.getZ() - halfExt.z };

        return min;
    }

    ECVec3 DYN_AABB::GetMaximum(TransformComponent& in)
    {
        glm::vec3 halfExt = in.scale.ConvertToGlmVec3Type() / 2.0f;
        ECVec3 max = ECVec3{ in.position.getX() + halfExt.x, in.position.getY() + halfExt.y, in.position.getZ() + halfExt.z };

        return max;
    }

    void DYN_AABB::UpdateMinMax(TransformComponent& in)
    {
        glm::vec3 scale = in.scale.ConvertToGlmVec3Type();
        glm::vec3 position = in.position.ConvertToGlmVec3Type();

        glm::vec3 halfExt = scale / 2.0f;
        glm::vec3 min{ position.x - halfExt.x, position.y - halfExt.y, position.z - halfExt.z };
        glm::vec3 max{ position.x + halfExt.x, position.y + halfExt.y, position.z + halfExt.z };

        minX = min.x;
        minY = min.y;
        minZ = min.z;

        maxX = max.x;
        maxY = max.y;
        maxZ = max.z;
    }

    void DYN_AABB::SetIsGrid(bool in)
    {
        IsGrid = in;
    }

    void DYN_AABB::SetMaxMin(ECVec3& Max, ECVec3& Min, unsigned int id)
    {
        SetMaximum(Max);
        SetMinimum(Min);
        SetEntityID(id);

        surfaceArea = CalculateSurfaceArea();
    }

    void DYN_AABB::SetMinimum(ECVec3& in)
    {
        minX = in.getX();
        minY = in.getY();
        minZ = in.getZ();
    }

    void DYN_AABB::SetMaximum(ECVec3& in)
    {
        maxX = in.getX();
        maxY = in.getY();
        maxZ = in.getZ();
    }

    float DYN_AABB::CalculateSurfaceArea()
    {
        return 2.0f * (GetWidth() * GetHeight() + GetWidth() * GetDepth() + GetHeight() * GetDepth());
    }

    bool DYN_AABB::Overlaps(const DYN_AABB& other) const
    {
        return maxX > other.minX && minX < other.maxX&&
            maxY > other.minY && minY < other.maxY&&
            maxZ > other.minZ && minZ < other.maxZ;
    }

    bool DYN_AABB::MouseOverlaps(glm::vec3 mousepos, const DYN_AABB& other) const
    {
        return maxX > other.minX && minX < other.maxX&&
            maxY > other.minY && minY < other.maxY&&
            maxZ > other.minZ && minZ < other.maxZ;
    }

    bool DYN_AABB::Contains(const DYN_AABB& other) const
    {
        return other.minX >= minX &&
            other.maxX <= maxX &&
            other.minY >= minY &&
            other.maxY <= maxY &&
            other.minZ >= minZ &&
            other.maxZ <= maxZ;
    }

    bool DYN_AABB::Contains(const glm::vec3& other) const
    {
        return
            other.x >= minX && other.x <= maxX
            && other.y >= minY && other.y <= maxY
            && other.z >= minZ && other.z <= maxZ;
    }
}