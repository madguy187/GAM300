#pragma once
#include "Global.h"
#include "PxPhysicsAPI.h"
namespace Eclipse
{
    struct EC_Shape
    {
        PxShapeType shape{ PxShapeType::Px_SPHERE};
        float hx{ 2.5f };
        float hy{ 2.5f };
        float hz{ 2.5f };
        float radius{ 1 };
        float hheight{ 1 };

        bool operator==(const EC_Shape& rhs) const
        {
            if (shape == rhs.shape && hx == rhs.hx &&
                hy == rhs.hy && hz == rhs.hz && radius == rhs.radius &&
                hheight == rhs.hheight)
            {
                return true;
            }

            return false;
        }
    };

    struct CollisionComponent
    {
        META_DATA(CollisionComponent);
        std::vector<Entity> Collided;
        EC_Shape shape;
        bool created{ false };
        bool isTrigger{ false };
    };
}