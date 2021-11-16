#ifndef BOUNDS_H
#define BOUNDS_H

namespace Eclipse
{
    enum class BoundTypes
    {
        None = 0,
        AABB = 1,
        SPHERE = 2,
        Macount
    };

    class BoundingRegion
    {
    public:
        BoundTypes type = BoundTypes::None;

        // sphere values
        glm::vec3 center{ 0 };
        float radius = 0.0f;

        // bounding box values
        glm::vec3 Minimum{ 0 };
        glm::vec3 Maximum{ 0 };
        glm::vec3 Position{ 0 };
        glm::vec3 Scale{ 0 };

        BoundingRegion(glm::vec3 center, float radius);
        BoundingRegion(glm::vec3 min, glm::vec3 max , glm::vec3 position, glm::vec3 scale);
        BoundingRegion(glm::vec3 position, glm::vec3 Scale);
        glm::vec3 calculateCenter();
        glm::vec3 calculateDimensions();
        bool containsPoint(glm::vec3 pt);
        bool containsRegion(BoundingRegion br);
        bool intersectsWith(BoundingRegion br);
    };
}
#endif