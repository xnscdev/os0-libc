/* wcsrtombs.c -- This file is part of OS/0 libc.
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
#include <errno.h>
#include <stdlib.h>
#include <wchar.h>

size_t
wcstombs (char *__restrict str, const wchar_t *__restrict pwc, size_t len)
{
  char *ptr = str;
  size_t max = len;
  char buffer[MB_LEN_MAX];
  int copy;
  int bytes;
  int i;
  __libc_mbstate._count = 0;
  if (str == NULL)
    {
      size_t nbytes = 0;
      while (*pwc != L'\0')
	{
	  bytes = wctomb (buffer, *pwc++);
	  if (bytes == -1)
	    return -1;
	  nbytes += bytes;
	}
      return nbytes;
    }
  else
    {
      while (len > 0)
	{
	  bytes = wctomb (buffer, *pwc++);
	  if (bytes == -1)
	    return -1;
	  copy = len > bytes ? bytes : len;
	  for (i = 0; i < copy; i++)
	    *ptr++ = buffer[i];
	  if (*pwc == L'\0')
	    return ptr - str - (len >= bytes);
	  pwc++;
	  len -= copy;
	}
      return max;
    }
}

size_t
wcsrtombs (char *__restrict dest, const wchar_t **__restrict src, size_t len,
	   mbstate_t *__restrict ps)
{
  return wcsnrtombs (dest, src, (size_t) -1, len, ps);
}

size_t
wcsnrtombs (char *__restrict dest, const wchar_t **__restrict src, size_t nwc,
	    size_t len, mbstate_t *__restrict ps)
{
  char *ptr = dest;
  char buffer[MB_LEN_MAX];
  wchar_t *pwc;
  size_t n = 0;
  int i;

  if (ps == NULL)
    ps = &__libc_mbstate;

  if (dest == NULL)
    len = (size_t) -1;

  pwc = (wchar_t *) *src;
  while (n < len && nwc-- > 0)
    {
      int count = ps->_count;
      wint_t wc = ps->_value._wch;
      int bytes = wcrtomb (buffer, *pwc, ps);
      if (bytes == -1)
	{
	  errno = EILSEQ;
	  ps->_count = 0;
	  return (size_t) -1;
	}
      if (n + bytes <= len)
	{
	  n += bytes;
	  if (dest != NULL)
	    {
	      for (i = 0; i < bytes; i++)
		*ptr++ = buffer[i];
	      (*src)++;
	    }
	  if (*pwc++ == L'\0')
	    {
	      if (dest != NULL)
		*src = NULL;
	      ps->_count = 0;
	      return n - 1;
	    }
	}
      else
	{
	  ps->_count = count;
	  ps->_value._wch = wc;
	  len = 0;
	}
    }
  return n;
}
