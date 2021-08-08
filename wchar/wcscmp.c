/* wcscmp.c -- This file is part of OS/0 libc.
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

#include <symbol.h>
#include <wchar.h>

wchar_t *
wmemchr (const wchar_t *str, wchar_t c, size_t len)
{
  size_t i;
  for (i = 0; i < len; str++, i++)
    {
      if (*str == c)
	return (wchar_t *) str;
    }
  return NULL;
}

wchar_t *
wmemrchr (const wchar_t *str, wchar_t c, size_t len)
{
  size_t i;
  const wchar_t *ptr = str + len - 1;
  for (i = 0; i < len; ptr--, i++)
    {
      if (*ptr == c)
	return (wchar_t *) ptr;
    }
  return NULL;
}

int
wcscmp (const wchar_t *a, const wchar_t *b)
{
  size_t i = 0;
  while (1)
    {
      if (a[i] > b[i])
	return 1;
      if (a[i] < b[i])
	return -1;
      if (a[i] == L'\0' || b[i] == L'\0')
	return 0;
      i++;
    }
}

int
wcsncmp (const wchar_t *a, const wchar_t *b, size_t len)
{
  size_t i;
  for (i = 0; i < len; i++)
    {
      if (a[i] > b[i])
	return 1;
      if (a[i] < b[i])
	return -1;
    }
  return 0;
}

int
wcscasecmp (const wchar_t *a, const wchar_t *b)
{
  size_t i = 0;
  while (1)
    {
      wchar_t ca = towlower (a[i]);
      wchar_t cb = towlower (b[i]);
      if (ca > cb)
	return 1;
      if (ca < cb)
	return -1;
      if (ca == L'\0' && cb == L'\0')
	return 0;
      i++;
    }
}

int
wcsncasecmp (const wchar_t *a, const wchar_t *b, size_t len)
{
  size_t i;
  for (i = 0; i < len; i++)
    {
      wchar_t ca = towlower (a[i]);
      wchar_t cb = towlower (b[i]);
      if (ca > cb)
	return 1;
      if (ca < cb)
	return -1;
      i++;
    }
  return 0;
}

int
wcscoll (const wchar_t *a, const wchar_t *b)
{
  return wcscmp (a, b);
}

size_t
wcsxfrm (wchar_t *__restrict dest, const wchar_t *__restrict src, size_t len)
{
  size_t i;
  for (i = 0; i < len - 1 && src[i] != '\0'; i++)
    dest[i] = src[i];
  dest[i] = L'\0';
  return i;
}

wchar_t *
wcschr (const wchar_t *ws, wchar_t wc)
{
  const wchar_t *ptr = ws;
  while (1)
    {
      if (*ptr == wc)
	return (wchar_t *) ptr;
      if (*ptr == L'\0')
	return NULL;
      ptr++;
    }
}

wchar_t *
wcsrchr (const wchar_t *ws, wchar_t wc)
{
  const wchar_t *ptr = ws;
  const wchar_t *save = NULL;
  while (1)
    {
      if (*ptr == wc)
	{
	  if (wc == L'\0')
	    return (wchar_t *) ptr;
	  save = ptr;
	}
      else if (*ptr == L'\0')
	return (wchar_t *) save;
      ptr++;
    }
}

weak_alias (wcsncmp, wmemcmp);
