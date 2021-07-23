/* stdmath.c -- This file is part of OS/0 libc.
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

#include <stdlib.h>

int
abs (int n)
{
  return n < 0 ? -n : n;
}

long
labs (long n)
{
  return n < 0 ? -n : n;
}

long long
llabs (long long n)
{
  return n < 0 ? -n : n;
}

div_t
div (int num, int denom)
{
  div_t result;
  result.quot = num / denom;
  result.rem = num % denom;
  return result;
}

ldiv_t
ldiv (long num, long denom)
{
  ldiv_t result;
  result.quot = num / denom;
  result.rem = num % denom;
  return result;
}

lldiv_t
lldiv (long long num, long long denom)
{
  lldiv_t result;
  result.quot = num / denom;
  result.rem = num % denom;
  return result;
}

imaxdiv_t
imaxdiv (intmax_t num, intmax_t denom)
{
  imaxdiv_t result;
  result.quot = num / denom;
  result.rem = num % denom;
  return result;
}
