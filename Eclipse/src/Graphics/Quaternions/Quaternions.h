#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include <ostream>
namespace Eclipse
{
	class Quaternion
	{
	public:
		Quaternion();
		Quaternion(float w_, float x_, float y_, float z_);
		Quaternion(const float s, const glm::vec3& v);
		friend float operator==(const Quaternion& q1, const Quaternion& q2);
		friend float operator!=(const Quaternion& q1, const Quaternion& q2);
		friend Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);
		friend glm::vec3 operator*(const Quaternion& q, const glm::vec3& v);
		friend std::ostream& operator <<(std::ostream& out, const Quaternion& q);
		float sqrMagnitude() const;
		float magnitude() const;
		Quaternion normalized() const;
		void normalize();
		void rotateTowards(const Quaternion& target, float maxRadiansDelta);
		static Quaternion angleAxis(const float angle, const glm::vec3& axis);
		static Quaternion euler(const glm::vec3& eulerAngles);
		static Quaternion fromToRotation(const glm::vec3& fromDirection, const glm::vec3& toDirection);
		static Quaternion lookRotation(const glm::vec3& forward);
		static Quaternion lookRotation(const glm::vec3& forward, const glm::vec3& upwards);
		Quaternion conjugate() const;
		Quaternion inverse() const;

	public:
		float w;
		float x;
		float y;
		float z;

	public:
		static const Quaternion IDENTITY;
	};

	float dot(const Quaternion& lhs, const Quaternion& rhs);
	float angle(const Quaternion& from, const Quaternion& to);
}

#endif