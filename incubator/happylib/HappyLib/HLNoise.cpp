
//
// This code is mostly stolen from the Shades project.  Here's the original comment:
//
// This file contains source code for the procedures described in 
// "Texturing and Modeling: A Procedural Approach" by Ebert, Musgrave,
// Peachey, Perlin, and Worley (copyright 1994 AP Professional).
//

#include "HLUtils.h"
#include "HLPoint.h"
#include "HLNoise.h"

#define B	0x100
#define BM	0xff
#define N	0x1000
#define NM	0xfff
#define NP	12
#define s_curve(t) ((t) * (t) * (3. - 2. * (t)))
#define LERP(t,x0,x1)  Lerp(x0,x1,t)

static p[B + B + 2];
static Vector g3[B + B + 2];
static start = 1;

#define setup(i,b0,b1,r0,r1)\
	t = vec[i] + N;\
	b0 = ((int)t) & BM;\
	b1 = (b0+1) & BM;\
	r0 = t - (int)t;\
	r1 = r0 - 1.;

static void
init (void)
{
	srand(42);	// we want the same thing every time we render!
	// [NOTE] you gotta call init() from a critical section, otherwise
	//        other threads may be using rand() at the same time...
	int i, j, k;
	
	for (i = 0; i < B; i++)
	{
		p[i] = i;
		for (j = 0; j < 3; j++) g3[i][j] = (double) ((rand () % (B + B)) - B) / B;
		g3[i].normalize();
	}
	
	while (--i)
	{
		k = p[i];
		p[i] = p[j = rand () % B];
		p[j] = k;
	}
	
	for (i = 0; i < B + 2; i++)
	{
		p[B + i] = p[i];
		for (j = 0; j < 3; j++) g3[B + i][j] = g3[i][j];
	}
}

double Pnoise (Point const & vec)
{
	int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
	double rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
	register i, j;
	
	if (start)
	{
		start = 0;
		init ();
	}
	
	setup (0, bx0, bx1, rx0, rx1);
	setup (1, by0, by1, ry0, ry1);
	setup (2, bz0, bz1, rz0, rz1);
	
	i = p[bx0];
	j = p[bx1];
	
	b00 = p[i + by0];
	b10 = p[j + by0];
	b01 = p[i + by1];
	b11 = p[j + by1];
	
	t = s_curve (rx0);
	sy = s_curve (ry0);
	sz = s_curve (rz0);
	
#define at3(X,Y,Z,V)\
		q = g3[b##X##Y + bz##Z];\
		V = rx##X * q[0] + ry##Y * q[1] + rz##Z * q[2]

	at3(0,0,0,u);
	at3(1,0,0,v);
	a = LERP (t, u, v);
	
	at3(0,1,0,u);
	at3(1,1,0,v);
	b = LERP (t, u, v);
	
	c = LERP (sy, a, b);
	
	at3(0,0,1,u);
	at3(1,0,1,v);
	a = LERP (t, u, v);
	
	at3(0,1,1,u);
	at3(1,1,1,v);
	b = LERP (t, u, v);
	
	d = LERP (sy, a, b);

#undef at3
	
	return LERP (sz, c, d);
}

double Fnoise (Point vec, int octaves, double scale)
{
	double v = 0.0;
	double s = 1.0 / scale;
	double a = 1.0;
	for (int o = 0; o < octaves - 1; o ++)
	{
		v += Pnoise(vec) * a;
		vec *= s;
		a *= scale;
	}
	v += Pnoise(vec) * a;
	return v;
}

double CFnoise (Point vec, int octaves, double scale)
{
	double v = Fnoise(vec, octaves, scale);
	v = (v + 1.0) * 0.5;
	Clamp(v, 0.0, 1.0);
	return v;
}

Vector ColorFnoise (Point v, int o, double s)
{
	return Vector(CFnoise(v, o, s),
		CFnoise(v+Vector(42, 4242, 424242), o, s),
		CFnoise(v+Vector(424242, 4242, 42), o, s));
}
