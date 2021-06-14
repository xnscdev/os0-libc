/* posix.c -- This file is part of OS/0 libc.
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
#include <string.h>

struct __locale __libc_posix_locale = {
  {[0 ... (__LC_LAST - 1)] = LOCALE_DATA_POSIX_INIT}
};

size_t
__libc_posix_mbrtowc (wchar_t *__restrict pwc, const char *__restrict str,
		      size_t len, mbstate_t *__restrict ps)
{
  if (pwc != NULL)
    *pwc = *str;
  memset (ps, 0, sizeof (mbstate_t));
  return *str != '\0';
}

size_t
__libc_posix_wcrtomb (char *__restrict str, wchar_t wc,
		      mbstate_t *__restrict ps)
{
  if (str != NULL)
    *str = (char) wc;
  memset (ps, 0, sizeof (mbstate_t));
  return 1;
}
