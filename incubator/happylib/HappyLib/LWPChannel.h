
#ifndef _LWPCHANNEL_H
#define _LWPCHANNEL_H

#include "LWPRender.h"
#include "HLPoint.h"
#include "HLMatrix3X3.h"

extern "C"
{
	#include <lwchannel.h>
}


//// wrapper for channel access ////

class LWPChannelAccess
{
public:
	LWChannelID		chan;
	LWFrame			frame;
	LWTime			time;
	double			value;
	void			(*getChannel)(LWChannelID, LWTime, double &);
	void			(*setChannel)(LWChannelID, double);
	const char *	(*channelName)(LWChannelID);
	// functions
	void set(double val) const { setChannel(chan, val); }
	// [FIXME] add more goodies
};


//// wrapper for channel instance ////

class LWPChannel : public LWPRender
{
public:
	// static callbacks
	XCALL_(static unsigned int) Flags(void *);
	XCALL_(static void) Evaluate(void *, LWChannelAccess const *);
	static char const * HClass() { return LWCHANNEL_HCLASS; }
	static char const * IClass() { return LWCHANNEL_ICLASS; }
	// virtuals
	virtual unsigned int flags() const;
	virtual void evaluate(LWPChannelAccess *);
	virtual ~LWPChannel() {}
	LWPChannel(char const *, int(*)(long,GlobalFunc*,void*,void*), int(*)(long,GlobalFunc*,void*,void*));
	LWPChannel() {}
};

// set-up macros

#define ChannelStuff(X,N) \
XCALL_(static void *) Create(void *, void *, char const ** err) \
{ if (!(*err = getCallBacks())) return new X(); return 0; } \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	if (version != LWCHANNEL_VERSION) return AFUNC_BADVERSION; \
	LWChannelHandler * local = (LWChannelHandler *)vlocal; \
	local->inst->create 	= Create; \
	local->inst->destroy	= Destroy; \
	local->inst->load		= Load; \
	local->inst->save		= Save; \
	local->inst->copy		= Copy; \
	local->inst->descln 	= DescLn; \
	if (local->item) \
	{ \
		local->item->useItems	= UseItems; \
		local->item->changeID	= ChangeID; \
	} \
	local->evaluate 		= Evaluate; \
	local->flags			= Flags; \
	globalFunc = global; \
	return AFUNC_OK; \
} \
static char const * Name() { return N; } \
char const * name() const { return Name(); } \
X(int(*face)(long,GlobalFunc*,void*,void*)) : LWPChannel(N, Handler, face) {}

#define ChannelStuffNoConstructor(X,N) \
XCALL_(static void *) Create(void *, void *, char const ** err) \
{ if (!(*err = getCallBacks())) return new X(); return 0; } \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	if (version != LWCHANNEL_VERSION) return AFUNC_BADVERSION; \
	LWChannelHandler * local = (LWChannelHandler *)vlocal; \
	local->inst->create 	= Create; \
	local->inst->destroy	= Destroy; \
	local->inst->load		= Load; \
	local->inst->save		= Save; \
	local->inst->copy		= Copy; \
	local->inst->descln 	= DescLn; \
	if (local->item) \
	{ \
		local->item->useItems	= UseItems; \
		local->item->changeID	= ChangeID; \
	} \
	local->evaluate 		= Evaluate; \
	local->flags			= Flags; \
	globalFunc = global; \
	return AFUNC_OK; \
} \
static char const * Name() { return N; } \
char const * name() const { return Name(); }

#endif
