
#include "LWPCustomObject.h"


//// LWPCustomObject implementation ////

XCALL_(unsigned int) LWPCustomObject::Flags(void * inst)
{
	return ((LWPCustomObject *)inst)->flags();
}

XCALL_(void) LWPCustomObject::Evaluate(void * inst, LWCustomObjAccess const * lwva)
{
	((LWPCustomObject *)inst)->evaluate((LWPCustomObjectAccess const *)lwva);
}

LWPCustomObject::LWPCustomObject(char const * nm, int (*handler)(long,GlobalFunc*,void*,void*), int (*face)(long,GlobalFunc*,void*,void*))
: item(0)
{
	if (pluginRegister(HClass(), nm, handler))	// register the plugin
	{
		if (face) pluginRegister(IClass(), nm, face);
	}
}
