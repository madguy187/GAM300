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
        BoundTypes type;

        // sphere values
        glm::vec3 center;
        float radius;

        // bounding box values
        glm::vec3 Minimum;
        glm::vec3 Maximum;
        glm::vec3 Position;
        glm::vec3 Scale;

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