
#ifndef _LWPGENERIC_H
#define _LWPGENERIC_H

#include "LWPPlugin.h"

extern "C"
{
	#include <lwgeneric.h>
}


//// wrapper for generic plugin instance ////

class LWPGeneric : public LWPPlugin
{
public:
	LWPGeneric(char const * nm, int(*handler)(long,GlobalFunc*,void*,void*))
	{
		pluginRegister(LWLAYOUTGENERIC_CLASS, nm, handler);	// register the plugin
	}
};

#define GenericStuff(X,N) \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	globalFunc = global; \
	unsigned long sysid = (unsigned long)globalFunc(LWSYSTEMID_GLOBAL, GFUSE_TRANSIENT); \
	long ver = (sysid & LWINF_BUILD) == 429 ? 2 : 4; \
	if (version != ver) return AFUNC_BADVERSION; \
	if (getCallBacks()) return AFUNC_BADGLOBAL; \
	Activate((LWLayoutGeneric*)vlocal); \
	freeCallBacks(); \
	return AFUNC_OK; \
} \
X() : LWPGeneric(N, Handler) {} \
char const * name() const { return N; }

#endif
