/* memcmp.c -- This file is part of OS/0 libc.
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

#include <string.h>

int
memcmp (const void *a, const void *b, size_t len)
{
  unsigned char *ca = (unsigned char *) a;
  unsigned char *cb = (unsigned char *) b;
  size_t i;
  for (i = 0; i < len; i++)
    {
      if (ca[i] > cb[i])
	return 1;
      else if (ca[i] < cb[i])
	return -1;
    }
  return 0;
}

void *
memchr (const void *str, int c, size_t len)
{
  size_t i;
  for (i = 0; i < len; str++, i++)
    {
      if (*((unsigned char *) str) == c)
	return str;
    }
  return NULL;
}
