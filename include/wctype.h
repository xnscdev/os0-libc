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

#include <bits/wctype.h>

typedef int wctype_t;
typedef int wctrans_t;

__BEGIN_DECLS

wint_t towctrans (wint_t wc, wctrans_t desc);
wint_t towctrans_l (wint_t wc, wctrans_t desc, locale_t loc);
wctrans_t wctrans (const char *property);
wctrans_t wctrans_l (const char *property, locale_t loc);

int iswctype (wint_t wc, wctype_t type);
int iswctype_l (wint_t wc, wctype_t type, locale_t loc);
wctype_t wctype (const char *property);
wctype_t wctype_l (const char *property, locale_t loc);

__END_DECLS

#endif
