
#ifndef _LWPIMAGEFILTER_H
#define _LWPIMAGEFILTER_H

#include "LWPRender.h"
#include "HLPoint.h"

extern "C"
{
	#include <lwfilter.h>
}


//// wrapper for filter access ////

class LWPFilterAccess
{
public:
	int const width;
	int const height;
	LWFrame const frame;
    LWTime const start;
	LWTime const end;
    float * (* const getLine)(int type, int y);
    void (* const setRGB)(int x, int y, Colorf const & rgb);
    void (* const setAlpha)(int x, int y, float alpha);
    LWMonitor * const monitor;
	// handy functions
	void getRGBA(Array2<Colorf> & rgb, Array2<float> & a) const
	{
		rgb.init(width, height);
		a.init(width, height);
		for (int y = 0; y < height; y ++)
		{
			float const * rPtr = getLine(LWBUF_RED, y);
			float const * gPtr = getLine(LWBUF_GREEN, y);
			float const * bPtr = getLine(LWBUF_BLUE, y);
			float const * aPtr = getLine(LWBUF_ALPHA, y);
			for (int x = 0; x < width; x ++)
			{
				rgb[x][y] = Colorf(rPtr[x], gPtr[x], bPtr[x]);
				a[x][y] = aPtr[x];
			}
		}
	}
	void setRGBA(Array2<Colorf> & rgb, Array2<float> & a) const
	{
		for (int y = 0; y < height; y ++)
		{
			for (int x = 0; x < width; x ++)
			{			
				setRGB(x, y, rgb[x][y]);
				setAlpha(x, y, a[x][y]);
			}
		}
	}
	void getFloat(int type, Array2<float> & f) const
	{
		f.init(width, height);
		for (int y = 0; y < height; y ++)
		{
			float const * fPtr = getLine(type, y);
			for (int x = 0; x < width; x ++)
			{
				f[x][y] = fPtr[x];
			}
		}
	}
};


//// wrapper for filter instance ////

class LWPImageFilter : public LWPRender
{
public:
	// member data
	int context;
	// static callbacks
	XCALL_(static unsigned int) Flags(void *);
    XCALL_(static void) Process(void *, LWFilterAccess const *);
	static char const * HClass() { return LWIMAGEFILTER_HCLASS; }
	static char const * IClass() { return LWIMAGEFILTER_ICLASS; }
	// virtuals
	virtual unsigned int flags() const;
	virtual void process(LWPFilterAccess const *);
	virtual ~LWPImageFilter() {}
	LWPImageFilter(char const *, int(*)(long,GlobalFunc*,void*,void*), int(*)(long,GlobalFunc*,void*,void*));
	LWPImageFilter(int context) : context(context) {}
	// non-virtuals
	int isPreProcess() const { return !!(context & LWFCF_PREPROCESS); }	// if RGBA only is available
};

// set-up macros

#define ImageFilterStuff(X,N) \
XCALL_(static void *) Create(void *, void * context, char const ** err) \
{ if (!(*err = getCallBacks())) return new X((int)context); return 0; } \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	if (version != LWIMAGEFILTER_VERSION) return AFUNC_BADVERSION; \
	LWImageFilterHandler * local = (LWImageFilterHandler *)vlocal; \
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
   	local->process 			= Process; \
	local->flags			= Flags; \
	globalFunc = global; \
	return AFUNC_OK; \
} \
static char const * Name() { return N; } \
char const * name() const { return Name(); } \
X(int(*face)(long,GlobalFunc*,void*,void*)=0) : LWPImageFilter(N, Handler, face) {}

#define ImageFilterStuffNoConstructor(X,N) \
XCALL_(static void *) Create(void *, int context, char const ** err) \
{ if (!(*err = getCallBacks())) return new X(context); return 0; } \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	if (version != LWIMAGEFILTER_VERSION) return AFUNC_BADVERSION; \
	LWImageFilterHandler * local = (LWImageFilterHandler *)vlocal; \
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
   	local->process 			= Process; \
	local->flags			= Flags; \
	globalFunc = global; \
	return AFUNC_OK; \
} \
static char const * Name() { return N; } \
char const * name() const { return Name(); }

#endif
