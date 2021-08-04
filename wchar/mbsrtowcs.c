/* mbsrtowcs.c -- This file is part of OS/0 libc.
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
mbstowcs (wchar_t *__restrict pwc, const char *__restrict str, size_t len)
{
  size_t ret = 0;
  char *ptr = (char *) str;
  int bytes;
  __libc_mbstate._count = 0;
  if (pwc == NULL)
    len = 1;
  while (len > 0)
    {
      bytes = mbrtowc (pwc, ptr, MB_CUR_MAX, &__libc_mbstate);
      if (bytes < 0)
	{
	  __libc_mbstate._count = 0;
	  return -1;
	}
      else if (bytes == 0)
	break;
      ptr += bytes;
      ret++;
      if (pwc != NULL)
	{
	  pwc++;
	  len--;
	}
    }
  return ret;
}

size_t
mbsrtowcs (wchar_t *__restrict dest, const char **__restrict src, size_t len,
	   mbstate_t *__restrict ps)
{
  return mbsnrtowcs (dest, src, (size_t) -1, len, ps);
}

size_t
mbsnrtowcs (wchar_t *__restrict dest, const char **__restrict src, size_t nms,
	    size_t len, mbstate_t *__restrict ps)
{
  wchar_t *ptr = dest;
  const char *temp;
  size_t count = 0;
  size_t max;
  int bytes;

  if (ps == NULL)
    ps = &__libc_mbstate;

  if (dest == NULL)
    {
      len = (size_t) -1;
      temp = *src;
      src = &temp;
    }

  max = len;
  while (len > 0)
    {
      bytes = mbrtowc (ptr, *src, nms, ps);
      if (bytes > 0)
	{
	  *src += bytes;
	  nms -= bytes;
	  count++;
	  len--;
	  if (ptr != NULL)
	    ptr++;
	}
      else if (bytes == -2)
	{
	  *src += nms;
	  return count;
	}
      else if (bytes == 0)
	{
	  *src = NULL;
	  return count;
	}
      else
	{
	  ps->_count = 0;
	  errno = EILSEQ;
	  return (size_t) -1;
	}
    }
  return max;
}
