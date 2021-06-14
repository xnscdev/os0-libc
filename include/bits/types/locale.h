/* locale.h -- This file is part of OS/0 libc.
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

#ifndef _BITS_TYPES_LOCALE_H
#define _BITS_TYPES_LOCALE_H

#include <sys/cdefs.h>
#include <stddef.h>

#include <bits/types/lconv.h>
#include <bits/types/mbstate.h>

#define __LC_wctype_max  12
#define __LC_wctrans_max 8

struct __locale_ctype_data
{
  int (*wctype[__LC_wctype_max]) (wint_t);
  const char *wctrans_props[__LC_wctrans_max];
  wint_t (*wctrans[__LC_wctrans_max]) (wint_t);
  size_t (*mbrtowc) (wchar_t *__restrict, const char *__restrict, size_t,
		     mbstate_t *__restrict);
  size_t (*wcrtomb) (char *__restrict, wchar_t, mbstate_t *__restrict);
};

struct __locale
{
  struct lconv __lconv;
  struct __locale_ctype_data __LC_CTYPE;
  const char *__names[__LC_LAST];
};

typedef struct __locale *locale_t;

#endif
