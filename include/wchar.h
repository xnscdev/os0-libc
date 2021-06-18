/* wchar.h -- This file is part of OS/0 libc.
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

#ifndef _WCHAR_H
#define _WCHAR_H

#include <limits.h>
#include <stdint.h>
#include <wctype.h>

#define WEOF ((wint_t) -1)

#ifndef __FILE_defined
typedef struct __FILE FILE;
#define __FILE_defined
#endif

#define MB_CUR_MAX MB_LEN_MAX

#include <bits/types/mbstate.h>

struct tm;

__BEGIN_DECLS

wint_t btowc (int c);
int wctob (wint_t wc);

size_t mblen (const char *str, size_t len);
size_t mbrlen (const char *__restrict str, size_t len,
	       mbstate_t *__restrict ps);

size_t mbrtowc (wchar_t *__restrict pwc, const char *__restrict str, size_t len,
		mbstate_t *__restrict ps);
size_t wcrtomb (char *__restrict str, wchar_t wc, mbstate_t *__restrict ps);

__END_DECLS

#endif