
#ifndef _HLCOLOR_H
#define _HLCOLOR_H

//
//	this file contains the 3-tuple class: ColorT.
//

//	OPERATOR OVERLOAD CONVENTIONS:
//	There is a number of overloaded operators for each type, to increase writability:

//	ALL TYPES:
//	x = y, x == y and x != y are defined with the expected behavior.
//	x + y, x += y, x - y, x -= y and -x are defined where it makes sense.
//	x * double, x *= double, x / double, x /= double are defined with the expected behavior.

//	ColorT:
//	color1 * color2		= product by component
//	color1 / color2		= quotient by component
//	color1 | color2		= perceptual color distance (0..1)
//	color1 || color2	= squared perceptual color distance (0..1)
//  pow, exp and log are also available

//	[NOTE] the following typedefs are defined:

//	typedef ColorT<double> Color;		(64-bit components)
//	typedef ColorT<float> Colorf;		(32-bit components)
//	typedef ColorT<WeeFloat> Colori;	(8-bit* components)
//	* WeeFloats can only take values in the range [0..1]

//	In addition, all types will cast themselves to a pointer of their
//	internal type for easy passing to LightWave's functions.
//	i.e. you can pass a Color into a function expecting a double *,
//	or pass a Colori into a function expecting an unsigned char *.

//  Also, for efficiency reasons, these classes do NOT initialize their values to 0.

#ifndef _HLTRIPLE_H
#include "HLTriple.h"
#endif


//// ColorT class ////

template <class Obj> class ColorT : public TripleT< Obj, ColorT<Obj> >
{
public:
	enum Channel{R,G,B};

	ColorT() {}
	explicit ColorT(Obj const *);
	ColorT(double, double, double);
	template <class Obj2, class ThatClass> ColorT(TripleT<Obj2, ThatClass> const & c)
	{
		_val[0] = (Obj)c[0]; _val[1] = (Obj)c[1]; _val[2] = (Obj)c[2];
	}
	void legalize();
	ColorT soft() const;
	double luminance() const;
	double chrominance() const;
	void setChrominance(double);
	void setLuminance(double);
	double importance() const { return luminance() * 0.9 + 0.1; }
	// statics
	static ColorT Spectral(double);
	static inline ColorT Black() { return ColorT((Obj)0.0, (Obj)0.0, (Obj)0.0); }
	static inline ColorT White() { return ColorT((Obj)1.0, (Obj)1.0, (Obj)1.0); }
	static inline ColorT Red() { return ColorT((Obj)1.0, (Obj)0.0, (Obj)0.0); }
	static inline ColorT Orange() { return ColorT((Obj)1.0, (Obj)0.5, (Obj)0.0); }
	static inline ColorT Yellow() { return ColorT((Obj)1.0, (Obj)1.0, (Obj)0.0); }
	static inline ColorT Green() { return ColorT((Obj)0.0, (Obj)1.0, (Obj)0.0); }
	static inline ColorT Blue() { return ColorT((Obj)0.0, (Obj)0.0, (Obj)1.0); }
	static inline ColorT Purple() { return ColorT((Obj)0.75, (Obj)0.0, (Obj)0.75); }
	template <class Obj2, class ThatClass> static ColorT RandColor(TripleT<Obj2, ThatClass> const & t, int seed = 0)
	{
		ColorT<Obj> c;
		c[0] = Obj(t.rand(seed)); c[1] = Obj(t.rand(seed+1)); c[2] = Obj(t.rand(seed+2));
		return c;
	}
};

typedef ColorT<double> Color;
typedef ColorT<float> Colorf;
typedef ColorT<WeeFloat> Colori;


//// non-member functions ////

template <class Obj, class Obj2> double operator | (ColorT<Obj> const & c1, ColorT<Obj2> const & c2)	// perceptual color distance
{
	double r, g, b, d, rmean;
	rmean = (c1[0] + c2[0]) * 0.5;
	r = c1[0] - c2[0];
	g = c1[1] - c2[1];
	b = c1[2] - c2[2];
	d = (2+rmean)*r*r + g*g*4 + (3-rmean)*b*b;
	return d * 0.100864652104997205922182452830536;
}

template <class Obj, class Obj2> double operator || (ColorT<Obj> const & c1, ColorT<Obj2> const & c2)	// squared perceptual color distance
{
	double d = c1 | c2;
	return d*d;
}


//// ColorT implementation ////

template <class Obj> ColorT<Obj>::ColorT(double x, double y, double z)
{
	_val[0] = (Obj)x; _val[1] = (Obj)y; _val[2] = (Obj)z;
}

template <class Obj> ColorT<Obj>::ColorT(Obj const * f)
{
	_val[0] = f[0]; _val[1] = f[1]; _val[2] = f[2];
}

template <class Obj> ColorT<Obj> ColorT<Obj>::soft() const
{
	return *this*0.75 + White*0.25;
}

template <class Obj> double ColorT<Obj>::luminance() const
{
	// return Y' given R'G'B'
	return _val[0]*0.299 + _val[1]*0.587 + _val[2]*0.114;
}

template <class Obj> double ColorT<Obj>::chrominance() const
{
	// [NOTE] undefined units
	double l = luminance();
	if (l == 0.0) return 0.0;
	return (fabs(_val[0]-l)*0.299 + fabs(_val[1]-l)*0.587 + fabs(_val[2]-l)*0.144) / l;
}

template <class Obj> void ColorT<Obj>::setChrominance(double ch)
{
	// solve for chrominance adjustment
	double c = chrominance();
	if (c == 0.0) return;
	double x = ch / c;
	// apply chrominance adjustment
	*this += (*this - Color::White()*luminance())*x;
}

template <class Obj> void ColorT<Obj>::setLuminance(double cl)
{
	double l = luminance();
	if (l == 0.0) return;
	*this *= cl / l;
}

template <class Obj> void ColorT<Obj>::legalize()
{
	clamp(0.0, 1.0);
}

template <class Obj>
ColorT<Obj> ColorT<Obj>::Spectral(double l)	// l is in nanometers
{
	// kind of matches Wavelength ColorPicker
	double r = l > 780 ? 0 : l > 680 ? (780-l)/100.0 : l > 650 ? 1.0 : l > 530 ? (l-530)/120.0 : l > 440 ? 0 : l > 400 ? (440-l)/100.0: l > 340 ? (400-l)/150.0 : 0;
	double g = l > 650 ? 0 : l > 580 ? (650-l)/70.0 : l > 530 ? 1 : l > 440 ? (l-440)/90.0 : 0;
	double b = l > 530 ? 0 : l > 490 ? (530-l)/40.0 : l > 340 ? (l-340)/150.0 : 0;
	return ColorT(r,g,b);
}

/*
template <class Obj>
ColorT<Obj> ColorT<Obj>::HSVLerp(ColorT<Obj> const & a, ColorT<Obj> const & b, double t)
{
	// [FIXME]
}
*/

#endif
