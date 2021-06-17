/* wctype.h -- This file is part of OS/0 libc.
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

#ifndef _BITS_WCTYPE_H
#define _BITS_WCTYPE_H

#if !defined _CTYPE_H && !defined _WCTYPE_H
#error "<bits/wctype.h> should not be included directly"
#endif

#include <sys/cdefs.h>

typedef int wctype_t;
typedef int wctrans_t;

#include <bits/types/locale.h>

__BEGIN_DECLS

int iswalnum (wint_t wc);
int iswalnum_l (wint_t wc, locale_t loc);
int iswalpha (wint_t wc);
int iswalpha_l (wint_t wc, locale_t loc);
int iswascii (wint_t wc);
int iswblank (wint_t wc);
int iswblank_l (wint_t wc, locale_t loc);
int iswcntrl (wint_t wc);
int iswcntrl_l (wint_t wc, locale_t loc);
int iswdigit (wint_t wc);
int iswdigit_l (wint_t wc, locale_t loc);
int iswgraph (wint_t wc);
int iswgraph_l (wint_t wc, locale_t loc);
int iswlower (wint_t wc);
int iswlower_l (wint_t wc, locale_t loc);
int iswprint (wint_t wc);
int iswprint_l (wint_t wc, locale_t loc);
int iswpunct (wint_t wc);
int iswpunct_l (wint_t wc, locale_t loc);
int iswspace (wint_t wc);
int iswspace_l (wint_t wc, locale_t loc);
int iswupper (wint_t wc);
int iswupper_l (wint_t wc, locale_t loc);
int iswxdigit (wint_t wc);
int iswxdigit_l (wint_t wc, locale_t loc);

wint_t towlower (wint_t wc);
wint_t towlower_l (wint_t wc, locale_t loc);
wint_t towupper (wint_t wc);
wint_t towupper_l (wint_t wc, locale_t loc);

__END_DECLS

#endif
