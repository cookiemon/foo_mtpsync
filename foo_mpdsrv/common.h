#ifndef COMMON_H
#define COMMON_H

//#pragma warning(disable: 4505)
#pragma warning(push, 2)
#pragma warning(disable: 4555 4350 4711)
// Microsoft, I hate you. I really really do.
// Breaks std::numeric_limits<>::max()
#define NOMINMAX
#include <winsdkver.h>
#define _WIN32_WINNT _WIN32_WINNT_WINXP
#include <SDKDDKVer.h>
#include <ATLHelpers.h>
#include <foobar2000.h>
#include <fstream>
#include <string>
#include <cctype>
#include <functional>
#include <algorithm>

// For the sake of good!
#undef NULL
#define NULL nullptr

namespace foo_mtpsync
{
	/**
	 * Name of this plugin
	 */
	extern const char* PLUGINNAME;
	/**
	 * Name of this plugin as wide char
	 */
	extern const wchar_t* PLUGINNAMEW;

	#ifdef FOO_MPDSRV_EXPORTS
	#define FOO_MPDSRV_API __declspec(dllexport)
	#else
	#define FOO_MPDSRV_API __declspec(dllimport)
	#endif
}

#pragma warning(pop)

#endif