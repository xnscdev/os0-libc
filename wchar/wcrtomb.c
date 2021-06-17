/* wcrtomb.c -- This file is part of OS/0 libc.
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
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

int
wctomb (char *str, wchar_t wc)
{
  if (str == NULL)
    {
      memset (&__libc_mbstate, 0, sizeof (mbstate_t));
      return __libc_locale->__LC_CTYPE.has_shift_states;
    }
  return wcrtomb (str, wc, &__libc_mbstate);
}

size_t
wcrtomb (char *__restrict str, wchar_t wc, mbstate_t *__restrict ps)
{
  if (ps == NULL)
    ps = &__libc_mbstate;
  return __libc_locale->__LC_CTYPE.wcrtomb (str, wc, ps);
}
