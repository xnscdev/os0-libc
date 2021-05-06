/* string.c -- This file is part of OS/0 libc.
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
#include <stdlib.h>
#include <string.h>

size_t
strlen (const char *s)
{
  size_t i = 0;
  while (s[i] != '\0')
    i++;
  return i;
}

char *
strdup (const char *s)
{
  size_t len = strlen (s);
  char *buffer = malloc (len + 1);
  if (unlikely (buffer == NULL))
    return NULL;
  strcpy (buffer, s);
  return buffer;
}
