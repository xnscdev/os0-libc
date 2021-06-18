/* btowc.c -- This file is part of OS/0 libc.
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

#include <libc-locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

wint_t
btowc (int c)
{
  wchar_t wc;
  unsigned char b;
  int ret;
  if (c == EOF)
    return WEOF;
  b = (unsigned char) c;
  memset (&__libc_mbstate, 0, sizeof (mbstate_t));
  ret = mbtowc (&wc, (const char *) &b, 1);
  if (ret != 0 && ret != 1)
    return WEOF;
  return wc;
}
