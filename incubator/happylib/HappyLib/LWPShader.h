
#ifndef _LWPSHADER_H
#define _LWPSHADER_H

#include "LWPRender.h"
#include "LWPShaderAccess.h"


//// wrapper for shader instance ////

class LWPShader : public LWPRender
{
public:
	// member data
	LWSurfaceID surface;
	// static callbacks
	XCALL_(static unsigned int) Flags(void *);
	XCALL_(static void) Evaluate(void *, LWShaderAccess *);
	static char const * HClass() { return LWSHADER_HCLASS; }
	static char const * IClass() { return LWSHADER_ICLASS; }
	// virtuals
	virtual unsigned int flags() const;
	virtual void evaluate(LWPShaderAccess *);
	virtual void evaluatePreview(LWPShaderAccess *);
	virtual void evaluateFilter(LWPShaderAccess *);
	virtual ~LWPShader() {}
	LWPShader(char const *, int(*)(long,GlobalFunc*,void*,void*), int(*)(long,GlobalFunc*,void*,void*));
	LWPShader(LWSurfaceID surface) : surface(surface) {}
};

// set-up macros

#define ShaderStuff(X,N) \
XCALL_(static void *) Create(void *, void * surf, char const ** err) \
{ if (!(*err = getCallBacks())) return new X((LWSurfaceID)surf); return 0; } \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	if (version != LWSHADER_VERSION) return AFUNC_BADVERSION; \
	LWShaderHandler * local = (LWShaderHandler *)vlocal; \
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
X(int(*face)(long,GlobalFunc*,void*,void*)=0) : LWPShader(N, Handler, face) {}

#define ShaderStuffNoConstructor(X,N) \
XCALL_(static void *) Create(void *, void * surf, char const ** err) \
{ if (!(*err = getCallBacks())) return new X((LWSurfaceID)surf); return 0; } \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	if (version != LWSHADER_VERSION) return AFUNC_BADVERSION; \
	LWShaderHandler * local = (LWShaderHandler *)vlocal; \
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
