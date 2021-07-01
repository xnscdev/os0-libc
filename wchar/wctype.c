/* wctype.c -- This file is part of OS/0 libc.
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

#include <ctype.h>
#include <errno.h>
#include <libc-locale.h>
#include <string.h>
#include <wctype.h>

#define WCTYPE_ALNUM  0
#define WCTYPE_ALPHA  1
#define WCTYPE_BLANK  2
#define WCTYPE_CNTRL  3
#define WCTYPE_DIGIT  4
#define WCTYPE_GRAPH  5
#define WCTYPE_LOWER  6
#define WCTYPE_PRINT  7
#define WCTYPE_PUNCT  8
#define WCTYPE_SPACE  9
#define WCTYPE_UPPER  10
#define WCTYPE_XDIGIT 11
#define WCTYPE_ASCII __LC_wctype_max

#define WCTRANS_TOLOWER 0
#define WCTRANS_TOUPPER 1

mbstate_t __libc_mbstate;

static const char *const wctype_props[__LC_wctype_max] = {
  "alnum",
  "alpha",
  "blank",
  "cntrl",
  "digit",
  "graph",
  "lower",
  "print",
  "punct",
  "space",
  "upper",
  "xdigit"
};

int
iswalnum (wint_t wc)
{
  return iswalnum_l (wc, __libc_locale);
}

int
iswalnum_l (wint_t wc, locale_t loc)
{
  return loc->__LC_CTYPE.wctype[WCTYPE_ALNUM] (wc);
}

int
iswalpha (wint_t wc)
{
  return iswalpha_l (wc, __libc_locale);
}

int
iswalpha_l (wint_t wc, locale_t loc)
{
  return loc->__LC_CTYPE.wctype[WCTYPE_ALPHA] (wc);
}

int
iswascii (wint_t wc)
{
  return isascii (wc);
}

int
iswblank (wint_t wc)
{
  return iswblank_l (wc, __libc_locale);
}

int
iswblank_l (wint_t wc, locale_t loc)
{
  return loc->__LC_CTYPE.wctype[WCTYPE_BLANK] (wc);
}

int
iswcntrl (wint_t wc)
{
  return iswcntrl_l (wc, __libc_locale);
}

int
iswcntrl_l (wint_t wc, locale_t loc)
{
  return loc->__LC_CTYPE.wctype[WCTYPE_CNTRL] (wc);
}

int
iswdigit (wint_t wc)
{
  return iswdigit_l (wc, __libc_locale);
}

int
iswdigit_l (wint_t wc, locale_t loc)
{
  return loc->__LC_CTYPE.wctype[WCTYPE_DIGIT] (wc);
}

int
iswgraph (wint_t wc)
{
  return iswgraph_l (wc, __libc_locale);
}

int
iswgraph_l (wint_t wc, locale_t loc)
{
  return loc->__LC_CTYPE.wctype[WCTYPE_GRAPH] (wc);
}

int
iswlower (wint_t wc)
{
  return iswlower_l (wc, __libc_locale);
}

int
iswlower_l (wint_t wc, locale_t loc)
{
  return loc->__LC_CTYPE.wctype[WCTYPE_LOWER] (wc);
}

int
iswprint (wint_t wc)
{
  return iswprint_l (wc, __libc_locale);
}

int
iswprint_l (wint_t wc, locale_t loc)
{
  return loc->__LC_CTYPE.wctype[WCTYPE_PRINT] (wc);
}

int
iswpunct (wint_t wc)
{
  return iswpunct_l (wc, __libc_locale);
}

int
iswpunct_l (wint_t wc, locale_t loc)
{
  return loc->__LC_CTYPE.wctype[WCTYPE_PUNCT] (wc);
}

int
iswspace (wint_t wc)
{
  return iswspace_l (wc, __libc_locale);
}

int
iswspace_l (wint_t wc, locale_t loc)
{
  return loc->__LC_CTYPE.wctype[WCTYPE_SPACE] (wc);
}

int
iswupper (wint_t wc)
{
  return iswupper_l (wc, __libc_locale);
}

int
iswupper_l (wint_t wc, locale_t loc)
{
  return loc->__LC_CTYPE.wctype[WCTYPE_UPPER] (wc);
}

int
iswxdigit (wint_t wc)
{
  return iswxdigit_l (wc, __libc_locale);
}

int
iswxdigit_l (wint_t wc, locale_t loc)
{
  return loc->__LC_CTYPE.wctype[WCTYPE_XDIGIT] (wc);
}

wint_t
towctrans (wint_t wc, wctrans_t desc)
{
  return towctrans_l (wc, desc, __libc_locale);
}

wint_t
towctrans_l (wint_t wc, wctrans_t desc, locale_t loc)
{
  wint_t (*func) (wint_t);
  if (desc < 0 || desc >= __LC_wctrans_max)
    {
      errno = EINVAL;
      return wc;
    }
  func = loc->__LC_CTYPE.wctrans[desc];
  if (func == NULL)
    {
      errno = EINVAL;
      return wc;
    }
  return func (wc);
}

wint_t
towlower (wint_t wc)
{
  return towlower_l (wc, __libc_locale);
}

wint_t
towlower_l (wint_t wc, locale_t loc)
{
  return loc->__LC_CTYPE.wctrans[WCTRANS_TOLOWER] (wc);
}

wint_t
towupper (wint_t wc)
{
  return towupper_l (wc, __libc_locale);
}

wint_t
towupper_l (wint_t wc, locale_t loc)
{
  return loc->__LC_CTYPE.wctrans[WCTRANS_TOUPPER] (wc);
}

wctrans_t
wctrans (const char *property)
{
  return wctrans_l (property, __libc_locale);
}

wctrans_t
wctrans_l (const char *property, locale_t loc)
{
  int i;
  for (i = 1; i <= __LC_wctrans_max; i++)
    {
      const char *prop = loc->__LC_CTYPE.wctrans_props[i];
      if (prop != NULL && strcmp (prop, property) == 0)
	return i;
    }
  errno = EINVAL;
  return 0;
}

int
iswctype (wint_t wc, wctype_t type)
{
  return iswctype_l (wc, type, __libc_locale);
}

int
iswctype_l (wint_t wc, wctype_t type, locale_t loc)
{
  if (type < 0 || type > __LC_wctype_max)
    {
      errno = EINVAL;
      return 0;
    }
  if (type == WCTYPE_ASCII)
    return isascii (wc);
  return loc->__LC_CTYPE.wctype[type] (wc);
}

wctype_t
wctype (const char *property)
{
  return wctype_l (property, __libc_locale);
}

wctype_t
wctype_l (const char *property, locale_t loc)
{
  int i;
  if (strcmp (property, "ascii") == 0)
    return WCTYPE_ASCII;
  for (i = 0; i < __LC_wctype_max; i++)
    {
      if (strcmp (wctype_props[i], property) == 0)
	return i;
    }
  errno = EINVAL;
  return 0;
}
