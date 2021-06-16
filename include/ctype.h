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

#include <sys/cdefs.h>

__BEGIN_DECLS

int isalnum (int c);
#define isalnum_l(c, loc) iswalnum_l (c, loc)
int isalpha (int c);
#define isalpha_l(c, loc) iswalpha_l (c, loc)
int isascii (int c);
int isblank (int c);
#define isblank_l(c, loc) iswblank_l (c, loc)
int iscntrl (int c);
#define iscntrl_l(c, loc) iswcntrl_l (c, loc)
int isdigit (int c);
#define isdigit_l(c, loc) iswdigit_l (c, loc)
int isgraph (int c);
#define isgraph_l(c, loc) iswgraph_l (c, loc)
int islower (int c);
#define islower_l(c, loc) iswlower_l (c, loc)
int isprint (int c);
#define isprint_l(c, loc) iswprint_l (c, loc)
int ispunct (int c);
#define ispunct_l(c, loc) iswpunct_l (c, loc)
int isspace (int c);
#define isspace_l(c, loc) iswspace_l (c, loc)
int isupper (int c);
#define isupper_l(c, loc) iswupper_l (c, loc)
int isxdigit (int c);
#define isxdigit_l(c, loc) iswxdigit_l (c, loc)

int toascii (int c);
int tolower (int c);
#define tolower_l(c, loc) towlower_l (c, loc)
int toupper (int c);
#define toupper_l(c, loc) towupper_l (c, loc)

__END_DECLS

#endif
