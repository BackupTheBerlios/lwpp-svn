
#ifndef _LWPSHADERACCESS_H
#define _LWPSHADERACCESS_H

#include "HLPoint.h"
#include "HLMatrix3X3.h"
#include "LWPPlugin.h"

extern "C"
{
	#include <lwshader.h>
}


//// wrapper for shader access ////

class LWPShaderAccess
{
public:
	int const			sx;
	int const			sy;
	Point const			oPos;
	Point const			wPos;
	Vector const		gNorm;
	double const		spotSize;
	Point const			raySource;
	double const		rayLength;
	double const		cosine;
	Matrix3X3 const		oXfrm;
	Matrix3X3 const		wXfrm;
	LWItemID const		objID;
	int const 			polNum;

	Vector				wNorm;
	Color				color;
	double				luminous;
	double				diffuse;
	double				specular;
	double				mirror;
	double				transparency;
	double				eta;
	double				roughness;
	
	LWIlluminateFunc * const	illuminate;
	LWRayTraceFunc * const		rayTrace;
	LWRayCastFunc * const		rayCast;
	LWRayShadeFunc * const		rayShade;
	int const 			flags;
	int const 			bounces;
	LWItemID const		sourceID;
	Vector const		wNorm0;

	double				bumpHeight;
	double				translucency;
	double				colorHL;
	double				colorFL;
	double				addTransparency;
	double				difSharpness;
	
	LWPntID const		verts[4];		// surrounding vertex IDs
	float const			weights[4]; 	// vertex weigths
	Pointf const		vertsWPos[4];	// vertex world positions
	LWPolID const 		polygon;		// polygon ID

#ifdef LW7
	double				replacementPercentage;
	Color				replacementColor;
	double				reflectionBlur;
	double				refractionBlur;
#endif

	operator LWShaderAccess * () { return (LWShaderAccess *)this; }
	operator LWShaderAccess const * () const { return (LWShaderAccess const *)this; }
	static LWPShaderAccess Empty()
	{
		char buf[sizeof(LWPShaderAccess)];
		memset(buf, sizeof(LWPShaderAccess), 0);
		return *(LWPShaderAccess*)buf;
	}
};

#endif
