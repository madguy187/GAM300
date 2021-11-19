#include "pch.h"
#include "Graphics/Quaternions/Quaternions.h"

//namespace Eclipse
//{
//	const glm::vec3 FORWARD = glm::vec3(0.0f, 0.0f, 1.0f);
//	const glm::vec3 BACK = glm::vec3(0.0f, 0.0f, -1.0f);
//	const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
//	const glm::vec3 DOWN = glm::vec3(0.0f, -1.0f, 0.0f);
//	const glm::vec3 RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
//	const glm::vec3 LEFT = glm::vec3(-1.0f, 0.0f, 0.0f);
//	const glm::vec3 ONE = glm::vec3(1.0f, 1.0f, 1.0f);
//	const glm::vec3 ZERO = glm::vec3(0.0f, 0.0f, 0.0f);
//
//	const Quaternion Quaternion::IDENTITY = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
//
//	Quaternion::Quaternion()
//		: w(1.0f)
//		, x(0.0f)
//		, y(0.0f)
//		, z(0.0f)
//	{
//
//	}
//
//	Quaternion::Quaternion(float w_, float x_, float y_, float z_)
//		: w(w_)
//		, x(x_)
//		, y(y_)
//		, z(z_)
//	{
//
//	}
//
//	Quaternion::Quaternion(const float s, const glm::vec3& v)
//		: w(s)
//		, x(v.x)
//		, y(v.y)
//		, z(v.z)
//	{
//
//	}
//
//	float operator==(const Quaternion& q1, const Quaternion& q2)
//	{
//		const float epsilon = 1e-6;
//		return std::abs(q1.w - q2.w) < epsilon && std::abs(q1.x - q2.x) < epsilon
//			&& std::abs(q1.y - q2.y) < epsilon && std::abs(q1.z - q2.z) < epsilon;
//	}
//
//	float operator!=(const Quaternion& q1, const Quaternion& q2)
//	{
//		return !(q1 == q2);
//	}
//
//	Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
//	{
//		return Quaternion(
//			lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z,
//			lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
//			lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
//			lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w
//		);
//	}
//
//	glm::vec3 operator*(const Quaternion& q, const glm::vec3& v)
//	{
//		const glm::vec3 qv = glm::vec3(q.x, q.y, q.z);
//		const glm::vec3 t = 2.0f * cross(qv, v);
//		return v + q.w * t + cross(qv, t);
//	}
//
//	std::ostream& operator <<(std::ostream& out, const Quaternion& q)
//	{
//		out << q.w << " + " << q.x << "i + " << q.y << "j + " << q.z << "k";
//		return out;
//	}
//
//	float Quaternion::sqrMagnitude() const
//	{
//		return w * w + x * x + y * y + z * z;
//	}
//
//	float Quaternion::magnitude() const
//	{
//		return sqrt(sqrMagnitude());
//	}
//
//	Quaternion Quaternion::normalized() const
//	{
//		assert(magnitude() > 0.0f);
//		const float invNorm = 1.0f / magnitude();
//
//		return Quaternion(w * invNorm, x * invNorm, y * invNorm, z * invNorm);
//	}
//
//	void Quaternion::rotateTowards(const Quaternion& target, float maxRadiansDelta)
//	{
//		// Relative unit quaternion rotation between this quaternion and the
//		// target quaternion.
//		const Quaternion relativeRotation = conjugate() * target;
//
//		// Calculate the angle and axis of the relative quaternion rotation.
//		assert(std::abs(relativeRotation.w) <= 1.0f);
//		const float angle = 2.0f * std::acos(relativeRotation.w);
//		const glm::vec3 axis(relativeRotation.x, relativeRotation.y, relativeRotation.z);
//
//		// Apply a step of the relative rotation.
//		if (angle > maxRadiansDelta)
//		{
//			const Quaternion delta = Quaternion::angleAxis(maxRadiansDelta, glm::normalize(axis));
//			(*this) = delta * (*this);
//		}
//		else
//		{
//			// We would overshoot, so just set this quaternion to the target.
//			(*this) = target;
//		}
//	}
//
//	void Quaternion::normalize()
//	{
//		assert(magnitude() > 0.0f);
//		const float invNorm = 1.0f / magnitude();
//
//		w *= invNorm;
//		x *= invNorm;
//		y *= invNorm;
//		z *= invNorm;
//	}
//
//	Quaternion Quaternion::angleAxis(const float angle, const glm::vec3& axis)
//	{
//		//assert(std::abs(axis.magnitude() - 1.0f) < 1e-6);
//
//		const float halfAngle = 0.5f * angle;
//		return Quaternion(std::cos(halfAngle), axis * std::sin(halfAngle));
//	}
//
//	Quaternion Quaternion::euler(const glm::vec3& eulerAngles)
//	{
//		const float halfPhi = 0.5f * eulerAngles.x; // Half the roll.
//		const float halfTheta = 0.5f * eulerAngles.y; // Half the pitch.
//		const float halfPsi = 0.5f * eulerAngles.z; // Half the yaw.
//
//		const float cosHalfPhi = std::cos(halfPhi);
//		const float sinHalfPhi = std::sin(halfPhi);
//		const float cosHalfTheta = std::cos(halfTheta);
//		const float sinHalfTheta = std::sin(halfTheta);
//		const float cosHalfPsi = std::cos(halfPsi);
//		const float sinHalfPsi = std::sin(halfPsi);
//
//		return Quaternion(
//			cosHalfPhi * cosHalfTheta * cosHalfPsi - sinHalfPhi * sinHalfTheta * sinHalfPsi,
//			sinHalfPhi * cosHalfTheta * cosHalfPsi + cosHalfPhi * sinHalfTheta * sinHalfPsi,
//			cosHalfPhi * sinHalfTheta * cosHalfPsi - sinHalfPhi * cosHalfTheta * sinHalfPsi,
//			cosHalfPhi * cosHalfTheta * sinHalfPsi + sinHalfPhi * sinHalfTheta * cosHalfPsi
//		);
//	}
//
//	Quaternion Quaternion::fromToRotation(const glm::vec3& fromDirection, const glm::vec3& toDirection)
//	{
//		const glm::vec3 unitFrom = glm::normalize(fromDirection);
//		const glm::vec3 unitTo = glm::normalize(toDirection);
//		const float d = dot(unitFrom, unitTo);
//
//		if (d >= 1.0f)
//		{
//			return Quaternion::IDENTITY;
//		}
//		else if (d <= -1.0f)
//		{
//			glm::vec3 axis = cross(unitFrom, RIGHT);
//			if (glm::length(axis) < 1e-6)
//			{
//				axis = cross(unitFrom, UP);
//			}
//
//			return angleAxis(M_PI, glm::normalize(axis));
//		}
//		else
//		{
//			const float s = sqrt(glm::length(unitFrom) * glm::length(unitTo))
//				+ dot(unitFrom, unitTo);
//
//			const glm::vec3 v = cross(unitFrom, unitTo);
//
//			return Quaternion(s, v).normalized();
//		}
//	}
//
//	Quaternion Quaternion::lookRotation(const glm::vec3& forward)
//	{
//		return Quaternion::fromToRotation(FORWARD, forward);
//	}
//
//	Quaternion Quaternion::lookRotation(const glm::vec3& forward, const glm::vec3& upwards)
//	{
//		const Quaternion q1 = Quaternion::lookRotation(forward);
//
//		if (glm::length(cross(forward, upwards)) < 1e-6)
//		{
//			return q1;
//		}
//
//		const glm::vec3 newUp = q1 * UP;
//		const Quaternion q2 = fromToRotation(newUp, upwards);
//		return q2 * q1;
//	}
//
//	Quaternion Quaternion::conjugate() const
//	{
//		return Quaternion(w, -x, -y, -z);
//	}
//
//	Quaternion Quaternion::inverse() const
//	{
//		const float sqr = sqrMagnitude();
//		assert(sqr > 0.0f);
//
//		const float invSqr = 1.0f / sqr;
//		return Quaternion(w * invSqr, -x * invSqr, -y * invSqr, -z * invSqr);
//	}
//
//	float dot(const Quaternion& lhs, const Quaternion& rhs)
//	{
//		return lhs.w * rhs.w + lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
//	}
//
//	float angle(const Quaternion& from, const Quaternion& to)
//	{
//		const Quaternion relativeRotation = from.conjugate() * to;
//		assert(std::abs(relativeRotation.w) <= 1.0f);
//		return 2.0f * std::acos(relativeRotation.w);
//	}
//}

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

	const Quaternion Quaternion::identity(1.0f, 0.0f, 0.0f, 0.0f);

	Quaternion::Quaternion() : w(0.0f), x(0.0f), y(0.0f), z(0.0f) { }
	Quaternion::Quaternion(float W, float X, float Y, float Z) : w(W), x(X), y(Y), z(Z) { }
	Quaternion::~Quaternion() { }

	Quaternion::Quaternion(const Quaternion& q) {
		w = q.w;
		x = q.x;
		y = q.y;
		z = q.z;
	}

	Quaternion& Quaternion::operator=(const Quaternion& q) {
		if (this == &q) return *this;
		w = q.w;
		x = q.x;
		y = q.y;
		z = q.z;
		return *this;
	}

	glm::mat4 Quaternion::ToMatrix(Quaternion& q)
	{
		//float sqw = q.w * q.w;
		//float sqx = q.x * q.x;
		//float sqy = q.y * q.y;
		//float sqz = q.z * q.z;
		//float invs = 1.0f / (sqx + sqy + sqz + sqw);

		//glm::mat4 matrix(1.0f);

		//matrix[0][0] = (sqx - sqy - sqz + sqw) * invs;
		//matrix[1][1] = (-sqx + sqy - sqz + sqw) * invs;
		//matrix[2][2] = (-sqx - sqy + sqz + sqw) * invs;

		//float tmp1 = q.x * q.y;
		//float tmp2 = q.z * q.w;
		//matrix[1][1] = 2.0 * (tmp1 + tmp2) * invs;
		//matrix[0][1] = 2.0 * (tmp1 - tmp2) * invs;

		//tmp1 = q.x * q.z;
		//tmp2 = q.y * q.w;
		//matrix[2][2] = 2.0 * (tmp1 - tmp2) * invs;
		//matrix[0][2] = 2.0 * (tmp1 + tmp2) * invs;

		//tmp1 = q.y * q.z;
		//tmp2 = q.x * q.w;
		//matrix[2][1] = 2.0 * (tmp1 + tmp2) * invs;
		//matrix[1][2] = 2.0 * (tmp1 - tmp2) * invs;

		glm::mat4 matrix(1.0f);

		glm::quat Hi;
		Hi.x = q.x;
		Hi.y = q.y;
		Hi.z = q.z;
		Hi.w = q.w;

		matrix = glm::toMat4(Hi);
		//

		return matrix;
	}

	bool Quaternion::operator==(Quaternion& rhs) {
		return (w == rhs.w && x == rhs.x && y == rhs.y && z == rhs.z);
	}

	bool Quaternion::operator!=(Quaternion& rhs) {
		return !(w == rhs.w && x == rhs.x && y == rhs.y && z == rhs.z);
	}

	Quaternion& Quaternion::operator*=(Quaternion& rhs) {
		Quaternion q;

		q.w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
		q.x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
		q.y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
		q.z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;

		*this = q;
		return *this;
	}

	Quaternion& Quaternion::operator*=(const Quaternion& rhs) {
		Quaternion q;

		q.w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
		q.x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
		q.y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
		q.z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;

		*this = q;
		return *this;
	}

	Quaternion Quaternion::operator*(Quaternion& rhs) {
		Quaternion q;

		q.w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
		q.x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
		q.y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
		q.z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;

		return q;
	}

	const Quaternion Quaternion::operator*(const Quaternion& rhs) {
		Quaternion q;

		q.w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
		q.x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
		q.y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
		q.z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;

		return q;
	}

	Quaternion Quaternion::Normalized() {
		float mag = sqrtf(w * w + x * x + y * y + z * z);
		return Quaternion(w / mag, x / mag, y / mag, z / mag);
	}

	Quaternion Quaternion::Conjugate() {
		return Quaternion(w, -x, -y, -z);
	}

	void Quaternion::Normalize()
	{
		float mag = sqrtf(w * w + x * x + y * y + z * z);

		w /= mag;
		x /= mag;
		y /= mag;
		z /= mag;
	}

	glm::vec4 Quaternion::operator*(glm::vec4& rhs)
	{
		return Quaternion::ToMatrix(*this) * rhs;
	}

	const glm::vec4 operator*(glm::vec4& v, const Quaternion& m)
	{
		return Quaternion::ToMatrix((Quaternion)m) * v;
	}

	glm::vec4 operator*(glm::vec4& v, Quaternion& m)
	{
		return Quaternion::ToMatrix(m) * v;
	}

	const glm::vec4 Quaternion::operator*(const glm::vec4& rhs)
	{
		return Quaternion::ToMatrix(*this) * rhs;
	}

	Quaternion Quaternion::AngleAxis(float angle, glm::vec4& axis) {
		glm::vec3 vn = glm::normalize(axis); // axis.Normalized();

		angle *= 0.0174532925f; // To radians!
		angle *= 0.5f;
		float sinAngle = sin(angle);

		return Quaternion(cos(angle), vn.x * sinAngle, vn.y * sinAngle, vn.z * sinAngle);
	}

	Quaternion Quaternion::Euler(float X, float Y, float Z) {
		X *= 0.0174532925f; // To radians!
		Y *= 0.0174532925f; // To radians!
		Z *= 0.0174532925f; // To radians!

		X *= 0.5f;
		Y *= 0.5f;
		Z *= 0.5f;

		float sinx = sinf(X);
		float siny = sinf(Y);
		float sinz = sinf(Z);
		float cosx = cosf(X);
		float cosy = cosf(Y);
		float cosz = cosf(Z);

		Quaternion q;

		q.w = cosx * cosy * cosz + sinx * siny * sinz;
		q.x = sinx * cosy * cosz + cosx * siny * sinz;
		q.y = cosx * siny * cosz - sinx * cosy * sinz;
		q.z = cosx * cosy * sinz - sinx * siny * cosz;

		return q;
	}

	void Quaternion::SetEuler(float X, float Y, float Z) {
		X *= 0.0174532925f; // To radians!
		Y *= 0.0174532925f; // To radians!
		Z *= 0.0174532925f; // To radians!

		X *= 0.5f;
		Y *= 0.5f;
		Z *= 0.5f;

		float sinx = sinf(X);
		float siny = sinf(Y);
		float sinz = sinf(Z);
		float cosx = cosf(X);
		float cosy = cosf(Y);
		float cosz = cosf(Z);

		w = cosx * cosy * cosz + sinx * siny * sinz;
		x = sinx * cosy * cosz + cosx * siny * sinz;
		y = cosx * siny * cosz - sinx * cosy * sinz;
		z = cosx * cosy * sinz - sinx * siny * cosz;
	}

	void Quaternion::ToAngleAxis(float* angle, glm::vec4* axis)
	{
		*angle = acosf(w);
		float sinz = sinf(*angle);

		if (fabsf(sinz) > 1e-4f) {
			sinz = 1.0f / sinz;

			axis->x = x * sinz;
			axis->y = y * sinz;
			axis->z = z * sinz;

			*angle *= 2.0f * 57.2957795f;
			if (*angle > 180.0f)
				*angle = 360.0f - *angle;
		}
		else {
			*angle = 0.0f;
			axis->x = 1.0f;
			axis->y = 0.0f;
			axis->z = 0.0f;
		}
	}

	Quaternion Quaternion::Inverse(Quaternion& rotation) {
		return Quaternion(rotation.w, -1.0f * rotation.x, -1.0f * rotation.y, -1.0f * rotation.z);
	}

	float Quaternion::Dot(Quaternion& a, Quaternion& b) {
		return (a.w * b.w + a.x * b.x * a.y * b.y + a.z * b.z);
	}

	float Quaternion::Dot(Quaternion& b) {
		return (w * b.w + x * b.x * y * b.y + z * b.z);
	}

	float Quaternion::Angle(Quaternion& a, Quaternion& b) {
		float degrees = acosf((b * Quaternion::Inverse(a)).w) * 2.0f * 57.2957795f;
		if (degrees > 180.0f)
			return 360.0f - degrees;
		return degrees;
	}

	Quaternion operator*(float f, Quaternion& m) {
		return Quaternion(m.w * f, m.x * f, m.y * f, m.z * f);
	}

	const Quaternion operator*(float f, const Quaternion& m) {
		return Quaternion(m.w * f, m.x * f, m.y * f, m.z * f);
	}

	Quaternion Quaternion::operator*(float& f) {
		return Quaternion(w * f, x * f, y * f, z * f);
	}

	const Quaternion Quaternion::operator*(const float& f) {
		return Quaternion(w * f, x * f, y * f, z * f);
	}

	Quaternion Quaternion::operator+(Quaternion& rhs) {
		return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
	}

	const Quaternion Quaternion::operator+(const Quaternion& rhs) {
		return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
	}

	Quaternion Quaternion::Lerp(Quaternion& from, Quaternion& to, float t) {
		Quaternion src = from * (1.0f - t);
		Quaternion dst = to * t;

		Quaternion q = src + dst;
		return q;
	}

	Quaternion Quaternion::Slerp(Quaternion& from, Quaternion& to, float t) {
		float cosTheta = Quaternion::Dot(from, to);
		Quaternion temp(to);

		if (cosTheta < 0.0f) {
			cosTheta *= -1.0f;
			temp = temp * -1.0f;
		}

		float theta = acosf(cosTheta);
		float sinTheta = 1.0f / sinf(theta);

		return sinTheta * (
			((Quaternion)(from * sinf(theta * (1.0f - t)))) +
			((Quaternion)(temp * sinf(t * theta)))
			);
	}

