/*
 * UNMULT.C -- LightWave Plugins ImageFilterClass
 * UnMult source code for ImageFilterClass.
 * 
 * Ported from the LScript by Arnie Cachelin
 */

#include "UnMult.h"
#include <lwmodule.h>

#define SERVER_NAME "LWPP_Test_UnMult"

ServerTagInfo TagInfo[] = {
	{ SERVER_NAME,					SRVTAG_USERNAME|LANGID_USENGLISH },
	{ SERVER_NAME,					SRVTAG_BUTTONNAME },
	{ SERVER_NAME,					SRVTAG_DESCRIPTION },
	{ "Manipulate the Alpha Channel of an Image",SRVTAG_DESCRIPTION},
	{ (const char *) NULL },
};

ServerRecord ServerDesc[] = {
	{ LWIMAGEFILTER_HCLASS, SERVER_NAME, lwpp::XPanelImageFilterAdaptor<UnMultiply>::Activate, TagInfo },
	{ LWIMAGEFILTER_ICLASS, SERVER_NAME, lwpp::XPanelImageFilterAdaptor<UnMultiply>::ActivateUI, TagInfo },
	//{ LWIMAGEFILTER_HCLASS, SERVER_NAME, lwpp::WrapXPanelImageFilter<UnMultiply>::Activate, TagInfo },
	//{ LWIMAGEFILTER_ICLASS, SERVER_NAME, lwpp::WrapXPanelImageFilter<UnMultiply>::ActivateUI, TagInfo },
	{ NULL }
};


void Shutdown (void *serverData)
{
}
void *Startup (void)
{
    return (void *) 4;
}


#ifdef __BORLANDC__
ModuleDescriptor mod_descrip =
#else
ModuleDescriptor _mod_descrip =
#endif
{
  MOD_SYSSYNC,
  MOD_SYSVER,
  MOD_MACHINE,
  Startup,
  Shutdown,
  ServerDesc
};