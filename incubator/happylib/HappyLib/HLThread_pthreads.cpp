
#include "HLThread.h"
#include "HLMutex.h"
#include HLIOSTREAM_H


//// Thread implementation using Posix ////

// [FIXME][LATER] add support for other platforms

#include <pthread.h>	// [NOTE] requires pthreads.lib
inline void usleep(int us) { Sleep(us / 1000); }


class RealThread
{
public:
	mutable Mutex _mutex;
	int _supposedToQuit;
	int _isRunning;
	pthread_t _id;
	static void * StartThread(void * thread)
	{
		((Thread*)thread)->doEverything();
		((RealThread*)thread)->_mutex.lock();
		((RealThread*)thread)->_isRunning = 0;
		((RealThread*)thread)->_mutex.unlock();
		return 0;
	}
	virtual void virtualStub() {}	// make sure we get a virtual table
	void construct()
	{
		_supposedToQuit = 0;
		_isRunning = 0;
		_id = 0;
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
		int errVal = pthread_create(&_id, 0, StartThread, this);
		_mutex.unlock();
		return !!errVal;
	}
	void lock() const
	{
		_mutex.lock();		// [FIXME] should this have a different mutex?
	}
	void unlock() const
	{
		_mutex.unlock();	// [FIXME] should this have a different mutex?
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
		quitNonBlocking();
		pthread_join(_id, 0);
	}
	static void sleep(int ms)	// go to sleep for a bit
	{
		usleep(ms*1000);
	}
};


//// Thread implementation (platform-independant stuff) ////

void Thread::doEverything() {}

Thread::Thread() { ((RealThread*)this)->construct(); }

Thread::~Thread() { if (isRunning()) quitBlocking(); }

int Thread::isRunning() const { return ((RealThread*)this)->isRunning(); }

int Thread::supposedToQuit() const { return ((RealThread*)this)->supposedToQuit(); }

int Thread::start() { return ((RealThread*)this)->start(); }

void Thread::lock() const { ((RealThread*)this)->lock(); }

void Thread::unlock() const { ((RealThread*)this)->unlock(); }

void Thread::quitNonBlocking() { ((RealThread*)this)->quitNonBlocking(); }

void Thread::quitBlocking() { ((RealThread*)this)->quitBlocking(); }

void Thread::sleep(int ms) { RealThread::sleep(ms); }
