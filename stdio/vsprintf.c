/* vsprintf.c -- This file is part of OS/0 libc.
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

#include <stdio.h>
#include <stream.h>

static int
__vsprintf_put (void *stream, char c)
{
  char **str = stream;
  *(*str)++ = c;
  return 0;
}

int
vsprintf (char *__restrict str, const char *__restrict fmt, va_list args)
{
  return __vxprintf ((void *) &str, fmt, __vsprintf_put, args);
}

int
vsnprintf (char *__restrict str, size_t size, const char *__restrict fmt,
	   va_list args)
{
  return EOF;
}
