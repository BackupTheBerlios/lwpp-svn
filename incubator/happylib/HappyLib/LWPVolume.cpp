
#include "LWPVolume.h"
#include "HLTrig.h"


//// LWPVolumeAccess implementation ////

double LWPVolumeAccess::addSurfaceSample(Color const & opacity, Color const & color) const
{
	double length = (farClip - nearClip) * 0.001;
	double start = farClip - length;
	addSample(LWPVolumeSample(start, length, opacity, color));
	return start;
}

void LWPVolumeAccess::addSurfaceSample(double start, Color const & opacity, Color const & color) const
{
	double length = Min((farClip - nearClip) * 0.001, farClip - start);
	addSample(LWPVolumeSample(start, length, opacity, color));
}


//// LWPVolume implementation ////

XCALL_(unsigned int) LWPVolume::Flags(void * inst)
{
	return ((LWPVolume *)inst)->flags();
}

XCALL_(double) LWPVolume::Evaluate(void * inst, LWVolumeAccess * lwva)
{
	if ((lwva->flags & LWVEF_COLOR)) return ((LWPVolume*)inst)->evaluate((LWPVolumeAccess*)lwva);
	return ((LWPVolume *)inst)->evaluateFilter((LWPVolumeAccess*)lwva);
}

LWPVolume::LWPVolume(char const * nm, int (*handler)(long,GlobalFunc*,void*,void*), int (*face)(long,GlobalFunc*,void*,void*))
: context(0)
{
	if (pluginRegister(HClass(), nm, handler))	// register the plugin
	{
		if (face) pluginRegister(IClass(), nm, face);
	}
}

// default virtuals

unsigned int LWPVolume::flags() const { return 0; }

double LWPVolume::evaluate(LWPVolumeAccess *) { return 1.0; }

double LWPVolume::evaluateFilter(LWPVolumeAccess * va) { return evaluate(va); }

// helpers

Color LWPVolume::foggify(Vector const & dir, double dist, Color const & color)
{
	if (fogInfo->type == LWFOG_NONE || dist >= 1e14) return color;
	// compute fog parameter
	double fogLength = Max(1e-10, (fogInfo->maxDist(timeInfo->time) - fogInfo->minDist(timeInfo->time)));
	double alpha = Clamped((dist - fogInfo->minDist(timeInfo->time)) / fogLength, 0.0, 1.0);
	// apply nonlinearity
	if (fogInfo->type == LWFOG_NONLINEAR1) alpha = 1.0 - Sq(1.0 - alpha);
	else if (fogInfo->type == LWFOG_NONLINEAR2) alpha = sin(acos(1.0 - alpha));
	// apply fog
	double fog = Lerp(fogInfo->minAmt(timeInfo->time), fogInfo->maxAmt(timeInfo->time), alpha);
	Color fogColor;
	if (fogInfo->flags & LWFOGF_BACKGROUND) backdropInfo->backdrop(timeInfo->time, dir, fogColor);
	else fogInfo->color(timeInfo->time, fogColor);
	return Lerp(color, fogColor, fog);
}

Color LWPVolume::fogFade(Vector const & dir, double dist, Color const & color)
{
	if (fogInfo->type == LWFOG_NONE || dist >= 1e14) return color;
	// compute fog parameter
	double fogLength = Max(1e-10, (fogInfo->maxDist(timeInfo->time) - fogInfo->minDist(timeInfo->time)));
	double alpha = Clamped((dist - fogInfo->minDist(timeInfo->time)) / fogLength, 0.0, 1.0);
	// apply nonlinearity
	if (fogInfo->type == LWFOG_NONLINEAR1) alpha = 1.0 - Sq(1.0 - alpha);
	else if (fogInfo->type == LWFOG_NONLINEAR2) alpha = sin(acos(1.0 - alpha));
	// apply fog
	double fog = Lerp(fogInfo->minAmt(timeInfo->time), fogInfo->maxAmt(timeInfo->time), alpha);
	return color * (1.0 - fog);
}
