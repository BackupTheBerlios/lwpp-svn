
#include "HLMutex.h"
#include "LWPRender.h"


//// RealMutex implementation using LW stuff ////

class RealMutex
{
public:
	LWMTUtilID mutex;
	volatile int locked;
	void create()
	{
		// must be safe for globals
		locked = 0;
		if (LWPRender::mutexFuncs) mutex = LWPRender::mutexFuncs->create();
		else mutex = 0;
	}
	void destroy()
	{
		if (LWPRender::mutexFuncs && mutex) LWPRender::mutexFuncs->destroy(mutex);
	}
	int lockIfConvenient()
	{
		if (!LWPRender::mutexFuncs) return 1;
		if (!mutex) create();	// handle statics
		int ret;
		LWPRender::mutexFuncs->lock(mutex, 0);
		if (locked) ret = 0;
		else
		{
			locked = ret = 1;
			LWPRender::mutexFuncs->lock(mutex, 1);
		}
		LWPRender::mutexFuncs->unlock(mutex, 0);
		return ret;
	}
	void lock()
	{
		if (!LWPRender::mutexFuncs) return;
		if (!mutex) create();	// handle statics
		LWPRender::mutexFuncs->lock(mutex, 0);
		locked ++;
		LWPRender::mutexFuncs->unlock(mutex, 0);
		LWPRender::mutexFuncs->lock(mutex, 1);
	}
	void unlock()
	{
		if (!LWPRender::mutexFuncs) return;
		if (!mutex) create();	// handle statics
		LWPRender::mutexFuncs->unlock(mutex, 1);
		LWPRender::mutexFuncs->lock(mutex, 0);
		locked --;
		LWPRender::mutexFuncs->unlock(mutex, 0);
	}
};

#include "HLMutexCommon.h"
