
#include "LWPRender.h"
#include "LWPScene.h"

int					LWPRender::callBackUsers = 0;
// acquisitions
LWBoneInfo *		LWPRender::boneInfo = 0;
LWCameraInfo *		LWPRender::cameraInfo = 0;
LWGlobalPool *		LWPRender::globalPool = 0;
LWItemInfo *		LWPRender::itemInfo = 0;
LWLightInfo *		LWPRender::lightInfo = 0;
LWObjectInfo *		LWPRender::objectInfo = 0;
LWPSysFuncs *		LWPRender::particleFuncs = 0;
LWCommandFunc		LWPRender::commandFunc = 0;
// transients
LWBackdropInfo *	LWPRender::backdropInfo = 0;
LWCompInfo *		LWPRender::compInfo = 0;
LWFogInfo *			LWPRender::fogInfo = 0;
LWInterfaceInfo *	LWPRender::interfaceInfo = 0;
LWSceneInfo *		LWPRender::sceneInfo = 0;
LWTimeInfo *		LWPRender::timeInfo = 0;

//// LWPRender implementation ////

#ifdef LW6
Queue<LWPRender::LightPos> LWPRender::LightPosQ;
#endif
Matrix4X4 LWPRender::TransW2S;

XCALL_(void) LWPRender::Destroy(void * inst)
{
	delete (LWPRender *)inst;
	freeCallBacks();
}

XCALL_(LWError) LWPRender::Copy(void * to, void * from)
{
	return ((LWPRender *)to)->copy((LWPRender *)from);
}

XCALL_(LWError) LWPRender::Load(void * inst, LWLoadState const * ls)
{
	return ((LWPRender *)inst)->load((LWPLoadState const*)ls);
}

XCALL_(LWError) LWPRender::Save(void * inst, LWSaveState const * ss)
{
	return ((LWPRender *)inst)->save((LWPSaveState const*)ss);
}

XCALL_(char const *) LWPRender::DescLn(void * inst)
{
	return ((LWPRender *)inst)->descLn();
}

XCALL_(LWError) LWPRender::Init(void * inst, int mode)
{
	return ((LWPRender *)inst)->init(mode);
}

XCALL_(void) LWPRender::CleanUp(void * inst)
{
	((LWPRender *)inst)->cleanUp();
}

XCALL_(LWError) LWPRender::NewTime(void * inst, LWFrame f, LWTime t)
{
	// get transients each NewTime

	if (!(backdropInfo = (LWBackdropInfo *)globalFunc(LWBACKDROPINFO_GLOBAL, GFUSE_TRANSIENT)))
		return "Couldn't get Backdrop Info";
	
	if (!(compInfo = (LWCompInfo *)globalFunc(LWCOMPINFO_GLOBAL, GFUSE_TRANSIENT)))
		return "Couldn't get Compositing Info";

	if (!(fogInfo = (LWFogInfo *)globalFunc(LWFOGINFO_GLOBAL, GFUSE_TRANSIENT)))
		return "Couldn't get Fog Info";

	if (!(sceneInfo = (LWSceneInfo *)globalFunc(LWSCENEINFO_GLOBAL, GFUSE_TRANSIENT)))
		return "Couldn't get Scene Info";

	if (!(timeInfo = (LWTimeInfo *)globalFunc(LWTIMEINFO_GLOBAL, GFUSE_TRANSIENT)))
		return "Couldn't get Time Info";

	if (!systemIsScreamerNet)
	{
		if (!(interfaceInfo = (LWInterfaceInfo *)globalFunc(LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT)))
			return "Couldn't get Interface Info";
	}

#ifdef LW6
	// read light positions [NOTE] to work around wPosition bug in LW
	LightPosQ.clear();
	for (LWPLight l = LWPLight::first(); l; l = l.next())
	{
		Point v;
		l.param(LWIP_W_POSITION, t, v);
		LightPosQ << LightPos(l, v);
	}
#endif
	// get world to screen matrix
	TransW2S = LWPCamera::renderCamera().w2sMatrix(t);

	return ((LWPRender *)inst)->newTime(f, t);
}

XCALL_(const LWItemID *) LWPRender::UseItems(void * inst)
{
	return ((LWPRender *)inst)->useItems();
}

XCALL_(void) LWPRender::ChangeID(void * inst, LWItemID const * ids)
{
	((LWPRender *)inst)->changeID(ids);
}

void LWPRender::QuietlyGetTransients()
{
	// get transients and don't complain if any failed (for layout-modeler plugins)
	backdropInfo = (LWBackdropInfo *)globalFunc(LWBACKDROPINFO_GLOBAL, GFUSE_TRANSIENT);
	compInfo = (LWCompInfo *)globalFunc(LWCOMPINFO_GLOBAL, GFUSE_TRANSIENT);
	fogInfo = (LWFogInfo *)globalFunc(LWFOGINFO_GLOBAL, GFUSE_TRANSIENT);
	sceneInfo = (LWSceneInfo *)globalFunc(LWSCENEINFO_GLOBAL, GFUSE_TRANSIENT);
	timeInfo = (LWTimeInfo *)globalFunc(LWTIMEINFO_GLOBAL, GFUSE_TRANSIENT);
	if (!systemIsScreamerNet) interfaceInfo = (LWInterfaceInfo *)globalFunc(LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT);
#ifdef LW6
	// read light positions [NOTE] to work around wPosition bug in LW
	LightPosQ.clear();
	for (LWPLight l = LWPLight::first(); l; l = l.next())
	{
		Point v;
		l.param(LWIP_W_POSITION, timeInfo->time, v);
		LightPosQ << LightPos(l, v);
	}
#endif
}

