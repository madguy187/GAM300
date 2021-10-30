#pragma once

#include <random>

namespace Eclipse
{
	class Random
	{
	public:
		template <typename T>
		static typename std::enable_if<std::is_integral<T>::value, T>::type Rand(const T& min, const T& max)
		{
			return min < max ? std::uniform_int_distribution<T>(min, max)(mt_rand) : min;
		}

		template <typename T>
		static typename std::enable_if<std::is_floating_point<T>::value, T>::type Rand(const T& min, const T& max)
		{
			return min < max ? std::uniform_real_distribution<T>(min, max)(mt_rand) : min;
		}
	private:
		static std::mt19937 mt_rand;
	};
}
