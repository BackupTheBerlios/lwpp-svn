
#include "LWPMeshTool.h"

// acquisitions
LWStateQueryFuncs * LWPMeshTool::stateQueryFuncs = 0;
LWStateQueryFuncs * LWPMeshEdit::stateQueryFuncs = 0;


//// LWPMeshEditOp implementation ////

char const * LWPMeshEditOp::done(EDError err, int selm)
{
	((MeshEditOp*)this)->done(((MeshEditOp*)this)->state, err, selm);
	switch (err)
	{
	case EDERR_NOMEMORY:	return "Out of memory.";
	case EDERR_BADLAYER:	return "Bad Layer.";
	case EDERR_BADSURF:		return "Bad Surface.";
	case EDERR_USERABORT:	return "User aborted operation.";
	case EDERR_BADVMAP:		return "Bad VMap.";
	case EDERR_BADARGS:		return "Failed.";
	}
	return 0;
}

int LWPMeshEditOp::done(EDError err)
{
	((MeshEditOp*)this)->done(((MeshEditOp*)this)->state, err, 0);
	return err;
}

int LWPMeshEditOp::pointCount(EltOpLayer layer, int mode)
{
	return ((MeshEditOp*)this)->pointCount(((MeshEditOp*)this)->state, layer, mode);
}

int LWPMeshEditOp::polyCount(EltOpLayer layer, int mode)
{
	return ((MeshEditOp*)this)->polyCount(((MeshEditOp*)this)->state, layer, mode);
}

EDError LWPMeshEditOp::pointScan(EDPointScanFunc * scan, void * data, EltOpLayer layer)
{
	return ((MeshEditOp*)this)->pointScan(((MeshEditOp*)this)->state, scan, data, layer);
}

EDError LWPMeshEditOp::polyScan(EDPolyScanFunc * scan, void * data, EltOpLayer layer)
{
	return ((MeshEditOp*)this)->polyScan(((MeshEditOp*)this)->state, scan, data, layer);
}

EDPointInfo * LWPMeshEditOp::pointInfo(LWPntID pnt)
{
	return ((MeshEditOp*)this)->pointInfo(((MeshEditOp*)this)->state, pnt);
}

EDPolygonInfo * LWPMeshEditOp::polyInfo(LWPolID pol)
{
	return ((MeshEditOp*)this)->polyInfo(((MeshEditOp*)this)->state, pol);
}

int LWPMeshEditOp::polyNormal(LWPolID pol, Vector & n)
{
	return ((MeshEditOp*)this)->polyNormal(((MeshEditOp*)this)->state, pol, n);
}

LWPntID LWPMeshEditOp::addPoint(Point const & pos)
{
	return ((MeshEditOp*)this)->addPoint(((MeshEditOp*)this)->state, (double *)(double const *)pos);
}

LWPolID LWPMeshEditOp::addFace(char const * surf, int pointCount, LWPntID const * point)
{
	return ((MeshEditOp*)this)->addFace(((MeshEditOp*)this)->state, surf, pointCount, point);
}

LWPolID LWPMeshEditOp::addCurve(char const * surf, int pointCount, LWPntID const * point, int flags)
{
	return ((MeshEditOp*)this)->addCurve(((MeshEditOp*)this)->state, surf, pointCount, point, flags);
}

EDError LWPMeshEditOp::addQuad(LWPntID p0, LWPntID p1, LWPntID p2, LWPntID p3)
{
	return ((MeshEditOp*)this)->addQuad(((MeshEditOp*)this)->state, p0, p1, p2, p3);
}

EDError LWPMeshEditOp::addTri(LWPntID p0, LWPntID p1, LWPntID p2)
{
	return ((MeshEditOp*)this)->addTri(((MeshEditOp*)this)->state, p0, p1, p2);
}

EDError LWPMeshEditOp::addPatch(int nr, int nc, int lr, int lc, EDBoundCv * r0, EDBoundCv * r1, EDBoundCv * c0, EDBoundCv * c1)
{
	return ((MeshEditOp*)this)->addPatch(((MeshEditOp*)this)->state, nr, nc, lr, lc, r0, r1, c0, c1);
}

EDError LWPMeshEditOp::remPoint(LWPntID pnt)
{
	return ((MeshEditOp*)this)->remPoint(((MeshEditOp*)this)->state, pnt);
}

EDError LWPMeshEditOp::remPoly(LWPolID pol)
{
	return ((MeshEditOp*)this)->remPoly(((MeshEditOp*)this)->state, pol);
}

EDError LWPMeshEditOp::pntMove(LWPntID pnt, Point const & pos)
{
	return ((MeshEditOp*)this)->pntMove(((MeshEditOp*)this)->state, pnt, pos);
}

EDError LWPMeshEditOp::polSurf(LWPolID pol, char const * surf)
{
	return ((MeshEditOp*)this)->polSurf(((MeshEditOp*)this)->state, pol, surf);
}

