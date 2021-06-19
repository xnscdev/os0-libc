/* math.h -- This file is part of OS/0 libc.
   Copyright (C) 2021 XNSC

   OS/0 libc is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   OS/0 libc is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with OS/0 libc. If not, see <https://www.gnu.org/licenses/>. */

#ifndef _MATH_H
#define _MATH_H

#include <sys/cdefs.h>

__BEGIN_DECLS

double acos (double x);
double asin (double x);
double atan (double x);
double atan2 (double y, double x);

double acosh (double x);
double asinh (double x);
double atanh (double x);

double cos (double x);
double sin (double x);
double tan (double x);

double cosh (double x);
double sinh (double x);
double tanh (double x);

double exp (double x);
double frexp (double x, int *exp);
double ldexp (double x, int exp);

double log (double x);
double log10 (double x);
double logb (double x);

double modf (double x, double *iptr);
double fmod (double x, double y);

double pow (double x, double y);
double sqrt (double x);
double cbrt (double x);

double ceil (double x);
double fabs (double x);
double floor (double x);

double erf (double x);
double erfc (double x);
double gamma (double x);
double lgamma (double x);
double hypot (double x, double y);

double isnan (double x);
double finite (double x);

double j0 (double x);
double j1 (double x);
double jn (int n, double x);
double y0 (double x);
double y1 (double x);
double yn (int n, double x);

double nextafter (double x, double y);
double remainder (double x, double y);
double scalb (double x, double exp);
double scalbn (double x, int exp);
double significand (double x);
double copysign (double x, double y);
double ilogb (double x);
double rint (double x);

double expm1 (double x);
double log1p (double x);

__END_DECLS

#endif
