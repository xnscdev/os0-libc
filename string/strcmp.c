/* strcmp.c -- This file is part of OS/0 libc.
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
strcmp (const char *a, const char *b)
{
  size_t i = 0;
  while (1)
    {
      if (a[i] > b[i])
	return 1;
      if (a[i] < b[i])
	return -1;
      if (a[i] == '\0' && b[i] == '\0')
	return 0;
      i++;
    }
}

int
strncmp (const char *a, const char *b, size_t len)
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

char *
strchr (const char *s, int c)
{
  const char *ptr = s;
  while (1)
    {
      if (*ptr == c)
	return (char *) ptr;
      if (*ptr == '\0')
	return NULL;
      ptr++;
    }
}

char *
strrchr (const char *s, int c)
{
  const char *ptr = s;
  const char *save = NULL;
  while (1)
    {
      if (*ptr == c)
	{
	  if (c == '\0')
	    return (char *) ptr;
	  save = ptr;
	}
      else if (*ptr == '\0')
	return (char *) save;
      ptr++;
    }
}