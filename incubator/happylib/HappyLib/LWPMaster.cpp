
#include "LWPMaster.h"
#include "LWPScene.h"

//// LWPMaster implementation ////

XCALL_(unsigned int) LWPMaster::Flags(void * inst)
{
	return ((LWPMaster *)inst)->flags();
}

XCALL_(double) LWPMaster::Event(void * inst, LWMasterAccess const * lwma)
{
	((LWPMaster *)inst)->event((LWPMasterAccess*)lwma);
	return 0;
}

LWPMaster::LWPMaster(char const * nm, int (*handler)(long,GlobalFunc*,void*,void*), int (*face)(long,GlobalFunc*,void*,void*))
{
	if (pluginRegister(HClass(), nm, handler))	// register the plugin
	{
		if (face) pluginRegister(IClass(), nm, face);
	}
}

// default virtuals

unsigned int LWPMaster::flags() const { return 0; }

void LWPMaster::event(LWPMasterAccess const *) {}
