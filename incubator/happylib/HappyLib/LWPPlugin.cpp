
#include "LWPPlugin.h"
#include <fstream.h>

GlobalFunc *			LWPPlugin::globalFunc = 0;
int						LWPPlugin::callBackUsers = 0;
// acquisitions
LWChannelInfo *			LWPPlugin::channelInfo = 0;
LWColorActivateFunc *	LWPPlugin::colorPickerFunc = 0;
LWDirInfoFunc *			LWPPlugin::dirInfoFunc = 0;
DynaConvertFunc *		LWPPlugin::dynaConvertFunc = 0;
DynaReqFuncs *			LWPPlugin::dynaRequestFuncs = 0;
//DynaMonitorFuncs	// [FIXME] get these! (maybe only in LWPMeshTool?)
LWEnvelopeFuncs *		LWPPlugin::envelopeFuncs = 0;
LWFileActivateFunc *	LWPPlugin::fileActivateFunc = 0;
LWFileIOFuncs *			LWPPlugin::fileIOFuncs = 0;
LWFileReqFunc *			LWPPlugin::fileReqFunc = 0;
LWFileTypeFunc *		LWPPlugin::fileTypeFunc = 0;
LWImageList *			LWPPlugin::imageList = 0;
LWImageUtil *			LWPPlugin::imageUtil = 0;
LWInstUpdate *			LWPPlugin::instUpdate = 0;
LWMessageFuncs *		LWPPlugin::messageFuncs = 0;
LWMTUtilFuncs *			LWPPlugin::mutexFuncs = 0;
LWObjectFuncs *			LWPPlugin::objectFuncs = 0;
LWPreviewFuncs *		LWPPlugin::previewFuncs = 0;
LWPanelFuncs *			LWPPlugin::panelFuncs = 0;
LWRasterFuncs *			LWPPlugin::rasterFuncs = 0;
LWShelfFuncs *			LWPPlugin::shelfFuncs = 0;
LWSurfaceFuncs *		LWPPlugin::surfaceFuncs = 0;
LWSurfEdFuncs *			LWPPlugin::surfaceEditFuncs = 0;
LWTxtrEdFuncs *			LWPPlugin::textureEditFuncs = 0;
LWTextureFuncs *		LWPPlugin::textureFuncs = 0;
LWVParmFuncs *			LWPPlugin::vParmFuncs = 0;
LWXPanelFuncs *			LWPPlugin::xPanelFuncs = 0;
// transients
unsigned long			LWPPlugin::localeInfo = 0;
unsigned long			LWPPlugin::productInfo = 0;
unsigned long			LWPPlugin::productID = 0;
unsigned long			LWPPlugin::productMajor = 0;
unsigned long			LWPPlugin::productMinor = 0;
unsigned long			LWPPlugin::productBuild = 0;
unsigned long			LWPPlugin::systemID = 0;
unsigned long			LWPPlugin::systemContext = 0;
unsigned long			LWPPlugin::systemSerial = 0;
int						LWPPlugin::systemIsModeler = 0;
int						LWPPlugin::systemIsLayout = 0;
int						LWPPlugin::systemIsScreamerNet = 0;


#ifndef _HL_CODEWARRIOR	// [FIXME] need a way to get this working under CodeWarrior
extern "C"
{
	ServerRecord ServerDesc[257];	// room for 256 plugins
}
#endif


//// LWPPlugin implementation ////

/*	[FIXME] handle tags properly
ServerTagInfo tags[] =
{
	{"LWPPlugin SRVTAG_USERNAME", LANGID_USENGLISH|SRVTAG_USERNAME},
	{"LWPPlugin SRVTAG_BUTTONNAME", LANGID_USENGLISH|SRVTAG_BUTTONNAME},
	{"LWPPlugin SRVTAG_CMDGROUP", LANGID_USENGLISH|SRVTAG_CMDGROUP},
	{"LWPPlugin SRVTAG_MENU", LANGID_USENGLISH|SRVTAG_MENU},
	{"LWPPlugin SRVTAG_DESCRIPTION", LANGID_USENGLISH|SRVTAG_DESCRIPTION},
	{"LWPPlugin SRVTAG_ENABLE", LANGID_USENGLISH|SRVTAG_ENABLE},
	{0, 0}
};
*/

int LWPPlugin::Plugins = 0;

int LWPPlugin::pluginRegister(char const * cn, char const * nm, int (*act)(long,GlobalFunc*,void*,void*))
{
#ifndef _HL_CODEWARRIOR	// [FIXME] need a way to get this working under CodeWarrior
	for (int p = 0; p < Plugins; p ++)	// check to see if it's already registered
		if (ServerDesc[p].className == cn && ServerDesc[p].name == nm) return 0;
	ServerDesc[Plugins].className = cn;
	ServerDesc[Plugins].name = nm;
	ServerDesc[Plugins].activate = act;
	ServerDesc[Plugins].tagInfo = 0;//tags;	// [FIXME] handle tags properly
	Plugins ++;
	ServerDesc[Plugins].className = 0;	// update end of records flag
	ServerDesc[Plugins].name = 0;
	ServerDesc[Plugins].activate = 0;
	ServerDesc[Plugins].tagInfo = 0;
#endif
	return 1;
}

