#include "pch.h"
#include "Graphics/Quaternions/Quaternions.h"

namespace Eclipse
{
	const glm::vec3 FORWARD = glm::vec3(0.0f, 0.0f, 1.0f);
	const glm::vec3 BACK = glm::vec3(0.0f, 0.0f, -1.0f);
	const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 DOWN = glm::vec3(0.0f, -1.0f, 0.0f);
	const glm::vec3 RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec3 LEFT = glm::vec3(-1.0f, 0.0f, 0.0f);
	const glm::vec3 ONE = glm::vec3(1.0f, 1.0f, 1.0f);
	const glm::vec3 ZERO = glm::vec3(0.0f, 0.0f, 0.0f);

	const Quaternion Quaternion::IDENTITY = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);

	Quaternion::Quaternion()
		: w(1.0f)
		, x(0.0f)
		, y(0.0f)
		, z(0.0f)
	{

	}

	Quaternion::Quaternion(float w_, float x_, float y_, float z_)
		: w(w_)
		, x(x_)
		, y(y_)
		, z(z_)
	{

	}

	Quaternion::Quaternion(const float s, const glm::vec3& v)
		: w(s)
		, x(v.x)
		, y(v.y)
		, z(v.z)
	{

	}

	float operator==(const Quaternion& q1, const Quaternion& q2)
	{
		const float epsilon = 1e-6;
		return std::abs(q1.w - q2.w) < epsilon && std::abs(q1.x - q2.x) < epsilon
			&& std::abs(q1.y - q2.y) < epsilon && std::abs(q1.z - q2.z) < epsilon;
	}

	float operator!=(const Quaternion& q1, const Quaternion& q2)
	{
		return !(q1 == q2);
	}

	Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
	{
		return Quaternion(
			lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z,
			lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
			lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w
		);
	}

	glm::vec3 operator*(const Quaternion& q, const glm::vec3& v)
	{
		const glm::vec3 qv = glm::vec3(q.x, q.y, q.z);
		const glm::vec3 t = 2.0f * cross(qv, v);
		return v + q.w * t + cross(qv, t);
	}

	std::ostream& operator <<(std::ostream& out, const Quaternion& q)
	{
		out << q.w << " + " << q.x << "i + " << q.y << "j + " << q.z << "k";
		return out;
	}

	float Quaternion::sqrMagnitude() const
	{
		return w * w + x * x + y * y + z * z;
	}

	float Quaternion::magnitude() const
	{
		return sqrt(sqrMagnitude());
	}

	Quaternion Quaternion::normalized() const
	{
		assert(magnitude() > 0.0f);
		const float invNorm = 1.0f / magnitude();

		return Quaternion(w * invNorm, x * invNorm, y * invNorm, z * invNorm);
	}

	void Quaternion::rotateTowards(const Quaternion& target, float maxRadiansDelta)
	{
		// Relative unit quaternion rotation between this quaternion and the
		// target quaternion.
		const Quaternion relativeRotation = conjugate() * target;

		// Calculate the angle and axis of the relative quaternion rotation.
		assert(std::abs(relativeRotation.w) <= 1.0f);
		const float angle = 2.0f * std::acos(relativeRotation.w);
		const glm::vec3 axis(relativeRotation.x, relativeRotation.y, relativeRotation.z);

		// Apply a step of the relative rotation.
		if (angle > maxRadiansDelta)
		{
			const Quaternion delta = Quaternion::angleAxis(maxRadiansDelta, glm::normalize(axis));
			(*this) = delta * (*this);
		}
		else
		{
			// We would overshoot, so just set this quaternion to the target.
			(*this) = target;
		}
	}

	void Quaternion::normalize()
	{
		assert(magnitude() > 0.0f);
		const float invNorm = 1.0f / magnitude();

		w *= invNorm;
		x *= invNorm;
		y *= invNorm;
		z *= invNorm;
	}

	Quaternion Quaternion::angleAxis(const float angle, const glm::vec3& axis)
	{
		//assert(std::abs(axis.magnitude() - 1.0f) < 1e-6);

		const float halfAngle = 0.5f * angle;
		return Quaternion(std::cos(halfAngle), axis * std::sin(halfAngle));
	}

	Quaternion Quaternion::euler(const glm::vec3& eulerAngles)
	{
		const float halfPhi = 0.5f * eulerAngles.x; // Half the roll.
		const float halfTheta = 0.5f * eulerAngles.y; // Half the pitch.
		const float halfPsi = 0.5f * eulerAngles.z; // Half the yaw.

		const float cosHalfPhi = std::cos(halfPhi);
		const float sinHalfPhi = std::sin(halfPhi);
		const float cosHalfTheta = std::cos(halfTheta);
		const float sinHalfTheta = std::sin(halfTheta);
		const float cosHalfPsi = std::cos(halfPsi);
		const float sinHalfPsi = std::sin(halfPsi);

		return Quaternion(
			cosHalfPhi * cosHalfTheta * cosHalfPsi - sinHalfPhi * sinHalfTheta * sinHalfPsi,
			sinHalfPhi * cosHalfTheta * cosHalfPsi + cosHalfPhi * sinHalfTheta * sinHalfPsi,
			cosHalfPhi * sinHalfTheta * cosHalfPsi - sinHalfPhi * cosHalfTheta * sinHalfPsi,
			cosHalfPhi * cosHalfTheta * sinHalfPsi + sinHalfPhi * sinHalfTheta * cosHalfPsi
		);
	}

	Quaternion Quaternion::fromToRotation(const glm::vec3& fromDirection, const glm::vec3& toDirection)
	{
		const glm::vec3 unitFrom = glm::normalize(fromDirection);
		const glm::vec3 unitTo = glm::normalize(toDirection);
		const float d = dot(unitFrom, unitTo);

		if (d >= 1.0f)
		{
			return Quaternion::IDENTITY;
		}
		else if (d <= -1.0f)
		{
			glm::vec3 axis = cross(unitFrom, RIGHT);
			if (glm::length(axis) < 1e-6)
			{
				axis = cross(unitFrom, UP);
			}

			return angleAxis(M_PI, glm::normalize(axis));
		}
		else
		{
			const float s = sqrt(glm::length(unitFrom) * glm::length(unitTo))
				+ dot(unitFrom, unitTo);

			const glm::vec3 v = cross(unitFrom, unitTo);

			return Quaternion(s, v).normalized();
		}
	}

	Quaternion Quaternion::lookRotation(const glm::vec3& forward)
	{
		return Quaternion::fromToRotation(FORWARD, forward);
	}

	Quaternion Quaternion::lookRotation(const glm::vec3& forward, const glm::vec3& upwards)
	{
		const Quaternion q1 = Quaternion::lookRotation(forward);

		if (glm::length(cross(forward, upwards)) < 1e-6)
		{
			return q1;
		}

		const glm::vec3 newUp = q1 * UP;
		const Quaternion q2 = fromToRotation(newUp, upwards);
		return q2 * q1;
	}

	Quaternion Quaternion::conjugate() const
	{
		return Quaternion(w, -x, -y, -z);
	}

	Quaternion Quaternion::inverse() const
	{
		const float sqr = sqrMagnitude();
		assert(sqr > 0.0f);

		const float invSqr = 1.0f / sqr;
		return Quaternion(w * invSqr, -x * invSqr, -y * invSqr, -z * invSqr);
	}

	float dot(const Quaternion& lhs, const Quaternion& rhs)
	{
		return lhs.w * rhs.w + lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	float angle(const Quaternion& from, const Quaternion& to)
	{
		const Quaternion relativeRotation = from.conjugate() * to;
		assert(std::abs(relativeRotation.w) <= 1.0f);
		return 2.0f * std::acos(relativeRotation.w);
	}
}