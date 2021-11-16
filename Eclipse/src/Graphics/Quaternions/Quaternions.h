#pragma once
#include "GLM/glm/gtx/euler_angles.hpp"
#include "GLM/glm/gtx/orthonormalize.hpp"

//namespace Eclipse
//{
//	class Quaternion
//	{
//	public:
//		Quaternion();
//		Quaternion(float w_, float x_, float y_, float z_);
//		Quaternion(const float s, const glm::vec3& v);
//		friend float operator==(const Quaternion& q1, const Quaternion& q2);
//		friend float operator!=(const Quaternion& q1, const Quaternion& q2);
//		friend Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);
//		friend glm::vec3 operator*(const Quaternion& q, const glm::vec3& v);
//		friend std::ostream& operator <<(std::ostream& out, const Quaternion& q);
//		float sqrMagnitude() const;
//		float magnitude() const;
//		Quaternion normalized() const;
//		void normalize();
//		void rotateTowards(const Quaternion& target, float maxRadiansDelta);
//		static Quaternion angleAxis(const float angle, const glm::vec3& axis);
//		static Quaternion euler(const glm::vec3& eulerAngles);
//		static Quaternion fromToRotation(const glm::vec3& fromDirection, const glm::vec3& toDirection);
//		static Quaternion lookRotation(const glm::vec3& forward);
//		static Quaternion lookRotation(const glm::vec3& forward, const glm::vec3& upwards);
//		Quaternion conjugate() const;
//		Quaternion inverse() const;
//
//	public:
//		float w;
//		float x;
//		float y;
//		float z;
//
//	public:
//		static const Quaternion IDENTITY;
//	};
//
//	float dot(const Quaternion& lhs, const Quaternion& rhs);
//	float angle(const Quaternion& from, const Quaternion& to);
//}
//
//#endif

namespace Eclipse
{
	class Quaternion {
	public:
		float w, x, y, z;
	public:
		Quaternion();
		Quaternion(float W, float X, float Y, float Z);
		Quaternion(const Quaternion& q);
		~Quaternion();
		Quaternion& operator=(const Quaternion&);

		bool operator==(Quaternion& rhs);
		bool operator!=(Quaternion& rhs);

		Quaternion& operator*=(Quaternion& rhs);
		Quaternion& operator*=(const Quaternion& rhs);
		Quaternion operator*(Quaternion& rhs);
		const Quaternion operator*(const Quaternion& rhs);
		Quaternion operator*(float& rhs);
		const Quaternion operator*(const float& rhs);
		Quaternion operator+(Quaternion& rhs);
		const Quaternion operator+(const Quaternion& rhs);
		glm::vec4 operator*(glm::vec4& rhs);
		const glm::vec4 operator*(const glm::vec4& rhs);

		Quaternion Normalized();
		void Normalize();
		Quaternion Conjugate(); // Same as inverse

		void ToAngleAxis(float* angle, glm::vec4* axis);
		void SetEuler(float X, float Y, float Z);
		float Dot(Quaternion& b);
		void SetLookRotation(glm::vec4& lookAt);
		void SetLookRotation(glm::vec4& lookAt, glm::vec4& upDirection);

		static Quaternion LookRotation(glm::vec4& lookAt);
		static Quaternion LookRotation(glm::vec4& lookAt, glm::vec4& upDirection);
		static Quaternion Slerp(Quaternion& from, Quaternion& to, float t);
		static Quaternion Lerp(Quaternion& from, Quaternion& to, float t);
		static float Angle(Quaternion& a, Quaternion& b);
		static float Dot(Quaternion& a, Quaternion& b);
		static Quaternion AngleAxis(float angle, glm::vec4& axis);
		static Quaternion Inverse(Quaternion& rotation);
		static Quaternion Euler(float X, float Y, float Z);
		static glm::mat4 ToMatrix(Quaternion& q);

		static const Quaternion identity;
	};

	const glm::vec4 operator*(glm::vec4& v, const Quaternion& m);
	glm::vec4 operator*(glm::vec4& v, Quaternion& m);
	Quaternion operator*(float f, Quaternion& m);
	const Quaternion operator*(float f, const Quaternion& m);
}