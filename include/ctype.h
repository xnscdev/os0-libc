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

#define isalnum(c) iswalnum (c)
#define isalnum_l(c, loc) iswalnum_l (c, loc)
#define isalpha(c) iswalpha (c)
#define isalpha_l(c, loc) iswalpha_l (c, loc)
#define isblank(c) iswblank (c)
#define isblank_l(c, loc) iswblank_l (c, loc)
#define iscntrl(c) iswcntrl (c)
#define iscntrl_l(c, loc) iswcntrl_l (c, loc)
#define isdigit(c) iswdigit (c)
#define isdigit_l(c, loc) iswdigit_l (c, loc)
#define isgraph(c) iswgraph (c)
#define isgraph_l(c, loc) iswgraph_l (c, loc)
#define islower(c) iswlower (c)
#define islower_l(c, loc) iswlower_l (c, loc)
#define isprint(c) iswprint (c)
#define isprint_l(c, loc) iswprint_l (c, loc)
#define ispunct(c) iswpunct (c)
#define ispunct_l(c, loc) iswpunct_l (c, loc)
#define isspace(c) iswspace (c)
#define isspace_l(c, loc) iswspace_l (c, loc)
#define isupper(c) iswupper (c)
#define isupper_l(c, loc) iswupper_l (c, loc)
#define isxdigit(c) iswxdigit (c)
#define isxdigit_l(c, loc) iswxdigit_l (c, loc)

#define tolower(c) towlower (c)
#define tolower_l(c, loc) towlower_l (c, loc)
#define toupper(c) towupper (c)
#define toupper_l(c, loc) towupper_l (c, loc)

#define isascii(c) ({				\
      int _c = c;				\
      _c >= 0 && _c < 0x80;			\
    })
#define toascii(c) (c & 0x7f)

#endif