EDError LWPMeshEditOp::polPnts(LWPolID pol, int pointCount, LWPntID const * point)
{
	return ((MeshEditOp*)this)->polPnts(((MeshEditOp*)this)->state, pol, pointCount, point);
}

EDError LWPMeshEditOp::polFlag(LWPolID pol, int mask, int value)
{
	return ((MeshEditOp*)this)->polFlag(((MeshEditOp*)this)->state, pol, mask, value);
}

EDError LWPMeshEditOp::polTag(LWPolID pol, LWID type, char const * tag)
{
	return ((MeshEditOp*)this)->polTag(((MeshEditOp*)this)->state, pol, type, tag);
}

EDError LWPMeshEditOp::pntVMap(LWPntID pol, LWID type, char const * name, int valueCount, float * value)
{
	return ((MeshEditOp*)this)->pntVMap(((MeshEditOp*)this)->state, pol, type, name, valueCount, value);
}

LWPolID LWPMeshEditOp::addPoly(LWID type, LWPolID templt, char const * surf, int pointCount, LWPntID const * point)
{
	return ((MeshEditOp*)this)->addPoly(((MeshEditOp*)this)->state, type, templt, surf, pointCount, point);
}

LWPntID LWPMeshEditOp::addIPnt(Point const & pos, int pointCount, LWPntID const * point, double const * weight)
{
	return ((MeshEditOp*)this)->addIPnt(((MeshEditOp*)this)->state, (double *)(double const *)pos, pointCount, point, weight);
}

EDError LWPMeshEditOp::initUV(float * uv)
{
	return ((MeshEditOp*)this)->initUV(((MeshEditOp*)this)->state, uv);
}

void * LWPMeshEditOp::pointVSet(void * vmap, LWID type, char const * name)
{
	return ((MeshEditOp*)this)->pointVSet(((MeshEditOp*)this)->state, vmap, type, name);
}

int LWPMeshEditOp::pointVGet(LWPntID pnt, float * value)
{
	return ((MeshEditOp*)this)->pointVGet(((MeshEditOp*)this)->state, pnt, value);
}

char const * LWPMeshEditOp::polyTag(LWPolID pol, LWID tag)
{
	return ((MeshEditOp*)this)->polyTag(((MeshEditOp*)this)->state, pol, tag);
}

EDError LWPMeshEditOp::pntSelect(LWPntID pnt, int select)
{
	return ((MeshEditOp*)this)->pntSelect(((MeshEditOp*)this)->state, pnt, select);
}

EDError LWPMeshEditOp::polSelect(LWPolID pol, int select)
{
	return ((MeshEditOp*)this)->polSelect(((MeshEditOp*)this)->state, pol, select);
}

int LWPMeshEditOp::pointVPGet(LWPntID pnt, LWPolID pol, float * value)
{
	return ((MeshEditOp*)this)->pointVPGet(((MeshEditOp*)this)->state, pnt, pol, value);
}

int LWPMeshEditOp::pointVEval(LWPntID pnt, LWPolID pol, float * value)
{
	return ((MeshEditOp*)this)->pointVEval(((MeshEditOp*)this)->state, pnt, pol, value);
}

EDError LWPMeshEditOp::pntVPMap(LWPntID pnt, LWPolID pol, LWID type, char const * name, int valueCount, float * value)
{
	return ((MeshEditOp*)this)->pntVPMap(((MeshEditOp*)this)->state, pnt, pol, type, name, valueCount, value);
}


//// LWPMeshTool implementation ////

XCALL_(void) LWPMeshTool::Done(void * inst)
{
	delete (LWPMeshTool *)inst;
}

XCALL_(void) LWPMeshTool::Draw(void * inst, LWWireDrawAccess * access)
{
	((LWPMeshTool *)inst)->draw(access);
}

XCALL_(char const *) LWPMeshTool::Help(void * inst, LWToolEvent * event)
{
	return ((LWPMeshTool *)inst)->help(event);
}

XCALL_(int) LWPMeshTool::Dirty(void * inst)
{
	return ((LWPMeshTool *)inst)->dirty();
}

XCALL_(int) LWPMeshTool::Count(void * inst, LWToolEvent * event)
{
	return ((LWPMeshTool *)inst)->count(event);
}

XCALL_(int) LWPMeshTool::Handle(void * inst, LWToolEvent * event, int i, LWDVector pos)
{
	return ((LWPMeshTool *)inst)->handle(event, i, *(Vector*)(void*)pos);
}

XCALL_(int) LWPMeshTool::Start(void * inst, LWToolEvent * event)
{
	return ((LWPMeshTool *)inst)->start(event);
}

XCALL_(int) LWPMeshTool::Adjust(void * inst, LWToolEvent * event, int i)
{
	return ((LWPMeshTool *)inst)->adjust(event, i);
}

XCALL_(int) LWPMeshTool::Down(void * inst, LWToolEvent * event)
{
	return ((LWPMeshTool *)inst)->down(event);
}

