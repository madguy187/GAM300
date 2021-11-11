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

	static wchar_t* charToWChar(const char* text)
	{
		const size_t size = strlen(text) + 1;
		wchar_t* wText = new wchar_t[size];
		mbstowcs(wText, text, size);
		return wText;
	}

	static constexpr unsigned int str2int(const char* str, int h = 0)
	{
		return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
	}
}
