
#ifndef _LWPMESHTOOL_H
#define _LWPMESHTOOL_H

#include "LWPPlugin.h"
#include "LWPSerialization.h"

extern "C"
{
	#include <lwmodtool.h>
}


//// wrapper for MeshEditOp ////

class LWPMeshEditOp
{
public:
	char const * done(EDError, int selm);
	int done(EDError err);
	int pointCount(EltOpLayer, int mode);
	int polyCount(EltOpLayer, int mode);
	EDError pointScan(EDPointScanFunc *, void *, EltOpLayer);
	EDError polyScan(EDPolyScanFunc *, void *, EltOpLayer);
	EDPointInfo * pointInfo(LWPntID);
	EDPolygonInfo * polyInfo(LWPolID);
	int polyNormal(LWPolID, Vector &);
	LWPntID addPoint(Point const &);
	LWPolID addFace(char const * surf, int pointCount, LWPntID const * point);
	LWPolID addCurve(char const * surf, int pointCount, LWPntID const * point, int flags);
	EDError addQuad(LWPntID, LWPntID, LWPntID, LWPntID);
	EDError addTri(LWPntID, LWPntID, LWPntID);
	EDError addPatch(int nr, int nc, int lr, int lc, EDBoundCv * r0, EDBoundCv * r1, EDBoundCv * c0, EDBoundCv * c1);
	EDError remPoint(LWPntID);
	EDError remPoly(LWPolID);
	EDError pntMove(LWPntID, Point const &);
	EDError polSurf(LWPolID, char const *);
	EDError polPnts(LWPolID, int, LWPntID const *);
	EDError polFlag(LWPolID, int mask, int value);
	EDError polTag(LWPolID, LWID, char const *);
	EDError pntVMap(LWPntID, LWID, char const *, int, float *);
	LWPolID addPoly(LWID type, LWPolID templt, char const * surf, int pointCount, LWPntID const * point);
	LWPntID addIPnt(Point const & pos, int pointCount, LWPntID const * point, double const * weight);
	EDError initUV(float * uv);
	void * pointVSet(void *, LWID, char const *);
	int pointVGet(LWPntID, float *);
	char const * polyTag(LWPolID, LWID);
	EDError pntSelect(LWPntID, int);
	EDError polSelect(LWPolID, int);
	int pointVPGet(LWPntID, LWPolID, float *);
	int pointVEval(LWPntID, LWPolID, float *);
	EDError pntVPMap(LWPntID, LWPolID, LWID, char const *, int, float *);
	// [FIXME] add more goodies as the need arises
};


//// wrapper for modeler mesh tool plugin instance ////

class LWPMeshTool : public LWPPlugin
{
public:
	// static callbacks
	XCALL_(static void) Done(void *);
	XCALL_(static void) Draw(void *, LWWireDrawAccess *);
	XCALL_(static char const *) Help(void *, LWToolEvent *);
	XCALL_(static int) Dirty(void *);
	XCALL_(static int) Count(void *, LWToolEvent *);
	XCALL_(static int) Handle(void *, LWToolEvent *, int, LWDVector);
	XCALL_(static int) Start(void *, LWToolEvent *);
	XCALL_(static int) Adjust(void *, LWToolEvent *, int);
	XCALL_(static int) Down(void *, LWToolEvent *);
	XCALL_(static void) Move(void *, LWToolEvent *);
	XCALL_(static void) Up(void *, LWToolEvent *);
	XCALL_(static void) Event(void *, int);
	XCALL_(static LWXPanelID) Panel(void *);
	XCALL_(static int) Test(void *);
	XCALL_(static LWError) Build(void *, MeshEditOp *);
	XCALL_(static void) End(void *, int);
	XCALL_(static void *) Get(void *, unsigned long);
	XCALL_(static LWXPRefreshCode) Set(void *, unsigned long, void *);
	static char const * HClass() { return LWMESHEDITTOOL_CLASS; }
	static char const * IClass() { return LWMESHEDITTOOL_CLASS; }
	// acquisitions
	static LWStateQueryFuncs * stateQueryFuncs;
	// constructors
	LWPMeshTool();
	LWPMeshTool(char const * nm, int (*handler)(long,GlobalFunc*,void*,void*));
	// functions
	static LWError getCallBacks();
	static void freeCallBacks();
	// virtuals
	virtual ~LWPMeshTool() {}
	// [FIXME] fix const correctness in all these
	virtual void draw(LWWireDrawAccess *);
	virtual char const * help(LWToolEvent *);
	virtual int dirty();
	virtual int count(LWToolEvent *);
	virtual int handle(LWToolEvent *, int, Vector &);
	virtual int start(LWToolEvent *);
	virtual int adjust(LWToolEvent *, int);
	virtual int down(LWToolEvent *);
	virtual void move(LWToolEvent *);
	virtual void up(LWToolEvent *);
	virtual void drop();
	virtual void reset();
	virtual void activate();
	virtual LWXPanelID panel();
	virtual int test();
	virtual LWError build(LWPMeshEditOp *);
	virtual void end(int);
	virtual void * get(unsigned long);
	virtual LWXPRefreshCode set(unsigned long, void *);
};

