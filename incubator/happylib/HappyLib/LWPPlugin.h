
#ifndef _LWPPLUGIN_H
#define _LWPPLUGIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "HLDefs.h"
#include "HLUtils.h"

extern "C"
{
	#include <lwserver.h>
	#include <lwhost.h>
	#include <lwdyna.h>
	#include <lwmtutil.h>
	#include <lwpreview.h>
	#include <lwshelf.h>
	#include <lwsurf.h>
	#include <lwsurfed.h>
	#include <lwtxtred.h>
	#include <lwvparm.h>
	#include <lwpanel.h>
	#include <lwxpanel.h>
}

#if LWPANELS_API_VERSION == 19
#define LW7
#else
#define LW6
#endif

#include "LWPUtils.h"


//// Command Global [REMOVEME] remove once it's in the SDK
typedef int (*LWCommandFunc)(const char *);


//// wrapper for plugin instance [DOCUMENTME] ////

class LWPPlugin
{
public:
	// statics
	static int Plugins;
	static GlobalFunc *				globalFunc;
	static int						callBackUsers;
	// acquisitions
	// [NOTE] HostDisplayInfo is not acquired here, because it #includes the Behemoth (windows.h)
	//        and plugins would take nine hours to compile.  Acquire it yourself if you need it.
	static LWChannelInfo *			channelInfo;
	static LWColorActivateFunc *	colorPickerFunc;
	static LWDirInfoFunc *			dirInfoFunc;
	static DynaConvertFunc *		dynaConvertFunc;
	static DynaReqFuncs *			dynaRequestFuncs;
	static LWEnvelopeFuncs *		envelopeFuncs;
	static LWFileActivateFunc *		fileActivateFunc;
	static LWFileIOFuncs *			fileIOFuncs;
	static LWFileReqFunc *			fileReqFunc;
	static LWFileTypeFunc *			fileTypeFunc;
	static LWImageList *			imageList;
	static LWImageUtil *			imageUtil;
	static LWInstUpdate *			instUpdate;
	static LWMessageFuncs *			messageFuncs;
	static LWMTUtilFuncs *			mutexFuncs;
	static LWObjectFuncs *			objectFuncs;
	static LWPanelFuncs *			panelFuncs;
	static LWPreviewFuncs *			previewFuncs;
	static LWRasterFuncs *			rasterFuncs;
	static LWShelfFuncs *			shelfFuncs;
	static LWSurfaceFuncs *			surfaceFuncs;
	static LWSurfEdFuncs *			surfaceEditFuncs;
	static LWTxtrEdFuncs *			textureEditFuncs;
	static LWTextureFuncs *			textureFuncs;
	static LWVParmFuncs *			vParmFuncs;
	static LWXPanelFuncs *			xPanelFuncs;
	// transients
	static unsigned long			localeInfo;
	static unsigned long			productInfo;
	static unsigned long			productID;
	static unsigned long			productMajor;
	static unsigned long			productMinor;
	static unsigned long			productBuild;
	static unsigned long			systemID;
	static unsigned long			systemContext;
	static unsigned long			systemSerial;
	static int						systemIsModeler;
	static int						systemIsLayout;
	static int						systemIsScreamerNet;
	// functions
	static LWError getCallBacks();
	static void freeCallBacks();
	// member functions
	static int pluginRegister(char const *, char const *, int (*)(long,GlobalFunc*,void*,void*));
	LWPPlugin() {}
	// virtual member functions
	virtual ~LWPPlugin() {}
	virtual char const * name() const = 0;
};


//// PluginRegister class [DOCUMENTME] ////

template<class Plugin> class RegisterPlugin
{
public:
	RegisterPlugin(int(*face)(long,GlobalFunc*,void*,void*)=0)
	{
		// register the plugin
		if (Plugin::pluginRegister(Plugin::HClass(), Plugin::Name(), Plugin::Handler))
		{
			// register the interface
			if (face) Plugin::pluginRegister(Plugin::IClass(), Plugin::Name(), face);
		}
	}
};

#endif
