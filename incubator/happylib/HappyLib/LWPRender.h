
#ifndef _LWPRENDER_H
#define _LWPRENDER_H

#include "LWPPlugin.h"
#include "LWPSerialization.h"

extern "C"
{
	#include <lwrender.h>
	#include <lwprtcl.h>
}


//// wrapper for render plugin instance ////

class LWPRender : public LWPPlugin
{
public:
	// static callbacks
	XCALL_(static void) Destroy(void *);
	XCALL_(static LWError) Copy(void *, void *);
	XCALL_(static LWError) Load(void *, LWLoadState const *);
	XCALL_(static LWError) Save(void *, LWSaveState const *);
	XCALL_(static char const *) DescLn(void *);
	XCALL_(static LWError) Init(void *, int);
	XCALL_(static void) CleanUp(void *);
	XCALL_(static LWError) NewTime(void *, LWFrame, LWTime);
	XCALL_(static LWItemID const *) UseItems(void *);
	XCALL_(static void) ChangeID(void *, LWItemID const *);
	static void QuietlyGetTransients();
	// data
	mutable char desc[256];
	LWPSerialization serialization;
	// statics
#ifdef LW6
	// [NOTE] LightPos is for a workaround for a bug in LW6.5
	class LightPos
	{
	public:
		LWItemID id;
		Pointf pos;
		LightPos() {}
		LightPos(LWItemID id, Pointf const & pos) : id(id), pos(pos) {}
	};
	static Queue<LightPos>		LightPosQ;
#endif
	static Matrix4X4			TransW2S;
	static int					callBackUsers;
	// acquisitions
	static LWBoneInfo *			boneInfo;
	static LWCameraInfo *		cameraInfo;
	static LWGlobalPool *		globalPool;
	static LWItemInfo *			itemInfo;
	static LWLightInfo *		lightInfo;
	static LWObjectInfo *		objectInfo;
	static LWPSysFuncs *		particleFuncs;
	static LWCommandFunc		commandFunc;
	// transients
	static LWBackdropInfo *		backdropInfo;
	static LWCompInfo *			compInfo;
	static LWFogInfo *			fogInfo;
	static LWInterfaceInfo *	interfaceInfo;
	static LWSceneInfo *		sceneInfo;
	static LWTimeInfo *			timeInfo;
	// functions
	static LWError getCallBacks();
	static void freeCallBacks();
	// virtuals
	virtual ~LWPRender() {}
	virtual LWError load(LWPLoadState const *);
	virtual LWError save(LWPSaveState const *) const;
	virtual LWError copy(LWPRender const *);
	virtual char const * descLn() const;
	virtual LWError init(int);
	virtual void cleanUp();
	virtual LWError newTime(LWFrame, LWTime);
	virtual LWItemID const * useItems() const;
	virtual void changeID(const LWItemID *);
};

#endif
