
#ifndef _HLNOISE_H
#define _HLNOISE_H


//// Noise functions [DOCUMENTME] ////

double Pnoise (Point const & vec);
double Fnoise (Point vec, int octaves, double scale);
double CFnoise (Point vec, int octaves, double scale);
Vector ColorFnoise (Point v, int o, double s);

#endif
