
#ifndef _LWPDISPLACEMENT_H
#define _LWPDISPLACEMENT_H

#include "LWPRender.h"
#include "LWPScene.h"
#include "HLPoint.h"
#include "HLMatrix3X3.h"

extern "C"
{
	#include <lwdisplce.h>
}


//// wrapper for displacement access ////

class LWPDisplacementAccess
{
public:
	Point const		oPos;
	Point			source;
	LWPntID 		point;
	LWMeshInfo *	info;
};


//// wrapper for displacement instance ////

class LWPDisplacement : public LWPRender
{
public:
	// member data
	LWPItem item;
	// static callbacks
	XCALL_(static unsigned int) Flags(void *);
	XCALL_(static void) Evaluate(void *, LWDisplacementAccess *);
	static char const * HClass() { return LWDISPLACEMENT_HCLASS; }
	static char const * IClass() { return LWDISPLACEMENT_ICLASS; }
	// virtuals
	virtual unsigned int flags() const;
	virtual void evaluate(LWPDisplacementAccess *);
	virtual void evaluatePreview(LWPDisplacementAccess *);
	virtual ~LWPDisplacement() {}
	LWPDisplacement(char const *, int(*)(long,GlobalFunc*,void*,void*), int(*)(long,GlobalFunc*,void*,void*));
	LWPDisplacement(LWItemID item) : item(item) {}
};

// set-up macros

#define DisplacementStuff(X,N) \
XCALL_(static void *) Create(void *, void * item, char const ** err) \
{ if (!(*err = getCallBacks())) return new X((LWItemID)item); return 0; } \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	if (version != LWDISPLACEMENT_VERSION) return AFUNC_BADVERSION; \
	LWDisplacementHandler * local = (LWDisplacementHandler *)vlocal; \
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
	local->rend->init		= Init; \
	local->rend->cleanup	= CleanUp; \
	local->rend->newTime	= NewTime; \
	local->evaluate 		= Evaluate; \
	local->flags			= Flags; \
	globalFunc = global; \
	return AFUNC_OK; \
} \
static char const * Name() { return N; } \
char const * name() const { return Name(); } \
X(int(*face)(long,GlobalFunc*,void*,void*)=0) : LWPDisplacement(N, Handler, face) {}

#define DisplacementStuffNoConstructor(X,N) \
XCALL_(static void *) Create(void *, void * item, char const ** err) \
{ if (!(*err = getCallBacks())) return new X((LWItemID)item); return 0; } \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	if (version != LWDISPLACEMENT_VERSION) return AFUNC_BADVERSION; \
	LWDisplacementHandler * local = (LWDisplacementHandler *)vlocal; \
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
	local->rend->init		= Init; \
	local->rend->cleanup	= CleanUp; \
	local->rend->newTime	= NewTime; \
	local->evaluate 		= Evaluate; \
	local->flags			= Flags; \
	globalFunc = global; \
	return AFUNC_OK; \
} \
static char const * Name() { return N; } \
char const * name() const { return Name(); }

#endif

