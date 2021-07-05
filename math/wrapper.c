/* wrapper.c -- This file is part of OS/0 libc.
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

#include <math.h>

float
acosf (float x)
{
  return acos (x);
}

long double
acosl (long double x)
{
  return acos (x);
}

float
asinf (float x)
{
  return asin (x);
}

long double
asinl (long double x)
{
  return asin (x);
}

float
atanf (float x)
{
  return atan (x);
}

long double
atanl (long double x)
{
  return atan (x);
}

float
atan2f (float y, float x)
{
  return atan2 (y, x);
}

long double
atan2l (long double y, long double x)
{
  return atan2 (y, x);
}

float
acoshf (float x)
{
  return acosh (x);
}

long double
acoshl (long double x)
{
  return acosh (x);
}

float
asinhf (float x)
{
  return asinh (x);
}

long double
asinhl (long double x)
{
  return asinh (x);
}

float
atanhf (float x)
{
  return atanh (x);
}

long double
atanhl (long double x)
{
  return atanh (x);
}

float
cosf (float x)
{
  return cos (x);
}

long double
cosl (long double x)
{
  return cos (x);
}

float
sinf (float x)
{
  return sin (x);
}

long double
sinl (long double x)
{
  return sin (x);
}

float
tanf (float x)
{
  return tan (x);
}

long double
tanl (long double x)
{
  return tan (x);
}

float
coshf (float x)
{
  return cosh (x);
}

long double
coshl (long double x)
{
  return cosh (x);
}

float
sinhf (float x)
{
  return sinh (x);
}

long double
sinhl (long double x)
{
  return sinh (x);
}

float
tanhf (float x)
{
  return tanh (x);
}

long double
tanhl (long double x)
{
  return tanh (x);
}

float
expf (float x)
{
  return exp (x);
}

long double
expl (long double x)
{
  return exp (x);
}

float
frexpf (float x, int *exp)
{
  return frexp (x, exp);
}

long double
frexpl (long double x, int *exp)
{
  return frexp (x, exp);
}

float
ldexpf (float x, int exp)
{
  return ldexp (x, exp);
}

long double
ldexpl (long double x, int exp)
{
  return ldexp (x, exp);
}

float
logf (float x)
{
  return log (x);
}

long double
logl (long double x)
{
  return log (x);
}

float
log10f (float x)
{
  return log10 (x);
}

long double
log10l (long double x)
{
  return log10 (x);
}

float
logbf (float x)
{
  return logb (x);
}

long double
logbl (long double x)
{
  return logb (x);
}

float
modff (float x, float *iptr)
{
  double diptr;
  float ret = modf (x, &diptr);
  *iptr = diptr;
  return ret;
}

long double
modfl (long double x, long double *iptr)
{
  double diptr;
  long double ret = modf (x, &diptr);
  *iptr = diptr;
  return ret;
}

float
fmodf (float x, float y)
{
  return fmod (x, y);
}

long double
fmodl (long double x, long double y)
{
  return fmod (x, y);
}

float
powf (float x, float y)
{
  return pow (x, y);
}

long double
powl (long double x, long double y)
{
  return pow (x, y);
}

float
sqrtf (float x)
{
  return sqrt (x);
}

long double
sqrtl (long double x)
{
  return sqrt (x);
}

float
cbrtf (float x)
{
  return cbrt (x);
}

long double
cbrtl (long double x)
{
  return cbrt (x);
}

float
ceilf (float x)
{
  return ceil (x);
}

long double
ceill (long double x)
{
  return ceil (x);
}

float
fabsf (float x)
{
  return fabs (x);
}

long double
fabsl (long double x)
{
  return fabs (x);
}

float
floorf (float x)
{
  return floor (x);
}

long double
floorl (long double x)
{
  return floor (x);
}

float
erff (float x)
{
  return erf (x);
}

long double
erfl (long double x)
{
  return erf (x);
}

float
erfcf (float x)
{
  return erfc (x);
}

long double
erfcl (long double x)
{
  return erfc (x);
}

float
gammaf (float x)
{
  return gamma (x);
}

long double
gammal (long double x)
{
  return gamma (x);
}

float
lgammaf (float x)
{
  return lgamma (x);
}

long double
lgammal (long double x)
{
  return lgamma (x);
}

float
lgammaf_r (float x, int *psigngam)
{
  return lgamma_r (x, psigngam);
}

long double
lgammal_r (long double x, int *psigngam)
{
  return lgamma_r (x, psigngam);
}

float
hypotf (float x, float y)
{
  return hypot (x, y);
}

long double
hypotl (long double x, long double y)
{
  return hypot (x, y);
}

float
nextafterf (float x, float y)
{
  return nextafter (x, y);
}

long double
nextafterl (long double x, long double y)
{
  return nextafter (x, y);
}

float
remainderf (float x, float y)
{
  return remainder (x, y);
}

long double
remainderl (long double x, long double y)
{
  return remainder (x, y);
}

float
scalbnf (float x, int exp)
{
  return scalbn (x, exp);
}

long double
scalbnl (long double x, int exp)
{
  return scalbn (x, exp);
}

double
scalbln (double x, long exp)
{
  return scalbn (x, exp);
}

float
scalblnf (float x, long exp)
{
  return scalbnf (x, exp);
}

long double
scalblnl (long double x, long exp)
{
  return scalbnl (x, exp);
}

float
significandf (float x)
{
  return significand (x);
}

long double
significandl (long double x)
{
  return significand (x);
}

float
copysignf (float x, float y)
{
  return copysign (x, y);
}

long double
copysignl (long double x, long double y)
{
  return copysign (x, y);
}

int
ilogbf (float x)
{
  return ilogb (x);
}

int
ilogbl (long double x)
{
  return ilogb (x);
}

float
rintf (float x)
{
  return rint (x);
}

long double
rintl (long double x)
{
  return rint (x);
}

long
lrint (double x)
{
  return (long) rint (x);
}

long
lrintf (float x)
{
  return (long) rintf (x);
}

long
lrintl (long double x)
{
  return (long) rintl (x);
}

long long
llrint (double x)
{
  return (long long) rint (x);
}

long long
llrintf (float x)
{
  return (long long) rintf (x);
}

long long
llrintl (long double x)
{
  return (long long) rintl (x);
}

float
expm1f (float x)
{
  return expm1 (x);
}

long double
expm1l (long double x)
{
  return expm1 (x);
}

float
log1pf (float x)
{
  return log1p (x);
}

long double
log1pl (long double x)
{
  return log1p (x);
}