XCALL_(void) LWPMeshTool::Move(void * inst, LWToolEvent * event)
{
	((LWPMeshTool *)inst)->move(event);
}

XCALL_(void) LWPMeshTool::Up(void * inst, LWToolEvent * event)
{
	((LWPMeshTool *)inst)->up(event);
}

XCALL_(void) LWPMeshTool::Event(void * inst, int code)
{
	switch (code)
	{
	case LWT_EVENT_DROP:
		((LWPMeshTool *)inst)->drop(); break;
	case LWT_EVENT_RESET:
		((LWPMeshTool *)inst)->reset(); break;
	case LWT_EVENT_ACTIVATE:
		((LWPMeshTool *)inst)->activate(); break;
	}
}

XCALL_(LWXPanelID) LWPMeshTool::Panel(void * inst)
{
	return ((LWPMeshTool *)inst)->panel();
}

XCALL_(int) LWPMeshTool::Test(void * inst)
{
	return ((LWPMeshTool *)inst)->test();
}

XCALL_(LWError) LWPMeshTool::Build(void * inst, MeshEditOp * op)
{
	return ((LWPMeshTool *)inst)->build((LWPMeshEditOp*)op);
}

XCALL_(void) LWPMeshTool::End(void * inst, int keep)
{
	((LWPMeshTool *)inst)->end(keep);
}

XCALL_(void *) LWPMeshTool::Get(void * inst, unsigned long vid)
{
	return ((LWPMeshTool *)inst)->get(vid);
}

XCALL_(LWXPRefreshCode) LWPMeshTool::Set(void * inst, unsigned long vid, void * vvalue)
{
	return ((LWPMeshTool *)inst)->set(vid, vvalue);
}

LWPMeshTool::LWPMeshTool()
{
}

LWPMeshTool::LWPMeshTool(char const * nm, int (*handler)(long,GlobalFunc*,void*,void*))
{
	pluginRegister(LWMESHEDITTOOL_CLASS, nm, handler);	// register the plugin
}

LWError LWPMeshTool::getCallBacks()
{
	LWError err = LWPPlugin::getCallBacks();
	if (err) return err;

	// acquisitions
	if (!(stateQueryFuncs = (LWStateQueryFuncs *)globalFunc(LWSTATEQUERYFUNCS_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get State Query Funcs";

	return 0;
}

void LWPMeshTool::freeCallBacks()
{
	LWPPlugin::freeCallBacks();
	globalFunc(LWSTATEQUERYFUNCS_GLOBAL, GFUSE_RELEASE);
}

// default virtuals

void LWPMeshTool::draw(LWWireDrawAccess *) {}

char const * LWPMeshTool::help(LWToolEvent *) { return ""; }

int LWPMeshTool::dirty() { return 0; }

int LWPMeshTool::count(LWToolEvent *) { return 0; }

int LWPMeshTool::handle(LWToolEvent *, int, Vector &) { return 0; }

int LWPMeshTool::start(LWToolEvent *) { return 0; }

int LWPMeshTool::adjust(LWToolEvent *, int hNum) { return hNum; }

int LWPMeshTool::down(LWToolEvent *) { return 0; }

void LWPMeshTool::move(LWToolEvent *) {}

void LWPMeshTool::up(LWToolEvent *) {}

void LWPMeshTool::drop() {}

void LWPMeshTool::reset() {}

void LWPMeshTool::activate() {}

LWXPanelID LWPMeshTool::panel() { return 0; }

int LWPMeshTool::test() { return LWT_TEST_NOTHING; }

LWError LWPMeshTool::build(LWPMeshEditOp *) { return 0; }

void LWPMeshTool::end(int) {}

void * LWPMeshTool::get(unsigned long) { return 0; }

LWXPRefreshCode LWPMeshTool::set(unsigned long, void *) { return LWXPRC_NONE; }


//// LWPMeshEdit implementation ////

LWPMeshEdit::LWPMeshEdit()
{
}

LWPMeshEdit::LWPMeshEdit(char const * nm, int (*handler)(long,GlobalFunc*,void*,void*))
{
	pluginRegister(LWMESHEDIT_CLASS, nm, handler);	// register the plugin
}

LWError LWPMeshEdit::getCallBacks()
{
	LWError err = LWPPlugin::getCallBacks();
	if (err) return err;

	// acquisitions
	if (!(stateQueryFuncs = (LWStateQueryFuncs *)globalFunc(LWSTATEQUERYFUNCS_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get State Query Funcs";

	return 0;
}

void LWPMeshEdit::freeCallBacks()
{
	LWPPlugin::freeCallBacks();
	globalFunc(LWSTATEQUERYFUNCS_GLOBAL, GFUSE_RELEASE);
}

// default virtuals

int LWPMeshEdit::build(LWPMeshEditOp * local) { return AFUNC_OK; }

int LWPMeshEdit::flags() const { return 0; }

int LWPMeshEdit::pointBufSize() const { return 0; }

int LWPMeshEdit::polygonBufSize() const { return 0; }