// [FIXME] make this pretty
LWCompInfo ModelerCompInfo = { 0, 0, 0 };
LWTimeInfo ModelerTimeInfo = { 0.0, 0 };

LWError LWPRender::getCallBacks()
{
	LWError err = LWPPlugin::getCallBacks();
	if (err) return err;

	// transients

	if (systemIsModeler)	// fake transients for modeler
	{
		//backdropInfo = (LWBackdropInfo *); [FIXME]
		compInfo = &ModelerCompInfo;
		//fogInfo = (LWFogInfo *); [FIXME]
		//sceneInfo = (LWSceneInfo *); [FIXME]
		timeInfo = &ModelerTimeInfo;
		return 0;	// don't try getting render-specifics in Modeler
	}

	if (!(backdropInfo = (LWBackdropInfo *)globalFunc(LWBACKDROPINFO_GLOBAL, GFUSE_TRANSIENT)))
		return "Couldn't get Backdrop Info";
	
	if (!(compInfo = (LWCompInfo *)globalFunc(LWCOMPINFO_GLOBAL, GFUSE_TRANSIENT)))
		return "Couldn't get Compositing Info";

	if (!(fogInfo = (LWFogInfo *)globalFunc(LWFOGINFO_GLOBAL, GFUSE_TRANSIENT)))
		return "Couldn't get Fog Info";

	if (!(sceneInfo = (LWSceneInfo *)globalFunc(LWSCENEINFO_GLOBAL, GFUSE_TRANSIENT)))
		return "Couldn't get Scene Info";

	if (!(timeInfo = (LWTimeInfo *)globalFunc(LWTIMEINFO_GLOBAL, GFUSE_TRANSIENT)))
		return "Couldn't get Time Info";

	// non-screamy transients

	if (!systemIsScreamerNet)
	{
		if (!(interfaceInfo = (LWInterfaceInfo *)globalFunc(LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT)))
			return "Couldn't get Interface Info";
	}

	// acquisitions

	if (callBackUsers ++) return 0;	// already got them

	if (!(boneInfo = (LWBoneInfo *)globalFunc(LWBONEINFO_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Bone Info";

	if (!(cameraInfo = (LWCameraInfo *)globalFunc(LWCAMERAINFO_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Camera Info";

	if (!(globalPool = (LWGlobalPool *)globalFunc(LWGLOBALPOOL_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Global Pool";

	if (!(itemInfo = (LWItemInfo *)globalFunc(LWITEMINFO_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Item Info";

	if (!(lightInfo = (LWLightInfo *)globalFunc(LWLIGHTINFO_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Light Info";

	if (!(objectInfo = (LWObjectInfo *)globalFunc(LWOBJECTINFO_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Object Info";

	if (!(particleFuncs = (LWPSysFuncs *)globalFunc(LWPSYSFUNCS_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Particle Functions";

	// non-screamy acquisitions

	if (!systemIsScreamerNet)
	{
		if (!(commandFunc = (LWCommandFunc)globalFunc("LW Command Interface", GFUSE_ACQUIRE)))
			return "Couldn't get LW Command Interface";
	}

	return 0;
}

void LWPRender::freeCallBacks()
{
	LWPPlugin::freeCallBacks();

	if (systemIsModeler) return;	// don't try freeing render-specifics in Modeler

	if (-- callBackUsers) return;	// still need them

	globalFunc(LWBONEINFO_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWCAMERAINFO_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWGLOBALPOOL_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWITEMINFO_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWOBJECTINFO_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWPSYSFUNCS_GLOBAL, GFUSE_RELEASE);

	if (systemIsScreamerNet) return;
	
	globalFunc("LW Command Interface", GFUSE_RELEASE);
}


// default virtuals

LWError LWPRender::load(LWPLoadState const * ls) { return serialization.load(ls); }

LWError LWPRender::save(LWPSaveState const * ss) const { return serialization.save(ss); }

LWError LWPRender::copy(LWPRender const * r) { return serialization.copy(r->serialization); }

char const * LWPRender::descLn() const
{
	sprintf(desc, name());
	return desc;
}

LWError LWPRender::init(int) { return 0; }

void LWPRender::cleanUp() {}

LWError LWPRender::newTime(LWFrame, LWTime) { return 0; }

LWItemID const * LWPRender::useItems() const
{
	// return an array of items we depend on
	static LWItemID id[1] = { 0 };
	return id;
}

void LWPRender::changeID(LWItemID const * id)
{
	// re-id any references to LWItems using the given array.
	// This would look something like the following in your object:

	/* [EXAMPLE]
	for (int i = 0; id[i]; i += 2)
	{
		if (some_member == id[i]) some_member = id[i + 1];
		if (some_other_member == id[i]) some_other_member = id[i + 1];
		...
	}
	*/
}

