
#include "LWPImageFilter.h"


//// LWPImageFilter implementation ////

XCALL_(unsigned int) LWPImageFilter::Flags(void * inst)
{
	return ((LWPImageFilter *)inst)->flags();
}

XCALL_(void) LWPImageFilter::Process(void * inst, LWFilterAccess const * lwfa)
{
	((LWPImageFilter *)inst)->process((LWPFilterAccess const *)lwfa);
}

// default virtuals

unsigned int LWPImageFilter::flags() const { return 0; }	// 0 means RGBA

void LWPImageFilter::process(LWPFilterAccess const *) {}

LWPImageFilter::LWPImageFilter(char const * nm, int (*handler)(long,GlobalFunc*,void*,void*), int (*face)(long,GlobalFunc*,void*,void*))
: context(0)
{
	if (pluginRegister(LWIMAGEFILTER_HCLASS, nm, handler))	// register the plugin
	{
		if (face) pluginRegister(LWIMAGEFILTER_ICLASS, nm, face);
	}
}
