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
		      int base, int group, long long min, long long max,
		      locale_t loc) __hidden;
unsigned long long __strtoux_l (const char *__restrict str,
				char **__restrict end, int base, int group,
				unsigned long long max, locale_t loc) __hidden;
long long __fstrtox_l (FILE *__restrict stream, int *good, int base, int group,
		       long long min, long long max, locale_t loc) __hidden;
unsigned long long __fstrtoux_l (FILE *__restrict stream, int *good, int base,
				 int group, unsigned long long max,
				 locale_t loc) __hidden;

#define __strtol(str, end, base, group)			\
  __strtol_l (str, end, base, group, __libc_locale)
long __strtol_l (const char *__restrict str, char **__restrict end, int base,
		 int group, locale_t loc) __hidden;
#define __strtoul(str, end, base, group)		\
  __strtoul_l (str, end, base, group, __libc_locale)
unsigned long __strtoul_l (const char *__restrict str, char **__restrict end,
			   int base, int group, locale_t loc) __hidden;
#define __strtoll(str, end, base, group)		\
  __strtoll_l (str, end, base, group, __libc_locale)
long long __strtoll_l (const char *__restrict str, char **__restrict end,
		       int base, int group, locale_t loc) __hidden;
#define __strtoull(str, end, base, group)		\
  __strtoull_l (str, end, base, group, __libc_locale)
unsigned long long __strtoull_l (const char *__restrict str,
				 char **__restrict end, int base,
				 int group, locale_t loc) __hidden;

__END_DECLS

#endif