LWError LWPPlugin::getCallBacks()
{
	if (!globalFunc) return "LWPPlugin::getCallbacks() : globalFunc not initialized!";

	// transients

	if (!(localeInfo = (unsigned long)globalFunc(LWLOCALEINFO_GLOBAL, GFUSE_TRANSIENT)))
		return "Couldn't get Locale Info";

	if (!(productInfo = (unsigned long)globalFunc(LWPRODUCTINFO_GLOBAL, GFUSE_TRANSIENT)))
		return "Couldn't get Product Info";
	productID = productInfo & LWINF_PRODUCT;
	productMajor = LWINF_GETMAJOR(productInfo);
	productMinor = LWINF_GETMINOR(productInfo);
	productBuild = LWINF_GETBUILD(productInfo);

	if (!(systemID = (unsigned long)globalFunc(LWSYSTEMID_GLOBAL, GFUSE_TRANSIENT)))
		return "Couldn't get System ID";
	systemContext = systemID & LWSYS_TYPEBITS;
	systemSerial = systemID & LWSYS_SERIALBITS;
	systemIsModeler = systemID & LWSYS_MODELER;
	systemIsLayout = 1;
	systemIsScreamerNet = systemID & LWSYS_SCREAMERNET;

	// acquisitions

	if (callBackUsers ++) return 0;	// already got them

	if (!(channelInfo = (LWChannelInfo *)globalFunc(LWCHANNELINFO_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Channel Info";

	if (!(dirInfoFunc = (LWDirInfoFunc *)globalFunc(LWDIRINFOFUNC_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Directory Info";

	if (!(dynaConvertFunc = (DynaConvertFunc *)globalFunc(LWDYNACONVERTFUNC_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Dynamic Conversion Funcsions";

	if (!(dynaRequestFuncs = (DynaReqFuncs *)globalFunc(LWDYNAREQFUNCS_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Dynamic Requestor Functions";

	if (!(envelopeFuncs = (LWEnvelopeFuncs *)globalFunc(LWENVELOPEFUNCS_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Envelops Functions";

	if (!(fileIOFuncs = (LWFileIOFuncs *)globalFunc(LWFILEIOFUNCS_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get File IO Functions";

	if (!(imageList = (LWImageList *)globalFunc(LWIMAGELIST_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Image List";

	if (!(imageUtil = (LWImageUtil *)globalFunc(LWIMAGEUTIL_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Image Utilities";

	if (!(instUpdate = (LWInstUpdate *)globalFunc(LWINSTUPDATE_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Instance Update Functions";

	if (!(messageFuncs = (LWMessageFuncs*)globalFunc(LWMESSAGEFUNCS_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Info Messages Functions";

	if (!(mutexFuncs = (LWMTUtilFuncs *)globalFunc(LWMTUTILFUNCS_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Mutex Functions";

	if (!(objectFuncs = (LWObjectFuncs *)globalFunc(LWOBJECTFUNCS_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Object Functions";
   
	if (!(surfaceFuncs = (LWSurfaceFuncs *)globalFunc(LWSURFACEFUNCS_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Surface Functions";

	if (!(textureFuncs = (LWTextureFuncs *)globalFunc(LWTEXTUREFUNCS_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Texture Functions";

	if (!(vParmFuncs = (LWVParmFuncs *)globalFunc(LWVPARMFUNCS_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Variable Parameter Functions";

	if (systemIsScreamerNet) return 0;

	// non-screamy acquisitions

	if (!(colorPickerFunc = (LWColorActivateFunc *)globalFunc(LWCOLORACTIVATEFUNC_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Color Activate Function";

	if (!(fileActivateFunc = (LWFileActivateFunc *)globalFunc(LWFILEACTIVATEFUNC_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get File Request Activation Function";

	if (!(fileReqFunc = (LWFileReqFunc *)globalFunc(LWFILEREQFUNC_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get File Request Function";

	if (!(fileTypeFunc = (LWFileTypeFunc *)globalFunc(LWFILETYPEFUNC_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get File Type Function";

	if (!(panelFuncs = (LWPanelFuncs *)globalFunc(LWPANELFUNCS_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Panel Functions";

	if (!(previewFuncs = (LWPreviewFuncs *)globalFunc(LWPREVIEWFUNCS_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Preview Functions";

	if (!(rasterFuncs = (LWRasterFuncs *)globalFunc(LWRASTERFUNCS_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Raster Functions";

	if (!(shelfFuncs = (LWShelfFuncs *)globalFunc(LWSHELFFUNCS_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Shelf Functions";

	if (!(surfaceEditFuncs = (LWSurfEdFuncs *)globalFunc(LWSURFEDFUNCS_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Surface Edit Functions";

	if (!(textureEditFuncs = (LWTxtrEdFuncs *)globalFunc(LWTXTREDFUNCS_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get Texture Edit Functions";

	if (!(xPanelFuncs = (LWXPanelFuncs *)globalFunc(LWXPANELFUNCS_GLOBAL, GFUSE_ACQUIRE)))
		return "Couldn't get XPanel Functions";

	return 0;
}

void LWPPlugin::freeCallBacks()
{
	if (-- callBackUsers) return;	// still need them

	globalFunc(LWCHANNELINFO_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWDIRINFOFUNC_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWDYNACONVERTFUNC_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWDYNAREQFUNCS_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWENVELOPEFUNCS_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWFILEIOFUNCS_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWIMAGELIST_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWIMAGEUTIL_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWINSTUPDATE_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWLIGHTINFO_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWMESSAGEFUNCS_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWMTUTILFUNCS_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWOBJECTFUNCS_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWSURFACEFUNCS_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWTEXTUREFUNCS_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWVPARMFUNCS_GLOBAL, GFUSE_RELEASE);

	if (systemIsScreamerNet) return;

	globalFunc(LWCOLORACTIVATEFUNC_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWFILEACTIVATEFUNC_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWFILEREQFUNC_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWFILETYPEFUNC_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWPANELFUNCS_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWPREVIEWFUNCS_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWRASTERFUNCS_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWSHELFFUNCS_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWSURFEDFUNCS_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWTXTREDFUNCS_GLOBAL, GFUSE_RELEASE);
	globalFunc(LWXPANELFUNCS_GLOBAL, GFUSE_RELEASE);
}
