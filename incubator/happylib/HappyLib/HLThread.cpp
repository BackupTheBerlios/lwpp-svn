
#include "HLDefs.h"
#include "HLThread.h"
#include "HLMutex.h"


//// HLThread implementation using OS stuff ////

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
#ifndef _INC_PROCESS
#include <process.h>
#endif


class RealThread
{
public:
	mutable Mutex _mutex;
	volatile int _supposedToQuit;
	volatile int _isRunning;
	volatile HANDLE _handle;
	//HANDLE _parent;
	static void StartThread (void * thread)
	{
		((Thread*)thread)->doEverything();
		((RealThread*)thread)->_mutex.lock();
		((RealThread*)thread)->_isRunning = 0;
		((RealThread*)thread)->_mutex.unlock();
		_endthread();
	}
	virtual void virtualStub() {}	// make sure we get a virtual table
	void construct()
	{
		_supposedToQuit = 0;
		_isRunning = 0;
		_handle = 0;
		//_parent = 0;
	}
	int supposedToQuit() const
	{
		_mutex.lock();
		int ret = _supposedToQuit;
		_mutex.unlock();
		return ret;
	}
	int isRunning() const
	{
		_mutex.lock();
		int ret = _isRunning;
		_mutex.unlock();
		return ret;
	}
	int start()
	{
		_mutex.lock();
		if (_isRunning)	// already started
		{
			_supposedToQuit = 0;
			_mutex.unlock();
			return 1;
		}
		_supposedToQuit = 0;
		_isRunning = 1;
		// [FIXME] _beginthread is an unpredictable piece of crap,
		//         so we keep trying until it succeeds.
		_handle = (void*)-1;
		//_parent = GetCurrentThread();
		while (_handle == (void*)-1) _handle = (void*)_beginthread(StartThread, 0, this);
		_mutex.unlock();
		return 1;
	}
	void lock() const
	{
		_mutex.lock();		// [FIXME] should this have a different mutex?
	}
	void unlock() const
	{
		_mutex.unlock();	// [FIXME] should this have a different mutex?
	}
	void pause() const
	{
		_mutex.lock();
		SuspendThread(_handle);
		_mutex.unlock();
	}
	void unpause() const
	{
		_mutex.lock();
		ResumeThread(_handle);
		_mutex.unlock();
	}
	void quitNonBlocking()
	{
		if (!isRunning()) return;
		_mutex.lock();
		_supposedToQuit = 1;
		_mutex.unlock();
	}
	void quitBlocking()
	{
		if (!isRunning()) return;
		_mutex.lock();
		_supposedToQuit = 1;
		_mutex.unlock();
		while (isRunning()) sleep(100);
	}
	static void sleep(int ms)	// go to sleep for a bit
	{
		Sleep(ms);
	}
};

#else // _WIN32

#error No support for Thread class on this platform yet.

// [FIXME] add support for other platforms

#endif // _WIN32

#include "HLThreadCommon.h"
