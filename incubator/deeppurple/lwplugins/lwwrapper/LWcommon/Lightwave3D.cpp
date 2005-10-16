#include "LWSceneGraph.h"
#include "Lightwave3D.h"
extern "C"
{
    ServerRecord ServerDesc[100];
}

GlobalFunc *Lightwave3D::GlobalFunction=0;
LWMessageFuncs *Lightwave3D::msg=0;             /* the message functions         */
LWPanelFuncs   *Lightwave3D::panf=0;              /* the panel functions           */
LWXPanelFuncs  *Lightwave3D::xpanf=0;
LWInstUpdate   *Lightwave3D::lwupdate = 0;
LWItemInfo     *Lightwave3D::iteminfo=0;
LWChannelInfo *Lightwave3D::chaninfo=0;
LWEnvelopeFuncs *Lightwave3D::envfunc=0;
LWTextureFuncs *Lightwave3D::txfunc=0;
LWImageUtil    *Lightwave3D::imgutil=0;
LWImageList    *Lightwave3D::imglist=0;
LWSceneInfo    *Lightwave3D::sceneinfo=0;
LWBackdropInfo *Lightwave3D::bkdropinfo=0;
LWTxtrEdFuncs  *Lightwave3D::txedf=0;
LWVParmFuncs   *Lightwave3D::vparmf=0;
LWPreviewFuncs *Lightwave3D::pvf=0;
LWDirInfoFunc  *Lightwave3D::dirinfo=0;
LWLightInfo    *Lightwave3D::lightinfo=0;
LWRasterFuncs *Lightwave3D::rastf=0;
LWObjectInfo *Lightwave3D::objinfo=0;
LWTimeInfo *Lightwave3D::timeinfo=0;
LWCameraInfo *Lightwave3D::caminfo=0;
LWShelfFuncs *Lightwave3D::shelff=0;
LWCommandFunc *Lightwave3D::cmdfunc=0;
LWFileActivateFunc * Lightwave3D::filereq = NULL;
LWLMonFuncs *Lightwave3D::lmonf=NULL;

unsigned long Lightwave3D::serialno=0;
unsigned long Lightwave3D::progid=0;

int Lightwave3D::ReferenceCounter=0;
int Lightwave3D::PluginCounter=0;

LWSceneGraph *Lightwave3D::SceneGraph = NULL;

void Lightwave3D::RegisterPlugin(const char *classname, const char *name, ActivateFunc *Starter, ServerTagInfo *srvinfo)
{
	ServerRecord FillIt;
	FillIt.className=classname;
	FillIt.name=name;
	FillIt.activate=Starter;
	FillIt.tagInfo=srvinfo;
	ServerDesc[PluginCounter]=FillIt;
	PluginCounter++;
	ServerDesc[PluginCounter].className=(const char *)0;
}

