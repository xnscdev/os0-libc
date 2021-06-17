/* ctype.c -- This file is part of OS/0 libc.
   Copyright (wc) 2021 XNSC

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

#include <libc-locale.h>

/* ctype functions for POSIX C locale */

int
__libc_posix_iswalnum (wint_t wc)
{
  return __libc_posix_iswalpha (wc) || __libc_posix_iswdigit (wc);
}

int
__libc_posix_iswalpha (wint_t wc)
{
  return __libc_posix_iswlower (wc) || __libc_posix_iswupper (wc);
}

int
__libc_posix_iswblank (wint_t wc)
{
  return wc == ' ' || wc == '\t';
}

int
__libc_posix_iswcntrl (wint_t wc)
{
  return (unsigned char) wc < ' ' || wc == '\177';
}

int
__libc_posix_iswdigit (wint_t wc)
{
  return wc >= '0' && wc <= '9';
}

int
__libc_posix_iswgraph (wint_t wc)
{
  return wc > ' ' && wc <= '~';
}

int
__libc_posix_iswlower (wint_t wc)
{
  return wc >= 'a' && wc <= 'z';
}

int
__libc_posix_iswprint (wint_t wc)
{
  return wc == ' ' || __libc_posix_iswgraph (wc);
}

int
__libc_posix_iswpunct (wint_t wc)
{
  return __libc_posix_iswgraph (wc) && !__libc_posix_iswalnum (wc);
}

int
__libc_posix_iswspace (wint_t wc)
{
  return __libc_posix_iswblank (wc) || wc == '\n' || wc == '\v' || wc == '\f'
    || wc == '\r';
}

int
__libc_posix_iswupper (wint_t wc)
{
  return wc >= 'A' && wc <= 'Z';
}

int
__libc_posix_iswxdigit (wint_t wc)
{
  return __libc_posix_iswdigit (wc) || (wc >= 'a' && wc <= 'f')
    || (wc >= 'A' && wc <= 'F');
}

wint_t
__libc_posix_towlower (wint_t wc)
{
  return __libc_posix_iswupper (wc) ? wc + 0x20 : wc;
}

wint_t
__libc_posix_towupper (wint_t wc)
{
  return __libc_posix_iswlower (wc) ? wc - 0x20 : wc;
}
