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

#include <bits/types/mbstate.h>

struct tm;

__BEGIN_DECLS

wint_t btowc (int c);
int wctob (wint_t wc);

size_t mbrlen (const char *__restrict str, size_t len,
	       mbstate_t *__restrict ps);
size_t mbrtowc (wchar_t *__restrict pwc, const char *__restrict str, size_t len,
		mbstate_t *__restrict ps);
size_t wcrtomb (char *__restrict str, wchar_t wc, mbstate_t *__restrict ps);

int wcwidth (wchar_t wc);

size_t wcslen (const wchar_t *ws);
size_t wcsnlen (const wchar_t *ws, size_t len);
wchar_t *wcsdup (const wchar_t *ws);
wchar_t *wcpcpy (wchar_t *dest, const wchar_t *src);
wchar_t *wcpncpy (wchar_t *dest, const wchar_t *src, size_t len);
wchar_t *wcscpy (wchar_t *dest, const wchar_t *src);
wchar_t *wcsncpy (wchar_t *dest, const wchar_t *src, size_t len);
int wcscmp (const wchar_t *a, const wchar_t *b);
int wcsncmp (const wchar_t *a, const wchar_t *b, size_t len);
int wcscasecmp (const wchar_t *a, const wchar_t *b);
int wcsncasecmp (const wchar_t *a, const wchar_t *b, size_t len);
int wcscoll (const wchar_t *a, const wchar_t *b);
wchar_t *wcschr (const wchar_t *ws, wchar_t wc);
wchar_t *wcsrchr (const wchar_t *ws, wchar_t wc);
wchar_t *wcsstr (const wchar_t *haystack, const wchar_t *needle);
wchar_t *wcswcs (const wchar_t *haystack, const wchar_t *needle);
wchar_t *wcscat (wchar_t *__restrict a, const wchar_t *__restrict b);
wchar_t *wcsncat (wchar_t *__restrict a, const wchar_t *__restrict b,
		  size_t len);
size_t wcsspn (const wchar_t *str, const wchar_t *accept);
size_t wcscspn (const wchar_t *str, const wchar_t *reject);
wchar_t *wcspbrk (const wchar_t *str, const wchar_t *accept);
wchar_t *wcstok (wchar_t *__restrict ws, const wchar_t *__restrict delims,
		 wchar_t **__restrict saveptr);

__END_DECLS

#endif