bool Lightwave3D::GetGlobals()
{
   if (ReferenceCounter)
      return true;

//   INFOLOGG("Getting Globals");
   SceneGraph=LWSceneGraph::GetSingletonPtr();

	unsigned long sysid = ( unsigned long )  GlobalFunction( LWSYSTEMID_GLOBAL,  GFUSE_TRANSIENT );
	serialno= sysid & LWSYS_SERIALBITS;
	progid  = sysid & LWSYS_TYPEBITS;
	lwupdate = (LWInstUpdate *)GlobalFunction( LWINSTUPDATE_GLOBAL, GFUSE_TRANSIENT );
	chaninfo = (LWChannelInfo *)GlobalFunction( LWCHANNELINFO_GLOBAL, GFUSE_TRANSIENT );
	envfunc = (LWEnvelopeFuncs *)GlobalFunction( LWENVELOPEFUNCS_GLOBAL, GFUSE_TRANSIENT );
	txfunc=(LWTextureFuncs *)GlobalFunction (LWTEXTUREFUNCS_GLOBAL, GFUSE_TRANSIENT);
	imgutil = (LWImageUtil *)GlobalFunction( LWIMAGEUTIL_GLOBAL, GFUSE_TRANSIENT );
	imglist = (LWImageList *)GlobalFunction( LWIMAGELIST_GLOBAL, GFUSE_TRANSIENT );
   vparmf = (LWVParmFuncs *)GlobalFunction( LWVPARMFUNCS_GLOBAL, GFUSE_TRANSIENT );
   dirinfo = (LWDirInfoFunc *)GlobalFunction( LWDIRINFOFUNC_GLOBAL, GFUSE_TRANSIENT );
   cmdfunc= (LWCommandFunc *)GlobalFunction( LWCOMMANDFUNC_GLOBAL, GFUSE_TRANSIENT);
	
	if ( !lwupdate || !chaninfo || !envfunc || !txfunc || !imgutil || !imglist || !vparmf || !dirinfo)
		return false;

	if (!ReferenceCounter && progid!=LWSYS_SCREAMERNET)
	{
	   msg = (LWMessageFuncs *)GlobalFunction( LWMESSAGEFUNCS_GLOBAL, GFUSE_TRANSIENT );

	   panf = (LWPanelFuncs *) GlobalFunction( LWPANELFUNCS_GLOBAL, GFUSE_TRANSIENT );
	   if (panf)
			panf->globalFun = GlobalFunction;
	   xpanf = (LWXPanelFuncs *)GlobalFunction( LWXPANELFUNCS_GLOBAL, GFUSE_TRANSIENT );

	   txedf = (LWTxtrEdFuncs *)GlobalFunction( LWTXTREDFUNCS_GLOBAL, GFUSE_TRANSIENT );

      filereq = (LWFileActivateFunc *)GlobalFunction( LWFILEACTIVATEFUNC_GLOBAL, GFUSE_TRANSIENT );

      rastf = (LWRasterFuncs *)GlobalFunction( LWRASTERFUNCS_GLOBAL, GFUSE_TRANSIENT );

      shelff = (LWShelfFuncs *)GlobalFunction( LWSHELFFUNCS_GLOBAL, GFUSE_TRANSIENT );

	   if ( !msg || !panf || !xpanf || !txedf || !filereq || !rastf || !shelff )
			return false;
	}

	if (!ReferenceCounter && progid!=LWSYS_MODELER)
	{
	   sceneinfo = (LWSceneInfo *)GlobalFunction( LWSCENEINFO_GLOBAL, GFUSE_TRANSIENT );
	   iteminfo = (LWItemInfo *)GlobalFunction( LWITEMINFO_GLOBAL, GFUSE_TRANSIENT );
	   bkdropinfo = (LWBackdropInfo *)GlobalFunction( LWBACKDROPINFO_GLOBAL, GFUSE_TRANSIENT );
      lightinfo=(LWLightInfo *)GlobalFunction( LWLIGHTINFO_GLOBAL, GFUSE_TRANSIENT );
      objinfo=(LWObjectInfo *)GlobalFunction( LWOBJECTINFO_GLOBAL, GFUSE_TRANSIENT );
      timeinfo = (LWTimeInfo *) GlobalFunction( LWTIMEINFO_GLOBAL, GFUSE_TRANSIENT );
      caminfo = (LWCameraInfo *)GlobalFunction( LWCAMERAINFO_GLOBAL, GFUSE_TRANSIENT );

      lmonf = (LWLMonFuncs *)GlobalFunction( LWLMONFUNCS_GLOBAL, GFUSE_TRANSIENT );            
      pvf = (LWPreviewFuncs *)GlobalFunction( LWPREVIEWFUNCS_GLOBAL, GFUSE_TRANSIENT );

		if ( !iteminfo || !sceneinfo || !bkdropinfo || !pvf || !lightinfo || !objinfo || !timeinfo || !caminfo || !lmonf )
			return false;
	}

	ReferenceCounter++;
	return true;
}

//-----------------------------------------------------------------------------------------------

void Lightwave3D::ReleaseGlobals()
{
   ReferenceCounter--;
   if (!ReferenceCounter)
   {
//      INFOLOGG("Releasing Globals");
//	   GlobalFunction( LWMESSAGEFUNCS_GLOBAL, GFUSE_RELEASE );
//	   GlobalFunction( LWPANELFUNCS_GLOBAL, GFUSE_RELEASE );
//	   GlobalFunction( LWXPANELFUNCS_GLOBAL, GFUSE_RELEASE );
//	   GlobalFunction( LWINSTUPDATE_GLOBAL, GFUSE_RELEASE );
//	   GlobalFunction( LWITEMINFO_GLOBAL, GFUSE_RELEASE );
//	   GlobalFunction( LWCHANNELINFO_GLOBAL, GFUSE_RELEASE );
//	   GlobalFunction( LWENVELOPEFUNCS_GLOBAL, GFUSE_RELEASE );
//	   GlobalFunction( LWTEXTUREFUNCS_GLOBAL, GFUSE_RELEASE);
//	   GlobalFunction( LWIMAGEUTIL_GLOBAL, GFUSE_RELEASE);
//	   GlobalFunction( LWIMAGELIST_GLOBAL, GFUSE_RELEASE);
//	   GlobalFunction( LWSCENEINFO_GLOBAL, GFUSE_RELEASE);
//	   GlobalFunction( LWBACKDROPINFO_GLOBAL, GFUSE_RELEASE);
//	   GlobalFunction( LWTXTREDFUNCS_GLOBAL, GFUSE_RELEASE);
//      GlobalFunction( LWVPARMFUNCS_GLOBAL, GFUSE_RELEASE );
//      GlobalFunction( LWPREVIEWFUNCS_GLOBAL, GFUSE_RELEASE );
//      GlobalFunction( LWLIGHTINFO_GLOBAL, GFUSE_RELEASE );
//      GlobalFunction( LWRASTERFUNCS_GLOBAL, GFUSE_RELEASE );
//      GlobalFunction( LWTIMEINFO_GLOBAL, GFUSE_RELEASE );
//      GlobalFunction( LWSHELFFUNCS_GLOBAL, GFUSE_RELEASE );
//      GlobalFunction( LWCOMMANDFUNC_GLOBAL, GFUSE_RELEASE);
//      GlobalFunction( LWLMONFUNCS_GLOBAL, GFUSE_RELEASE );            
      LWSceneGraph::DestroySingleton();
      SceneGraph=NULL;

      //Logger::DestroySingleton();
   }

}
