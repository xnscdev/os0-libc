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

#define LC_COLLATE  0
#define LC_CTYPE    1
#define LC_MESSAGES 2
#define LC_MONETARY 3
#define LC_NUMERIC  4
#define LC_TIME     5
#define LC_ALL      6
#define __LC_LAST   6

#include <bits/types/lconv.h>
#include <bits/types/mbstate.h>

#define __LC_wctype_max  13
#define __LC_wctrans_max 8

#ifndef __wint_defined
typedef unsigned int wint_t;
#define __wint_defined
#endif

struct __locale_ctype_data
{
  int (*wctype[__LC_wctype_max]) (wint_t);
  const char *wctrans_props[__LC_wctrans_max];
  wint_t (*wctrans[__LC_wctrans_max]) (wint_t);
  size_t (*mbrtowc) (wchar_t *__restrict, const char *__restrict, size_t,
		     mbstate_t *__restrict);
  size_t (*wcrtomb) (char *__restrict, wchar_t, mbstate_t *__restrict);
  int has_shift_states;
  char *codeset;
};

struct __locale_messages_data
{
  char *yesexpr;
  char *noexpr;
};

struct __locale_monetary_data
{
  char *currsym;
};

struct __locale_time_data
{
  char *dt_fmt;
  char *d_fmt;
  char *t_fmt;
  char *t_fmt_ampm;
  char *am;
  char *pm;
  char *days[7];
  char *abdays[7];
  char *months[12];
  char *abmonths[12];
  char *era;
  char *era_d_fmt;
  char *era_dt_fmt;
  char *era_t_fmt;
  char *altdigits;
};

struct __locale
{
  struct lconv __lconv;
  struct __locale_ctype_data __LC_CTYPE;
  struct __locale_messages_data __LC_MESSAGES;
  struct __locale_monetary_data __LC_MONETARY;
  struct __locale_time_data __LC_TIME;
  const char *__names[__LC_LAST];
};

#ifndef __locale_defined
typedef struct __locale *locale_t;
#define __locale_defined
#endif

#endif
