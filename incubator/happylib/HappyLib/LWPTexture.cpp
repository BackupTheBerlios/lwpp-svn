
#include "LWPTexture.h"


//// LWPTexture implementation ////

XCALL_(unsigned int) LWPTexture::Flags(void * inst)
{
	return ((LWPTexture*)inst)->flags();
}

XCALL_(double) LWPTexture::Evaluate(void * inst, LWTextureAccess * lwta)
{
	return ((LWPTexture*)inst)->evaluate((LWPTextureAccess*)lwta);
}

LWPTexture::LWPTexture(char const * nm, int (*handler)(long,GlobalFunc*,void*,void*), int (*face)(long,GlobalFunc*,void*,void*))
: layer(0)
{
	if (pluginRegister(LWTEXTURE_HCLASS, nm, handler))	// register the plugin
	{
		if (face) pluginRegister(LWTEXTURE_ICLASS, nm, face);
	}
}

// default virtuals

unsigned int LWPTexture::flags() const { return 0; }

double LWPTexture::evaluate(LWPTextureAccess *) { return 0.0; }

