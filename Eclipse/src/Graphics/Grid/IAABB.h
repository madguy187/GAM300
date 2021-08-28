#ifndef I_AABB_H
#define I_AABB_H

namespace Eclipse
{
    struct AABB;

    class IAABB
    {
    public:
        virtual ~IAABB() = default;
        virtual AABB getAABB() const = 0;
    };
}
#endif // I_AABB_H