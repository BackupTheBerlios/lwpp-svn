
#ifndef _LWPMASTER_H
#define _LWPMASTER_H

#include "LWPRender.h"

extern "C"
{
	#include <lwmaster.h>
}


//// wrapper for master access ////

class LWPMasterAccess
{
public:
	int const eventCode;
	void * const eventData;
	void * const data;
	LWCommandCode (* const _lookup) (void *, char const * cmdName);
	int (* const _execute) (void *, LWCommandCode cmd, int argc,
		DynaValue * const argv, DynaValue * result);
	int (* const _evaluate) (void *, char const * command);

	operator LWMasterAccess * () { return (LWMasterAccess *)this; }
	operator LWMasterAccess const * () const { return (LWMasterAccess const *)this; }
};


//// wrapper for master instance ////

class LWPMaster : public LWPRender
{
public:
	// static callbacks
	XCALL_(static unsigned int) Flags(void *);
	XCALL_(static double) Event(void *, LWMasterAccess const *);
	static char const * HClass() { return LWMASTER_HCLASS; }
	static char const * IClass() { return LWMASTER_ICLASS; }
	// virtuals
	virtual unsigned int flags() const;
	virtual void event(LWPMasterAccess const *);
	virtual ~LWPMaster() {}
	LWPMaster(char const *, int(*)(long,GlobalFunc*,void*,void*), int(*)(long,GlobalFunc*,void*,void*));
	LWPMaster(void * context) {}
	// data
	int type;	// [NOTE] defaults to LWMAST_SCENE
};

// set-up macros

#define MasterStuff(X,N) \
XCALL_(static void *) Create(void *, void * context, char const ** err) \
{ if (!(*err = getCallBacks())) return new X(context); return 0; } \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	if (version != LWMASTER_VERSION) return AFUNC_BADVERSION; \
	LWMasterHandler * local = (LWMasterHandler *)vlocal; \
	local->type				= LWMAST_SCENE; \
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
	local->event			= Event; \
	local->flags			= Flags; \
	globalFunc = global; \
	return AFUNC_OK; \
} \
static char const * Name() { return N; } \
char const * name() const { return Name(); } \
X(int(*face)(long,GlobalFunc*,void*,void*)=0) : LWPMaster(N, Handler, face) {}

#define MasterStuffNoConstructor(X,N) \
XCALL_(static void *) Create(void *, void * context, char const ** err) \
{ if (!(*err = getCallBacks())) return new X(context); return 0; } \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	if (version != LWMASTER_VERSION) return AFUNC_BADVERSION; \
	LWMasterHandler * local = (LWMasterHandler *)vlocal; \
	local->type				= LWMAST_SCENE; \
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
	local->event	 		= Event; \
	local->flags			= Flags; \
	globalFunc = global; \
	return AFUNC_OK; \
} \
static char const * Name() { return N; } \
char const * name() const { return Name(); }

#endif
