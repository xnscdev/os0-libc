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
#include <string.h>
#include <wctype.h>

#define NR_wctype_properties  13
#define NR_wctrans_properties 2

static int (*wctype_funcs_posix[]) (wint_t) = {
  NULL,
  (int (*) (wint_t)) isalnum,
  (int (*) (wint_t)) isalpha,
  (int (*) (wint_t)) isascii,
  (int (*) (wint_t)) isblank,
  (int (*) (wint_t)) iscntrl,
  (int (*) (wint_t)) isdigit,
  (int (*) (wint_t)) isgraph,
  (int (*) (wint_t)) islower,
  (int (*) (wint_t)) isprint,
  (int (*) (wint_t)) ispunct,
  (int (*) (wint_t)) isspace,
  (int (*) (wint_t)) isupper,
  (int (*) (wint_t)) isxdigit
};

static const char *const wctype_props[] = {
  0,
  "alnum",
  "alpha",
  "ascii",
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

static wint_t (*wctrans_funcs_posix[]) (wint_t) = {
  0,
  (wint_t (*) (wint_t)) tolower,
  (wint_t (*) (wint_t)) toupper
};

static const char *const wctrans_props[] = {
  0,
  "lower",
  "upper"
};

int
iswalnum (wint_t wc)
{
  return iswctype (wc, wctype ("alnum"));
}

int
iswalpha (wint_t wc)
{
  return iswctype (wc, wctype ("alpha"));
}

int
iswascii (wint_t wc)
{
  return iswctype (wc, wctype ("ascii"));
}

int
iswblank (wint_t wc)
{
  return iswctype (wc, wctype ("blank"));
}

int
iswcntrl (wint_t wc)
{
  return iswctype (wc, wctype ("cntrl"));
}

int
iswdigit (wint_t wc)
{
  return iswctype (wc, wctype ("digit"));
}

int
iswgraph (wint_t wc)
{
  return iswctype (wc, wctype ("graph"));
}

int
iswlower (wint_t wc)
{
  return iswctype (wc, wctype ("lower"));
}

int
iswprint (wint_t wc)
{
  return iswctype (wc, wctype ("print"));
}

int
iswpunct (wint_t wc)
{
  return iswctype (wc, wctype ("punct"));
}

int
iswspace (wint_t wc)
{
  return iswctype (wc, wctype ("space"));
}

int
iswupper (wint_t wc)
{
  return iswctype (wc, wctype ("upper"));
}

int
iswxdigit (wint_t wc)
{
  return iswctype (wc, wctype ("xdigit"));
}

wint_t
towctrans (wint_t wc, wctrans_t desc)
{
  if (desc <= 0 || desc > NR_wctrans_properties)
    {
      errno = EINVAL;
      return wc;
    }
  return wctrans_funcs_posix[desc] (wc);
}

wint_t
towlower (wint_t wc)
{
  return towctrans (wc, wctrans ("lower"));
}

wint_t
towupper (wint_t wc)
{
  return towctrans (wc, wctrans ("upper"));
}

wctrans_t
wctrans (const char *property)
{
  int i;
  for (i = 1; i <= NR_wctrans_properties; i++)
    {
      if (strcmp (wctrans_props[i], property) == 0)
	return i;
    }
  errno = EINVAL;
  return 0;
}

int
iswctype (wint_t wc, wctype_t type)
{
  if (type <= 0 || type > NR_wctype_properties)
    {
      errno = EINVAL;
      return 0;
    }
  return wctype_funcs_posix[type] (wc);
}

wctype_t
wctype (const char *property)
{
  int i;
  for (i = 1; i <= NR_wctype_properties; i++)
    {
      if (strcmp (wctype_props[i], property) == 0)
	return i;
    }
  errno = EINVAL;
  return 0;
}
