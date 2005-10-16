#ifndef LWINIT_H
#define LWINIT_H

class LWSceneGraph;

extern "C" {
#include "lwserver.h"            /* all plug-ins need this        */
#include "lwpanel.h"             /* for "classic" panels          */
#include "lwhost.h"              /* for the LWMessageFuncs global */
#include "lwxpanel.h"
#include "lwhandler.h"
#include "lwrender.h"
#include "lwdialog.h"
#include "lwtxtr.h"
#include "lwtxtred.h"
#include "lwimage.h"
#include "lwvparm.h"
#include "lwpreview.h"
#include "lwimageio.h"
#include "lwshelf.h"
#include "lwmonitor.h"
#include "lwmath.h"
}

#define LWCOMMANDFUNC_GLOBAL "LW Command Interface"
typedef int LWCommandFunc(const char *);

class Lightwave3D
{
public:
    static LWFileActivateFunc * filereq;
    static GlobalFunc *GlobalFunction;
    static LWMessageFuncs *msg;             /* the message functions         */
    static LWPanelFuncs *panf;              /* the panel functions           */
    static LWXPanelFuncs *xpanf;
    static LWInstUpdate *lwupdate;
    static LWItemInfo *iteminfo;
    static LWChannelInfo *chaninfo;
    static LWEnvelopeFuncs *envfunc;
    static LWTextureFuncs *txfunc;
    static LWImageUtil *imgutil;
    static LWImageList *imglist;
    static LWSceneInfo *sceneinfo;
    static LWBackdropInfo *bkdropinfo;
    static LWTxtrEdFuncs *txedf;
    static LWVParmFuncs *vparmf;
    static LWPreviewFuncs *pvf;
    static LWDirInfoFunc *dirinfo;
    static LWLightInfo *lightinfo;
    static LWRasterFuncs *rastf;
    static LWObjectInfo *objinfo;
    static LWTimeInfo *timeinfo;   
    static LWCameraInfo *caminfo;
    static LWShelfFuncs *shelff;
    static LWCommandFunc *cmdfunc;
    static LWLMonFuncs *lmonf;
    
    static unsigned long serialno;
    static unsigned long progid;
    static int ReferenceCounter;
    static int PluginCounter;
    static LWSceneGraph * SceneGraph;   
    
    static void RegisterPlugin(const char *classname, const char *name, ActivateFunc *Starter, ServerTagInfo *srvinfo);
    static bool GetGlobals();
    static void ReleaseGlobals();
};

#endif