// set-up macros

#define MeshToolStuff(X,N) \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	LWMeshEditTool * local = (LWMeshEditTool *)vlocal; \
	if (version != LWMESHEDITTOOL_VERSION) return AFUNC_BADVERSION; \
	globalFunc = global; \
	if (getCallBacks()) return AFUNC_BADGLOBAL; \
	local->instance		= new X(0); \
	local->tool->done	= Done; \
	local->tool->draw	= Draw; \
	local->tool->help	= Help; \
	local->tool->dirty	= Dirty; \
	local->tool->count	= Count; \
	local->tool->handle	= Handle; \
	local->tool->start	= Start; \
	local->tool->adjust	= Adjust; \
	local->tool->down	= Down; \
	local->tool->move	= Move; \
	local->tool->up		= Up; \
	local->tool->event	= Event; \
	local->tool->panel	= Panel; \
	local->test			= Test; \
	local->build		= Build; \
	local->end			= End; \
	return AFUNC_OK; \
} \
X() : LWPMeshTool(N, Handler) {} \
static char const * Name() { return N; } \
char const * name() const { return Name(); }


//// wrapper for modeler mesh edit plugin instance ////

class LWPMeshEdit : public LWPPlugin
{
public:
	// static callbacks
	static char const * HClass() { return LWMESHEDIT_CLASS; }
	static char const * IClass() { return LWMESHEDIT_CLASS; }
	// acquisitions
	static LWStateQueryFuncs * stateQueryFuncs;
	// constructors
	LWPMeshEdit();
	LWPMeshEdit(char const * nm, int (*handler)(long,GlobalFunc*,void*,void*));
	// functions
	static LWError getCallBacks();
	static void freeCallBacks();
	// virtuals
	virtual int build(LWPMeshEditOp * local);
	virtual int flags() const;
	virtual int pointBufSize() const;
	virtual int polygonBufSize() const;	
};

// set-up macros

#define MeshEditStuff(X,N) \
XCALL_(static int) Handler(long version, GlobalFunc * global, void * vlocal, void * serverData) \
{ \
	if (version != LWMESHEDIT_VERSION) return AFUNC_BADVERSION; \
	globalFunc = global; \
	if (getCallBacks()) return AFUNC_BADGLOBAL; \
	X x(0); \
	LWPMeshEditOp * op = (LWPMeshEditOp *)((MeshEditBegin *)vlocal)(x.pointBufSize(), x.polygonBufSize(), x.flags()); \
	if (!op) return AFUNC_BADLOCAL;	\
	return x.build(op); \
} \
X(int) : LWPMeshEdit() {} \
X() : LWPMeshEdit(N, Handler) {} \
static char const * Name() { return N; } \
char const * name() const { return Name(); }

#endif
