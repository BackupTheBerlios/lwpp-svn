
#include "LWPDisplacement.h"


//// LWPDisplacement implementation ////

XCALL_(unsigned int) LWPDisplacement::Flags(void * inst)
{
	return ((LWPDisplacement *)inst)->flags();
}

XCALL_(void) LWPDisplacement::Evaluate(void * inst, LWDisplacementAccess * lwda)
{
	((LWPDisplacement *)inst)->evaluate((LWPDisplacementAccess*)lwda);
	// [FIXME] would be nice to evaluatePreview when not rendering...
}

LWPDisplacement::LWPDisplacement(char const * nm, int (*handler)(long,GlobalFunc*,void*,void*), int (*face)(long,GlobalFunc*,void*,void*))
: item(0)
{
	if (pluginRegister(LWDISPLACEMENT_HCLASS, nm, handler))	// register the plugin
	{
		if (face) pluginRegister(LWDISPLACEMENT_ICLASS, nm, face);
	}
}

// default virtuals

unsigned int LWPDisplacement::flags() const { return 0; }

void LWPDisplacement::evaluate(LWPDisplacementAccess *) {}

void LWPDisplacement::evaluatePreview(LWPDisplacementAccess * sa) { evaluate(sa); }
