/* mbrtowc.c -- This file is part of OS/0 libc.
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
mbtowc (wchar_t *__restrict pwc, const char *__restrict str, size_t len)
{
  size_t ret;
  if (str == NULL)
    {
      memset (&__libc_mbstate, 0, sizeof (mbstate_t));
      return __libc_locale->__LC_CTYPE.has_shift_states;
    }
  ret = mbrtowc (pwc, str, len, &__libc_mbstate);
  if (ret == (size_t) -2)
    return -1;
  return ret;
}

size_t
mbrtowc (wchar_t *__restrict pwc, const char *__restrict str, size_t len,
	 mbstate_t *__restrict ps)
{
  if (ps == NULL)
    ps = &__libc_mbstate;
  return __libc_locale->__LC_CTYPE.mbrtowc (pwc, str, len, ps);
}
