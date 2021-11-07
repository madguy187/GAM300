/**********************************************************************************
* \file Vector.h
* \brief Contains the Vector Library for any Vector size.
* \author Fikrul Islami Bin Abdullah, 100% Code Contribution
*
*
* \copyright Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "math.h"
#include <iostream>
#include <array>
#include <stdexcept>
#include <glm.hpp>

namespace Eclipse
{
	template <typename T>
	T Division(T num, T den)
	{
		if (fabs(den) <= EPSILON)
			throw std::runtime_error("Math error: Attempted to divide by Zero");

		return (num / den);
	}

	template <typename T, size_t N>
	class Vector
	{
		std::array<T, N> _data;
	public:
		// READ ONLY
		T x{};
		T y{};
		T z{};
		T w{};

		Vector() :
			Vector<T, N>(T(0))
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 0.0f;
		}

		Vector(T allValues)
		{
			for (size_t i = 0; i < N; ++i)
				_data[i] = allValues;

			x = allValues;
			y = allValues;
			z = allValues;
			w = allValues;
		}

		Vector(T xx, T yy)
		{
			_data[0] = x = xx;
			_data[1] = y = yy;
		}

		Vector(T xx, T yy, T zz)
		{
			_data[0] = x = xx;
			_data[1] = y = yy;
			_data[2] = z = zz;
		}

		Vector(T xx, T yy, T zz, T ww)
		{
			_data[0] = x = xx;
			_data[1] = y = yy;
			_data[2] = z = zz;
			_data[3] = w = ww;
		}

		Vector(const std::array<T, N> data)
		{
			this->_data = data;

			for (size_t i = 0; i < N; ++i)
			{
				switch (i)
				{
				case 0:
					x = this->_data[i];
					break;
				case 1:
					y = this->_data[i];
					break;
				case 2:
					z = this->_data[i];
					break;
				case 3:
					w = this->_data[i];
					break;
				}
			}
		}

		// Assignment Operators
		template <typename U>
		Vector<T, N>& operator=(const U& rhs)
		{
			for (auto i = 0; i < N; ++i)
			{
				this->_data[i] = rhs[i];

				switch (i)
				{
				case 0:
					x = this->_data[i];
					break;
				case 1:
					y = this->_data[i];
					break;
				case 2:
					z = this->_data[i];
					break;
				case 3:
					w = this->_data[i];
					break;
				default:
					break;
				}
			}

			return *this;
		}

		Vector<T, N>& operator+=(const Vector<T, N>& rhs)
		{
			for (size_t i = 0; i < N; ++i)
			{
				this->_data[i] += rhs[i];

				switch (i)
				{
				case 0:
					x = this->_data[i];
					break;
				case 1:
					y = this->_data[i];
					break;
				case 2:
					z = this->_data[i];
					break;
				case 3:
					w = this->_data[i];
					break;
				default:
					break;
				}
			}

			return *this;
		}

		Vector<T, N>& operator+=(const glm::vec2& rhs)
		{
			x = this->_data[0] += rhs.x;
			y = this->_data[1] += rhs.y;

			return *this;
		}

		Vector<T, N>& operator+=(const glm::vec3& rhs)
		{
			x = this->_data[0] += rhs.x;
			y = this->_data[1] += rhs.y;
			z = this->_data[2] += rhs.z;

			return *this;
		}

		Vector<T, N>& operator+=(const glm::vec4& rhs)
		{
			x = this->_data[0] += rhs.x;
			y = this->_data[1] += rhs.y;
			z = this->_data[2] += rhs.z;
			w = this->_data[3] += rhs.w;

			return *this;
		}

		Vector<T, N>& operator-=(const Vector<T, N>& rhs)
		{
			for (size_t i = 0; i < N; ++i)
			{
				this->_data[i] -= rhs[i];

				switch (i)
				{
				case 0:
					x = this->_data[i];
					break;
				case 1:
					y = this->_data[i];
					break;
				case 2:
					z = this->_data[i];
					break;
				case 3:
					w = this->_data[i];
					break;
				default:
					break;
				}
			}

			return *this;
		}

		Vector<T, N>& operator-=(const glm::vec2& rhs)
		{
			x = this->_data[0] -= rhs.x;
			y = this->_data[1] -= rhs.y;

			return *this;
		}

		Vector<T, N>& operator-=(const glm::vec3& rhs)
		{
			x = this->_data[0] -= rhs.x;
			y = this->_data[1] -= rhs.y;
			z = this->_data[2] -= rhs.z;

			return *this;
		}

		Vector<T, N>& operator-=(const glm::vec4& rhs)
		{
			x = this->_data[0] -= rhs.x;
			y = this->_data[1] -= rhs.y;
			z = this->_data[2] -= rhs.z;
			w = this->_data[3] -= rhs.w;

			return *this;
		}

		Vector<T, N>& operator*=(const Vector<T, N>& rhs)
		{
			for (size_t i = 0; i < N; ++i)
			{
				this->_data[i] *= rhs[i];

				switch (i)
				{
				case 0:
					x = this->_data[i];
					break;
				case 1:
					y = this->_data[i];
					break;
				case 2:
					z = this->_data[i];
					break;
				case 3:
					w = this->_data[i];
					break;
				default:
					break;
				}
			}

			return *this;
		}

		Vector<T, N>& operator*=(const glm::vec2& rhs)
		{
			x = this->_data[0] *= rhs.x;
			y = this->_data[1] *= rhs.y;

			return *this;
		}

		Vector<T, N>& operator*=(const glm::vec3& rhs)
		{
			x = this->_data[0] *= rhs.x;
			y = this->_data[1] *= rhs.y;
			z = this->_data[2] *= rhs.z;

			return *this;
		}

		Vector<T, N>& operator*=(const glm::vec4& rhs)
		{
			x = this->_data[0] *= rhs.x;
			y = this->_data[1] *= rhs.y;
			z = this->_data[2] *= rhs.z;
			w = this->_data[3] *= rhs.w;

			return *this;
		}

		template <typename Scalar>
		Vector<T, N>& operator*=(const Scalar factor)
		{
			for (size_t i = 0; i < N; ++i)
			{
				this->_data[i] *= static_cast<T>(factor);

				switch (i)
				{
				case 0:
					x = this->_data[i];
					break;
				case 1:
					y = this->_data[i];
					break;
				case 2:
					z = this->_data[i];
					break;
				case 3:
					w = this->_data[i];
					break;
				default:
					break;

				}
			}

			return *this;
		}

		template <typename Scalar>
		Vector<T, N>& operator/=(const Scalar factor)
		{
			try {
				for (size_t i = 0; i < N; ++i)
				{
					_data[i] = Division<T>(_data[i], static_cast<T>(factor));

					switch (i)
					{
					case 0:
						x = this->_data[i];
						break;
					case 1:
						y = this->_data[i];
						break;
					case 2:
						z = this->_data[i];
						break;
					case 3:
						w = this->_data[i];
						break;
					default:
						break;
					}
				}
			}
			catch (std::runtime_error& e) {
				std::cout << e.what() << std::endl;
				throw - 1;
			}

			return *this;
		}

		// Negate Operator
		Vector<T, N> operator-() const
		{
			Vector<T, N> temp{ *this };
			T NegValue = -1;

			for (size_t i = 0; i < N; ++i)
				temp._data[i] *= NegValue;

			return temp;
		}

		T& operator[](size_t pos)
		{
			try {
				if (this != nullptr)
					return this->_data.at(pos);
			}
			catch (std::out_of_range const&) {
				std::cout << "Accessing Out of Vector Size!" << std::endl;
			}

			return this->_data.at(pos);
		}

		T operator[](size_t pos) const
		{
			try {
				if (this != nullptr)
					return this->_data.at(pos);
			}
			catch (std::out_of_range const&) {
				std::cout << "Accessing Out of Vector Size!" << std::endl;
			}

			return this->_data.at(pos);
		}

		void setX(T value)
		{
			try {
				_data.at(0) = value;
				x = value;
			}
			catch (std::out_of_range const&) {
				std::cout << "Setting Index Out of Vector Size!" << std::endl;
			}

		}

		void setY(T value)
		{
			try {
				_data.at(1) = value;
				y = value;
			}
			catch (std::out_of_range const&) {
				std::cout << "Setting Index Out of Vector Size!" << std::endl;
			}
		}

		void setZ(T value)
		{
			try {
				_data.at(2) = value;
				z = value;
			}
			catch (std::out_of_range const&) {
				std::cout << "Setting Index Out of Vector Size!" << std::endl;
			}
		}

		void setW(T value)
		{
			try {
				_data.at(3) = value;
				w = value;
			}
			catch (std::out_of_range const&) {
				std::cout << "Setting Index Out of Vector Size!" << std::endl;
			}
		}

		T getX() const
		{
			return this->operator[](0);
		}

		T getY() const
		{
			return this->operator[](1);
		}

		T getZ() const
		{
			return this->operator[](2);
		}

		T getW() const
		{
			return this->operator[](3);
		}

		const T* data() const
		{
			return _data.data();
		}

		glm::vec2 ConvertToGlmVec2Type() const
		{
			glm::vec2 temp{ this->_data[0], this->_data[1] };
			return temp;
		}

		glm::vec3 ConvertToGlmVec3Type() const
		{
			glm::vec3 temp{ this->_data[0], this->_data[1],  this->_data[2] };
			return temp;
		}

		glm::vec4 ConvertToGlmVec4Type() const
		{
			glm::vec4 temp{ this->_data[0], this->_data[1], this->_data[2], this->_data[3] };
			return temp;
		}
	};

	// Binary operators
	template <typename T, size_t N>
	Vector<T, N> operator+(const Vector<T, N>& lhs, const Vector<T, N>& rhs)
	{
		Vector<T, N> temp{ lhs };
		temp += rhs;
		return temp;
	}

	template <typename T, size_t N>
	Vector<T, N> operator+(const Vector<T, N>& lhs, const glm::vec2& rhs)
	{
		Vector<T, N> temp{ lhs };
		temp += rhs;
		return temp;
	}

	template <typename T, size_t N>
	Vector<T, N> operator+(const glm::vec2& lhs, const Vector<T, N>& rhs)
	{
		Vector<T, N> temp{ rhs };
		temp += lhs;
		return temp;
	}

	template <typename T, size_t N>
	Vector<T, N> operator+(const Vector<T, N>& lhs, const glm::vec3& rhs)
	{
		Vector<T, N> temp{ lhs };
		temp += rhs;
		return temp;
	}

	template <typename T, size_t N>
	Vector<T, N> operator+(const glm::vec3& lhs, const Vector<T, N>& rhs)
	{
		Vector<T, N> temp{ rhs };
		temp += lhs;
		return temp;
	}

	template <typename T, size_t N>
	Vector<T, N> operator+(const Vector<T, N>& lhs, const glm::vec4& rhs)
	{
		Vector<T, N> temp{ lhs };
		temp += rhs;
		return temp;
	}

	template <typename T, size_t N>
	Vector<T, N> operator+(const glm::vec4& lhs, const Vector<T, N>& rhs)
	{
		Vector<T, N> temp{ rhs };
		temp += lhs;
		return temp;
	}

	template <typename T, size_t N>
	Vector<T, N> operator-(const Vector<T, N>& lhs, const Vector<T, N>& rhs)
	{
		Vector<T, N> temp{ lhs };
		temp -= rhs;
		return temp;
	}

	template <typename T, size_t N>
	Vector<T, N> operator-(const Vector<T, N>& lhs, glm::vec2& rhs)
	{
		Vector<T, N> temp{ lhs };
		temp -= rhs;
		return temp;
	}

	template <typename T, size_t N>
	Vector<T, N> operator-(const glm::vec2& lhs, const Vector<T, N>& rhs)
	{
		Vector<T, N> temp{ rhs };
		temp -= lhs;
		return temp;
	}

	template <typename T, size_t N>
	Vector<T, N> operator-(const Vector<T, N>& lhs, glm::vec3& rhs)
	{
		Vector<T, N> temp{ lhs };
		temp -= rhs;
		return temp;
	}

	template <typename T, size_t N>
	Vector<T, N> operator-(const glm::vec3& lhs, const Vector<T, N>& rhs)
	{
		Vector<T, N> temp{ rhs };
		temp -= lhs;
		return temp;
	}

	template <typename T, size_t N>
	Vector<T, N> operator-(const Vector<T, N>& lhs, glm::vec4& rhs)
	{
		Vector<T, N> temp{ lhs };
		temp -= rhs;
		return temp;
	}

	template <typename T, size_t N>
	Vector<T, N> operator-(const glm::vec4& lhs, const Vector<T, N>& rhs)
	{
		Vector<T, N> temp{ rhs };
		temp -= lhs;
		return temp;
	}

	template <typename T, size_t N, typename Scalar>
	Vector<T, N> operator*(const Vector<T, N>& lhs, Scalar value)
	{
		Vector<T, N> temp{ lhs };
		temp *= static_cast<T>(value);
		return temp;
	}

	template <typename T, size_t N, typename Scalar>
	Vector<T, N> operator*(Scalar value, const Vector<T, N>& rhs)
	{
		Vector<T, N> temp{ rhs };
		temp *= static_cast<T>(value);
		return temp;
	}

	template <typename T, size_t N, typename Scalar>
	Vector<T, N> operator/(const Vector<T, N>& lhs, Scalar value)
	{
		Vector<T, N> temp{ lhs };
		temp /= static_cast<T>(value);
		return temp;
	}

	template <typename T, size_t N>
	inline void VectorNormalize(Vector<T, N>& pResult, const Vector<T, N>& pVec0)
	{
		T magnitude = VectorLength(pVec0);
		Vector<T, N> temp{ pVec0 };

		for (size_t i = 0; i < N; ++i)
		{
			if (i == 3)
				continue;

			temp[i] = Division(temp[i], magnitude);
		}

		pResult = temp;
	}

	template <typename T, size_t N>
	T	VectorLength(const Vector<T, N>& pVec0)
	{
		T result = 0.0f;

		for (size_t i = 0; i < N; ++i)
			result += (pVec0[i] * pVec0[i]);

		return sqrtf(result);
	}

	template <typename T, size_t N>
	T	VectorSquareLength(const Vector<T, N>& pVec0)
	{
		T result = 0.0f;

		for (size_t i = 0; i < N; ++i)
			result += (pVec0[i] * pVec0[i]);

		return result;
	}

	template <typename T, size_t N>
	T	VectorDistance(const Vector<T, N>& pVec0, const Vector<T, N>& pVec1)
	{
		T result = 0.0f;

		for (size_t i = 0; i < N; ++i)
			result += (pVec1[i] - pVec0[i]) * (pVec1[i] - pVec0[i]);

		return sqrtf(result);
	}

	template <typename T, size_t N>
	T	VectorSquareDistance(const Vector<T, N>& pVec0, const Vector<T, N>& pVec1)
	{
		T result = 0.0f;

		for (size_t i = 0; i < N; ++i)
			result += (pVec1[i] - pVec0[i]) * (pVec1[i] - pVec0[i]);

		return result;
	}

	template <typename T, size_t N>
	T	VectorDotProduct(const Vector<T, N>& pVec0, const Vector<T, N>& pVec1)
	{
		T result = 0.0f;

		for (size_t i = 0; i < N; ++i)
			result += (pVec0[i] * pVec1[i]);

		return result;
	}

	template <typename T>
	T	Vector2DCrossProductMag(const Vector<T, 2>& pVec0, const Vector<T, 2>& pVec1)
	{
		return (pVec0.getX() * pVec1.getY()) - (pVec0.getY() * pVec1.getX());
	}

	template <typename T>
	T	Vector3DCrossProductMag(const Vector<T, 3>& pVec0, const Vector<T, 3>& pVec1)
	{
		T result1 = pVec0[1] * pVec1[2] - pVec0[2] * pVec1[1];
		T result2 = pVec0[2] * pVec1[0] - pVec0[0] * pVec1[2];
		T result3 = pVec0[0] * pVec1[1] - pVec0[1] * pVec1[0];

		return sqrtf(result1 * result1 + result2 * result2 + result3 * result3);
	}

	float Vector3DMag(const ECVec3& pVec)
	{
		return std::fabsf(sqrtf(pVec[0] + pVec[1] + pVec[2]));
	}

	template <typename T, size_t N>
	std::ostream& operator<<(std::ostream& stream, const Vector<T, N>& rhs)
	{
		for (size_t i = 0; i < N; ++i)
		{
			if (i == (N - 1))
				stream << rhs[i];
			else
				stream << rhs[i] << " ";
		}

		return stream;
	}

	template <typename T, size_t N>
	bool operator==(const Vector<T, N>& lhs, const Vector<T, N>& rhs)
	{
		size_t counter = 0;

		for (size_t i = 0; i < N; ++i)
		{
			if (lhs[i] == rhs[i])
				counter++;
		}

		if (counter == N)
			return true;

		return false;
	}

	template <typename T, size_t N>
	inline bool operator!=(const Vector<T, N>& lhs, const Vector<T, N>& rhs)
	{
		return !(lhs == rhs);
	}

	typedef Vector<float, 2> ECVec2;
	typedef Vector<float, 3> ECVec3;
	typedef Vector<float, 4> ECVec4;
}
