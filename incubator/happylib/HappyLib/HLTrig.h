
#ifndef _HLTRIG_H
#define _HLTRIG_H

#ifndef _HLUTILS_H
#include "HLUtils.h"
#endif
#ifndef _HLPOINT_H
#include "HLPoint.h"
#endif


//// Useful constants ////

#define Pi 3.14159265358979323846264338327950
#define IPi 0.31830988618379067153776752674503


//// HLTrig - useful trigonometric functions ////

class HLTrig
{
public:
	static double aCos(double theta)	// safe arcCosine
	{
		if (theta >= 1.0) return 0.0;
		if (theta <= -1.0) return Pi;
		return acos(theta);
	}
	static double aSin(double theta)	// safe arcSine
	{
		return asin(theta);
	}
	static double aTan(double theta)	// safe arcTan
	{
		return atan(theta);
	}
	static double SASOpposite(double aSide, double cAngle, double bSide)
	{
		// use the SAS rule (law of cosines) to find missing opposite sidelength (cSide)
		return sqrt(Sq(aSide) + Sq(bSide) - 2.0*aSide*bSide*cos(cAngle));
	}
	static double SASAdjacentLesser(double aSide, double cAngle, double cSide)
	{
		// use the SAS rule (law of cosines) to find missing adjacent sidelength (bSide)
		double det = Sq(cSide) - Sq(aSide*sin(cAngle));
		if (det < 0.0) return -1.0;				// none possible
		return aSide*cos(cAngle) - sqrt(det);	// return lesser length
	}
	static double SASAdjacentGreater(double aSide, double cAngle, double cSide)
	{
		// use the SAS rule (law of cosines) to find missing adjacent sidelength (bSide)
		double det = Sq(cSide) - Sq(aSide*sin(cAngle));
		if (det < 0.0) return -1.0;				// none possible
		return aSide*cos(cAngle) + sqrt(det);	// return greater length
	}
	static double RightLegFromLH(double leg, double hypoteneuse)
	{
		double det = Sq(hypoteneuse) - Sq(leg);
		if (det < 0.0) return -1.0;				// none possible
		return sqrt(det);
	}
	static double RightHypoteneuse(double leg0, double leg1)
	{
		return sqrt(Sq(leg0) + Sq(leg1));
	}
	static Vector FromSpherical(double theta, double phi)
	{
		double sinTheta = sin(theta);
		return Vector(cos(phi)*sinTheta, cos(theta), sin(phi)*sinTheta);
	}
};

#endif
