
#ifndef _HLHSL
#define _HLHSL

#include "HLPoint.h"


//// HSL ////

// A Fast HSL-to-RGB Transform
// by Ken Fishkin
// from "Graphics Gems", Academic Press, 1990

// RGB-HSL transforms.
// Ken Fishkin, Pixar Inc., January 1989.

// modified by Graham Fyffe, January 2001.

// given rgb on [0 ... 1], return hsl on [0 ... 1]

Color RGB2HSL(Color const & rgb)
{
	Color hsl(0.0, 0.0, 0.0);

	double v = Max(Max(rgb[0],rgb[1]),rgb[2]);
	double m = Min(Min(rgb[0],rgb[1]),rgb[2]);

	if ((hsl[2] = (m + v) / 2.0) <= 0.0) return hsl;

	double vm;
	if ((hsl[1] = vm = v - m) <= 0.0) return hsl;

	hsl[1] /= (hsl[2] <= 0.5) ? (v + m) : (2.0 - v - m);

	double r2 = (v - rgb[0]) / vm;
	double g2 = (v - rgb[1]) / vm;
	double b2 = (v - rgb[2]) / vm;

	if (rgb[0] == v) hsl[0] = (rgb[1] == m ? 5.0 + b2 : 1.0 - g2);
	else if (rgb[1] == v) hsl[0] = (rgb[2] == m ? 1.0 + r2 : 3.0 - b2);
	else hsl[0] = (rgb[0] == m ? 3.0 + g2 : 5.0 - r2);

	hsl[0] /= 6;

	return hsl;
}

// given hsl on [0..1], return rgb on [0..1]

Color HSL2RGB(Color const & hsl)
{
	double v = (hsl[2] <= 0.5) ? (hsl[2] * (1.0 + hsl[1])) : (hsl[2] + hsl[1] - hsl[2] * hsl[1]);
	if (v <= 0) return Color::Black();
	double m = hsl[2] + hsl[2] - v;
	double sv = (v - m) / v;
	double h = hsl[0] * 6.0;
	int sextant = int(h);	
	double fract = h - sextant;
	double vsf = v * sv * fract;
	double mid1 = m + vsf;
	double mid2 = v - vsf;
	switch (sextant)
	{
	case 6:
	case 0: return Color(v, mid1, m);
	case 1: return Color(mid2, v, m);
	case 2: return Color(m, v, mid1);
	case 3: return Color(m, mid2, v);
	case 4: return Color(mid1, m, v);
	case 5: return Color(v, m, mid2);
	}
	return Color::Black();
}

Color HSLLerp(Color const & a, Color const & b, double alpha)
{
	Color hslA = RGB2HSL(a);
	Color hslB = RGB2HSL(b);
	if (hslA[1] == 0.0 || hslA[2] == 0.0) hslA[0] = hslB[0];
	else if (hslB[1] == 0.0 || hslB[2] == 0.0) hslB[0] = hslA[0];
	Color col;
	col[0] = fabs(hslA[0] - hslB[0]) < 0.5 ? Lerp(hslA[0], hslB[0], alpha)
		: hslA[0] < hslB[0] ? fmod(Lerp(hslA[0] + 1.0, hslB[0], alpha), 1.0)
		: fmod(Lerp(hslA[0], hslB[0] + 1.0, alpha), 1.0);
	col[1] = Lerp(hslA[1], hslB[1], alpha);
	col[2] = Lerp(hslA[2], hslB[2], alpha);
	col = HSL2RGB(col);
	return col;
}


#endif
