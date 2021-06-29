/* ctype.h -- This file is part of OS/0 libc.
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

#ifndef _CTYPE_H
#define _CTYPE_H

#include <bits/wctype.h>

__BEGIN_DECLS

int isalnum (int c);
int isalnum_l (int c, locale_t loc);
int isalpha (int c);
int isalpha_l (int c, locale_t loc);
int isblank (int c);
int isblank_l (int c, locale_t loc);
int iscntrl (int c);
int iscntrl_l (int c, locale_t loc);
int isdigit (int c);
int isdigit_l (int c, locale_t loc);
int isgraph (int c);
int isgraph_l (int c, locale_t loc);
int islower (int c);
int islower_l (int c, locale_t loc);
int isprint (int c);
int isprint_l (int c, locale_t loc);
int ispunct (int c);
int ispunct_l (int c, locale_t loc);
int isspace (int c);
int isspace_l (int c, locale_t loc);
int isupper (int c);
int isupper_l (int c, locale_t loc);
int isxdigit (int c);
int isxdigit_l (int c, locale_t loc);

int tolower (int c);
int tolower_l (int c, locale_t loc);
int toupper (int c);
int toupper_l (int c, locale_t loc);

int isascii (int c);
int toascii (int c);

__END_DECLS

#endif
