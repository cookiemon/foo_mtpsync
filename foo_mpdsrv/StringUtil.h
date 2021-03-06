#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include "common.h"
#include "Win32Util.h"
#include <string>
#include <limits>
#include <Windows.h>

/**
	* Typedef for unicode/non-unicode strings
	* equivalent of TCHAR for stl strings
	* @author Cookiemon
	*/
typedef std::basic_string<TCHAR> tstring;
	
/**
	* Does a case insensitive comparison of two characters
	* @author Cookiemon
	*/
class CompareTolower
{
public:
	/**
		* Compares the lowercase version of both characters
		* @author Cookiemon
		* @param left A character
		* @param right A character
		* @return true iff lowercase version of both characters are equal,
		*/
	bool operator() (const char left, const char right)
	{
		return tolower(left) == tolower(right);
	}
};

/**
	* Checks if a string starts with a specified sequence
	* Does case insensitive comparison
	* @author Cookiemon
	* @param left String whose start is checked
	* @param right Start sequence that is checked for
	* @return true iff left starts with right as a sequence
	*/
inline bool StrStartsWithLC(const std::string& str, const std::string& with)
{
	if(str.length() < with.length())
		return false;

	return std::equal(with.begin(), with.end(), str.begin(), CompareTolower());
}

inline bool StrStartsWithLC(const char* str, const char* with)
{
	CompareTolower cmp;
	while(str != '\0' && with != '\0' && cmp(*str, *with))
	{
		str += 1;
		with += 1;
	}
	return *with == '\0';
}

inline bool StrStartsWith(const char* str, const char* with)
{
	while(*str != '\0' && *with != '\0' && *str == *with)
	{
		str += 1;
		with += 1;
	}
	return *with == '\0';
}

/**
	* Checks if a given character is space
	* introduced because char is signed by default
	* and isspace casts them internally to unsigned
	* which produces values > 256
	* Also isspace uses locales
	* @author Cookiemon
	* @param c Character to check
	*/
inline bool CharIsSpace(char c)
{
	return c == 0x20 || (0x09 <= c && c <= 0x0D);
}

/**
	* Removes whitespaces from the beginning of a string
	* @author Cookiemon
	* @param str String from which the whitespaces are removed
	*/
inline void TrimLeft(std::string& str)
{
	str.erase(str.begin(), std::find_if_not(str.begin(), str.end(), &CharIsSpace));
}

/**
	* Removes whitespaces from the end of a string
	* @author Cookiemon
	* @param str String from which the whitespaces are removed
	*/
inline void TrimRight(std::string& str)
{
	str.erase(std::find_if_not(str.rbegin(), str.rend(), &CharIsSpace).base(), str.end());
}

/**
	* Removes whitespaces from the beginning and end of a string
	* @author Cookiemon
	* @param str String from which the whitespaces are removed
	*/
inline void Trim(std::string& str)
{
	TrimRight(str);
	TrimLeft(str);
}

/**
	* Converts a wide string to UTF-8
	* @author Cookiemon
	* @param str String to convert
	*/
inline void ToUtf8(const std::wstring& str, std::string& out)
{
	size_t numChars = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	if(numChars == 0)
		throw Win32Exception();
	char* outBuf = new char[numChars];
	numChars = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, outBuf, numChars, NULL, NULL);
	if(numChars == 0)
	{
		delete[] outBuf;
		throw Win32Exception();
	}

	out = outBuf;
	delete[] outBuf;
}

/**
	* Converts a wide string to UTF-8
	* @author Cookiemon
	* @param str String to convert
	* @tparam T type of string to convert to (default: pfc::string8)
	*/
template<typename T>
T ToUtf8(const std::wstring& str)
{
	size_t numChars = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	if(numChars == 0)
		throw Win32Exception();
	std::vector<char> outBuf(numChars);
	numChars = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, &outBuf[0], numChars, NULL, NULL);
	if(numChars == 0)
		throw Win32Exception();

	return T(&outBuf[0]);
}

/**
	* Converts a UTF-8 string to wide char
	* @author Cookiemon
	* @param str String to convert
	*/
inline std::wstring ToWChar(const char* str)
{
	size_t numChars = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	if(numChars == 0)
		throw Win32Exception();
	std::vector<WCHAR> outBuf(numChars);
	numChars = MultiByteToWideChar(CP_UTF8, 0, str, -1, &outBuf[0], numChars);
	if(numChars == 0)
		throw Win32Exception();

	return std::wstring(outBuf.begin(), outBuf.end());
}

/**
	* Converts a UTF-8 string to wide char
	* @author Cookiemon
	* @param str String to convert
	*/
inline std::wstring ToWChar(const std::string& str)
{
	return ToWChar(str.c_str());
}

/**
	* Returns if a name is contained in a path by TODO
	*/
bool PathStartsWith(const std::string& path, std::string with)
{
	if(path.size() > with.size() && !with.empty())
		return StrStartsWithLC(path, with + "\\");
	else
		return StrStartsWithLC(path, with);
}

/**
	* Returns if a name is contained in a path by TODO
	*/
bool PathStartsWith(const pfc::string_base& path, pfc::string8 with)
{
	if(path.get_length() > with.get_length() && !with.is_empty())
		with.add_char('\\');
	return StrStartsWithLC(path, with);
}

pfc::string8 ConcatenateFolder(pfc::string8 base, const pfc::string8& toAdd)
{
	if(!base.is_empty() && !toAdd.is_empty() && !base.ends_with('\\'))
		base.add_char('\\');
	base.add_string(toAdd);
	return base;
}

#endif