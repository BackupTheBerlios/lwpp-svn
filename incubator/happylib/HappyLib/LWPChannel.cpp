
#include "LWPChannel.h"


//// LWPChannel implementation ////

XCALL_(unsigned int) LWPChannel::Flags(void * inst)
{
	return ((LWPChannel*)inst)->flags();
}

XCALL_(void) LWPChannel::Evaluate(void * inst, LWChannelAccess const * lwca)
{
	// [NOTE] channels don't get a NewTime call so we get stuff here
	LWPRender::QuietlyGetTransients();
	((LWPChannel*)inst)->evaluate((LWPChannelAccess*)lwca);
}

LWPChannel::LWPChannel(char const * nm, int (*handler)(long,GlobalFunc*,void*,void*), int (*face)(long,GlobalFunc*,void*,void*))
{
	if (pluginRegister(LWCHANNEL_HCLASS, nm, handler))	// register the plugin
	{
		if (face) pluginRegister(LWCHANNEL_ICLASS, nm, face);
	}
}

// default virtuals

unsigned int LWPChannel::flags() const { return 0; }

void LWPChannel::evaluate(LWPChannelAccess *) {}

