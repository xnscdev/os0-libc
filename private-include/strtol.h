/* strtol.h -- This file is part of OS/0 libc.
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

#ifndef _STRTOL_H
#define _STRTOL_H

#include <libc-locale.h>
#include <stdio.h>

__BEGIN_DECLS

int __strtox_baseval (char c, int base, locale_t loc) __hidden;
char __strtox_get_grouping (int group, locale_t loc) __hidden;

long long __strtox_l (const char *__restrict str, char **__restrict end,
		      int base, int group, unsigned long width, long long min,
		      long long max, locale_t loc) __hidden;
unsigned long long __strtoux_l (const char *__restrict str,
				char **__restrict end, int base, int group,
				unsigned long width, unsigned long long max,
				locale_t loc) __hidden;
long long __fstrtox_l (FILE *__restrict stream, int *good, int base, int group,
		       unsigned long max_field_width, long long min,
		       long long max, locale_t loc) __hidden;
unsigned long long __fstrtoux_l (FILE *__restrict stream, int *good, int base,
				 int group, unsigned long max_field_width,
				 unsigned long long max, locale_t loc) __hidden;

#define __strtold(str, end, group)		\
  __strtold_l (str, end, group, __libc_locale)
long double __strtold_l (const char *__restrict str, char **__restrict end,
			 int group, locale_t loc) __hidden;

__END_DECLS

#endif
