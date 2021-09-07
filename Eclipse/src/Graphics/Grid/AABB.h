#ifndef DYNAMICTREE_AABB_H
#define DYNAMICTREE_AABB_H

#include <algorithm>
#include "ECS/ComponentManager/Components/AabbComponent.h"

namespace Eclipse
{
    class DYN_AABB
    {
    public:
        glm::vec3 Min;
        glm::vec3 Max;
        float surfaceArea;
        float DistanceToObject = 0.0f;

    private:
        unsigned int EntityID = 0;
        bool IsGrid = false;

    public:
        DYN_AABB();
        DYN_AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ); // :

        bool Overlaps(const DYN_AABB& other) const;
        bool Overlaps(const AabbComponent& other) const;
        bool MouseOverlaps(glm::vec3 mousepos, const DYN_AABB& other) const;
        bool Contains(const DYN_AABB& other) const;
        bool Contains(const glm::vec3& other) const;
        DYN_AABB Merge(const DYN_AABB& rhs) const;
        DYN_AABB Intersection(const DYN_AABB& rhs) const;
        static AabbComponent& SetAABB(TransformComponent& in, AabbComponent& aabbin);

        float GetWidth();
        float GetHeight();
        float GetDepth();
        unsigned int GetEntityID();
        bool CheckIsGrid();
        void SetIsGrid(bool in);
        void SetMaxMin(ECVec3& Max, ECVec3& Min , unsigned int id);
        void SetEntityID(unsigned int id);
        ECVec3 GetMinimum(TransformComponent& in);
        ECVec3 GetMaximum(TransformComponent& in);
        void UpdateMinMax(TransformComponent& in);

    private:
        float CalculateSurfaceArea();
        void SetMinimum(ECVec3& in);
        void SetMaximum(ECVec3& in);
    };
}
#endif // DYNAMICTREE_AABB_H