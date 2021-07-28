/* wcsstr.c -- This file is part of OS/0 libc.
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
wcsstr (const wchar_t *haystack, const wchar_t *needle)
{
  wchar_t b = *needle;
  wchar_t wc;
  if (b == L'\0')
    return (wchar_t *) haystack;

  haystack--;
  do
    {
      wc = *++haystack;
      if (wc == L'\0')
	return NULL;
    }
  while (wc != b);

  wc = *++needle;
  if (wc == L'\0')
    return (wchar_t *) haystack;
  needle++;

  while (1)
    {
      wchar_t a = *++haystack;
      const wchar_t *rhaystack;
      const wchar_t *rneedle;
      if (a == L'\0')
	return NULL;
      if (a != wc)
	goto loop;

      rhaystack = haystack-- + 1;
      rneedle = needle;
      a = *rneedle;
      if (*rhaystack == a)
	{
	  do
	    {
	      if (a == L'\0')
		return (wchar_t *) haystack;
	      a = *++needle;
	      if (*++rhaystack != a)
		break;
	      if (a == L'\0')
		return (wchar_t *) haystack;
	      a = *++needle;
	    }
	  while (*++rhaystack == a);
	}

      needle = rneedle;
      if (a == L'\0')
	break;

    loop:
      while (a != b)
	{
	  a = *++haystack;
	  if (a == L'\0')
	    return NULL;
	  if (a == b)
	    break;
	  a = *++haystack;
	  if (a == L'\0')
	    return NULL;
	}
    }
  return (wchar_t *) haystack;
}

weak_alias (wcsstr, wcswcs);
