#include "pch.h"
#include "Graphics/Grid/bounds.h"

namespace Eclipse
{
    BoundingRegion::BoundingRegion(BoundTypes type) :
        type(type)
    {

    }

    BoundingRegion::BoundingRegion(glm::vec3 center, float radius) :
        type(BoundTypes::SPHERE),
        center(center),
        radius(radius)
    {

    }

    //initialize as AABB
    BoundingRegion::BoundingRegion(glm::vec3 mina, glm::vec3 maxa, bool in) :
        type(BoundTypes::AABB),
        Minimum(mina),
        Maximum(maxa)
    {

    }

    BoundingRegion::BoundingRegion(glm::vec3 position, glm::vec3 Scale)
    {
        type = BoundTypes::AABB;

        Minimum = { -(Scale.x / 2) ,-(Scale.y / 2)  , -(Scale.z / 2) };
        Maximum = { (Scale.x / 2) ,  (Scale.y / 2)  ,  (Scale.z / 2) };
    }

    glm::vec3 BoundingRegion::calculateCenter()
    {
        if (type == BoundTypes::AABB)
        {
            return ((Minimum + Maximum) / 2.0f);
        }
        else
        {
            return center;
        }
    }

    // calculate dimensions
    glm::vec3 BoundingRegion::calculateDimensions()
    {
        if (type == BoundTypes::AABB)
        {
            return (Maximum - Minimum);
        }
        else
        {
            return glm::vec3(2.0f * radius);
        }
    }

    bool BoundingRegion::containsPoint(glm::vec3 pt)
    {
        if (type == BoundTypes::AABB)
        {
            return (pt.x >= Minimum.x) && (pt.x <= Maximum.x) && (pt.y >= Minimum.y) && (pt.y <= Maximum.y) && (pt.z >= Minimum.z) && (pt.z <= Minimum.z);
        }
        else
        {
            float distSquared = 0.0f;

            for (int i = 0; i < 3; i++)
            {
                distSquared += (center[i] - pt[i]) * (center[i] - pt[i]);
            }

            return distSquared <= (radius * radius);
        }
    }

    bool BoundingRegion::containsRegion(BoundingRegion br)
    {
        if (br.type == BoundTypes::AABB)
        {
            return containsPoint(br.Minimum) && containsPoint(br.Maximum);
        }
        else if (type == BoundTypes::SPHERE && br.type == BoundTypes::SPHERE)
        {
            return glm::length(center - br.center) + br.radius < radius;
        }
        else
        {
            if (!containsPoint(br.center))
            {
                return false;
            }
            for (int i = 0; i < 3; i++)
            {
                if (abs(Maximum[i] - br.center[i]) < br.radius || abs(br.center[i] - Minimum[i]) < br.radius)
                {
                    return false;
                }
            }

            return true;
        }
    }

    bool BoundingRegion::intersectsWith(BoundingRegion br)
    {
        if (type == BoundTypes::AABB && br.type == BoundTypes::AABB)
        {
            // both boxes

            glm::vec3 rad = calculateDimensions() / 2.0f;
            glm::vec3 radBr = br.calculateDimensions() / 2.0f;

            glm::vec3 center = calculateCenter();
            glm::vec3 centerBr = br.calculateCenter();

            glm::vec3 dist = abs(center - centerBr);

            for (int i = 0; i < 3; i++)
            {
                if (dist[i] > rad[i] + radBr[i])
                {
                    // no overlap on this axis
                    return false;
                }
            }

            return true;
        }
        else if (type == BoundTypes::SPHERE && br.type == BoundTypes::SPHERE)
        {
            return glm::length(center - br.center) < (radius + br.radius);
        }
        else if (type == BoundTypes::SPHERE)
        {
            float distSquared = 0.0f;

            for (int i = 0; i < 3; i++)
            {
                if (center[i] < br.Minimum[i])
                {
                    distSquared += (br.Minimum[i] - center[i]) * (br.Minimum[i] * center[i]);
                }
                else if (center[i] > br.Maximum[i])
                {
                    distSquared += (center[i] - br.Maximum[i]) * (center[i] - br.Maximum[i]);
                }
            }

            return distSquared < (radius* radius);
        }
        else
        {
            return br.intersectsWith(*this);
        }
    }
}