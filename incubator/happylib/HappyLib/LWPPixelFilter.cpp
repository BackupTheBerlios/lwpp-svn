
#include "LWPPixelFilter.h"


//// LWPPixelFilter implementation ////

XCALL_(unsigned int) LWPPixelFilter::Flags(void * inst)
{
	return ((LWPPixelFilter *)inst)->flags();
}

XCALL_(void) LWPPixelFilter::Evaluate(void * inst, LWPixelAccess const * lwpa)
{
	((LWPPixelFilter *)inst)->process((LWPPixelAccess const *)lwpa);
}

// default virtuals

unsigned int LWPPixelFilter::flags() const { return 0; }	// 0 means RGBA

void LWPPixelFilter::process(LWPPixelAccess const *) {}

LWPPixelFilter::LWPPixelFilter(char const * nm, int (*handler)(long,GlobalFunc*,void*,void*), int (*face)(long,GlobalFunc*,void*,void*))
: context(0)
{
	if (pluginRegister(HClass(), nm, handler))	// register the plugin
	{
		if (face) pluginRegister(IClass(), nm, face);
	}
}
