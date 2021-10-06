#pragma once
#include "GLM/glm/gtc/matrix_transform.hpp"

namespace Eclipse
{
# define PI           3.14159265358979323846f  /* pi */


    using Vec2 = glm::vec2;
    using Vec3 = glm::vec3;
    using Vec4 = glm::vec4;
    using Mat4 = glm::mat4;

    const float  TWO_PI = 2.0 * PI;
    const float  HALF_PI = 0.5 * PI;
    const float  QUARTER_PI = 0.25 * PI;
    const float  EIGHTH_PI = 0.125 * PI;
    const float  SIXTEENTH_PI = 0.0625 * PI;

    const float  DEG_TO_RAD = PI / 180.0f;
    const float  RAD_TO_DEG = 180.0f / PI;

    const int XINDEX = 0;
    const int YINDEX = 1;
    const int ZINDEX = 2;

    const Vec3 XAXIS(1.0f, 0.0f, 0.0f);
    const Vec3 YAXIS(0.0f, 1.0f, 0.0f);
    const Vec3 ZAXIS(0.0f, 0.0f, 1.0f);

    static inline Mat4 Translate(const Vec3& t)
    {
        return glm::translate(Mat4(1.0f), t);
    }

    static inline Mat4 Rotate(float angle, const Vec3& axis)
    {
        return glm::rotate(Mat4(1.0f), angle, axis);
    }

    static inline float RoundDecimal(float input)
    {
        return std::floor(input * 10000.0f + 0.5f) / 10000.0f;
    }

    static inline Vec3 RoundDecimal(const Vec3& input)
    {
        return Vec3(RoundDecimal(input[0]), RoundDecimal(input[1]), RoundDecimal(input[2]));
    }
}