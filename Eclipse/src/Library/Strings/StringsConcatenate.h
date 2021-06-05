#pragma once

namespace Eclipse
{
	// Single String
	template <typename T>
	std::string my_strcat(const T& value)
	{
		std::ostringstream oss;
		oss << value;
		return oss.str();
	}

	// Recursive
	template <typename T, typename... Args >
	std::string my_strcat(const T& value, const Args&... args)
	{
		return my_strcat(value) + my_strcat(args...);
	}
}
