#include "pch.h"
#include "Graphics/Grid/AABB.h"

namespace Eclipse
{
    DYN_AABB::DYN_AABB() :
        surfaceArea(0.0f)
    {
        Min = { 0.0f , 0.0f, 0.0f };
        Max = { 0.0f , 0.0f, 0.0f };
    }

    DYN_AABB::DYN_AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
    {
        Min = { minX , minY, minZ };
        Max = { maxX , maxY, maxZ };

        surfaceArea = CalculateSurfaceArea();
    }

    DYN_AABB DYN_AABB::Merge(const DYN_AABB& rhs) const
    {
        ECVec3 min = { std::min<float>(Min.x, rhs.Min.x), std::min<float>(Min.y, rhs.Min.y), std::min<float>(Min.z, rhs.Min.z) };
        ECVec3 max = { std::max<float>(Max.x, rhs.Max.x), std::max<float>(Max.y, rhs.Max.y), std::max<float>(Max.z, rhs.Max.z) };

        return DYN_AABB({ min.getX() , min.getY() , min.getZ(), max.getX() , max.getY() , max.getZ() });
    }

    DYN_AABB DYN_AABB::Intersection(const DYN_AABB& rhs) const
    {
        ECVec3 min = { std::min<float>(Min.x, rhs.Min.x), std::min<float>(Min.y, rhs.Min.y), std::min<float>(Min.z, rhs.Min.z) };
        ECVec3 max = { std::max<float>(Max.x, rhs.Max.x), std::max<float>(Max.y, rhs.Max.y), std::max<float>(Max.z, rhs.Max.z) };

        return DYN_AABB({ min.getX() , min.getY() , min.getZ(), max.getX() , max.getY() , max.getZ() });
    }

    float DYN_AABB::GetWidth()
    {
        return Max.x - Min.x;
    }

    float DYN_AABB::GetHeight()
    {
        return Max.y - Min.y;
    }

    float DYN_AABB::GetDepth()
    {
        return Max.z - Min.z;
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

        Min = { min.x , min.y , min.z };
        Max = { max.x , max.y , max.z };
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
        Min = { in.getX() , in.getY() , in.getZ() };
    }

    void DYN_AABB::SetMaximum(ECVec3& in)
    {
        Max = { in.getX() , in.getY() , in.getZ() };
    }

    float DYN_AABB::CalculateSurfaceArea()
    {
        return 2.0f * (GetWidth() * GetHeight() + GetWidth() * GetDepth() + GetHeight() * GetDepth());
    }

    bool DYN_AABB::Overlaps(const AABBComponent& other) const
    {
        return Max.x > other.Min.getX() && Min.x < other.Max.getX() &&
            Max.y > other.Min.getY() && Min.y < other.Max.getY() &&
            Max.z > other.Min.getZ() && Min.z < other.Max.getZ();
    }

    bool DYN_AABB::Overlaps(const DYN_AABB& other) const
    {
        return Max.x > other.Min.x && Min.x < other.Max.x&&
            Max.y > other.Min.y && Min.y < other.Max.y&&
            Max.z > other.Min.z && Min.z < other.Max.z;
    }

    bool DYN_AABB::MouseOverlaps(glm::vec3 mousepos, const DYN_AABB& other) const
    {
        return Max.x > other.Min.x && Min.x < other.Max.x&&
            Max.y > other.Min.y && Min.y < other.Max.y&&
            Max.z > other.Min.z && Min.z < other.Max.z;
    }

    bool DYN_AABB::Contains(const DYN_AABB& other) const
    {
        return other.Min.x >= Min.x &&
            other.Max.x <= Max.x &&
            other.Min.y >= Min.y &&
            other.Max.y <= Max.y &&
            other.Min.z >= Min.z &&
            other.Max.z <= Max.z;
    }

    bool DYN_AABB::Contains(const glm::vec3& other) const
    {
        return
            other.x >= Min.x && other.x <= Max.x
            && other.y >= Min.y && other.y <= Max.y
            && other.z >= Min.z && other.z <= Max.z;
    }

    AABBComponent& DYN_AABB::SetAABB(TransformComponent& in, AABBComponent& aabbin)
    {
        glm::vec3 scale = in.scale.ConvertToGlmVec3Type();
        glm::vec3 position = in.position.ConvertToGlmVec3Type();

        glm::vec3 halfExt = scale / 2.0f;
        glm::vec3 min{ position.x - halfExt.x, position.y - halfExt.y, position.z - halfExt.z };
        glm::vec3 max{ position.x + halfExt.x, position.y + halfExt.y, position.z + halfExt.z };

        aabbin.center = ECVec3{ position.x , position.y , position.z };
        aabbin.Min = ECVec3{ min.x,min.y,min.z };
        aabbin.Max = ECVec3{ max.x,max.y,max.z };

        return aabbin;
    }

    AABBComponent& DYN_AABB::SetFrustrumAABB(CameraComponent::CameraType CameraType)
    {
        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraType));
        auto& Transform = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(CameraType));

        AABBComponent Frustrum;

        glm::vec3 Half = { 2,2,2 };
        glm::vec3 scale = glm::vec3{ _camera.farPlane, _camera.farPlane,_camera.farPlane };

        glm::vec3 position = (Transform.position.ConvertToGlmVec3Type()) / Half;
        glm::vec3 halfExt = scale / 2.0f;
        glm::vec3 min{ position.x - halfExt.x, position.y - halfExt.y, position.z - halfExt.z };
        glm::vec3 max{ position.x + halfExt.x, position.y + halfExt.y, position.z + halfExt.z };

        Frustrum.center = ECVec3{ position.x , position.y , position.z };
        Frustrum.Min = ECVec3{ min.x,min.y,min.z };
        Frustrum.Max = ECVec3{ max.x,max.y,max.z };

        return Frustrum;
    }

}