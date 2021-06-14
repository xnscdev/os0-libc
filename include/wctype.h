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

#ifndef _WCTYPE_H
#define _WCTYPE_H

#include <sys/cdefs.h>
#include <stddef.h>

typedef int wctype_t;
typedef int wctrans_t;

#ifndef __wint_defined
typedef int wint_t;
#define __wint_defined
#endif

__BEGIN_DECLS

int iswalnum (wint_t wc);
int iswalpha (wint_t wc);
int iswascii (wint_t wc);
int iswblank (wint_t wc);
int iswcntrl (wint_t wc);
int iswdigit (wint_t wc);
int iswgraph (wint_t wc);
int iswlower (wint_t wc);
int iswprint (wint_t wc);
int iswpunct (wint_t wc);
int iswspace (wint_t wc);
int iswupper (wint_t wc);
int iswxdigit (wint_t wc);

wint_t towctrans (wint_t wc, wctrans_t desc);
wint_t towlower (wint_t wc);
wint_t towupper (wint_t wc);
wctrans_t wctrans (const char *property);

int iswctype (wint_t wc, wctype_t type);
wctype_t wctype (const char *property);

__END_DECLS

#endif