#define m00 right.x
#define m01 up.x
#define m02 forward.x
#define m10 right.y
#define m11 up.y
#define m12 forward.y
#define m20 right.z
#define m21 up.z
#define m22 forward.z

	Quaternion Quaternion::LookRotation(glm::vec4& lookAt, glm::vec4& upDirection) {
		glm::vec3 forward = lookAt; glm::vec3 up = upDirection;

		glm::orthonormalize(forward, up);
		glm::vec3 right = glm::cross(up, forward);

		Quaternion ret;
		ret.w = sqrtf(1.0f + m00 + m11 + m22) * 0.5f;
		float w4_recip = 1.0f / (4.0f * ret.w);
		ret.x = (m21 - m12) * w4_recip;
		ret.y = (m02 - m20) * w4_recip;
		ret.z = (m10 - m01) * w4_recip;

		return ret;
	}

	Quaternion Quaternion::LookRotation(glm::vec4& lookAt)
	{
		glm::vec3 up = (glm::vec3)UP;
		glm::vec3 forward = lookAt;
		glm::orthonormalize(forward, up);
		glm::vec3 right = glm::cross(up, forward);

		Quaternion ret;
		ret.w = sqrtf(1.0f + m00 + m11 + m22) * 0.5f;
		float w4_recip = 1.0f / (4.0f * ret.w);
		ret.x = (m21 - m12) * w4_recip;
		ret.y = (m02 - m20) * w4_recip;
		ret.z = (m10 - m01) * w4_recip;

		return ret;
	}

	void Quaternion::SetLookRotation(glm::vec4& lookAt)
	{
		glm::vec3 up = (glm::vec3)UP;
		glm::vec3 forward = lookAt;
		glm::orthonormalize(forward, up);
		glm::vec3 right = glm::cross(up, forward);

		w = sqrtf(1.0f + m00 + m11 + m22) * 0.5f;
		float w4_recip = 1.0f / (4.0f * w);
		x = (m21 - m12) * w4_recip;
		y = (m02 - m20) * w4_recip;
		z = (m10 - m01) * w4_recip;
	}

	void Quaternion::SetLookRotation(glm::vec4& lookAt, glm::vec4& upDirection)
	{
		glm::vec3 forward = lookAt; glm::vec3 up = upDirection;
		glm::orthonormalize(forward, up);
		glm::vec3 right = glm::cross(up, forward);

		w = sqrtf(1.0f + m00 + m11 + m22) * 0.5f;
		float w4_recip = 1.0f / (4.0f * w);
		x = (m21 - m12) * w4_recip;
		y = (m02 - m20) * w4_recip;
		z = (m10 - m01) * w4_recip;
	}
}
#undef m00
#undef m01
#undef m02
#undef m10
#undef m11
#undef m12
#undef m20
#undef m21
#undef m22

