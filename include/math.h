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

#define M_E        2.718281828459045235360287471352662498
#define M_LOG2E    1.442695040888963407359924681001892137
#define M_LOG10E   0.434294481903251827651128918916605082
#define M_LN2      0.693147180559945309417232121458176568
#define M_LN10     2.302585092994045684017991454684364208
#define M_PI       3.141592653589793238462643383279502884
#define M_PI_2     1.570796326794896619231321691639751442
#define M_PI_4     0.785398163397448309615660845819875721
#define M_1_PI     0.318309886183790671537767526745028724
#define M_2_PI     0.636619772367581343075535053490057448
#define M_2_SQRTPI 1.128379167095512573896158903121545172
#define M_SQRT2    1.414213562373095048801688724209698079
#define M_SQRT1_2  0.707106781186547524400844362104849039

#define FP_INFINITE  1
#define FP_NAN       2
#define FP_NORMAL    3
#define FP_SUBNORMAL 4
#define FP_ZERO      5

#define HUGE_VAL  __builtin_huge_val ()
#define HUGE_VALF __builtin_huge_valf ()
#define HUGE_VALL __builtin_huge_vall ()
#define INFINITY  __builtin_inff ()
#define NAN       __builtin_nanf ("")

__BEGIN_DECLS

double acos (double x);
float acosf (float x);
long double acosl (long double x);

double asin (double x);
float asinf (float x);
long double asinl (long double x);

double atan (double x);
float atanf (float x);
long double atanl (long double x);

double atan2 (double y, double x);
float atan2f (float y, float x);
long double atan2l (long double y, long double x);

double acosh (double x);
float acoshf (float x);
long double acoshl (long double x);

double asinh (double x);
float asinhf (float x);
long double asinhl (long double x);

double atanh (double x);
float atanhf (float x);
long double atanhl (long double x);

double cos (double x);
float cosf (float x);
long double cosl (long double x);

double sin (double x);
float sinf (float x);
long double sinl (long double x);

double tan (double x);
float tanf (float x);
long double tanl (long double x);

double cosh (double x);
float coshf (float x);
long double coshl (long double x);

double sinh (double x);
float sinhf (float x);
long double sinhl (long double x);

double tanh (double x);
float tanhf (float x);
long double tanhl (long double x);

double exp (double x);
float expf (float x);
long double expl (long double x);

double frexp (double x, int *exp);
float frexpf (float x, int *exp);
long double frexpl (long double x, int *exp);

double ldexp (double x, int exp);
float ldexpf (float x, int exp);
long double ldexpl (long double x, int exp);

double log (double x);
float logf (float x);
long double logl (long double x);

double log10 (double x);
float log10f (float x);
long double log10l (long double x);

double logb (double x);
float logbf (float x);
long double logbl (long double x);

double modf (double x, double *iptr);
float modff (float x, float *iptr);
long double modfl (long double x, long double *iptr);

double fmod (double x, double y);
float fmodf (float x, float y);
long double fmodl (long double x, long double y);

double pow (double x, double y);
float powf (float x, float y);
long double powl (long double x, long double y);

double sqrt (double x);
float sqrtf (float x);
long double sqrtl (long double x);

double cbrt (double x);
float cbrtf (float x);
long double cbrtl (long double x);

double ceil (double x);
float ceilf (float x);
long double ceill (long double x);

double fabs (double x);
float fabsf (float x);
long double fabsl (long double x);

double floor (double x);
float floorf (float x);
long double floorl (long double x);

double erf (double x);
float erff (float x);
long double erfl (long double x);

double erfc (double x);
float erfcf (float x);
long double erfcl (long double x);

double gamma (double x);
float gammaf (float x);
long double gammal (long double x);

double lgamma (double x);
float lgammaf (float x);
long double lgammal (long double x);
double lgamma_r (double x, int *psigngam);
float lgammaf_r (float x, int *psigngam);
long double lgammal_r (long double x, int *psigngam);

double hypot (double x, double y);
float hypotf (float x, float y);
long double hypotl (long double x, long double y);

int finite (double x);

#define fpclassify(x) __builtin_fpclassify (FP_NAN, FP_INFINITE, FP_NORMAL, \
					    FP_SUBNORMAL, FP_ZERO, (x))
#define isfinite(x) __builtin_isfinite (x)
#define isgreater(x, y) (x > y)
#define isgreaterequal(x, y) (x >= y)
#define isinf(x) __builtin_isinf_sign (x)
#define isless(x, y) (x < y)
#define islessequal(x, y) (x <= y)
#define islessgreater(x, y) (x != y)
#define isnan(x) __builtin_isnan (x)
#define isnormal(x) __builtin_isnormal (x)
#define isunordered(x, y) __builtin_isunordered (x, y)
#define signbit(x) __builtin_signbit (x)

double j0 (double x);
double j1 (double x);
double jn (int n, double x);
double y0 (double x);
double y1 (double x);
double yn (int n, double x);

double nextafter (double x, double y);
float nextafterf (float x, float y);
long double nextafterl (long double x, long double y);

double remainder (double x, double y);
float remainderf (float x, float y);
long double remainderl (long double x, long double y);

double scalb (double x, double exp);
double scalbn (double x, int exp);
float scalbnf (float x, int exp);
long double scalbnl (long double x, int exp);
double scalbln (double x, long exp);
float scalblnf (float x, long exp);
long double scalblnl (long double x, long exp);

double significand (double x);
float significandf (float x);
long double significandl (long double x);

double copysign (double x, double y);
float copysignf (float x, float y);
long double copysignl (long double x, long double y);

int ilogb (double x);
int ilogbf (float x);
int ilogbl (long double x);

double rint (double x);
float rintf (float x);
long double rintl (long double x);
long lrint (double x);
long lrintf (float x);
long lrintl (long double x);
long long llrint (double x);
long long llrintf (float x);
long long llrintl (long double x);

double expm1 (double x);
float expm1f (float x);
long double expm1l (long double x);

double log1p (double x);
float log1pf (float x);
long double log1pl (long double x);

__END_DECLS

#endif
