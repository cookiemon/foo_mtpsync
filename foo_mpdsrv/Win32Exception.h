#ifndef WIN32EXCEPTION_H
#define WIN32EXCEPTION_H

#include <Windows.h>
#include <stdexcept>

class Win32Exception : public std::runtime_error
{
private:
	DWORD ErrNum = 0;
	const char* buff = nullptr;
public:
	Win32Exception()
	{
		ErrNum = GetLastError();
	}
	Win32Exception(DWORD err)
		: ErrNum(err)
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
			&buff, 0,
			NULL);
		return buff;
	}
};

#endif