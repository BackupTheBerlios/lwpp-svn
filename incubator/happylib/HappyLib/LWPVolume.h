
#ifndef _LWPVOLUME_H
#define _LWPVOLUME_H

#include "LWPRender.h"
#include "LWPScene.h"
#include "HLPoint.h"
#include "HLColor.h"

extern "C"
{
	#include <lwvolume.h>
}


// [NOTE] LW computes the final color as: color + background * exp(-opacity)

// [NOTE] Let's say you have a function unitTrans(x) that defines at each point x
//        the unit transparency of the volume at x, i.e. the transparency of
//        1 meter of a homogeneous medium like the medium at the point x,
//        then this is how you figure out what to use for opacity:
//        opacity = -ln(transparency) = -Integral( ln(unitTrans(x)) dx, x = start..end )
//        (really, this is for each channel of the opacity color)
//        Use Maple or whatever to evaluate the integral.
//        Hint: for a homogeneous medium, it's just -ln(unitTrans) * length
//        Have fun figuring out how to divy up the interval for a ray marcher :)

// [NOTE] Do NOT set length to 0 for surface samples.  Setting length to 0 works
//        in the camera view, but results in black splotches in raytraced reflections.
//        Use some small non-zero value instead, like (farClip - nearClip) * 0.001.
//        Accordingly, set start to (farClip - length) instead of farClip * 0.9999.
//        Oh heck, I'll put an addSurfaceSample() in LWPVolumeAccess for you, and make
//        it return the start value it used.  Happy now?

// [NOTE] When in the raytracing context, the polygon that the ray hit (i.e. the surface
//        that is at farClip) is NOT visible to rayCast, rayShade, rayTrace or illuminate.
//        So if you want to get information about that surface, tough luck.  All in all,
//        in the raytracing context, rayTrace is absolutely useless (because of artifacts
//        that will occur), illuminate should work just as well as it does from a shader,
//        and the rest are only useful if you're tracing in generally the opposite
//        direction from the ray's direction (i.e. the dot product of the trace direction
//        with the ray direction is negative)  This is important to consider if you want
//        to have your plugin appear in reflections.


//// wrapper for volume sample ////

class LWPVolumeSample
{
public:
	double start;		// distance from origin to start of sample
	double length;		// length of sample
	Color opacity;		// LW opacity value, i.e. -ln(transparency)
	Color color;		// absolute color contribution
	LWPVolumeSample(double start, double length, Color const & opacity, Color const & color)
	: start(start), length(length), opacity(opacity), color(color) {}
	operator LWVolumeSample * () { return (LWVolumeSample *)this; }
	operator LWVolumeSample const * () const { return (LWVolumeSample const *)this; }
};


//// wrapper for volume access ////

class LWPVolumeAccess
{
public:
	void const * const	ray;
	int const			flags;
	LWPItem const		source;

	Point const			origin;
	Vector const		dir;
	Color const			rayColor;

	double const		farClip;
	double const		nearClip;
	double const		oDist;
	double const		frustum;	// size of the pixel at a distance of 1.0

	void				(*_addSample)(void const * const ray, LWVolumeSample const * smp);
	double				(*_getOpacity)(void const * const ray, double dist, Color & opacity);

	LWIlluminateFunc * const	illuminate;
	LWRayTraceFunc * const		rayTrace;
	LWRayCastFunc * const		rayCast;
	LWRayShadeFunc * const		rayShade;

	void addSample(LWPVolumeSample const & smp) const { _addSample(ray, smp); }
	double addSurfaceSample(Color const & opacity, Color const & color) const;
	double addSurfaceSample(Color const & color) const
	{ return addSurfaceSample(Opaque(), color); }
	void addSurfaceSample(double start, Color const & opacity, Color const & color) const;
	void addSurfaceSample(double start, Color const & color) const
	{ addSurfaceSample(start, Opaque(), color); }
	double addShadow() const { return addSurfaceSample(Color::Black()); }
	double getOpacity(double dist, Color & opacity) const { return _getOpacity(ray, dist, opacity); }
	int needOpacity() const { return !!(flags & LWVEF_OPACITY); } // [NOTE] rarely ever false
	int needColor() const { return !!(flags & LWVEF_COLOR); }
	int isRaytracing() const { return !!(flags & LWVEF_RAYTRACE); }
	int hitsBackdrop() const { return farClip > 0.99e+012; }

	static Color Opaque() { return Color::White() * 1000000.0; }

	operator LWVolumeAccess * () { return (LWVolumeAccess *)this; }
	operator LWVolumeAccess const * () const { return (LWVolumeAccess const *)this; }
};


//// wrapper for volume instance ////

class LWPVolume : public LWPRender
{
public:
	// member data
	void * context;
	// static callbacks
	XCALL_(static unsigned int) Flags(void *);
	XCALL_(static double) Evaluate(void *, LWVolumeAccess *);
	static char const * HClass() { return LWVOLUMETRIC_HCLASS; }
	static char const * IClass() { return LWVOLUMETRIC_ICLASS; }
	// virtuals
	virtual unsigned int flags() const;
	virtual double evaluate(LWPVolumeAccess *);			// color for e.g. a view ray
	virtual double evaluateFilter(LWPVolumeAccess *);	// filter color for e.g. a shadow ray
	virtual ~LWPVolume() {}
	LWPVolume(char const *, int(*)(long,GlobalFunc*,void*,void*), int(*)(long,GlobalFunc*,void*,void*));
	LWPVolume(void * context) : context(context) {}
	// helpers
	static Color foggify(Vector const & dir, double dist, Color const & color);
	static Color fogFade(Vector const & dir, double dist, Color const & color);
};

// set-up macros

#define VolumeStuff(X,N) \
XCALL_(static void *) Create(void *, void * context, char const ** err) \
{ if (!(*err = getCallBacks())) return new X(context); return 0; } \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	if (version != LWVOLUMETRIC_VERSION) return AFUNC_BADVERSION; \
	LWVolumetricHandler * local = (LWVolumetricHandler *)vlocal; \
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
X(int(*face)(long,GlobalFunc*,void*,void*)=0) : LWPVolume(N, Handler, face) {}

#define VolumeStuffNoConstructor(X,N) \
XCALL_(static void *) Create(void *, void * context, char const ** err) \
{ if (!(*err = getCallBacks())) return new X(context); return 0; } \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	if (version != LWVOLUMETRIC_VERSION) return AFUNC_BADVERSION; \
	LWVolumetricHandler * local = (LWVolumetricHandler *)vlocal; \
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
