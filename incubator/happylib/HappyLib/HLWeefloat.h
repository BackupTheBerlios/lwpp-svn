
#ifndef _HLWEEFLOAT_H
#define _HLWEEFLOAT_H

//
//	this file contains the WeeFloat and ShortFloat classes,
//	which can represent numbers from 0.0 to 1.0 inclusively,
//	spread evenly over the range of integral internal values.
//

class WeeFloat	// an 8-bit value in the range [0.0 1.0]
{
public:
	unsigned char val;
	WeeFloat() {}
	WeeFloat(WeeFloat const & w) : val(w.val) {}
	WeeFloat(double const & d) : val((unsigned char)(d * 255.0 + 0.5)) {}
	WeeFloat(unsigned char val, int) : val(val) {}
	WeeFloat & operator = (WeeFloat const & d) { val = d.val; return *this; }
	WeeFloat & operator = (double d) { val = (unsigned char)(d * 255.0 + 0.5); return *this; }
	WeeFloat & operator += (WeeFloat const & d) { val += d.val; return *this; }
	WeeFloat & operator -= (WeeFloat const & d) { val -= d.val; return *this; }
	WeeFloat & operator *= (double d) { val = (unsigned char)(val * d + 0.25); return *this; }
	operator double () const { return (double)val*0.00392156862745098039215686274509804; }
};

class ShortFloat	// a 16-bit value in the range [0.0 1.0]
{
public:
	unsigned short val;
	ShortFloat() {}
	ShortFloat(ShortFloat const & w) : val(w.val) {}
	ShortFloat(double const & d) : val((unsigned short)(d * 65535.0 + 0.5)) {}
	ShortFloat(unsigned short val, int) : val(val) {}
	ShortFloat & operator = (ShortFloat const & d) { val = d.val; return *this; }
	ShortFloat & operator = (double d) { val = (unsigned short)(d * 65535.0 + 0.5); return *this; }
	ShortFloat & operator += (ShortFloat const & d) { val += d.val; return *this; }
	ShortFloat & operator -= (ShortFloat const & d) { val -= d.val; return *this; }
	ShortFloat & operator *= (ShortFloat const & d) { val = (unsigned short)((float)val * d + 0.25); return *this; }
	operator double () const { return (double)val*1.5259021896696421759365224689097e-5; }
};

#endif
