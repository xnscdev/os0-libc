/* strtod_l.c -- This file is part of OS/0 libc.
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
#include <strtol.h>

long double
__strtold_l (const char *__restrict str, char **__restrict end, int group,
	     locale_t loc)
{
  return 0; /* TODO Implement */
}

double
strtod_l (const char *__restrict str, char **__restrict end, locale_t loc)
{
  return (double) __strtold_l (str, end, 0, loc);
}

float
strtof_l (const char *__restrict str, char **__restrict end, locale_t loc)
{
  return (float) __strtold_l (str, end, 0, loc);
}

long double
strtold_l (const char *__restrict str, char **__restrict end, locale_t loc)
{
  return __strtold_l (str, end, 0, loc);
}
