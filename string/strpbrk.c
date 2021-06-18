/* strpbrk.c -- This file is part of OS/0 libc.
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

char *
strpbrk (const char *str, const char *accept)
{
  while (*str != '\0')
    {
      const char *c;
      for (c = accept; *c != '\0'; c++)
	{
	  if (*str == *c)
	    break;
	}
      if (*c != '\0')
	return (char *) str;
      str++;
    }
  return NULL;
}
