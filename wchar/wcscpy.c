/* wcscpy.c -- This file is part of OS/0 libc.
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

#include <branch.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>

wchar_t *
wcsdup (const wchar_t *ws)
{
  size_t len = wcslen (ws);
  wchar_t *buffer = malloc (sizeof (wchar_t) * (len + 1));
  if (unlikely (buffer == NULL))
    return NULL;
  wcscpy (buffer, ws);
  return buffer;
}

wchar_t *
wcpcpy (wchar_t *dest, const wchar_t *src)
{
  while (1)
    {
      *dest = *src++;
      if (*dest == L'\0')
	return dest;
      dest++;
    }
}

wchar_t *
wcpncpy (wchar_t *dest, const wchar_t *src, size_t len)
{
  size_t i = 0;
  while (i < len)
    {
      dest[i] = src[i];
      if (dest[i] == L'\0')
	break;
      i++;
    }
  while (i < len)
    dest[i++] = L'\0';
  return dest + len - 1;
}

wchar_t *
wcscpy (wchar_t *dest, const wchar_t *src)
{
  size_t i = 0;
  while (1)
    {
      dest[i] = src[i];
      if (dest[i] == L'\0')
	return dest;
      i++;
    }
}

wchar_t *
wcsncpy (wchar_t *dest, const wchar_t *src, size_t len)
{
  size_t i = 0;
  while (i < len)
    {
      dest[i] = src[i];
      if (dest[i] == L'\0')
	break;
      i++;
    }
  while (i < len)
    dest[i++] = L'\0';
  return dest;
}
