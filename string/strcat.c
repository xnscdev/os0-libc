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

#include <string.h>

char *
strcat (char *__restrict a, const char *__restrict b)
{
  char *ptr = strchr (a, '\0');
  strcpy (ptr, b);
  return a;
}

char *
strncat (char *__restrict a, const char *__restrict b, size_t len)
{
  char *ptr = strchr (a, '\0');
  strncpy (ptr, b, len);
  ptr[len] = '\0';
  return a;
}
