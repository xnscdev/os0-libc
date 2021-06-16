/* strcpy.c -- This file is part of OS/0 libc.
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

#include <stdint.h>
#include <string.h>

char *
stpcpy (char *dest, const char *src)
{
  while (1)
    {
      *dest = *src++;
      if (*dest == '\0')
	return dest;
      dest++;
    }
}

char *
stpncpy (char *dest, const char *src, size_t len)
{
  size_t i = 0;
  while (i < len)
    {
      *dest = *src++;
      if (*dest == '\0')
        break;
      dest++;
      i++;
    }
  while (i < len)
    dest[i++] = '\0';
  return dest;
}

char *
strcpy (char *dest, const char *src)
{
  size_t i = 0;
  while (1)
    {
      dest[i] = src[i];
      if (dest[i] == '\0')
	return dest;
      i++;
    }
}

char *
strncpy (char *dest, const char *src, size_t len)
{
  size_t i = 0;
  while (i < len)
    {
      dest[i] = src[i];
      if (dest[i] == '\0')
        break;
      i++;
    }
  while (i < len)
    dest[i++] = '\0';
  return dest;
}
