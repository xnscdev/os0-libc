/* fputs.c -- This file is part of OS/0 libc.
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

int
fputs (const char *__restrict str, FILE *__restrict stream)
{
  int ret;
  flockfile (stream);
  ret = fputs_unlocked (str, stream);
  funlockfile (stream);
  return ret;
}

int
fputs_unlocked (const char *__restrict str, FILE *__restrict stream)
{
  for (; *str != '\0'; str++)
    {
      if (fputc_unlocked (*str, stream) == EOF)
	return EOF;
    }
  return 0;
}

int
puts (const char *str)
{
  int ret;
  flockfile (stdout);
  for (; *str != '\0'; str++)
    {
      if (putchar_unlocked (*str) == EOF)
	{
	  funlockfile (stdout);
	  return EOF;
	}
    }
  ret = putchar_unlocked ('\n');
  funlockfile (stdout);
  return ret == EOF ? EOF : 0;
}
