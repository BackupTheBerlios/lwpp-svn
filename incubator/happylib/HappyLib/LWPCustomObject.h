
#ifndef _LWPCUSTOMOBJECT_H
#define _LWPCUSTOMOBJECT_H

#include "LWPRender.h"
#include "LWPScene.h"
#include "HLPoint.h"
#include "HLColor.h"

extern "C"
{
#include <lwcustobj.h>
}


//// wrapper for LWCustomObjAccess ////

#ifdef LW6
class LWPCustomObjectAccess
{
public:
	// data
	int view;
	int flags;
	void * _dispData;
	void (*_setColor)(void *, Colorf const &);
	void (*_setPattern)(void *, int);
	void (*_point)(void *, Point const &, int);
	void (*_line)(void *, Point const &, Point const &, int);
	void (*_triangle)(void *, Point const &, Point const &, Point const &, int);
	void (*_circle)(void *, Point const &, double, int);
	void (*_text)(void *, Point const &, const char *, int);
	// functions
	void setColor(Colorf const & rgb) const { _setColor(_dispData, rgb); }
	void setPattern(int lpat) const { _setPattern(_dispData, lpat); }
	void point(Point const & pos, int coordSys) const { _point(_dispData, pos, coordSys); }
	void line(Point const & pos0, Point const & pos1, int coordSys) const
	{ _line(_dispData, pos0, pos1, coordSys); }
	void triangle(Point const & pos0, Point const & pos1, Point const & pos2, int coordSys) const
	{ _triangle(_dispData, pos0, pos1, pos2, coordSys); }
	void circle(Point const & center, double radius, int coordSys) const
	{ _circle(_dispData, center, radius, coordSys); }
	void text(Point const & pos, const char * str, int coordSys) const
	{ _text(_dispData, pos, str, coordSys); }
	operator LWCustomObjAccess * () { return (LWCustomObjAccess *)this; }
	operator LWCustomObjAccess const * () const { return (LWCustomObjAccess const *)this; }
};
#else
class LWPCustomObjectAccess
{
public:
	// data
	int view;
	int flags;
	void * _dispData;
	void (*_setColor)(void *, float[4]);
	void (*_setPattern)(void *, int);
	void (*_setTexture)(void *, int, unsigned char *);
	void (*_setUVs)(void *, double[2], double[2], double[2], double[2]);	
	void (*_point)(void *, Point const &, int);
	void (*_line)(void *, Point const &, Point const &, int);
	void (*_triangle)(void *, Point const &, Point const &, Point const &, int);
	void (*_quad)(void *, Point const &, Point const &, Point const &, Point const &, int);
	void (*_circle)(void *, Point const &, double, int);
	void (*_text)(void *, Point const &, const char *, int, int);
	Point viewPos;
	Vector viewDir;
	// functions
	void setColor(Colorf const & rgb, float alpha) const
	{ float rgba[4]; rgb.get(rgba); rgba[3] = alpha; _setColor(_dispData, rgba); }
	void setPattern(int lpat) const { _setPattern(_dispData, lpat); }
	void point(Point const & pos, int coordSys) const { _point(_dispData, pos, coordSys); }
	void line(Point const & pos0, Point const & pos1, int coordSys) const
	{ _line(_dispData, pos0, pos1, coordSys); }
	void triangle(Point const & pos0, Point const & pos1, Point const & pos2, int coordSys) const
	{ _triangle(_dispData, pos0, pos1, pos2, coordSys); }
	void circle(Point const & center, double radius, int coordSys) const
	{ _circle(_dispData, center, radius, coordSys); }
	void text(Point const & pos, const char * str, int just, int coordSys) const
	{ _text(_dispData, pos, str, just, coordSys); }
	operator LWCustomObjAccess * () { return (LWCustomObjAccess *)this; }
	operator LWCustomObjAccess const * () const { return (LWCustomObjAccess const *)this; }
};

#endif

//// wrapper for volume instance ////

class LWPCustomObject : public LWPRender
{
public:
	// member data
	LWPItem item;
	// static callbacks
	XCALL_(static unsigned int) Flags(void *);
	XCALL_(static void) Evaluate(void *, LWCustomObjAccess const *);
	static char const * HClass() { return LWCUSTOMOBJ_HCLASS; }
	static char const * IClass() { return LWCUSTOMOBJ_ICLASS; }
	// virtuals
	virtual unsigned int flags() const { return 0; }
	virtual void evaluate(LWPCustomObjectAccess const *) {}
	virtual ~LWPCustomObject() {}
	LWPCustomObject(char const *, int(*)(long,GlobalFunc*,void*,void*), int(*)(long,GlobalFunc*,void*,void*));
	LWPCustomObject(LWItemID item) : item(item) {}
};

// set-up macros

#define CustomObjectStuff(X,N) \
XCALL_(static void *) Create(void *, void * item, char const ** err) \
{ if (!(*err = getCallBacks())) return new X((LWItemID)item); return 0; } \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	if (version != LWCUSTOMOBJ_VERSION) return AFUNC_BADVERSION; \
	LWCustomObjHandler * local = (LWCustomObjHandler *)vlocal; \
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
X(int(*face)(long,GlobalFunc*,void*,void*)=0) : LWPCustomObject(N, Handler, face) {}

#define CustomObjectStuffNoConstructor(X,N) \
XCALL_(static void *) Create(void *, void * item, char const ** err) \
{ if (!(*err = getCallBacks())) return new X((LWItemID)item); return 0; } \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	if (version != LWCUSTOMOBJ_VERSION) return AFUNC_BADVERSION; \
	LWCustomObjHandler * local = (LWCustomObjHandler *)vlocal; \
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
