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

class EventObject
{
private:
	HANDLE hand;

	EventObject(const EventObject&);
	EventObject& operator=(const EventObject&);
public:
	EventObject(bool manualReset = true)
	{
		hand = CreateEvent(NULL, manualReset, FALSE, NULL);
		if(hand == NULL)
			throw Win32Exception();
	}
	~EventObject()
	{
		CloseHandle(hand);
	}
	bool Wait(DWORD timeout = INFINITE)
	{
		DWORD retVal = WaitForSingleObject(hand, timeout);
		if(retVal == WAIT_FAILED)
			throw Win32Exception();
		return retVal == WAIT_OBJECT_0;
	}
	void Set()
	{
		if(!SetEvent(hand))
			throw Win32Exception();
	}
	void Reset()
	{
		if(!ResetEvent(hand))
			throw Win32Exception();
	}
};

#endif