
#include "HLMutex.h"


//// RealMutex implementation using OS stuff ////

#ifdef _WIN32

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define NOSERVICE
#define NOMCX
#define NOIME
#define NOSOUND
#define NOCOMM
#define NOKANJI
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#include <windows.h>		// [NOTE] the Behemoth requires M$-specific language extensions
#endif


class RealMutex
{
public:
	HANDLE hLock;
	void create()
	{
		hLock = CreateMutex(0, FALSE, 0);
	}
	void destroy()
	{
		ReleaseMutex(hLock);
		CloseHandle(hLock);
	}
	int lockIfConvenient()
	{
		return (WaitForSingleObject(hLock, 0) != WAIT_TIMEOUT);
	}
	void lock()
	{
		WaitForSingleObject(hLock, INFINITE);
	}
	void unlock()
	{
		ReleaseMutex(hLock);
	}
};

#else // _WIN32

#error No support for Mutex class on this platform yet.

// [FIXME] add support for other platforms

#endif // _WIN32

#include "HLMutexCommon.h"
