
#ifndef _HLCOMPLEX_H
#define _HLCOMPLEX_H

#ifdef _WIN32
#ifndef _INC_MATH
#include <math.h>
#endif
#else // _WIN32
#ifndef _MATH_H
#include <math.h>
#endif
#endif // _WIN32

class Complex
{
public:
	double real;
	double imaginary;
	Complex() {}
	Complex(double real, double imaginary) : real(real), imaginary(imaginary) {}
	Complex(double real) : real(real), imaginary(0) {}
	// unary operators
	Complex const & operator + () const { return *this; }
	Complex operator - () const { return Complex(-real, -imaginary); }
	// complex - real operators
	Complex & operator += (double b) { real += b; return *this; }
	Complex & operator -= (double b) { real -= b; return *this; }
	Complex & operator *= (double b) { real *= b; imaginary *= b; return *this; }
	Complex & operator /= (double b) { real /= b; imaginary /= b; return *this; }
	// complex - complex operators
	Complex & operator += (Complex const & b) { real += b.real; imaginary += b.imaginary; return *this; }
	Complex & operator -= (Complex const & b) { real -= b.real; imaginary -= b.imaginary; return *this; }
	Complex & operator *= (Complex const & b)
	{
		double realNew = real*b.real - imaginary*b.imaginary;
		imaginary = imaginary*b.real + real*b.imaginary;
		real = realNew;
		return *this;
	}
	Complex & operator /= (Complex const & b)
	{
		double denom = Sq(b.real) + Sq(b.imaginary);
		double realNew = (real*b.real + imaginary*b.imaginary) / denom;
		imaginary = (imaginary*b.real - real*b.imaginary) / denom;
		real = realNew;
		return *this;
	}
	// handy stuff
	double angle() const { return atan2(imaginary, real); }
	double sqLen() const { return Sq(imaginary) + Sq(real); }
};

//// complex - real operators ////

inline Complex operator + (Complex a, double b) { return a += b; }
inline Complex operator - (Complex a, double b) { return a -= b; }
inline Complex operator * (Complex a, double b) { return a *= b; }
inline Complex operator / (Complex a, double b) { return a /= b; }

//// complex - complex operators ////

inline Complex operator + (Complex a, Complex const & b) { return a += b; }
inline Complex operator - (Complex a, Complex const & b) { return a -= b; }
inline Complex operator * (Complex a, Complex const & b) { return a *= b; }
inline Complex operator / (Complex a, Complex const & b) { return a /= b; }
inline Complex operator == (Complex a, Complex const & b) { return a.real == b.real && a.complex == b.complex; }
inline Complex operator != (Complex a, Complex const & b) { return a.real != b.real || a.complex != b.complex; }

//// handy overloaded functions ////

inline int Sign(Complex const & a)
{
	if (a.real < 0.0) return -1;
	if (a.real > 0.0) return 1;
	if (a.imaginary < 0) return -1;
	if (a.imaginary > 0) return 1;
	return 1;
}

inline double fabs(Complex const & a)
{
	return sqrt(Sq(a.real) + Sq(a.imaginary));
}

inline Complex sqrt(Complex const & a)
{
	double t = fabs(a);
	double real = 0.5 * sqrt(2.0 * (t + a.real));
	double imaginary = Sign(Complex(a.imaginary, -a.real)) * 0.5 * sqrt(2.0 * (t - a.real));
	return Complex(real, imaginary);
}

inline Complex exp(Complex const & a)
{
	return Complex(cos(a.imaginary), sin(a.imaginary)) * exp(a.real);
}

inline Complex log(Complex const & a)
{
	return Complex(0.5 * log(a.sqLen()), a.angle());
}

inline Complex pow(Complex const & a, double x)
{
	double theta = x * a.angle();
	return Complex(cos(theta), sin(theta)) * pow(a.sqLen(), 0.5 * x);
}

inline Complex pow(Complex const & a, Complex const & b)
{
	double lnSq = 0.5 * log(a.sqLen());
	double aAngle = a.angle();
	double theta = b.imaginary * lnSq + b.real * aAngle;
	return Complex(cos(theta), sin(theta)) * exp(b.real * lnSq - b.imaginary * aAngle);
}

inline Complex sin(Complex const & a)
{
	return Complex(sin(a.real)*cosh(a.imaginary), cos(a.real)*sinh(a.imaginary));
}

inline Complex cos(Complex const & a)
{
	return Complex(cos(a.real)*cosh(a.imaginary), -sin(a.real)*sinh(a.imaginary));
}

#endif
