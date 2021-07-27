/* asprintf.c -- This file is part of OS/0 libc.
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
#include <stdio.h>
#include <stdlib.h>

int
asprintf (char **ret, const char *__restrict fmt, ...)
{
  va_list args;
  int len;
  va_start (args, fmt);
  len = vasprintf (ret, fmt, args);
  va_end (args);
  return len;
}

int
vasprintf (char **ret, const char *__restrict fmt, va_list args)
{
  va_list ap;
  int len;
  char *buffer;

  va_copy (ap, args);
  len = vsnprintf (NULL, 0, fmt, ap);
  va_end (ap);
  if (len < 0)
    {
      *ret = NULL;
      return -1;
    }

  buffer = malloc (len + 1);
  if (unlikely (buffer == NULL))
    {
      *ret = NULL;
      return -1;
    }
  len = vsnprintf (buffer, len + 1, fmt, args);
  if (len < 0)
    {
      free (buffer);
      *ret = NULL;
      return -1;
    }
  *ret = buffer;
  return len;
}
