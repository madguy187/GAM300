#ifndef I_AABB_H
#define I_AABB_H

namespace Eclipse
{
    struct DYN_AABB;

    class IAABB
    {
    public:
        virtual ~IAABB() = default;
        virtual DYN_AABB getAABB() const = 0;
    };
}
#endif // I_AABB_H