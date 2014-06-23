#ifndef WIN32EXCEPTION_H
#define WIN32EXCEPTION_H

#include <atlbase.h>
#include <stdexcept>
#include <Windows.h>

class Win32Exception : public std::exception
{
private:
	const DWORD ErrNum;
	char* buff;

private:
	Win32Exception& operator=(const Win32Exception&);
public:
	Win32Exception()
		: ErrNum(GetLastError()),
		buff(nullptr)
	{
	}
	explicit Win32Exception(DWORD err)
		: ErrNum(err),
		buff(nullptr)
	{
	}
	Win32Exception(const Win32Exception& other)
		: ErrNum(other.ErrNum), buff(nullptr)
	{
	}

	~Win32Exception()
	{
		if(buff != nullptr)
			delete buff;
	}

	const char* what() throw()
	{
		if(buff == nullptr)
			FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			NULL,
			ErrNum, 0,
			reinterpret_cast<char*>(&buff), 0,
			NULL);
		return buff;
	}
};

#endif