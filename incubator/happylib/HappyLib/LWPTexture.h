
#ifndef _LWPTEXTURE_H
#define _LWPTEXTURE_H

#include "LWPRender.h"
#include "HLPoint.h"
#include "HLMatrix3X3.h"

extern "C"
{
	#include <lwtexture.h>
}


//// wrapper for texture access ////

class LWPTextureAccess
{
public:
	Point const		wPos;
	Point const		tPos;
	Vector const	size;
	double const	amp;
	double const	spotSize;

	Color			txGrad;

	int const       axis;
	int const		flags;
	double const	octaves;

	Color			rgb;
	double			alpha;
};

//// wrapper for texture instance ////

class LWPTexture : public LWPRender
{
public:
	// member data
	LWTLayerID layer;
	// static callbacks
	XCALL_(static unsigned int) Flags(void *);
	XCALL_(static double) Evaluate(void *, LWTextureAccess *);
	// virtuals
	virtual unsigned int flags() const;
	virtual double evaluate(LWPTextureAccess *);
	virtual ~LWPTexture() {}
	LWPTexture(char const *, int(*)(long,GlobalFunc*,void*,void*), int(*)(long,GlobalFunc*,void*,void*));
	LWPTexture(LWTLayerID layer) : layer(layer) {}
};

// set-up macros

#define TextureStuff(X,N) \
XCALL_(static void *) Create(void *, void * layer, char const ** err) \
{ if (!(*err = getCallBacks())) return new X((LWTLayerID)layer); return 0; } \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	if (version != LWTEXTURE_VERSION) return AFUNC_BADVERSION; \
	LWTextureHandler * local = (LWTextureHandler *)vlocal; \
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
char const * name() const { return N; } \
X(int(*face)(long,GlobalFunc*,void*,void*)=0) : LWPTexture(N, Handler, face) {}

#define TextureStuffNoConstructor(X,N) \
XCALL_(static void *) Create(void *, void * layer, char const ** err) \
{ if (!(*err = getCallBacks())) return new X((LWTLayerID)layer); return 0; } \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	if (version != LWTEXTURE_VERSION) return AFUNC_BADVERSION; \
	LWTextureHandler * local = (LWTextureHandler *)vlocal; \
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
char const * name() const { return N; }

#endif
