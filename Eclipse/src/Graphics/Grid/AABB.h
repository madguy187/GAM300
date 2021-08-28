#ifndef DYNAMICTREE_AABB_H
#define DYNAMICTREE_AABB_H
#include <algorithm>

namespace Eclipse
{
    struct AABB
    {
    public:
        unsigned int EntityID = 0;
        float minX;
        float minY;
        float minZ;
        float maxX;
        float maxY;
        float maxZ;
        float surfaceArea;

        AABB();
        AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ); // :

        bool Overlaps(const AABB& other) const;
        bool Contains(const AABB& other) const;
        AABB Merge(const AABB& rhs) const;
        AABB Intersection(const AABB& rhs) const;

        float GetWidth();
        float GetHeight();
        float GetDepth();

    private:
        float CalculateSurfaceArea();
    };
}
#endif // DYNAMICTREE_AABB_H