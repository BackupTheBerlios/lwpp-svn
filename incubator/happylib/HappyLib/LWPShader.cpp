
#include "LWPShader.h"
#include "LWPScene.h"

//// LWPShader implementation ////

XCALL_(unsigned int) LWPShader::Flags(void * inst)
{
	return ((LWPShader *)inst)->flags();
}

XCALL_(void) LWPShader::Evaluate(void * inst, LWShaderAccess * lwsa)
{
	if (lwsa->objID == (void*)0x10007fffu)	// preview object [FIXME] more robust test?
	{
		((LWPShader *)inst)->evaluatePreview((LWPShaderAccess*)lwsa);
		return;
	}
	if (lwsa->flags & LWSAF_SHADOW)	// shadow ray [FIXME] this is not yet implemented in LW :(
	{
		((LWPShader *)inst)->evaluateFilter((LWPShaderAccess*)lwsa);
		return;
	}
	((LWPShader *)inst)->evaluate((LWPShaderAccess*)lwsa);	// view ray
}

LWPShader::LWPShader(char const * nm, int (*handler)(long,GlobalFunc*,void*,void*), int (*face)(long,GlobalFunc*,void*,void*))
: surface(0)
{
	if (pluginRegister(HClass(), nm, handler))	// register the plugin
	{
		if (face) pluginRegister(IClass(), nm, face);
	}
}

// default virtuals

unsigned int LWPShader::flags() const { return 0; }

void LWPShader::evaluate(LWPShaderAccess *) {}

void LWPShader::evaluatePreview(LWPShaderAccess * sa) { evaluate(sa); }

void LWPShader::evaluateFilter(LWPShaderAccess * sa) { evaluate(sa); }

