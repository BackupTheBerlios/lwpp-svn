
#ifndef _LWPPIXELFILTER_H
#define _LWPPIXELFILTER_H

#include "LWPRender.h"
#include "HLColor.h"

extern "C"
{
	#include <lwfilter.h>
}


//// wrapper for pixel access ////

class LWPPixelAccess
{
public:
	double const				sx;
	double const				sy;
	void (* const getVal)(int, int, float *);
	void (* const _setRGBA)(const float[4]);
	void (* const setVal)(int, int, float const *);
	LWIlluminateFunc * const	illuminate;
	LWRayTraceFunc * const		rayTrace;
	LWRayCastFunc * const		rayCast;
	LWRayShadeFunc * const		rayShade;
	// handy functions
	float getFloat(int type) const
	{
		float f;
		getVal(type, 1, &f);
		return f;
	}
	void setFloat(int type, float f) const
	{
		setVal(type, 1, &f);
	}
	Colorf getRGB() const
	{
		Colorf rgb;
		getVal(LWBUF_RED, 3, rgb);
		return rgb;
	}
	void setRGB(Colorf const & rgb) const
	{
		setVal(LWBUF_RED, 3, rgb);
	}
	void getRGBA(Colorf & rgb, float & a) const
	{
		getVal(LWBUF_RED, 3, rgb);
		getVal(LWBUF_ALPHA, 1, &a);
	}
	void setRGBA(Colorf const & rgb, float const a) const
	{
		setVal(LWBUF_RED, 3, rgb);
		setVal(LWBUF_ALPHA, 1, &a);
	}
};


//// wrapper for pixel instance ////

class LWPPixelFilter : public LWPRender
{
public:
	// member data
	int context;
	// static callbacks
	XCALL_(static unsigned int) Flags(void *);
    XCALL_(static void) Evaluate(void *, LWPixelAccess const *);
	static char const * HClass() { return LWPIXELFILTER_HCLASS; }
	static char const * IClass() { return LWPIXELFILTER_ICLASS; }
	// virtuals
	virtual unsigned int flags() const;
	virtual void process(LWPPixelAccess const *);
	virtual ~LWPPixelFilter() {}
	LWPPixelFilter(char const *, int(*)(long,GlobalFunc*,void*,void*), int(*)(long,GlobalFunc*,void*,void*));
	LWPPixelFilter(int context) : context(context) {}
	// non-virtuals
	int isPreProcess() const { return !!(context & LWFCF_PREPROCESS); }	// if RGBA only is available
};


// set-up macros

#define PixelFilterStuff(X,N) \
XCALL_(static void *) Create(void *, void * context, char const ** err) \
{ if (!(*err = getCallBacks())) return new X((int)context); return 0; } \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	if (version != LWPIXELFILTER_VERSION) return AFUNC_BADVERSION; \
	LWPixelFilterHandler * local = (LWPixelFilterHandler *)vlocal; \
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
X(int(*face)(long,GlobalFunc*,void*,void*)=0) : LWPPixelFilter(N, Handler, face) {}

#define PixelFilterStuffNoConstructor(X,N) \
XCALL_(static void *) Create(void *, void * context, char const ** err) \
{ if (!(*err = getCallBacks())) return new X((int)context); return 0; } \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	if (version != LWPIXELFILTER_VERSION) return AFUNC_BADVERSION; \
	LWPixelFilterHandler * local = (LWPixelFilterHandler *)vlocal; \
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
